#include <controller.hpp>

#include <cxxopts.hpp>
#include <fmt/core.h>
#include <fmt/color.h>
#include <spdlog/spdlog.h>
#include <spdlog/cfg/env.h>

int main(int argc, char *argv[]) try {

  spdlog::cfg::load_env_levels();

  // =================================================================================================
  // CLI
  cxxopts::Options options(argv[0], "Description");
  options.positional_help("[filter]").show_positional_help();

  // clang-format off
  options.add_options()
    ("filter", "Filter to select command", cxxopts::value<std::string>()->default_value(""), "FILTER")
    ("d,directory", "Directory that contain input files", cxxopts::value<std::string>(), "directory")
    ("f,filename", "Input filename", cxxopts::value<std::string>()->default_value(""), "filename")
    ("h,help", "Print help")
    ;
  // clang-format on
  options.parse_positional({"filter"});
  auto cli = options.parse(argc, argv);

  if (cli.count("help")) {
    std::cout << options.help() << std::endl;
    return EXIT_SUCCESS;
  }

  if (!cli.count("directory")) {
    spdlog::error("Missing directory");
    return EXIT_FAILURE;
  }

  // =================================================================================================
  // Code
  aoc::Controller engine(cli["filter"].as<std::string>());
  engine.run(cli["directory"].as<std::string>(), cli["filename"].as<std::string>());
  // engine.show();

  return EXIT_SUCCESS;

} catch (const cxxopts::OptionException &e) {
  spdlog::error("Parsing options : {}", e.what());
  return EXIT_FAILURE;

} catch (const std::exception &e) {
  spdlog::error("{}", e.what());
  return EXIT_FAILURE;

}
