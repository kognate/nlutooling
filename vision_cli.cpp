//
// Created by Josh Smith on 3/2/17.
//

#include "vision_cli.h"

watson::vision_cli::vision_cli() {
    setEnvironment_map({{"VISUAL_RECOGNITION_API_KEY", "api_key"}});
}

void watson::vision_cli::run(boost::program_options::parsed_options opts_in) {
    options_description desc("Allowed options");
    desc.add_options()
            ("help,h", "print usage message")
            ("api_key,k", value<std::string>()->required(), "Alchemy API key")
            ("url,u", value<std::string>(), "url to fetch and classify")
            ("file,f", value<std::string>(), "file to post and classify")
            ("verbose,v", "set verbose mode (gives connection info");

    variables_map vm;

    std::vector<std::string> opts = collect_unrecognized(opts_in.options, include_positional);
    opts.erase(opts.begin());
    store(command_line_parser(opts).options(desc).run(), vm);

    using std::placeholders::_1;
    std::function<std::string(std::string)> nameMapperFunc = std::bind(&watson::vision_cli::nameMapper, this, _1);
    store(parse_environment(desc, nameMapperFunc),vm);

    if (vm.count("help")) {
        cout << desc << "\n";
        return;
    }
    notify(vm);

    watson::v3::vision *v = new watson::v3::vision(vm["api_key"].as<std::string>());
    v->setVerbose(vm.count("verbose") > 0);
    std::cout << v->classify_url(vm["url"].as<std::string>(), {}) << std::endl;

}
