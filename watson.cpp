//
// Created by Josh Smith on 2/21/17.
//

#include "watson.h"

watson::watson::watson(std::string api_key) {
    setApi_key(api_key);
}

watson::watson::watson(std::string &username, std::string password) {
    setUsername(username);
    setPassword(password);
}

const std::string &watson::watson::getUsername() const {
    return username;
}

void watson::watson::setUsername(const std::string &username) {
    watson::username = username;
}

const std::string &watson::watson::getPassword() const {
    return password;
}

void watson::watson::setPassword(const std::string &password) {
    watson::password = password;
}

const std::string &watson::watson::getApi_key() const {
    return api_key;
}

void watson::watson::setApi_key(const std::string &api_key) {
    watson::api_key = api_key;
}

const std::string &watson::watson::getApi_url() const {
    return api_url;
}

void watson::watson::setApi_url(const std::string &api_url) {
    watson::api_url = api_url;
}

size_t watson::watson::data_write(void *buf, size_t size, size_t nmemb, void *userp) {
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

CURLcode watson::watson::do_get(long timeout, std::string url, std::ostream *output_stream) {
    CURLcode code(CURLE_FAILED_INIT);
    CURL* curl = curl_easy_init();
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

CURLcode watson::watson::do_post(long timeout, std::string url, std::string filename, std::ostream *output_stream) {
    struct curl_httppost* post = NULL;
    struct curl_httppost* last = NULL;

    curl_formadd(&post,
                 &last,
                 CURLFORM_COPYNAME,
                 "file",
                 CURLFORM_FILE,
                 filename.c_str(),
                 CURLFORM_END);

    CURLcode code(CURLE_FAILED_INIT);
    CURL* curl = curl_easy_init();
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

        long http_code = 0;
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);

        curl_easy_cleanup(curl);
        curl_formfree(post);
        free((void *) authCString);
    }
    return code;
}


