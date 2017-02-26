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
#include <json.hpp>

#include <boost/program_options.hpp>
using namespace boost::program_options;
#include <boost/filesystem.hpp>

namespace watson {
    namespace v1 {
        enum QueryKind { Html, Url, Text };
        enum Features { Concepts, Keywords, Entities, Relations, SemanticRoles, Categories, Emotion, Sentiment };

        void run_nlu(parsed_options opts_in);

        struct ApiResponse {
            CURLcode code;
            long statusCode;
        };

        class nlu_client {

        private:
            std::string url, html, text;
            std::string username, password, api_url;
            std::vector<std::string> targeted_emotion, targeted_sentiment;
            bool return_analyzed_text;
        public:
            bool isReturn_analyzed_text() const;
            void setReturn_analyzed_text(bool return_analyzed_text);

        public:
            const std::vector<std::string> &getTargeted_emotion() const;

            void setTargeted_emotion(const std::vector<std::string> &targeted_emotion);

            const std::vector<std::string> &getTargeted_sentiment() const;

            void setTargeted_sentiment(const std::vector<std::string> &targeted_sentiment);

        public:

        private:
            const std::string &getPassword() const;
            ApiResponse api_analyze(long timeout, std::ostream *output_stream);
            std::set<Features> features;
            bool verbose = false;
        public:
            void setVerbose(bool verbose);

        private:

            nlohmann::json &updateQueryForKind(nlohmann::json &payload);

            nlohmann::json &buildFeatures(nlohmann::json &features);

            nlohmann::json preparePayload();

            char *getAuthString();

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
}


#endif //NLUTOOLING_NLU_CLIENT_H
