//
// Created by Josh Smith on 2/26/17.
//

#ifndef WATSON_VISION_H
#define WATSON_VISION_H

#include "BaseWatsonService.h"
#include <boost/algorithm/string/join.hpp>
#include "post_parameter.h"

namespace watson {
    namespace v3 {

        class vision : public BaseWatsonService {
        public:
            vision(const std::string &api_key);

        protected:
            char *getAuthString() override;

        public:
            nlohmann::json classify(std::string filename, std::vector<std::string> classifier_ids);

            nlohmann::json classify_url(std::string target_url, std::vector<std::string> classifier_ids);

            nlohmann::json extract_text(std::string filename);

            nlohmann::json find_faces(std::string filename);
            nlohmann::json find_faces();

            nlohmann::json find_food(std::string filename);

            nlohmann::json list_classifiers();

            nlohmann::json get_classifier(std::string classifier_id);

            nlohmann::json create_classifier(std::string name, std::map<std::string, std::string> classifier_zipfiles);

            nlohmann::json retrain_classifier(std::string classifier_id, std::map<std::string, std::string> classifier_zipfiles);

            nlohmann::json delete_classifier(std::string classifier_id);
        };

    }
}


#endif //WATSON_VISION_H
