#include <filesystem>
#include "processor.h"
#include "keyword_functions.h"

using namespace std;
namespace fs = std::filesystem;

int include(std::vector<token> tokens, int index) {
    if ((unsigned int)index + 1 > tokens.size() 
        || tokens[index+1].type != token_type::literal) {
        cerr << "ERROR: Expected filename after include";
        exit(2);
    }
    string filename = tokens[index+1].value;
    fs::path p(filename);

    if (fs::is_directory(filename)) {
        include_dir(filename);
    } else {
        include_file(filename);
    }

    return index+1;
}

void include_file(std::string filename) {
    fstream f;
    f.open(filename, ios::in);
    if (f.is_open()) {
        string line;
        while (getline(f, line)) {
          process(line);
          cout << endl;
        }
    } else {
        cerr << "ERROR: Could not open file '" << filename << "'";
        exit(2);
    }
}

void include_dir(std::string path) {
    for (const auto &entry : fs::directory_iterator(path)) {
        include_file(entry.path().string());
    }
}
