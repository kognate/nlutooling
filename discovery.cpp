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

    discovery::discovery(std::string username, std::string password) {
        setUsername(username);
        setPassword(password);
        setApi_url("https://gateway.watsonplatform.net/discovery/api");
    }

    char *discovery::getAuthString() {
        std::ostringstream auth;
        auth << getUsername() << ":" << getPassword();
        char* authCString;
        std::string authString = auth.str();
        int len = 1 + authString.length();
        authCString = (char *)malloc(sizeof(char) * len);
        bzero(authCString, len);
        std::copy(authString.begin(), authString.end(), authCString);
        return authCString;
    }

    CURLcode discovery::do_get(long timeout,  std::string url, std::ostream *output_stream) {
        CURLcode code(CURLE_FAILED_INIT);
        CURL* curl = curl_easy_init();
        const char *authCString = this->getAuthString();

        if(curl) {
            if(CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &data_write))
               && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, output_stream))
               && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L))
               && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_USERPWD, authCString))
               && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L))
               && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout))
               && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_URL, url.c_str()))) {
                code = curl_easy_perform(curl);
            }

            long http_code = 0;
            curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);

            curl_easy_cleanup(curl);
            free((void *) authCString);
        }
        return code;
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

    const std::string &discovery::getUsername() const {
        return username;
    }

    void discovery::setUsername(const std::string &username) {
        discovery::username = username;
    }

    const std::string &discovery::getPassword() const {
        return password;
    }

    void discovery::setPassword(const std::string &password) {
        discovery::password = password;
    }

    const std::string &discovery::getApi_url() const {
        return api_url;
    }

    void discovery::setApi_url(const std::string &api_url) {
        discovery::api_url = api_url;
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
