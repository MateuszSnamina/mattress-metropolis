
#include <boost/program_options.hpp>


#include <iostream>

#include <metropolis/raw_program_options.hpp>

namespace {

void emit_help(std::ostream& s,
               const boost::program_options::options_description& desc) {
    s << "Program: metropolis" << std::endl;
    s << desc << std::endl;
}
}  // namespace

RawProgramOptions grep_program_options(int argc, char** argv) {
    RawProgramOptions program_options;
    // standard arguments:
    boost::program_options::options_description desc("Options");
    desc.add_options()
            // --help, -h:
            ("help,h", "Print help messages")
            // --temperature_steps,-s:
            ("temperature_steps,s",
             boost::program_options::value<std::string>(&program_options.temerature_steps_string)->default_value("(0.0,0.2)@0.1;[0.2,0.7)@0.05;[0.7,1)@0.1;1"))
            // --n_thermal,-t:
            ("n_thermal,t",
             boost::program_options::value<std::string>(&program_options.n_thermal_steps_string)->default_value("10k"))
            // --n_average,-a:
            ("n_average,a",
             boost::program_options::value<std::string>(&program_options.n_average_steps_string)->default_value("100k"))
            // --model,-m:
            ("model,m",
             boost::program_options::value<std::string>(&program_options.model_type_string)->default_value("ising::doublet"))
            // --energy,-e:
            ("energy,e",
             boost::program_options::value<std::string>(&program_options.energy_getter_type_string)->default_value("4nn"))
            // --ising_multiplicity,-n:
            ("ising_multiplicity,n",
             boost::program_options::value<unsigned>(&program_options.ising_multiplicity)->default_value(2))
            // --path_to_chached_data,-p:
            ("path_to_chached_data,p",
             boost::program_options::value<std::string>(&program_options.path_to_chached_data)->default_value("data.yaml"));
    boost::program_options::positional_options_description p;
    // positional arguments:
    p.add("model", 1);
    p.add("n_thermal", 1);
    p.add("n_average", 1);
    boost::program_options::variables_map vm;
    try {
        boost::program_options::store(
                    boost::program_options::command_line_parser(argc, argv).options(desc).positional(p).run(),
                    vm);  // may throw
        if (vm.count("help")) {
            emit_help(std::cout, desc);
            exit(0);
        }
        // sets auto variables (eq. class_specification_file_path),
        // throw is required variable is missing:
        boost::program_options::notify(vm);  // may throw
    } catch (boost::program_options::error& e) {
        std::cerr << "[GLOBAL ERROR] [PROGRAM OPTIONS ERROR]: " << e.what()
                  << std::endl;
        std::cerr << std::endl;
        emit_help(std::cerr, desc);
        exit(1);
    }
    return program_options;
}
