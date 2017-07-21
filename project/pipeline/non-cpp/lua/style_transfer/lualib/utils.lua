require 'nn'
require 'unsup'
require 'lfs'


local function matSqrt(x)
    local U,D,V = torch.svd(x)
    local result = U*(D:pow(0.5):diag())*V:t()
    return result
end


-- Prepares an RGB image in [0,1] for VGG
function getPreprocessConv()
    local mean_pixel = torch.Tensor({103.939, 116.779, 123.68})
    local conv = nn.SpatialConvolution(3,3, 1,1)
    conv.weight:zero()
    conv.weight[{1,3}] = 255
    conv.weight[{2,2}] = 255
    conv.weight[{3,1}] = 255
    conv.bias = -mean_pixel
    conv.gradBias = nil
    conv.gradWeight = nil
    conv.parameters = function() --[[nop]] end
    conv.accGradParameters = function() --[[nop]] end
    return conv
end

function extractImageNamesRecursive(dir)
    local files = {}
    print("Extracting image paths: " .. dir)
  
    local function browseFolder(root, pathTable)
        for entity in lfs.dir(root) do
            if entity~="." and entity~=".." then
                local fullPath=root..'/'..entity
                local mode=lfs.attributes(fullPath,"mode")
                if mode=="file" then
                    local filepath = paths.concat(root, entity)
                    if string.find(filepath, 'jpg$')
                    or string.find(filepath, 'png$')
                    or string.find(filepath, 'jpeg$')
                    or string.find(filepath, 'JPEG$')
                    or string.find(filepath, 'ppm$') then
                        table.insert(pathTable, filepath)
                    end
                elseif mode=="directory" then
                    browseFolder(fullPath, pathTable);
                end
            end
        end
    end

    browseFolder(dir, files)
    return files
end

 -- correlation alignment
function coral(source, target)
    assert(source:size(1) == 3)
    assert(target:size(1) == 3)
    local H, W = source:size(2), source:size(3)
    local source_flatten = source:view(3, -1)
    local target_flatten = target:view(3, -1)
    local source_flatten_mean = source_flatten:mean(2)
    local source_flatten_std = source_flatten:std(2)
    local source_flatten_norm = torch.cdiv(source_flatten - source_flatten_mean:expandAs(source_flatten), 
                                source_flatten_std:expandAs(source_flatten))
    local target_flatten_mean = target_flatten:mean(2)
    local target_flatten_std = target_flatten:std(2)
    local target_flatten_norm = torch.cdiv(target_flatten - target_flatten_mean:expandAs(target_flatten), 
                                target_flatten_std:expandAs(target_flatten))
    local source_flatten_cov_eye = source_flatten_norm * source_flatten_norm:t() + torch.eye(3):float()
    local target_flatten_cov_eye = target_flatten_norm * target_flatten_norm:t() + torch.eye(3):float()
    local source_flatten_norm_transfer = matSqrt(target_flatten_cov_eye) * torch.inverse(matSqrt(source_flatten_cov_eye)) * source_flatten_norm
    local source_flatten_transfer = torch.cmul(source_flatten_norm_transfer, target_flatten_std:expandAs(source_flatten_norm)) +
                                     target_flatten_mean:expandAs(source_flatten_norm)
    return source_flatten_transfer:viewAs(source)
end

-- image size preprocessing
function sizePreprocess(x, crop, newSize)
    assert(x:dim() == 3)
    local minSize = torch.min(torch.Tensor({x:size(2), x:size(3)}))
    if crop then
        x = image.crop(x, 'c', minSize, minSize) -- center crop
    end
    if newSize ~= 0 then
        x = image.scale(x, '^' .. newSize)
    end
    return x
end

-- adds first dummy dimension, copied from https://github.com/DmitryUlyanov/texture_nets/blob/master/src/utils.lua
function torch.FloatTensor:add_dummy()
  local sz = self:size()
  local new_sz = torch.Tensor(sz:size()+1)
  new_sz[1] = 1
  new_sz:narrow(1,2,sz:size()):copy(torch.Tensor{sz:totable()})

  if self:isContiguous() then
    return self:view(new_sz:long():storage())
  else
    return self:reshape(new_sz:long():storage())
  end
end

-- copied from torchx: https://github.com/nicholas-leonard/torchx/blob/master/find.lua
function torch.find(tensor, val, dim)
   local i = 1
   local indice = {}
   if dim then
      assert(tensor:dim() == 2, "torch.find dim arg only supports matrices for now")
      assert(dim == 2, "torch.find only supports dim=2 for now")
            
      local colSize, rowSize = tensor:size(1), tensor:size(2)
      local rowIndice = {}
      tensor:apply(function(x)
            if x == val then
               table.insert(rowIndice, i)
            end
            if i == rowSize then
               i = 1
               table.insert(indice, rowIndice)
               rowIndice = {}
            else
               i = i + 1
            end
         end)
   else
      tensor:apply(function(x)
         if x == val then
            table.insert(indice, i)
         end
         i = i + 1
      end)
   end
   return indice
end
