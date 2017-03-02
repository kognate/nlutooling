//
// Created by Josh Smith on 2/26/17.
//

#include "vision.h"
using json = nlohmann::json;

watson::v3::vision::vision(const std::string &api_key) : BaseWatsonService(api_key) {
    setApi_key(api_key);
    setApi_url(std::string("https://gateway-a.watsonplatform.net/visual-recognition/api"));
    setVersion(std::string("2016-05-20"));
}

nlohmann::json watson::v3::vision::list_classifiers() {
    std::ostringstream buffer;
    std::string url = getApi_url() + "/v3/classifiers" + "?api_key=" + getApi_key() + "&version=" + getVersion();
    // 6d2ff6fd28d756d57f12140f9cef4e9103893a72
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

char *watson::v3::vision::getAuthString() {
    return nullptr;
}

nlohmann::json watson::v3::vision::get_classifier(std::string classifier_id) {
    std::ostringstream buffer;
    std::string url = getApi_url() + "/v3/classifiers/" + classifier_id + "?api_key=" + getApi_key() + "&version=" + getVersion();
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

nlohmann::json watson::v3::vision::classify_url(std::string target_url, std::vector<std::string> classifier_ids) {
    std::ostringstream buffer;
    std::string url = getApi_url() + "/v3/classify?api_key=" + getApi_key() + "&version=" + getVersion() + "&url=" + target_url;
    if (classifier_ids.size() > 0) {
        url += "&classifier_ids=" + boost::algorithm::join(classifier_ids, ",");
    }
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

nlohmann::json watson::v3::vision::classify(std::string filename, std::vector<std::string> classifier_ids) {
    std::shared_ptr<file_post_parameter> fpp = std::make_shared<file_post_parameter>(std::string("images_file"), std::string("bogus.jpg"), filename);
    addPostParameter(fpp);
    std::ostringstream buffer;
    std::string url = getApi_url() + "/v3/classify?api_key=" + getApi_key() + "&version=" + getVersion();
    if (classifier_ids.size() > 0) {
        url += "&classifier_ids=" + boost::algorithm::join(classifier_ids, ",");
    }
    CURLcode res = do_post(30, url, &buffer);
    if (res == CURLE_OK) {
        return json::parse(buffer.str());
    } else {
        json results = json::object();
        results["code"] = res;
        results["body"] = "Error!";
        return results;
    }
}
