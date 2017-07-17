#include "style_trans.h"

#include <lua.hpp>

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

void StyleTrans::initialize(string &contentName, string &styleName) {
    contentName_ = contentName;
    styleName_ = styleName;
    content_ = imread(contentName_);
    style_ = imread(styleName_);
}

void StyleTrans::initialize(const Mat &content, const Mat &style) {
    content_ = content.clone();
    style_ = style.clone();
}

int StyleTrans::apply() {
    contentName_ = prefix_ + "content.jpg";
    styleName_ = prefix_ + "style.jpg";

    imwrite(contentName_, content_);
    imwrite(styleName_, style_);

    // New lua state
    lua_State* L;
    L = luaL_newstate();
    luaL_openlibs(L);
    if (luaL_dofile(L, "testFromCpp.lua") != 0) {
        // Log::error("Loadfile Error! " + lua_tostring(L, -1));
    }
    luaL_openlibs(L);

    // Call parseOpt & runTest
    lua_getglobal(L, "parseAndRun");
    lua_pushstring(L, contentName_);
    lua_pushstring(L, styleName_);
    lua_pushinteger(L, gpu_?0:-1);
    if (lua_pcall(L, 3, 0, 0) != 0) {
        // Log::error("Call Function Error! " + lua_tostring(L, -1));
    }

    dstName_ = prefix_ + "dst.jpg";
    dst_ = imread(dstName_);
}

Mat StyleTrans::get_dst() const {
    return dst_.clone();
}
