//
// Created by Josh Smith on 2/21/17.
//

#ifndef NLUTOOLING_WATSON_H
#define NLUTOOLING_WATSON_H

#include <json.hpp>
#include <curl/curl.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <set>
#include "post_parameter.h"

namespace watson {
    class BaseWatsonService {
    private:
        std::string username, password, api_key, api_url, version;
    public:
        const std::string &getVersion() const;

        void setVersion(const std::string &version);

    private:
        static size_t data_write(void* buf, size_t size, size_t nmemb, void* userp);
        std::vector<std::shared_ptr<post_parameter>> post_parameters = std::vector<std::shared_ptr<watson::post_parameter>>();
        bool verbose = {false};

        void getStatusCode(CURL *curl);

    public:
        bool isVerbose() const;

        void setVerbose(bool verbose);

    public:
        const std::string &getUsername() const;

        void setUsername(const std::string &username);

        const std::string &getPassword() const;

        void setPassword(const std::string &password);

        const std::string &getApi_key() const;

        void setApi_key(const std::string &api_key);

        const std::string &getApi_url() const;

        void setApi_url(const std::string &api_url);

    protected:
    public:
        BaseWatsonService(std::string &username, std::string password);
        BaseWatsonService(std::string api_key);

    protected:
        virtual char *getAuthString()= 0;
        CURLcode do_get(long timeout,  std::string url, std::ostream *output_stream);

        void addPostParameter(std::shared_ptr<post_parameter> param);
        std::vector<std::shared_ptr<post_parameter>> getPostParameters();

        CURLcode do_post(long timeout,
                         std::string url,
                         std::ostream *output_stream);
    };
}


#endif //NLUTOOLING_WATSON_H
