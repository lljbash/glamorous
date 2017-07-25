//
//  Txt2Img_GAN.hpp
//  saliency
//
//  Created by 朱祺 on 2017/7/17.
//  Copyright © 2017年 朱祺. All rights reserved.
//

#ifndef Txt2Img_GAN_hpp
#define Txt2Img_GAN_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <string>
#include <assert.h>
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "luajit.h"
};

using std::string;
class Txt2Img_GAN{
public:
    string root_dir = "icml2016/";  // TODO: to icml2016 dir
    string net_gen;
    string net_txt;
    string dataset;
    string queries = root_dir + "scripts/queries.txt";
    string lua_path = root_dir + "txt2img_demo.lua";
    string output_dir = root_dir + "result";
    
    const int offset_x_pix = 2;
    const int offset_y_pix = 2;
    
    string text;
    
    int txt2img(const char net_gen[],
                         const char net_txt[],
                         const char queries[],
                         const char dataset[],
                const char output_dir[]);

    Txt2Img_GAN(std::string text, std::string dataset);
    ~Txt2Img_GAN();
    
    cv::Mat generate();
};

#endif /* Txt2Img_GAN_hpp */
