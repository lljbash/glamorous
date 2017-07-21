#include<iostream>
using namespace std;

#include <lua.hpp>

#define my_lua_dofile(L, filename) (luaL_loadfile((L), (filename)) || lua_pcall((L), 0, LUA_MULTRET, 0))

lua_State *GlobalL;

int main()
{
    cout<<"Start!"<<endl;
    GlobalL = luaL_newstate();
    int x = 10;

    lua_gc(GlobalL, LUA_GCSTOP, 0);
    luaL_openlibs(GlobalL);
    lua_gc(GlobalL, LUA_GCRESTART, 0);

    //luaL_dofile(GlobalL, "luafun.lua");//执行lua文件

    // int loaderr = luaL_loadfile(GlobalL, "test2.lua");
    // cout<<"loaderr="<<loaderr<<endl;
    // lua_pcall(GlobalL, 0, LUA_MULTRET, 0);
    // my_lua_dofile(GlobalL, "test2.lua");//代替上面三行

    // lua_getfield(GlobalL, LUA_GLOBALSINDEX, "fact");
    lua_getglobal(GlobalL, "fact");//代替上面一行

    lua_pushnumber(GlobalL, x);//参数
    int callerr = lua_pcall(GlobalL, 1, 0, 0);
    cout<<"callerr="<<callerr<<endl;
    cout<<lua_tostring(GlobalL, -1)<<endl;
    lua_close(GlobalL);
    return 0;
}
