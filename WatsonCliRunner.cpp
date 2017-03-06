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

void watson::WatsonCliRunner::run(parsed_options opts_in) {
    using placeholders::_1;

    options_description desc = getOptions();
    variables_map vm;

    auto eparse = std::bind(&watson::WatsonCliRunner::extra_parser, this, _1);

    vector<string> opts = collect_unrecognized(opts_in.options, include_positional);
    opts.erase(opts.begin());


    store(command_line_parser(opts).options(desc).extra_parser(eparse).run(), vm);


    function<string(string)> nameMapperFunc { std::bind(&watson::WatsonCliRunner::nameMapper, this, _1) };
    store(parse_environment(desc, nameMapperFunc),vm);

    if (vm.count("help")) {
        cout << desc << "\n";
    } else {
        try {
            notify(vm);
            performActions(vm);
        } catch (const exception& e) {
            cerr << e.what() << endl;
            cout << desc << endl;
        }
    }
}

std::pair<std::string, std::string> watson::WatsonCliRunner::extra_parser(const std::string &arg) {
    return std::make_pair(std::string(), std::string());
};

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