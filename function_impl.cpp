#include <filesystem>
#include <algorithm>
#include "processor.h"
#include "function_impl.h"

using namespace std;
namespace fs = std::filesystem;

string include(std::vector<token> parameters) {
    if (parameters.size() < 1 || parameters[0].type != token_type::literal) {
        cerr << "ERROR: Expected filename after include";
        exit(2);
    }
    string filename = parameters[0].value;
    fs::path p(filename);

    if (fs::is_directory(filename)) {
        return include_dir(filename);
    } else {
        return include_file(filename);
    }
}

string include_file(std::string filename) {
    fstream f;
    ostringstream os;
    f.open(filename, ios::in);
    if (f.is_open()) {
        string line;
        while (getline(f, line)) {
          process(filename, line);
          os << endl;
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