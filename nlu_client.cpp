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
        nlu_client::url = url;
        this->queryKind = Url;
    }

    const std::string &nlu_client::getUrl() const {
        return url;
    }

    const std::string &nlu_client::getHtml() const {
        return html;
    }

    void nlu_client::setHtml(const std::string &html) {
        this->queryKind = Html;
        nlu_client::html = html;
    }

    const std::string &nlu_client::getText() const {
        return text;
    }

    void nlu_client::setText(const std::string &text) {
        this->queryKind = Text;
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

  CURLcode nlu_client::curl_read(long timeout, std::ostream *output_stream)
  {

    CURLcode code(CURLE_FAILED_INIT);
    CURL* curl = curl_easy_init();
    std::ostringstream auth;
    auth << this->getUsername() << ":" << this->getPassword();

      json payload;

      switch (this->queryKind) {
          case Html:
              payload["html"] = this->getHtml();
              break;
          case Text:
              payload["text"] = this->getText();
              break;
          default:
              payload["url"] = this->getUrl();
      }

      json features;

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

      payload["features"] = features;

    if(curl)
    {
        struct curl_slist *chunk = NULL;
        chunk = curl_slist_append(chunk, "Content-Type: application/json");

        // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        std::ostringstream payload_out;
        payload_out << payload.dump();
        std::string payload_str = payload_out.str();
        const char* payload_cstr = payload_str.c_str();

        // std::cout << payload.dump(4);

        if(CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &data_write))
           && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, output_stream))
           && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L))
           && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_USERPWD, auth.str().c_str()))
           && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L))
           && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk))
           && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload_cstr))
           && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout))
           && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_URL, this->getApi_url().c_str())))
        {
            code = curl_easy_perform(curl);
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(chunk);
    }
    return code;
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
        this->curl_read(30, &buffer);
        return json::parse(buffer.str());
    }
}

