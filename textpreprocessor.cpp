#include <iostream>
#include <fstream>
#include <string>
#include "processor.h"
#include "colormod.h"

using namespace std;

void print_help();

int main(int argc, char **argv) {
    if (argc < 2) {
        print_help();
    }
    bool debug = false;

    for (int i = 1; i < argc-1; i++) {
        string input(argv[i]);
        if (input == "-d") {
            set_debug(true);
            debug = true;
            cout << "<textpreprocessor>" << endl;
        }
    }

    fstream f;
    f.open(argv[argc-1], ios::in);
    if (f.is_open()) {
        string line;
        while (getline(f, line)) {
            process(string(argv[argc-1]), line);
        }
    }
    if (debug) {
        cout << "</textpreprocessor>";
    }
}

void print_help() {
    Color::Modifier red(Color::FG_RED);
    Color::Modifier def(Color::FG_DEFAULT);
    Color::Modifier white(Color::FG_WHITE);
    cout << white
         << "lpp: " 
         << red 
         << "fatal error: " 
         << def 
         << "no input files";
}
