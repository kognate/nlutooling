//
// Created by Josh Smith on 2/18/17.
//

#include "nlu_client.h"

using json = nlohmann::json;

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

const char *BASE_URL="https://gateway-s.watsonplatform.net/natural-language-understanding/api/v1/analyze?version=2016-01-23";

namespace watson {
    nlu_client::nlu_client() {
        this->setApi_url(std::string(BASE_URL));
    }

    nlu_client::nlu_client(std::string username, std::string password) {
        this->setApi_url(std::string(BASE_URL));
        this->setUsername(username);
        this->setPassword(password);
    }

    void nlu_client::setUrl(const std::string &url) {
        setQueryKind(Url);
        nlu_client::url = url;
    }

    const std::string &nlu_client::getUrl() const {
        return url;
    }

    const std::string &nlu_client::getHtml() const {
        return html;
    }

    void nlu_client::setHtml(const std::string &html) {
        setQueryKind(Html);
        nlu_client::html = html;
    }

    const std::string &nlu_client::getText() const {
        return text;
    }

    void nlu_client::setText(const std::string &text) {
        setQueryKind(Text);
        nlu_client::text = text;
    }

    nlu_client::~nlu_client() {

    }

    const std::string &nlu_client::getApi_url() const {
        return api_url;
    }

    void nlu_client::setApi_url(const std::string &api_url) {
        nlu_client::api_url = api_url;
    }

    ApiResponse nlu_client::api_analyze(long timeout, std::ostream *output_stream)
    {
        ApiResponse response;
        CURLcode code(CURLE_FAILED_INIT);
        CURL* curl = curl_easy_init();
        const char *authCString = getAuthString();

        json payload = preparePayload();

        if(curl) {
            struct curl_slist *chunk = NULL;
            chunk = curl_slist_append(chunk, "Content-Type: application/json; charset=utf-8");

            std::ostringstream payload_out;
            payload_out << payload.dump();
            std::string payload_str = payload_out.str();
            const char* payload_cstr = payload_str.c_str();

            if (verbose) {
                curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
                std::cerr << payload_cstr << std::endl;
            }

            if(CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &data_write))
               && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, output_stream))
               && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L))
               && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_USERPWD, authCString))
               && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L))
               && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk))
               && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload_cstr))
               && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, payload_str.length()))
               && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout))
               && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_URL, this->getApi_url().c_str()))) {
                code = curl_easy_perform(curl);
            }

            long http_code = 0;
            curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);

            curl_easy_cleanup(curl);
            curl_slist_free_all(chunk);
            free((void *) authCString);
            response.statusCode = http_code;
        }
        response.code = code;

        return response;
    }

    char *nlu_client::getAuthString() {
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

    json nlu_client::preparePayload() {
        json payload;
        payload = updateQueryForKind(payload);
        json features;
        features = buildFeatures(features);
        payload["features"] = features;
        return payload;
    }

    json &nlu_client::buildFeatures(json &features) {
        for (auto f : this->getFeatures()) {
            switch (f) {
                case Concepts:
                    features["concepts"] = json::object();
                    break;
                case Keywords:
                    features["keywords"] = json::object();
                    break;
                case Entities:
                    features["entities"] = json::object();
                    break;
                case Relations:
                    features["relations"] = json::object();
                    break;
                case SemanticRoles:
                    features["semantic_roles"] = json::object();
                    break;
                case Categories:
                    features["categories"] = json::object();
                    break;
                case Emotion:
                    features["emotion"] = json::object();
                    break;
                case Sentiment:
                    features["sentiment"] = json::object();
                    break;
                default:
                    break;
            }
        }
        return features;
    }

    json &nlu_client::updateQueryForKind(json &payload)  {
        switch (getQueryKind()) {
            case Html:
                payload["html"] = this->getHtml();
                break;
            case Text:
                payload["text"] = this->getText();
                break;
            default:
                payload["url"] = this->getUrl();
        }
        return payload;
    }

    const std::string &nlu_client::getUsername() const {
        return username;
    }

    void nlu_client::setUsername(const std::string &username) {
        nlu_client::username = username;
    }

    const std::string &nlu_client::getPassword() const {
        return password;
    }

    void nlu_client::setPassword(const std::string &password) {
        nlu_client::password = password;
    }

    QueryKind nlu_client::getQueryKind() const {
        return queryKind;
    }

    void nlu_client::setQueryKind(QueryKind queryKind) {
        nlu_client::queryKind = queryKind;
    }

    const std::set<Features> &nlu_client::getFeatures() const {
        return features;
    }

    void nlu_client::setFeatures(const std::set<Features> &features) {
        nlu_client::features = features;
    }

    json nlu_client::analyze() {
        std::ostringstream buffer;
        ApiResponse res = this->api_analyze(30, &buffer);
        if (res.code == CURLE_OK && res.statusCode == 200) {
            return json::parse(buffer.str());
        } else {
            json results = json::object();
            results["code"] = res.statusCode;
            results["body"] = buffer.str();
            return results;
        }
    }

    void nlu_client::setVerbose(bool verbose) {
        nlu_client::verbose = verbose;
    }
}

