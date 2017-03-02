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

const std::shared_ptr<std::string> &watson::post_parameter::getContent_type() const {
    return content_type;
}

void watson::post_parameter::setContent_type(const std::shared_ptr<std::string> &content_type) {
    post_parameter::content_type = content_type;
}

watson::file_post_parameter::file_post_parameter(std::string name, std::string file_name, std::string file_path) {
    setName(name);
    setFile_name(file_name);
    setFile_path(file_path);
}

watson::file_post_parameter::file_post_parameter(std::string name, std::string file_path) {
    setName(name);
    setFile_path(file_path);
}

void watson::file_post_parameter::form_add(struct curl_httppost **post, struct curl_httppost **last) {
    int form_size = 3;
    form_size += getFile_name().size() == 0 ? 0 : 1;
    form_size += getContent_type() == nullptr ? 0 : 1;
    struct curl_forms forms[form_size];

    form_size--;

    forms[form_size].option = CURLFORM_END;

    form_size--;

    if (getFile_name().size() > 0) {
        forms[form_size].option = CURLFORM_FILENAME;
        forms[form_size].value = getFile_name().c_str();
        form_size--;
    }

    if (getContent_type() != nullptr) {
        forms[form_size].option = CURLFORM_CONTENTTYPE;
        forms[form_size].value = getContent_type()->c_str();
        form_size--;
    }

    forms[form_size].option = CURLFORM_COPYNAME;
    forms[form_size].value = getName().c_str();

    form_size--;

    forms[form_size].option = CURLFORM_FILE;
    forms[form_size].value = getFile_path().c_str();

    curl_formadd(post,last, CURLFORM_ARRAY, forms, CURLFORM_END);
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
