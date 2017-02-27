//
// Created by Josh Smith on 2/26/17.
//

#include "post_parameter.h"

bool watson::post_parameter::isMultipart() const {
    return multipart;
}

void watson::post_parameter::setMultipart(bool multipart) {
    post_parameter::multipart = multipart;
}

const std::string &watson::post_parameter::getName() const {
    return name;
}

void watson::post_parameter::setName(const std::string &name) {
    post_parameter::name = name;
}

const std::string &watson::post_parameter::getContent_type() const {
    return content_type;
}

void watson::post_parameter::setContent_type(const std::string &content_type) {
    post_parameter::content_type = content_type;
}

watson::file_post_parameter::file_post_parameter(std::string name, std::string file_name, std::string file_path) {
}

watson::file_post_parameter::file_post_parameter(std::string name, std::string file_path) {
    setName(name);
    setFile_path(file_path);
    std::string basename = boost::filesystem::basename(file_path);
    boost::filesystem::path file = boost::filesystem::change_extension(basename, std::string(""));
    setFile_name(file.string());
}

void watson::file_post_parameter::form_add(struct curl_httppost *post, struct curl_httppost *last) {
    curl_formadd(&post,
                 &last,
                 CURLFORM_COPYNAME,
                 getFile_name().c_str(),
                 CURLFORM_FILE,
                 getFile_path().c_str(),
                 CURLFORM_END);
}

void watson::file_post_parameter::setFile_name(const std::string &file_name) {
    file_post_parameter::file_name = file_name;
}

std::string watson::file_post_parameter::getFile_name() {
    return file_name;
}

std::string & watson::file_post_parameter::getFile_path() {
    return file_path;
}

void watson::file_post_parameter::setFile_path(const std::string &file_path) {
    file_post_parameter::file_path = file_path;
}
