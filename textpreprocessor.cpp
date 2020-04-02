#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <functional>
#include "processor.h"
#include "colormod.h"

using namespace std;

void print_help();

int main(int argc, char **argv) {
    if (argc < 2) {
        print_help();
    }

    fstream f;
    f.open(argv[1], ios::in);
    if (f.is_open()) {
        string line;
        while (getline(f, line)) {
            process(string(argv[1]), line);
            cout << endl;
        }
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
