//
// Created by Josh Smith on 2/21/17.
//

#include <curl/curl.h>
#include "discovery.h"

using json = nlohmann::json;

namespace watson {

    discovery::discovery(std::string &username, const std::string &password) : BaseWatsonService(username, password) {
        setApi_url("https://gateway.watsonplatform.net/discovery/api");
    }

    char *discovery::getAuthString() {
        std::ostringstream auth;
        auth << getUsername() << ":" << getPassword();
        char* authCString;
        std::string authString = auth.str();
        int len = (int) (1 + authString.length());
        authCString = (char *)malloc(sizeof(char) * len);
        bzero(authCString, len);
        std::copy(authString.begin(), authString.end(), authCString);
        return authCString;
    }

    json discovery::get_collections(std::string environment_id) {
        std::string url = getApi_url() + "/v1/environments/" + environment_id + "/collections?version=2016-12-29";
        std::ostringstream buffer;
        CURLcode res = do_get(30, url, &buffer);
        if (res == CURLE_OK) {
            return json::parse(buffer.str());
        } else {
            json results = json::object();
            results["code"] = curl_easy_strerror(res);
            results["body"] = "Error!";
            return results;
        }
    }

    json discovery::upload_document(std::string environment_id, std::string collection_id, std::string filename) {
        std::string url = getApi_url() + "/v1/environments/" + environment_id + "/collections/" + collection_id + "/documents?version=2016-12-29";
        std::ostringstream buffer;
        std::shared_ptr<file_post_parameter> fpp = std::make_shared<file_post_parameter>(std::string("file"), filename);
        addPostParameter(fpp);
        CURLcode res = do_post(30, url, &buffer);
        if (res == CURLE_OK) {
            return json::parse(buffer.str());
        } else {
            json results = json::object();
            results["code"] = res;
            results["body"] = "Error!";
            return results;
        }
    }

    json discovery::get_collection(std::string environment_id, std::string collection_id) {
        std::string url = getApi_url() + "/v1/environments/" + environment_id + "/collections/" + collection_id + "?version=2016-12-29";
        std::ostringstream buffer;
        CURLcode res = do_get(30, url, &buffer);
        if (res == CURLE_OK) {
            return json::parse(buffer.str());
        } else {
            json results = json::object();
            results["code"] = curl_easy_strerror(res);
            results["body"] = "Error!";
            return results;
        }
    }

    json discovery::get_document(std::string environment_id, std::string collection_id, std::string document_id) {
        std::string url = getApi_url() + "/v1/environments/" + environment_id + "/collections/" + collection_id + "/documents/" + document_id + "?version=2016-12-29";
        std::ostringstream buffer;
        CURLcode res = do_get(30, url, &buffer);
        if (res == CURLE_OK) {
            return json::parse(buffer.str());
        } else {
            json results = json::object();
            results["code"] = res;
            results["body"] = "Error!";
            return results;
        }
    }

    json discovery::query(std::string environment_id, std::string collection_id, std::string query, std::string aggregations) {
        std::string url = getApi_url() + "/v1/environments/" + environment_id + "/collections/" + collection_id + "/query?version=2016-12-29&" + query;
        if (aggregations.size() > 0) {
            url = url + "&" + aggregations;
        }
        std::ostringstream buffer;
        CURLcode res = do_get(30, url, &buffer);
        if (res == CURLE_OK) {
            return json::parse(buffer.str());
        } else {
            json results = json::object();
            results["code"] = res;
            results["body"] = "Error!";
            return results;
        }
    }

    json discovery::get_environments() {
        std::ostringstream buffer;
        std::string url = getApi_url() + "/v1/environments?version=2016-12-29";
        CURLcode res = do_get(30, url, &buffer);
        if (res == CURLE_OK) {
            return json::parse(buffer.str());
        } else {
            json results = json::object();
            results["code"] = res;
            results["body"] = "Error!";
            return results;
        }
    }
}
