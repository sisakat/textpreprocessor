#include "colormod.h"
#include "configuration.h"
#include "parser.h"
#include "tokenizer.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

void print_help();

int main(int argc, char** argv) {
  if (argc < 2) {
    print_help();
    return 0;
  }

  for (int i = 1; i < argc - 1; i++) {
    string input(argv[i]);
    if (input == "-dt") {
      set_config(config_type::debug_token);
    } else if (input == "-dp") {
      set_config(config_type::debug_parse);
    }
  }

  string filename = argv[argc - 1];
  ifstream f(filename);
  string contents((std::istreambuf_iterator<char>(f)),
                  std::istreambuf_iterator<char>());

  auto tokens = tokenize(filename, contents);
  if (is_config_set(config_type::debug_token)) {
    cout << tokens << endl;
  }

  auto stmts = parse(tokens);
  if (is_config_set(config_type::debug_parse) || true) {
    cout << stmts << endl;
  }
}

void print_help() {
  Color::Modifier red(Color::FG_RED);
  Color::Modifier def(Color::FG_DEFAULT);
  Color::Modifier white(Color::FG_WHITE);
  cout << white << "lpp: " << red << "fatal error: " << def << "no input files"
       << endl;
}
