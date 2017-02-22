#include <iostream>
using namespace std;
#include <boost/program_options.hpp>
using namespace boost::program_options;

#include <boost/filesystem.hpp>

#include <fstream>
#include <streambuf>
#include <cerrno>
#include "nlu_client.h"

std::string get_contents(std::string *filename) {
    try {
        if (boost::filesystem::exists(*filename)) {
            std::ifstream in(filename->c_str(), std::ios::in | std::ios::binary);
            if (in) {
                return (std::string((std::istreambuf_iterator<char>(in)),
                                    std::istreambuf_iterator<char>()));
            }
            throw (errno);
        } else {
            return *filename;
        }
    } catch (const boost::filesystem::filesystem_error &e) {
        return *filename;
    }
}

void usage() {
    std::cout << "only command supported is nlu" << std::endl;
}

void run_nlu(parsed_options opts_in) {
    variables_map vm;

    options_description desc("Allowed options");

    desc.add_options()
            ("help,h", "print usage message")
            ("username,u", value<std::string>()->required(), "username (required)")
            ("password,p", value<std::string>()->required(), "password (required)")
            ("text,t", value<std::string>(), "text or file name containing text to analyze")
            ("html,l", value<std::string>(), "html string or file name containing html to analyze")
            ("url,r", value<std::string>(), "string url to fetch and analyze")
            ;

    std::vector<std::string> opts = collect_unrecognized(opts_in.options, include_positional);
    opts.erase(opts.begin());
    store(command_line_parser(opts).options(desc).run(), vm);

    if (vm.count("help")) {
        cout << desc << "\n";
        return;
    }

    try {
        notify(vm);
    } catch (...) {
        std::cout << desc << std::endl;
        return;
    }

    watson::nlu_client *client;
    std:string username, password;
    username = vm["username"].as<std::string>();
    password = vm["password"].as<std::string>();
    client = new watson::nlu_client(username, password);

    if (vm.count("verbose")) {
        client->setVerbose(true);
    }

    if (vm.count("text")) {
        std::string text = vm["text"].as<std::string>();
        client->setText(get_contents(&text));
    }

    if (vm.count("html")) {
        std::string html = vm["html"].as<std::string>();
        client->setHtml(get_contents(&html));
    }

    if (vm.count("url")) {
        std::string url = vm["url"].as<std::string>();
        client->setUrl(url);
    }

    std::set<watson::Features> features = { watson::Emotion, watson::Sentiment, watson::Concepts, watson::Keywords, watson::Entities };
    client->setFeatures(features);
    std::cout << client->analyze().dump(4) << std::endl;
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
        run_nlu(parsed);
    } else if (cmd == std::string("help")) {
        usage();
        return 0;
    }

    return 0;
}
