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
    } else {
        usage();
        return 0;
    }

    return 0;
}
