require 'torch'
require 'nn'
require 'image'
require 'paths'
require 'lib/AdaptiveInstanceNormalization'
require 'lib/utils'

function parseAndRun( content_, style_, gpu_, preserveColor_, alpha_, styleInter_, mask_ )
    local opt = {
        content = content_,
        style = style_,
        vgg = 'models/vgg_normalised.t7',
        decoder = 'models/decoder.t7',
        contentSize = 0,
        styleSize = 0,
        -- contentSize = 512,
        -- styleSize = 512,
        crop = false,
        saveExt = 'jpg',
        gpu = gpu_,
        outputDir = 'tmp',
        saveOriginal = false,
        preserveColor = preserveColor_,
        alpha = alpha_,
        styleInterpWeights = styleInter_,
        mask = mask_
    }
    if opt.preserveColor == nil then opt.preserveColor = true end
    if opt.alpha == nil then opt.alpha = 1 end
    if opt.styleInterpWeights == nil then opt.styleInterpWeights = '' end
    if opt.mask == nil then opt.mask = '' end
    print(opt)

    return runTest(opt)
end

function runTest(opt)
    if opt.gpu >= 0 then
        require 'cudnn'
        require 'cunn'
    end

    assert(paths.filep(opt.decoder), 'Decoder ' .. opt.decoder .. ' does not exist.')

    vgg = torch.load(opt.vgg)
    for i=53,32,-1 do
        vgg:remove(i)
    end
    local adain = nn.AdaptiveInstanceNormalization(vgg:get(#vgg-1).nOutputPlane)
    decoder = torch.load(opt.decoder)

    if opt.gpu >= 0 then
        cutorch.setDevice(opt.gpu+1)
        vgg = cudnn.convert(vgg, cudnn):cuda()
        -- vgg:cuda()
        adain:cuda()
        -- decoder = cudnn.convert(decoder, cudnn):cuda()
        decoder:cuda()
    else
        vgg:float()
        adain:float()
        decoder:float()
    end

    local function styleTransfer(content, style)

        if opt.gpu >= 0 then
            content = content:cuda()
            style = style:cuda()
        else
            content = content:float()
            style = style:float()
        end

        styleFeature = vgg:forward(style):clone()
        contentFeature = vgg:forward(content):clone()

        if opt.mask ~= '' then -- spatial control
            assert(styleFeature:size(1) == 2) -- expect two style images
            local styleFeatureFG = styleFeature[1]
            local styleFeatureBG = styleFeature[2]
            local C, H, W = contentFeature:size(1), contentFeature:size(2), contentFeature:size(3)
            local maskResized = image.scale(mask, W, H, 'simple')
            local maskView = maskResized:view(-1)
            local fgmask = torch.LongTensor(torch.find(maskView, 1)) -- foreground indices
            local bgmask = torch.LongTensor(torch.find(maskView, 0)) -- background indices

            local contentFeatureView = contentFeature:view(C, -1)
            local contentFeatureFG = contentFeatureView:index(2, fgmask):view(C, fgmask:nElement(), 1) -- C * #fg
            local contentFeatureBG = contentFeatureView:index(2, bgmask):view(C, bgmask:nElement(), 1) -- C * #bg

            targetFeatureFG = adain:forward({contentFeatureFG, styleFeatureFG}):clone():squeeze()
            targetFeatureBG = adain:forward({contentFeatureBG, styleFeatureBG}):squeeze()
            targetFeature = contentFeatureView:clone():zero() -- C * (H*W)
            targetFeature:indexCopy(2, fgmask ,targetFeatureFG)
            targetFeature:indexCopy(2, bgmask ,targetFeatureBG)
            targetFeature = targetFeature:viewAs(contentFeature)

        elseif opt.styleInterpWeights ~= '' then -- style interpolation
            assert(styleFeature:size(1) == #styleInterpWeights, '-styleInterpWeights and -style must have the same number of elements')
            targetFeature = contentFeature:clone():zero()
            for i=1,styleFeature:size(1) do
                targetFeature:add(styleInterpWeights[i], adain:forward({contentFeature, styleFeature[i]}))
            end
        else
            targetFeature = adain:forward({contentFeature, styleFeature})
        end

        targetFeature = targetFeature:squeeze()
        targetFeature = opt.alpha * targetFeature + (1 - opt.alpha) * contentFeature
        return decoder:forward(targetFeature)
    end

    print('Creating save folder at ' .. opt.outputDir)
    paths.mkdir(opt.outputDir)

    if opt.mask ~= '' then
        mask = image.load(opt.mask, 1, 'float') -- binary mask
    end

    local contentPaths = {}
    local stylePaths = {}

    table.insert(contentPaths, opt.content)

    style_image_list = opt.style:split(',')
    if #style_image_list == 1 then
        style_image_list = style_image_list[1]
    end
    table.insert(stylePaths, style_image_list)

    if opt.styleInterpWeights ~= '' then
        styleInterpWeights = opt.styleInterpWeights:split(',')
        local styleInterpWeightsSum = torch.Tensor(styleInterpWeights):sum()
        for i=1,#styleInterpWeights do -- normalize weights so they sum to 1
            styleInterpWeights[i] = styleInterpWeights[i] / styleInterpWeightsSum
        end
    end

    local numContent = #contentPaths
    local numStyle = #stylePaths
    print("# Content images: " .. numContent)
    print("# Style images: " .. numStyle)

    for i=1,numContent do
        local contentPath = contentPaths[i]
        local contentExt = paths.extname(contentPath)
        local contentImg = image.load(contentPath, 3, 'float')
        local contentName = paths.basename(contentPath, contentExt)
        local contentImg = sizePreprocess(contentImg, opt.crop, opt.contentSize)

        for j=1,numStyle do -- generate a transferred image for each (content, style) pair
            local stylePath = stylePaths[j]
            if type(stylePath) == "table" then -- style blending
                styleImg = {}
                styleName = ''
                for s=1,#stylePath do
                    local style = image.load(stylePath[s], 3, 'float')
                    styleExt = paths.extname(stylePath[s])
                    styleName = styleName .. '_' .. paths.basename(stylePath[s], styleExt)
                    style = sizePreprocess(style, opt.crop, opt.styleSize)
                    if opt.preserveColor then
                        style = coral(style, contentImg)
                    end
                    style = style:add_dummy()
                    table.insert(styleImg, style)
                end
                styleImg = torch.cat(styleImg, 1)
                styleName = styleName:sub(2)
            else
                styleExt = paths.extname(stylePath)
                styleImg = image.load(stylePath, 3, 'float')
                styleImg = sizePreprocess(styleImg, opt.crop, opt.styleSize)
                if opt.preserveColor then
                    styleImg = coral(styleImg, contentImg)
                end
                styleName = paths.basename(stylePath, styleExt)
            end

            local output = styleTransfer(contentImg, styleImg)

            local savePath = paths.concat(opt.outputDir, contentName .. '_stylized_' .. styleName .. '.' .. opt.saveExt)
            print('Output image saved at: ' .. savePath)
            image.save(savePath, output)

            if opt.saveOriginal then
                -- also save the original images
                image.save(paths.concat(opt.outputDir, contentName .. '.' .. contentExt), contentImg)
                image.save(paths.concat(opt.outputDir, styleName .. '.' .. styleExt), styleImg)
            end
        end
    end
end
