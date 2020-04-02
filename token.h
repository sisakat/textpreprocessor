#pragma once
#include <algorithm>
#include <string>
#include "functions.h"

enum token_type { literal, op, function, variable };

struct token {
    token_type type;
    std::string value;
    token (std::string value) : value { value } { 
        tokenize();
    }

    token (std::string value, token_type type) : value { value }, type { type } {}

    void tokenize() {
        if (value[0] == '\"') {
            value = value.substr(1, value.length() - 2);
            type = token_type::literal;
        } else if (std::find(functions.begin(), functions.end(), value) != functions.end()) {
            type = token_type::function;
        }
    }
};
