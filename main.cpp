#include <iostream>
using namespace std;
#include <boost/program_options.hpp>
using namespace boost::program_options;

#include <boost/filesystem.hpp>

#include <fstream>
#include <streambuf>
#include <cerrno>
#include "nlu_client.h"
#include "discovery_cli.h"
#include "vision.h"

void usage() {
    std::cout << "Commands are nlu or discovery or vision (try `watson nlu -h`)" << std::endl;
}

int main(int argc, const char **argv) {

    options_description global("Global options");
    global.add_options()
            ("command", value<std::string>()->required(), "command to execute")
            ("subargs", value<std::vector<std::string> >(), "Arguments for command");

    positional_options_description pos;
    pos.add("command", 1).
            add("subargs", -1);

    variables_map vm;
    parsed_options parsed = command_line_parser(argc, argv).
            options(global).
            positional(pos).
            allow_unregistered().
            run();

    store(parsed, vm);

    try {
        notify(vm);
    } catch (...) {
        usage();
        return 0;
    }

    std::string cmd = vm["command"].as<std::string>();

    if (cmd == std::string("nlu")) {
        watson::v1::run_nlu(parsed);
    } else if (cmd == std::string("discovery")) {
        watson::run_discovery(parsed);
    } else if (cmd == std::string("vision")) {
        watson::v3::vision * v = new watson::v3::vision(std::string("6d2ff6fd28d756d57f12140f9cef4e9103893a72"));
        v->setVerbose(true);
        std::vector<std::string> classifiers = { std::string("default"), std::string("SatelliteImagery_1875788612")};
        std::cout << v->classify_url(std::string("http://static1.businessinsider.com/image/4e8f228becad044778000029-1200/oriole-park-at-camden-yards-baltimore-orioles.jpg"), classifiers);
        //std::cout << v->classify(std::string("3.jpg"), {}) << std::endl;
        //std::cout << v->delete_classifier(std::string("testClassifier_1879811305")) << std::endl;
        //std::cout << v->list_classifiers() << std::endl;
    } else {
        usage();
        return 0;
    }

    return 0;
}
