#include <stack>
#include "processor.h"
#include "functions.h"
#include "function_impl.h"
#include "util.h"

using namespace std;

static const string STARTEXPR = "%?";
static const string ENDEXPR   = "?%";
static string filename        = "";

bool command_mode = false;

void process(string scope, string& line) {
    filename = scope;
    ostringstream oss;
    char last;
    bool lastchar = false;

    for (char& c : line) {
        if (STARTEXPR == string { last, c }) {
            command_mode = true;
            lastchar = false;
            continue;
        } else if (ENDEXPR == string { last, c }) {
            command_mode = false;
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
                cout << last;
            }
        }

        last = c;
        lastchar = true;
    }

    if (lastchar)
        cout << last;
}

string current_scope() {
    return filename;
}

void invoke_command(string command) {
    command = trim(command);
    ostringstream oss;
    vector<token> tokens;
    bool quote = false;

    for (char& c : command) {
        if (c == '\n')
            continue;
        if (c == '\"')
            quote = !quote;
        if ((c != ' ' || quote) && c != ';') {
            if (c == '(') {
                tokens.push_back(token(oss.str()));
                oss.str("");
                oss.clear();
                tokens.push_back(token("(", token_type::op));
            } else if (c == ')') {
                tokens.push_back(token(oss.str()));
                oss.str("");
                oss.clear();
                tokens.push_back(token(")", token_type::op));
            } else if (c == ',') {
                tokens.push_back(token(oss.str()));
                oss.str("");
                oss.clear();
                tokens.push_back(token(",", token_type::op));  
            } else {
                oss << c;
            }
        } else if (oss.str().length() > 0) {
            tokens.push_back(token(oss.str()));
            oss.str("");
            oss.clear();
        }
    }

    if (oss.str().length() > 0) {
        tokens.push_back(token(oss.str()));
        oss.str("");
        oss.clear();
    }

    interpret(move(tokens));
}

void interpret(vector<token> tokens) {
    int function = 0;
    int paranthesis = 0;
    stack<string> function_names;
    map<string, vector<token>> function_parameters;

    for (unsigned int i = 0; i < tokens.size(); i++) {
        if (tokens[i].type == token_type::function) {
            function++;
            function_names.push(tokens[i].value);
        } else if (tokens[i].type == token_type::op) {
            if (tokens[i].value == "(") {
                paranthesis++;
            } else if (tokens[i].value == ")") {
                paranthesis--;
                if (function > 0) {
                    function--;
                    string function_name = function_names.top();
                    string return_value = function_impl[function_name](function_parameters[function_name]);
                    function_names.pop();
                    function_parameters[function_name].clear();
                    if (function_names.size() > 0) {
                        function_parameters[function_names.top()].push_back(token(return_value, token_type::literal));
                    } else {
                        cout << return_value;
                    }
                }
            }
        } else if (function > 0 && paranthesis > 0) {
            if (tokens[i].type != token_type::op) {
                function_parameters[function_names.top()].push_back(tokens[i]);
            }
        }

        //cout << tokens[i].value << " type: " << tokens[i].type << endl;
    }
}
