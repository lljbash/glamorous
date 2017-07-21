#include "style_trans.h"

#include <cstdio>
#include <lua.hpp>
#include "log.h"

using namespace glamorous;
using namespace std;
using namespace cv;

StyleTrans::StyleTrans(string prefix)
    : prefix_(prefix), gpu_(false) {

}

StyleTrans::~StyleTrans() {

}

void StyleTrans::setPrefix(string prefix) {
    prefix_ = prefix;
}

void StyleTrans::setGpu(bool gpu) {
    gpu_ = gpu;
}

void StyleTrans::initialize(const string &contentName, const string &styleName) {
    contentName_ = contentName;
    styleName_ = styleName;
    content_ = imread(contentName_);
    style_ = imread(styleName_);
    // resize(content_, content_, {128, 128});
    // resize(style_, style_, {128, 128});
}

void StyleTrans::initialize(const Mat &content, const Mat &style) {
    content_ = content.clone();
    style_ = style.clone();
}

int StyleTrans::apply() {
    contentName_ = prefix_ + "-content.jpg";
    styleName_ = prefix_ + "-style.jpg";

    imwrite(contentName_, content_);
    imwrite(styleName_, style_);

    // New lua state
    lua_State* L;
    L = luaL_newstate();
    luaL_openlibs(L);
    if (luaL_dofile(L, "lua-style/testFromCpp.lua") != 0) {
        Log::error("Loadfile Error! %s", lua_tostring(L, -1));
    }
    luaL_openlibs(L);

    // Call parseOpt & runTest
    lua_getglobal(L, "parseAndRun");
    lua_pushstring(L, prefix_.c_str());
    lua_pushinteger(L, gpu_?0:-1);
    if (lua_pcall(L, 2, 0, 0) != 0) {
        Log::error("Call Function Error! %s", lua_tostring(L, -1));
        return -1;
    }

    dstName_ = prefix_ + "-dst.jpg";
    dst_ = imread(dstName_);
    return 0;
}

Mat StyleTrans::get_dst() const {
    return dst_.clone();
}
