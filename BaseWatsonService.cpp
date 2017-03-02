//
// Created by Josh Smith on 2/21/17.
//

#include "BaseWatsonService.h"

watson::BaseWatsonService::BaseWatsonService(std::string api_key) {
    setApi_key(api_key);
}

watson::BaseWatsonService::BaseWatsonService(std::string &username, std::string password) {
    setUsername(username);
    setPassword(password);
}

const std::string &watson::BaseWatsonService::getUsername() const {
    return username;
}

void watson::BaseWatsonService::setUsername(const std::string &username) {
    BaseWatsonService::username = username;
}

const std::string &watson::BaseWatsonService::getPassword() const {
    return password;
}

void watson::BaseWatsonService::setPassword(const std::string &password) {
    BaseWatsonService::password = password;
}

const std::string &watson::BaseWatsonService::getApi_key() const {
    return api_key;
}

void watson::BaseWatsonService::setApi_key(const std::string &api_key) {
    BaseWatsonService::api_key = api_key;
}

const std::string &watson::BaseWatsonService::getApi_url() const {
    return api_url;
}

void watson::BaseWatsonService::setApi_url(const std::string &api_url) {
    BaseWatsonService::api_url = api_url;
}

size_t watson::BaseWatsonService::data_write(void *buf, size_t size, size_t nmemb, void *userp) {
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

CURLcode watson::BaseWatsonService::do_get(long timeout, std::string url, std::ostream *output_stream) {
    CURLcode code(CURLE_FAILED_INIT);
    CURL* curl = curl_easy_init();
    const char *authCString = this->getAuthString();

    if (authCString) {
        if (CURLE_OK != (code = curl_easy_setopt(curl, CURLOPT_USERPWD, authCString))) {
            return code;
        }
    }

    if (isVerbose()) {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    }

    if(curl) {
        if(CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &data_write))
           && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, output_stream))
           && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L))
           && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L))
           && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout))
           && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_URL, url.c_str()))) {
            code = curl_easy_perform(curl);
        }

       getStatusCode(curl);
        curl_easy_cleanup(curl);
        free((void *) authCString);
    }
    return code;
}

CURLcode watson::BaseWatsonService::do_post(long timeout, std::string url, std::string filename, std::ostream *output_stream) {
    std::map<std::string, std::string> filename_map = { {std::string("file"), filename } };
    return do_post(timeout, url, filename_map, output_stream);
}

CURLcode watson::BaseWatsonService::do_post(long timeout,
                                            std::string url,
                                            std::map<std::string, std::string> filename_map,
                                            std::ostream *output_stream) {
    struct curl_httppost* post = NULL;
    struct curl_httppost* last = NULL;

    for (auto param: getPostParameters()) {
        param->form_add(&post, &last);
    }

    for (auto file: filename_map) {
        curl_formadd(&post,
                     &last,
                     CURLFORM_FILENAME,
                     file.first.c_str(),
                     CURLFORM_FILE,
                     file.second.c_str(),
                     CURLFORM_END);
    }

    CURLcode code(CURLE_FAILED_INIT);
    CURL* curl = curl_easy_init();


    if (isVerbose()) {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    }


    const char *authCString = this->getAuthString();
    if (authCString) {
        if (CURLE_OK != (code = curl_easy_setopt(curl, CURLOPT_USERPWD, authCString))) {
            return code;
        }
    }

    if(curl) {
        if(CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &data_write))
           && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, output_stream))
           && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L))
           && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L))
           && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout))
           && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_HTTPPOST, post))
           && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_URL, url.c_str()))) {
            code = curl_easy_perform(curl);
        }

        getStatusCode(curl);

        curl_easy_cleanup(curl);
        curl_formfree(post);
        free((void *) authCString);
    }
    return code;
}

void watson::BaseWatsonService::getStatusCode(CURL *curl) {
    long http_code = 0;
    curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);

    if (this->isVerbose()) {
        std::cerr << "Status Code: " << http_code << std::endl;
        }
}

CURLcode watson::BaseWatsonService::do_post(long timeout, std::string url, std::ostream *output_stream) {
    return do_post(timeout, url, std::map<std::string, std::string> {}, output_stream);
}

void watson::BaseWatsonService::addPostParameter(std::shared_ptr<watson::post_parameter> param) {
    post_parameters.push_back(param);
}

std::vector<std::shared_ptr<watson::post_parameter>> watson::BaseWatsonService::getPostParameters() {
    return post_parameters;
}

bool watson::BaseWatsonService::isVerbose() const {
    return verbose;
}

void watson::BaseWatsonService::setVerbose(bool verbose) {
    BaseWatsonService::verbose = verbose;
}

const std::string &watson::BaseWatsonService::getVersion() const {
    return version;
}

void watson::BaseWatsonService::setVersion(const std::string &version) {
    BaseWatsonService::version = version;
}
