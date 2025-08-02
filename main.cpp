#include <argparse/argparse.hpp>
#include <lacam.hpp>

GridType stringToGridType(const std::string& str) {
  if (str == "hex") return GridType::HEX;
  return GridType::SQUARE;
}

HexConfig stringToHexConfig(const std::string& str) {
  if (str == "even-row") return HexConfig::EVEN_ROW;
  if (str == "odd-column") return HexConfig::ODD_COLUMN;
  if (str == "even-column") return HexConfig::EVEN_COLUMN;
  return HexConfig::ODD_ROW;
}

int main(int argc, char* argv[])
{
  // arguments parser
  argparse::ArgumentParser program("lacam", "0.1.0");
  program.add_argument("-m", "--map").help("map file").required();
  program.add_argument("-i", "--scen")
      .help("scenario file")
      .default_value(std::string(""));
  program.add_argument("-N", "--num").help("number of agents").required();
  program.add_argument("-s", "--seed")
      .help("seed")
      .default_value(std::string("0"));
  program.add_argument("-v", "--verbose")
      .help("verbose")
      .default_value(std::string("0"));
  program.add_argument("-t", "--time_limit_sec")
      .help("time limit sec")
      .default_value(std::string("10"));
  program.add_argument("-o", "--output")
      .help("output file")
      .default_value(std::string("./build/result.txt"));
  program.add_argument("-l", "--log_short")
      .default_value(false)
      .implicit_value(true);
  
  // grid type parameters
  program.add_argument("-g", "--grid_type")
      .help("grid type: square or hex")
      .default_value(std::string("square"));
  program.add_argument("-hc", "--hex_config")
      .help("hexagonal configuration: odd-row, even-row, odd-column, even-column")
      .default_value(std::string("odd-row"));

  try {
    program.parse_known_args(argc, argv);
  } catch (const std::runtime_error& err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    std::exit(1);
  }

  // setup instance
  const auto verbose = std::stoi(program.get<std::string>("verbose"));
  const auto time_limit_sec =
      std::stoi(program.get<std::string>("time_limit_sec"));
  const auto scen_name = program.get<std::string>("scen");
  const auto seed = std::stoi(program.get<std::string>("seed"));
  auto MT = std::mt19937(seed);
  const auto map_name = program.get<std::string>("map");
  const auto output_name = program.get<std::string>("output");
  const auto log_short = program.get<bool>("log_short");
  const auto N = std::stoi(program.get<std::string>("num"));
  const auto grid_type_str = program.get<std::string>("grid_type");
  const auto hex_config_str = program.get<std::string>("hex_config");
  const auto grid_type = stringToGridType(grid_type_str);
  const auto hex_config = stringToHexConfig(hex_config_str);
  const auto ins = scen_name.size() > 0 ? Instance(scen_name, map_name, N, grid_type, hex_config)
                                        : Instance(map_name, &MT, N, grid_type, hex_config);
  if (!ins.is_valid(1)) return 1;

  // solve
  const auto deadline = Deadline(time_limit_sec * 1000);
  const auto solution = solve(ins, verbose - 1, &deadline, &MT);
  const auto comp_time_ms = deadline.elapsed_ms();

  // failure
  if (solution.empty()) info(1, verbose, "failed to solve");

  // check feasibility
  if (!is_feasible_solution(ins, solution, verbose)) {
    info(0, verbose, "invalid solution");
    return 1;
  }

  // post processing
  print_stats(verbose, ins, solution, comp_time_ms);
  make_log(ins, solution, output_name, comp_time_ms, map_name, seed, log_short);
  return 0;
}
