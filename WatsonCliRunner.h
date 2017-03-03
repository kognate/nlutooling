//
// Created by Josh Smith on 3/2/17.
//

#ifndef WATSON_WATSONCLIRUNNER_H
#define WATSON_WATSONCLIRUNNER_H

#include <iostream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <map>
#include "vision.h"
#include <functional>

namespace watson {

    class WatsonCliRunner {

    public:
        virtual void run(boost::program_options::parsed_options opts_in) =0;

    protected:
        const std::map<std::string, std::string> &getEnvironment_map() const;

        void setEnvironment_map(const std::map<std::string, std::string> &environment_map);

        std::map<std::string, std::string> environment_map = {};

        std::string nameMapper(std::string arg);

        bool has_set_options(boost::program_options::variables_map vm, std::vector<const char *> options);
    };

}


#endif //WATSON_WATSONCLIRUNNER_H
