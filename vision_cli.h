//
// Created by Josh Smith on 3/2/17.
//

#ifndef WATSON_VISION_CLI_H
#define WATSON_VISION_CLI_H

#include <iostream>
using namespace std;
#include <boost/program_options.hpp>
using namespace boost::program_options;

#include <boost/filesystem.hpp>

#include <fstream>
#include <streambuf>
#include <cerrno>
#include "vision.h"
#include "WatsonCliRunner.h"
#include "vision.h"

namespace watson {

    class vision_cli : public virtual WatsonCliRunner {

    public:
        vision_cli();
        void performActions(const variables_map &vm);
        options_description getOptions();
    };
};


#endif //WATSON_VISION_CLI_H
