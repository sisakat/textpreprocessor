#include "processor.h"
#include "function_impl.h"
#include "functions.h"
#include "util.h"
#include <stack>

using namespace std;

static bool debug = false;

static const string STARTEXPR = "%?";
static const string ENDEXPR = "?%";
static string filename = "";

bool command_mode = false;

void process(string scope, string& line) {
  line += '\0';
  filename = scope;
  char last;
  bool lastchar = false;
  ostringstream oss;

  for (char& c : line) {
    if (lastchar && STARTEXPR == string{last, c}) {
      command_mode = true;
      lastchar = false;
      continue;
    } else if (lastchar && ENDEXPR == string{last, c}) {
      command_mode = false;
      invoke_command(oss.str());
      oss.str("");
      oss.clear();
      lastchar = false;
      continue;
    } else if (lastchar && command_mode && &c == &line.back()) {
      oss << last;
      invoke_command(oss.str());
      oss.str("");
      oss.clear();
      lastchar = false;
      continue;
    }

    if (lastchar) {
      if (command_mode) {
        oss << last;
      } else {
        if (!debug)
          cout << last;
      }
    }

    last = c;
    lastchar = true;
  }

  if (lastchar && !debug)
    cout << last;

  if (!command_mode)
    cout << endl;
}

string current_scope() { return filename; }

static void push_back_token(vector<token>& tokens, ostringstream& oss) {
  if (oss.str().length() > 0) {
    tokens.push_back(token(oss.str()));
    oss.str("");
    oss.clear();
  }
}

vector<token> invoke_tokens;
void invoke_command(string command) {
  int start_index = invoke_tokens.size();
  command = trim(command);
  ostringstream oss;
  bool quote = false;

  for (char& c : command) {
    if (c == '\n')
      continue;

    if (c == '\"')
      quote = !quote;
    if ((c != ' ' || quote)) {
      if (quote) {
        oss << c;
      } else if (c == '(') {
        push_back_token(invoke_tokens, oss);
        invoke_tokens.push_back(token("(", token_type::op));
      } else if (c == ')') {
        push_back_token(invoke_tokens, oss);
        invoke_tokens.push_back(token(")", token_type::op));
      } else if (c == '{') {
        push_back_token(invoke_tokens, oss);
        invoke_tokens.push_back(token("{", token_type::op));
      } else if (c == '}') {
        push_back_token(invoke_tokens, oss);
        invoke_tokens.push_back(token("}", token_type::op));
      } else if (c == ',') {
        push_back_token(invoke_tokens, oss);
        invoke_tokens.push_back(token(",", token_type::op));
      } else if (c == '$') {
        push_back_token(invoke_tokens, oss);
        invoke_tokens.push_back(token("$", token_type::op));
      } else if (c == '=') {
        push_back_token(invoke_tokens, oss);
        invoke_tokens.push_back(token("=", token_type::op));
      } else if (c == ';') {
        push_back_token(invoke_tokens, oss);
        invoke_tokens.push_back(token(";", token_type::op));
      } else {
        oss << c;
      }
    } else if (oss.str().length() > 0) {
      push_back_token(invoke_tokens, oss);
    }
  }

  if (oss.str().length() > 0) {
    push_back_token(invoke_tokens, oss);
  }

  interpret(invoke_tokens, start_index);
}

static void debug_token(const token& t) {
  cout << "<token>";
  cout << "<value>" << t.value << "</value>";
  cout << "<type>" << t.type << "</type>";
  cout << "</token>";
}

static void debug_output(string output) {
  cout << "<text>" << output << "</text>";
}

static void debug_function_call(string function_name, vector<token> parameters,
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

static int function_counter = 0;
static int paranthesis = 0;
static bool variable_counter = false;
static bool assignment = false;
static string variable_name = "";
static stack<string> function_names;
static map<string, vector<token>> function_parameters;
static map<string, string> variables;
static int next_token_index = -1;
static int loop_index = -1;

void interpret_seperator(token& tok);
void interpret_semicolon(token& tok);
void interpret_dollar(token& tok);
void interpret_assignment(token& tok);
void interpret_paranthesis(token& tok);
void interpret_curlybracket(token& tok);

void interpret(vector<token>& tokens, int start_index) {
  if (debug) {
    for (int i = start_index; i < tokens.size(); i++) {
      debug_token(tokens[i]);
      cout << endl;
    }
    cout << endl;
  }

  for (unsigned int i = start_index; i < tokens.size(); i++) {
    if (tokens[i].type == token_type::function) {
      if (tokens[i].value == "while") {
        loop_index = i; 
      }
      function_counter++;
      function_names.push(tokens[i].value);
    } else if (tokens[i].type == token_type::op) {
      auto curr_tok = tokens[i];
      auto curr_val = curr_tok.value;

      if (curr_val == ",") {
        interpret_seperator(curr_tok);
      } else if (curr_val == ";") {
        interpret_semicolon(curr_tok);
      } else if (curr_val == "$") {
        interpret_dollar(curr_tok);
      } else if (curr_val == "=") {
        interpret_assignment(curr_tok);
      } else if (curr_val == "(" || curr_val == ")") {
        interpret_paranthesis(curr_tok);
      } else if (curr_val == "{" || curr_val == "}") {
        interpret_curlybracket(curr_tok);
      }
    } else if (function_counter > 0) {
      if (variable_counter && !assignment) {
        // variable mode but no assignment
        // equals usage of variable
        variable_name = tokens[i].value;
        function_parameters[function_names.top()].push_back(
            token(variables[variable_name], token_type::literal));
        variable_counter = false;
      } else if (variable_counter && assignment &&
                 function_names.top() == variable_name) {
        // variable mode and assignment
        // and current function level is the variable
        if (tokens[i].type == token_type::literal) {
          variables[variable_name] = tokens[i].value;
        } else {
          variables[variable_name] =
              function_parameters[function_names.top()][0].value;
        }
        function_names.pop();
        function_counter--;
        variable_counter = false;
        assignment = false;
      } else if (tokens[i].type != token_type::op) {
        function_parameters[function_names.top()].push_back(tokens[i]);
      }
    } else if (variable_counter && function_counter == 0) {
      variable_name = tokens[i].value;
    } else {
      cerr << "ERROR: unknown statement '" << tokens[i].value << "'" << endl;
      exit(2);
    }

    if (next_token_index != -1) {
      i = next_token_index - 1;
      next_token_index = -1;
    }
  }
}

void interpret_seperator(token& tok) {
  // disable variable mode
  variable_counter = false;
}

void interpret_semicolon(token& tok) {
  if (assignment) {
    variables[variable_name] = function_parameters[variable_name][0].value;
    assignment = false;
    function_counter--;
    function_names.pop();
  }
}

void interpret_dollar(token& tok) {
  // enable variable mode
  variable_counter = true;
}

void interpret_assignment(token& tok) {
  function_counter++;
  function_names.push(variable_name);
  assignment = true;
}

void interpret_paranthesis(token& tok) {
  if (tok.value == "(") {
    paranthesis++;
  } else {
    paranthesis--;
    if (function_counter > 0) {
      function_counter--;
      string function_name = function_names.top();
      string return_value =
          function_impl[function_name](function_parameters[function_name]);
      function_names.pop();

      if (debug) {
        debug_function_call(function_name, function_parameters[function_name],
                            return_value);
      }

      function_parameters[function_name].clear();

      if (function_names.size() > 0) {
        function_parameters[function_names.top()].push_back(
            token(return_value, token_type::literal));
      } else {
        if (debug) {
          debug_output(return_value);
        } else {
          if (function_name != "while") {
            cout << return_value;
          }
        }
      }

      if (function_name == "while" && return_value == "false") {
        loop_index = -1;
      }
    }
  }
}

void interpret_curlybracket(token& tok) {
  if (tok.value == "{") {

  } else {
    if (loop_index != -1) {
      next_token_index = loop_index; 
    }
  }
}

void set_debug(bool state) { debug = state; }
