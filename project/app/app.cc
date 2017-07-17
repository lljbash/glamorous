#include "app.h"

#include <vector>
#include <random>
#include <functional>
#include <algorithm>

#include <opencv2/opencv.hpp>

#include "component/component_factory.h"
#include "log.h"

using namespace glamorous;

typedef std::vector<char> char_array;

char_array charset() {
    return char_array({
        '0', '1', '2', '3', '4',
        '5', '6', '7', '8', '9',
        'A', 'B', 'C', 'D', 'E', 'F',
        'G', 'H', 'I', 'J', 'K',
        'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U',
        'V', 'W', 'X', 'Y', 'Z',
        'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k',
        'l', 'm', 'n', 'o', 'p',
        'q', 'r', 's', 't', 'u',
        'v', 'w', 'x', 'y', 'z'
    });
}

// given a function that generates a random character,
// return a string of the requested length
std::string random_string(size_t length, std::function<char(void)> rand_char) {
    std::string str(length,0);
    std::generate_n(str.begin(), length, rand_char);
    return str;
}

std::string random_filename() {
    const auto ch_set = charset();
    std::default_random_engine rng(std::random_device{}());
    std::uniform_int_distribution<> dist(0, ch_set.size() - 1);
    auto randchar = [ch_set, &dist, &rng]() { return ch_set[dist(rng)]; };
    auto length = 32;
    return random_string(length, randchar);
}

void GlamorousApp::initialize(const char *db_path) {
    Log::info("Initializing...");
    
    ColorTransferComponentFactory ctcf(0.8);
    ComponentPointer cp_hsv = ctcf.create();
    MeanContrastTransferComponentFactory mctcf;
    ComponentPointer cp_mc = mctcf.create();
    OilpaintTransferComponentFactory otcf;
    ComponentPointer cp_oil = otcf.create();
    ColorAttributeExtractorComponentFactory caecf;
    ComponentPointer cp_ce = caecf.create();
    SimilarPic5colorComponentFactory sp5cf;
    ComponentPointer cp_5c = sp5cf.create();
    DatabaseMatchComponentFactory dmcf(db_path);
    ComponentPointer cp_dm = dmcf.create();
    InkTransferComponentFactory itcf;
    ComponentPointer cp_ink = itcf.create();
    IdleComponentFactory icf;
    ComponentPointer cp_idle1 = icf.create("Idle1");
    ComponentPointer cp_idle2 = icf.create("Idle2");
    
    cps_ = {cp_dm, cp_ce, cp_5c, cp_hsv, cp_mc, cp_idle1, cp_oil, cp_ink, cp_idle2};
    cp_dm->set_next_component(cp_ce);
    cp_ce->set_next_component(cp_5c);
    cp_5c->set_next_component(cp_hsv);
    cp_hsv->set_next_component(cp_mc);
    cp_mc->set_next_component(cp_idle1);
    cp_idle1->set_next_component_func([cp_ink, cp_oil](RequestStatusPointer rsp) {
        return rsp->request_type == RequestStatus::RequestType::Shuimo ? cp_ink : cp_oil;
    });
    cp_oil->set_next_component(cp_idle2);
    cp_ink->set_next_component(cp_idle2);
    
    pipeline_.set_custom_pipeline(cps_);
    Log::info("Initialization done!");
}

std::string GlamorousApp::transfer(const char *text, int type) {
    Log::info("New request %d %s", type, text);
    
    RequestStatusPointer rs = std::make_shared<RequestStatus>();
    
    rs->id = random_filename();
    rs->src_img = cv::imread("/home/lljbash/b.jpg");
    rs->res_img = rs->src_img.clone();
    
    switch (type) {
        case 1:
            rs->request_type = RequestStatus::RequestType::Abstract;
            break;
        case 2:
            rs->request_type = RequestStatus::RequestType::PostImpressionism;
            break;
        case 3:
            rs->request_type = RequestStatus::RequestType::Pointillism;
            break;
        case 4:
            rs->request_type = RequestStatus::RequestType::Shuimo;
            break;
        case 5:
            rs->request_type = RequestStatus::RequestType::Suprematism;
            break;
        case 6:
            rs->request_type = RequestStatus::RequestType::Impressionism;
            break;
        default:
            Log::error("Invalid type!");
    }
    
    auto oq = std::make_shared<Component::WaitQueue>();
    pipeline_.start_serial(oq, {rs});
    RequestStatusPointer rso;
    oq->wait_and_pop(rso);
    
    cv::imwrite(std::string(rso->id + ".jpg"), rso->res_img);
    
    return std::string(rso->id + ".jpg");
}

