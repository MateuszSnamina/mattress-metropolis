
#include <boost/program_options.hpp>

/*
#include <iostream>

#include <img_to_rset/program_options.hpp>

namespace {

void emit_help(std::ostream& s,
               const boost::program_options::options_description& desc) {
    s << "Program: img-to-rset" << std::endl;
    s << desc << std::endl;
}
}  // namespace

ProgramOptions grep_program_options(int argc, char** argv) {
    ProgramOptions program_options;
    // standard arguments:
    boost::program_options::options_description desc("Options");
    desc.add_options()
        // --help, -h:
        ("help,h", "Print help messages")
        // --path-to-conductivity-image,-c:
        ("path-to-image,c",
         boost::program_options::value<std::string>(&program_options.path_to_conductivity_image)->default_value("conductivity.ppm"))
        // --path-to-dimension-image,-d:
        ("path-to-dimension-image,d",
         boost::program_options::value<std::string>(&program_options.path_to_dimension_image)->default_value("dimension.pgm"));
    boost::program_options::positional_options_description p;
    // positional arguments:
    p.add("script-file-path", 1);
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
*/
