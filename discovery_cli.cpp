#include <iostream>
using namespace std;
#include <boost/program_options.hpp>
using namespace boost::program_options;

#include <boost/filesystem.hpp>

#include <fstream>
#include <streambuf>
#include <cerrno>
#include "discovery.h"

int main(int argc, const char **argv) {
    std::string username, password, envid, collectionid, query, documentid;
    options_description desc("Allowed options");

    desc.add_options()
            ("help,h", "print usage message")
            ("username,u", value<std::string>(&username)->required(), "username")
            ("password,p", value<std::string>(&password)->required(), "password")
            ("environment,e", value<std::string>(&envid), "environment quid")
            ("collection,c", value<std::string>(&collectionid), "collection guid")
            ("query,q", value<std::string>(&query), "query")
            ("document,d", value<std::string>(&documentid), "get a document")
            ;

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {
        cout << desc << "\n";
        return 0;
    }

    notify(vm);

    watson::discovery *disco = new watson::discovery(username, password);

    if (vm.count("collection") && vm.count("environment") && vm.count("document")) {
        std::cout << disco->get_document(envid, collectionid, documentid).dump(4) << std::endl;
    } else if (vm.count("collection") && vm.count("environment") && vm.count("query")) {
        std::cout << disco->query(envid, collectionid, query).dump(4) << std::endl;
    } else if (vm.count("collection") && vm.count("environment")) {
        std::cout << disco->get_collection(envid, collectionid).dump(4) << std::endl;
    } else if (vm.count("environment")) {
        std::cout << disco->get_collections(envid).dump(4) << std::endl;
    } else {
        std::cout << disco->get_environments().dump(4) << std::endl;
    }

    return 0;
}