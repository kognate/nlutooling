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

int main(int argc, const char **argv) {
    std::string ofile, username, password, text, html, url;


    options_description desc("Allowed options");

    desc.add_options()
            ("help,h", "print usage message")
            ("output,o", value<std::string>(&ofile), "pathname for output")
            ("username,u", value<std::string>(&username)->required(), "username")
            ("password,p", value<std::string>(&password)->required(), "password")
            ("text,t", value<std::string>(&text), "text or file name containing text to analyze")
            ("html,l", value<std::string>(&html), "html string or file name containing html to analyze")
            ("url,r", value<std::string>(&url), "string url to fetch and analyze")
            ("verbose,v", "set's verbose output")
            ;

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {
        cout << desc << "\n";
        return 0;
    }

    notify(vm);

    watson::nlu_client *client;
    client = new watson::nlu_client(username, password);

    if (vm.count("verbose")) {
        client->setVerbose(true);
    }

    if (vm.count("text")) {
        client->setText(get_contents(&text));
    }

    if (vm.count("html")) {
        client->setHtml(get_contents(&html));
    }

    if (vm.count("url")) {
        client->setUrl(url);
    }

    std::set<watson::Features> features = { watson::Emotion, watson::Sentiment, watson::Concepts, watson::Keywords };
    client->setFeatures(features);
    std::cout << client->analyze().dump(4) << std::endl;

    return 0;
}
