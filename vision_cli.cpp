//
// Created by Josh Smith on 3/2/17.
//

#include "vision_cli.h"

watson::vision_cli::vision_cli() {
    setEnvironment_map({{"VISUAL_RECOGNITION_API_KEY", "api_key"}});
}

options_description watson::vision_cli::getOptions() {
    options_description desc("Allowed options");
    desc.add_options()
            ("help,h", "print usage message")
            ("api_key,k", value<string>()->required(), "Alchemy API key")
            ("url,u", value<string>(), "url to fetch and classify")
            ("file,f", value<string>(), "file to post and classify")
            ("verbose,v", "set verbose mode (gives connection info");
    return desc;
}

void watson::vision_cli::performActions(const variables_map &vm) {
    watson::v3::vision *v = new watson::v3::vision(vm["api_key"].as<string>());
    v->setVerbose(vm.count("verbose") > 0);
    auto runurl = [&v, &vm]() {
        cout << v->classify_url(vm["url"].as<string>(), {}) << endl;
        return true;
    };

    auto runfile = [&v, &vm]() {
        cout << v->classify(vm["file"].as<string>(), {}) << endl;
        return true;
    };
    this->has_set_options(vm, {"url"}) && runurl();
    this->has_set_options(vm, {"file"}) && runfile();
    delete(v);
}
