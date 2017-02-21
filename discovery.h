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

namespace watson {
    class discovery {

    private:
        std::string username, password, api_url;
        void setApi_url(const std::string &api_url);

    public:
        const std::string &getUsername() const;

        void setUsername(const std::string &username);

        const std::string &getPassword() const;

        void setPassword(const std::string &password);

        const std::string &getApi_url() const;

        discovery(std::string username, std::string password);
        nlohmann::json get_environments();
        nlohmann::json get_collections(std::string environment_id);
        nlohmann::json get_collection(std::string environment_id, std::string collection_id);
        nlohmann::json get_document(std::string environment_id, std::string collection_id, std::string document_id);
        nlohmann::json query(std::string environment_id, std::string collection_id, std::string query);

    private:
        char *getAuthString();
        CURLcode do_get(long timeout,  std::string url, std::ostream *output_stream);

    public:
        nlohmann::json list_environments();
    };
}

#endif //NLUTOOLING_DISCOVERY_H
