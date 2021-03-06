//
// Created by Josh Smith on 2/26/17.
//

#ifndef WATSON_POST_PARAMETER_H
#define WATSON_POST_PARAMETER_H
#import <string>
#import <json.hpp>
#import <curl/curl.h>

#import <boost/filesystem.hpp>

namespace watson {

    class post_parameter {
    private:
        bool multipart = false;
        std::string name = std::string("");
        std::shared_ptr<std::string> content_type = nullptr;
    public:
        const std::shared_ptr<std::string> &getContent_type() const;

        void setContent_type(const std::shared_ptr<std::string> &content_type);

    public:
        virtual void form_add(struct curl_httppost **post, struct curl_httppost **last) =0;
        virtual nlohmann::json json() { return nlohmann::json::object(); };
        virtual std::string url_encoded() { return std::string(""); };
        const std::string &getName() const;
        void setName(const std::string &name);
        bool isMultipart() const;
        void setMultipart(bool multipart);
    };

    class file_post_parameter: public virtual post_parameter {
    private:
        std::string file_name = std::string("");
        std::string file_path = std::string("");
    public:
        std::string & getFile_path();

        void setFile_path(const std::string &file_path);

        void setFile_name(const std::string &file_name);
        std::string getFile_name();

        file_post_parameter(std::string name, std::string file_name, std::string file_path);
        file_post_parameter(std::string name, std::string file_path);

        void form_add(curl_httppost **post, curl_httppost **last);
    };
}


#endif //WATSON_POST_PARAMETER_H
