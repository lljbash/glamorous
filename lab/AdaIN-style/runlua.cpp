#include <cstdio>
#include <cstdlib>
#include <lua.hpp>

void bail(lua_State *L, const char *msg){
    fprintf(stderr, "\nFATAL ERROR:\n  %s: %s\n\n",
        msg, lua_tostring(L, -1));
    exit(1);
}

int main(int argc, char** argv)
{
    const char stylePic[]= "input/style/shuimo3_g.jpg";
    const char contentPic[]= "input/content/aaaaa.jpg";

    // New lua state
    lua_State* L;
    L = luaL_newstate();
    luaL_openlibs(L);
    if (luaL_dofile(L, "testFromCpp.lua") != 0) {
        bail(L, "loadfile Error");
    }
    luaL_openlibs(L);

    // Call parseOpt & runTest
    lua_getglobal(L, "parseAndRun");
    lua_pushstring(L, contentPic);
    lua_pushstring(L, stylePic);
    lua_pushinteger(L, -1);
    if (lua_pcall(L, 3, 1, 0) != 0) {
        bail(L, "parseAndRun Error");
    }

    return 0;
}
