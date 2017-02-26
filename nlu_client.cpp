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

const char *BASE_URL="https://gateway.watsonplatform.net/natural-language-understanding/api/v1/analyze?version=2016-01-23";

namespace watson {
    namespace v1 {

        std::string get_contents(std::string *filename) {
            try {
                if (boost::filesystem::exists(*filename)) {
                    std::ifstream in(filename->c_str(), std::ios::in | std::ios::binary);
                    if (in) {
                        return (std::string((std::istreambuf_iterator<char>(in)),
                                            std::istreambuf_iterator<char>()));
                    }
                    throw (errno);
                } else {
                    return *filename;
                }
            } catch (const boost::filesystem::filesystem_error &e) {
                return *filename;
            }
        }

        const std::string name_mapper(std::string arg) {
            std::map<std::string, std::string> env_mapper = { {"NATURAL_LANGUAGE_UNDERSTANDING_USERNAME", "username"},
                                                              {"NATURAL_LANGUAGE_UNDERSTANDING_PASSWORD", "password"},
                                                              {"NATURAL_LANGUAGE_UNDERSTANDING_URL", "api_url"} };
            std::map<std::string, std::string>::iterator found;
            found = env_mapper.find(arg);
            if (found == env_mapper.end()) {
                return std::string("");
            } else {
                return found->second;
            }
        }

        void run_nlu(parsed_options opts_in) {
            variables_map nluVariablesMap;

            options_description nlu_options("Allowed options");

            nlu_options.add_options()
                    ("help,h", "print usage message")
                    ("verbose,v", "verbose")
                    ("api_url,a", value<std::string>(), "API Url (not the url to fetch)" )
                    ("username,u", value<std::string>()->required(), "username (required)")
                    ("password,p", value<std::string>()->required(), "password (required)")
                    ("text,t", value<std::string>(), "text or file name containing text to analyze")
                    ("html,l", value<std::string>(), "html string or file name containing html to analyze")
                    ("emotion,e", value<std::vector<std::string>>()->multitoken(), "targeted emotion string")
                    ("sentiment,s", value<std::vector<std::string>>()->multitoken(), "targeted sentiment string")
                    ("feature,f", value<std::vector<std::string>>()->multitoken(), "set of features to use")
                    ("url,r", value<std::string>(), "string url to fetch and analyze")
                    ;


            std::vector<std::string> opts = collect_unrecognized(opts_in.options, include_positional);
            opts.erase(opts.begin());
            store(command_line_parser(opts).options(nlu_options).run(), nluVariablesMap);
            store(parse_environment(nlu_options, &name_mapper),nluVariablesMap);
            if (nluVariablesMap.count("help")) {
                std::cout << nlu_options << "\n";
                return;
            }

            try {
                notify(nluVariablesMap);
            } catch (...) {
                std::cout << nlu_options << std::endl;
                return;
            }

            watson::v1::nlu_client *client;
            std::string username, password;
            username = nluVariablesMap["username"].as<std::string>();
            password = nluVariablesMap["password"].as<std::string>();
            client = new watson::v1::nlu_client(username, password);

            if (nluVariablesMap.count("verbose")) {
                client->setVerbose(true);
            }

            if (nluVariablesMap.count("sentiment")) {
                client->setTargeted_sentiment(nluVariablesMap["sentiment"].as<std::vector<std::string>>());
            }

            if (nluVariablesMap.count("emotion")) {
                client->setTargeted_emotion(nluVariablesMap["emotion"].as<std::vector<std::string>>());
            }

            if (nluVariablesMap.count("text")) {
                std::string text = nluVariablesMap["text"].as<std::string>();
                client->setText(get_contents(&text));
            }

            if (nluVariablesMap.count("html")) {
                std::string html = nluVariablesMap["html"].as<std::string>();
                client->setHtml(get_contents(&html));
            }

            if (nluVariablesMap.count("url")) {
                std::string url = nluVariablesMap["url"].as<std::string>();
                client->setUrl(url);
            }

            std::set<watson::v1::Features> features;
            std::map<std::string, watson::v1::Features> feature_map = { {std::string("sentiment"), watson::v1::Sentiment },
                                                                    {std::string("keywords"), watson::v1::Keywords },
                                                                    {std::string("entities"), watson::v1::Entities},
                                                                    {std::string("relations"), watson::v1::Relations},
                                                                    {std::string("semanticroles"), watson::v1::SemanticRoles},
                                                                    {std::string("categories"), watson::v1::Categories},
                                                                    {std::string("emotion"), watson::v1::Emotion}};
            if (nluVariablesMap.count("feature")) {
                for (auto feature : nluVariablesMap["feature"].as<std::vector<std::string>>()) {
                    features.insert(feature_map[feature]);
                }
            } else {
                for (auto const& feature : feature_map) {
                    features.insert(feature.second);
                }
            }

            client->setFeatures(features);
            std::cout << client->analyze().dump(4) << std::endl;
        }

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
                        if (targeted_emotion.size() > 0) {
                            features["emotion"]["targets"] = targeted_emotion;
                        }
                        break;
                    case Sentiment:
                        features["sentiment"] = json::object();
                        if (targeted_sentiment.size() > 0) {
                            features["sentiment"]["targets"] = targeted_sentiment;
                        }
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

        const std::vector<std::string> &nlu_client::getTargeted_emotion() const {
            return targeted_emotion;
        }

        void nlu_client::setTargeted_emotion(const std::vector<std::string> &targeted_emotion) {
            nlu_client::targeted_emotion = targeted_emotion;
        }

        const std::vector<std::string> &nlu_client::getTargeted_sentiment() const {
            return targeted_sentiment;
        }

        void nlu_client::setTargeted_sentiment(const std::vector<std::string> &targeted_sentiment) {
            nlu_client::targeted_sentiment = targeted_sentiment;
        }
    }
}

