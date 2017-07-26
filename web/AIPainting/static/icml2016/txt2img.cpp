extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "luajit.h"
};

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int txt2img(const char net_gen[],
	const char net_txt[],
	const char queries[],
	const char dataset[],
	const char output_dir[])
{
	// printf("%s\n", net_gen);
	lua_State* L = lua_open();                    //load Lua base libraries
        luaL_openlibs(L);                  //load the script
        luaL_dofile(L, "txt2img_demo.lua");         //call the add function       
        
    lua_getglobal(L,"wrapper");         //the function name           
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
    }          //call 2 arguments, return 1 result.         
    // sum = (float)lua_tonumber(L, -1);//get the result.        
        // lua_pop(L,-1);                   //cleanup the return
        // return sum;
    lua_close(L);
    return queries_num;
}

int main(int argc, char *argv[])
{    
    
    int queries_num = txt2img(
    	"coco_fast_t70_nc3_nt128_nz100_bs64_cls0.5_ngf196_ndf196_100_net_G.t7",
    	"coco_gru18_bs64_cls0.5_ngf128_ndf128_a10_c512_80_net_T.t7",
    	"scripts/coco_queries.txt",
    	"coco",
    	"results2"); 
    printf("total txt: %d\n", queries_num);
    return 0;
}
