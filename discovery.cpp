//
// Created by Josh Smith on 2/21/17.
//

#include <curl/curl.h>
#include "discovery.h"

using json = nlohmann::json;

namespace watson {

    static size_t data_write(void* buf, size_t size, size_t nmemb, void* userp)
    {
        if(userp)
        {
            std::ostream& os = *static_cast<std::ostream*>(userp);
            size_t len = size * nmemb;
            if(os.write(static_cast<char*>(buf), len)) {
                os.flush();
                return len;
            }
        }

        return 0;
    }

    discovery::discovery(std::string &username, const std::string &password) : watson(username, password) {
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
            results["code"] = res;
            results["body"] = "Error!";
            return results;
        }
    }

    json discovery::upload_document(std::string environment_id, std::string collection_id, std::string filename) {
        std::string url = getApi_url() + "/v1/environments/" + environment_id + "/collections/" + collection_id + "/documents?version=2016-12-29";
        std::ostringstream buffer;
        CURLcode res = do_post(30, url, filename, &buffer);
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
            results["code"] = res;
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

    json discovery::query(std::string environment_id, std::string collection_id, std::string query) {
        std::string url = getApi_url() + "/v1/environments/" + environment_id + "/collections/" + collection_id + "/query?version=2016-12-29&" + query;
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
