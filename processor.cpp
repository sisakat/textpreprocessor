#include "processor.h"
#include "keywords.h"
#include "keyword_functions.h"
#include "util.h"

using namespace std;

static const string STARTEXPR = "%?";
static const string ENDEXPR   = "?%";

bool command_mode = false;

void process(string& line) {
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

void invoke_command(string command) {
    command = trim(command);
    ostringstream oss;
    vector<token> tokens;
    bool quote = false;

    for (char& c : command) {
        if (c == '\"')
            quote = not quote;
        if (c != ' ' or quote) {
            oss << c;
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
    for (unsigned int i = 0; i < tokens.size(); i++) {
        if (tokens[i].type == token_type::keyword) {
            i += keyword_functions[tokens[i].value](tokens, i);
        }
    }
}
