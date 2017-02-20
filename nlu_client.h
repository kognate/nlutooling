//
// Created by Josh Smith on 2/18/17.
//

#ifndef NLUTOOLING_NLU_CLIENT_H
#define NLUTOOLING_NLU_CLIENT_H

#include <curl/curl.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <set>
#include "json.hpp"

namespace watson {

    enum QueryKind { Html, Url, Text };
    enum Features { Concepts, Keywords, Entities, Relations, SemanticRoles, Categories, Emotion, Sentiment };

    class nlu_client {

    private:
        std::string url, html, text;
        std::string username, password, api_url;
        const std::string &getPassword() const;
        CURLcode curl_read(long timeout, std::ostream *output_stream);
        std::set<Features> features;
    public:
        const std::set<Features> &getFeatures() const;

        void setFeatures(const std::set<Features> &features);

    protected:
        QueryKind queryKind;
        QueryKind getQueryKind() const;
        void setQueryKind(QueryKind queryKind);

    public:
        const std::string &getUsername() const;

        void setUsername(const std::string &username);

        void setPassword(const std::string &password);

        const std::string &getApi_url() const;

        void setApi_url(const std::string &api_url);

        nlu_client(std::string username, std::string password);

        const std::string &getUrl() const;

        const std::string &getHtml() const;

        void setHtml(const std::string &html);

        const std::string &getText() const;

        void setText(const std::string &text);

        nlu_client();
        ~nlu_client();

        void setUrl(const std::string &url);

        nlohmann::json analyze();
    };
}


#endif //NLUTOOLING_NLU_CLIENT_H
