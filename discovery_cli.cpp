#include "discovery_cli.h"

namespace watson {

    const std::string name_mapper(std::string arg) {
        std::map<std::string, std::string> env_mapper = { {"DISCOVERY_USERNAME", "username"},
                                                          {"DISCOVERY_PASSWORD", "password"},
                                                          {"DISCOVERY_URL", "api_url"} };
        std::map<std::string, std::string>::iterator found;
        found = env_mapper.find(arg);
        if (found == env_mapper.end()) {
            return std::string("");
        } else {
            return found->second;
        }
    }

    bool has_set_options(variables_map vm, std::vector<const char *> options) {
        int i = 0;
        for (auto item: options) {
            i += vm.count(item);
        }
        return i >= options.size();
    }

    void run_discovery(parsed_options opts_in) {
        std::string username, password, envid, collectionid, documentid, filename;
        std::string aggregations = std::string("");
        std::string query = std::string("");
        options_description desc("Allowed options");

        desc.add_options()
                ("help,h", "print usage message")
                ("username,u", value<std::string>(&username)->required(), "username")
                ("password,p", value<std::string>(&password)->required(), "password")
                ("environment,e", value<std::string>(&envid), "environment quid")
                ("collection,c", value<std::string>(&collectionid), "collection guid")
                ("file,f", value<std::string>(&filename), "document to add")
                ("query,q", value<std::string>(&query), "query")
                ("aggregations,a", value<std::string>(&aggregations), "aggregtations")
                ("verbose,v", "set verbose mode (gives connection info")
                ("document,d", value<std::string>(&documentid), "get a document");

        variables_map vm;

        std::vector<std::string> opts = collect_unrecognized(opts_in.options, include_positional);
        opts.erase(opts.begin());
        store(command_line_parser(opts).options(desc).run(), vm);
        store(parse_environment(desc, &name_mapper),vm);

        if (vm.count("help")) {
            cout << desc << "\n";
            return;
        }

        notify(vm);

        watson::discovery *disco = new watson::discovery(username, password);

        disco->setVerbose(vm.count("verbose"));

        if (has_set_options(vm, {"collection", "environment", "file"})) {
            std::cout << disco->upload_document(envid, collectionid, filename).dump(4) << std::endl;
        } else if (has_set_options(vm, {"collection", "environment", "document"})) {
            std::cout << disco->get_document(envid, collectionid, documentid).dump(4) << std::endl;
        } else if (has_set_options(vm, {"collection", "environment", "aggregations"}) ||
                has_set_options(vm, {"collection", "environment", "query"})) {
            std::cout << disco->query(envid, collectionid, query, aggregations).dump(4) << std::endl;
        } else if (has_set_options(vm, {"collection", "environment"})) {
            std::cout << disco->get_collection(envid, collectionid).dump(4) << std::endl;
        } else if (has_set_options(vm, {"environment"})) {
            std::cout << disco->get_collections(envid).dump(4) << std::endl;
        } else {
            std::cout << disco->get_environments().dump(4) << std::endl;
        }

        return;
    }

}