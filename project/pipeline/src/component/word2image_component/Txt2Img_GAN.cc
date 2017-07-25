//
//  Txt2Img_GAN.cpp
//  saliency
//
//  Created by 朱祺 on 2017/7/17.
//  Copyright © 2017年 朱祺. All rights reserved.
//

#include "Txt2Img_GAN.h"


Txt2Img_GAN::Txt2Img_GAN(std::string text, std::string dataset){
    this->text = text;
    this->dataset = dataset;
    assert(dataset=="coco"||dataset=="flower"||dataset=="cub");
    this->net_txt = this->root_dir + "model/" + dataset + "_net_txt.t7";
    this->net_gen = this->root_dir + "model/" + dataset + "_net_gen.t7"; 
}

Txt2Img_GAN::~Txt2Img_GAN(){
}

cv::Mat Txt2Img_GAN::generate(){
    // write text into file for queries
    std::ofstream f(queries.c_str());
    f<<text.c_str()<<std::endl;
    f.close();
    
    //
    int queries_num = txt2img(net_gen.c_str(), net_txt.c_str(), queries.c_str(), dataset.c_str(), output_dir.c_str());
    if (queries_num==-1) {
        printf("FAILED !");
    }
//    std::cout<<(output_dir+"/img_1.png")<<std::endl;
    cv::Mat pic = cv::imread(output_dir+"/"+dataset+"/img_1.png");
    cv::Mat first(64,64,CV_8UC3);
    for (int i = 0; i<first.rows; i++) {
        for (int j = 0; j<first.cols; j++) {
            first.at<cv::Vec3b>(i,j) = pic.at<cv::Vec3b>(i+offset_x_pix,j+offset_y_pix);
        }
    }
    return first;
}

int Txt2Img_GAN::txt2img(const char net_gen[],
            const char net_txt[],
            const char queries[],
            const char dataset[],
            const char output_dir[])
{
    lua_State* L = lua_open();
    luaL_openlibs(L);
    
    // lua file
    luaL_dofile(L, lua_path.c_str());
    
    //the function name
    lua_getglobal(L,"wrapper");
    
    // push args
    lua_pushstring(L,net_gen);
    lua_pushstring(L,net_txt);
    lua_pushstring(L,queries);
    lua_pushstring(L,dataset);
    lua_pushstring(L,output_dir);
    
    int queries_num = -1;
    
    if(lua_pcall(L, 5, 1, 0)!=0){
        printf("ERROR:%s\n", lua_tostring(L, -1));
    }else{
        queries_num = lua_tonumber(L, -1);
        lua_pop(L, -1);
    }
    lua_close(L);
    return queries_num;
}
