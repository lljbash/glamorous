#include "database_match_component.h"

#include <algorithm>

#include "component/component_factory.h"
#include "log.h"

using namespace glamorous;

DatabaseMatchComponentFactory::DatabaseMatchComponentFactory(std::string root_dir)
    : root_dir_(root_dir) {}

ComponentPointer DatabaseMatchComponentFactory::create(std::string name) {
    return std::make_shared<DatabaseMatchComponent>(root_dir_, name);
}

DatabaseMatchComponent::DatabaseMatchComponent(std::string root_dir, std::string name)
    : Component(name), root_dir_(root_dir) {}

void DatabaseMatchComponent::process(RequestStatusPointer request) {
    RequestStatus::RequestType type = request->request_type;
    std::string filename;
    switch (type) {
        case RequestStatus::RequestType::Abstract:
            filename = "abstract.tsv";
            break;
        case RequestStatus::RequestType::Impressionism:
            filename = "impressionism.tsv";
            break;
        case RequestStatus::RequestType::Pointillism:
            filename = "pointillism.tsv";
            break;
        case RequestStatus::RequestType::PostImpressionism:
            filename = "post_impressionism.tsv";
            break;
        case RequestStatus::RequestType::Shuimo:
            filename = "shuimo.tsv";
            break;
        case RequestStatus::RequestType::Suprematism:
            filename = "suprematism.tsv";
            break;
        default:
            Log::error("Invalid Request Type");
    }
    request->image_db = std::make_shared<ImageDataBase>();
    request->image_db->load_tsv(root_dir_, filename);
}
