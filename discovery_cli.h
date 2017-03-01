//
// Created by Josh Smith on 2/28/17.
//

#ifndef WATSON_DISCOVERY_CLI_H_H
#define WATSON_DISCOVERY_CLI_H_H

#include <iostream>
using namespace std;
#include <boost/program_options.hpp>
using namespace boost::program_options;

#include <boost/filesystem.hpp>

#include <fstream>
#include <streambuf>
#include <cerrno>

#include "discovery.h"

namespace watson {
    void run_discovery(parsed_options opts_in);
}


#endif //WATSON_DISCOVERY_CLI_H_H

