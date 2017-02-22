//
// Created by Josh Smith on 2/21/17.
//

#ifndef NLUTOOLING_DISCOVERY_H
#define NLUTOOLING_DISCOVERY_H

#include <json.hpp>
#include <curl/curl.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <set>
#include "watson.h"

namespace watson {
    class discovery: public watson {

    public:

        discovery(std::string &username, const std::string &password);

        discovery(const std::string &api_key);

        nlohmann::json get_environments();
        nlohmann::json get_collections(std::string environment_id);
        nlohmann::json get_collection(std::string environment_id, std::string collection_id);
        nlohmann::json get_document(std::string environment_id, std::string collection_id, std::string document_id);
        nlohmann::json upload_document(std::string environment_id, std::string collection_id, std::string filename);
        nlohmann::json query(std::string environment_id, std::string collection_id, std::string query);

    protected:
        char *getAuthString() override;
    };
}

#endif //NLUTOOLING_DISCOVERY_H
