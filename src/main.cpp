#include <controller.hpp>

#include <cxxopts.hpp>
#include <fmt/color.h>
#include <fmt/core.h>
#include <spdlog/cfg/env.h>
#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) try {

  spdlog::cfg::load_env_levels();

  // =================================================================================================
  // CLI
  cxxopts::Options options(argv[0], "Description");
  options.positional_help("[filter]").show_positional_help();

  // clang-format off
  options.add_options()
    ("filter", "Filter to select command", cxxopts::value<std::string>()->default_value(""), "FILTER")
    ("i,input", "Filename or directory containing input files", cxxopts::value<std::string>(), "INPUT")
    ("n", "Number of run for bench", cxxopts::value<int>()->default_value("1"), "NRUN")
    ("h,help", "Print help")
    ;
  // clang-format on
  options.parse_positional({"filter"});
  auto cli = options.parse(argc, argv);

  if (cli.count("help")) {
    std::cout << options.help() << std::endl;
    return EXIT_SUCCESS;
  }

  if (!cli.count("input")) {
    spdlog::error("Missing input parameter");
    return EXIT_FAILURE;
  }

  // =================================================================================================
  // Code
  aoc::Controller engine(cli["filter"].as<std::string>());
  engine.run(cli["input"].as<std::string>(), std::max(1, cli["n"].as<int>()));

  return EXIT_SUCCESS;

} catch (const cxxopts::OptionException &e) {
  spdlog::error("Parsing options : {}", e.what());
  return EXIT_FAILURE;

} catch (const std::exception &e) {
  spdlog::error("{}", e.what());
  return EXIT_FAILURE;
}
