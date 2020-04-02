#include <filesystem>
#include <algorithm>
#include <regex>
#include "processor.h"
#include "function_impl.h"

using std::string;
using std::vector;
using std::ostringstream;
using std::cout;
using std::cerr;
using std::endl;
using std::fstream;
using std::ios;
namespace fs = std::filesystem;

string include(std::vector<token> parameters) {
    if (parameters.size() < 1 || parameters[0].type != token_type::literal) {
        cerr << "ERROR: Expected filename after include";
        exit(2);
    }

    string filename = parameters[0].value;

    if (parameters.size() > 1) {
        string pattern = parameters[1].value;
        return include_file_regex(filename, pattern);
    }

    fs::path p(filename);
    if (fs::is_directory(filename)) {
        return include_dir(filename);
    } else {
        return include_file(filename);
    }
}

string include_file_regex(string path, string pattern) {
    ostringstream oss;

    for (const auto &entry : fs::directory_iterator(path)) {
        if (regex_match(entry.path().filename().string(), std::regex(pattern))) {
            oss << include_file(entry.path().string());
        }
    }

    return oss.str();
    return "lol";
}

string include_file(std::string filename) {
    fstream f;
    ostringstream os;
    f.open(filename, ios::in);
    if (f.is_open()) {
        string line;
        while (getline(f, line)) {
          process(filename, line);
        }
    } else {
        cerr << "ERROR: Could not open file '" << filename << "'";
    }

    return os.str();
}

string include_dir(std::string path) {
    ostringstream os;
    for (const auto &entry : fs::directory_iterator(path)) {
        os << include_file(entry.path().string());
    }
    return os.str();
}

string scope(std::vector<token> parameters) {
    return current_scope();
}

std::string echo(std::vector<token> parameters) {
    ostringstream os;
    for (const auto& param : parameters) {
        os << param.value;
    }
    return os.str();
}

std::string struppercase(std::vector<token> parameters) {
    ostringstream os;
    for (const auto& param : parameters) {
        os << param.value;
    }
    
    string output = os.str();
    std::transform(output.begin(), output.end(), output.begin(), toupper);
    return output;
}

std::string strlowercase(std::vector<token> parameters) {
    ostringstream os;
    for (const auto& param : parameters) {
        os << param.value;
    }
    
    string output = os.str();
    std::transform(output.begin(), output.end(), output.begin(), tolower);
    return output;
}