#include "debug.h"
#include <iostream>

using std::cout;
using std::string;
using std::vector;

void debug_token(const token& t) {
  cout << "<token>";
  cout << "<value>" << t.value << "</value>";
  cout << "<type>" << t.type << "</type>";
  cout << "</token>";
}

void debug_output(string output) { cout << "<text>" << output << "</text>"; }

void debug_function_call(string function_name, vector<token> parameters,
                         string return_value) {
  cout << "<function>";
  cout << "<name>" << function_name << "</name>";
  cout << "<parameters>";

  for (int i = 0; i < parameters.size(); i++) {
    debug_token(parameters[i]);
  }

  cout << "</parameters>";
  cout << "<return>" << return_value << "</return>";
  cout << "</function>";
}