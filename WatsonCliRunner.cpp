//
// Created by Josh Smith on 3/2/17.
//

#include "vision_cli.h"
#include "WatsonCliRunner.h"

const std::map<std::string, std::string> &watson::WatsonCliRunner::getEnvironment_map() const {
    return environment_map;
}

void watson::WatsonCliRunner::setEnvironment_map(const std::map<std::string, std::string> &environment_map) {
    WatsonCliRunner::environment_map = environment_map;
}

std::string watson::WatsonCliRunner::nameMapper(std::string arg) {
    std::map<std::string, std::string> env_mapper = getEnvironment_map();
    std::map<std::string, std::string>::iterator found;
    found = env_mapper.find(arg);
    if (found == env_mapper.end()) {
        return std::string("");
    } else {
        return found->second;
    }
}

bool watson::WatsonCliRunner::has_set_options(boost::program_options::variables_map vm, std::vector<const char *> options) {
    int i = 0;
    for (auto item: options) {
        i += vm.count(item);
    }
    return i >= options.size();
}