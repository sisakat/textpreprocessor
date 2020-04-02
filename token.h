#pragma once
#include <algorithm>
#include <string>
#include "keywords.h"

enum token_type { literal, op, keyword, variable };

struct token {
    token_type type;
    std::string value;
    token (std::string value) : value { value } { 
        tokenize();
    }

    void tokenize() {
        if (value[0] == '\"') {
            value = value.substr(1, value.length() - 2);
            type = token_type::literal;
        } else if (std::find(keywords.begin(), keywords.end(), value) != keywords.end()) {
            type = token_type::keyword;
        }
    }
};
