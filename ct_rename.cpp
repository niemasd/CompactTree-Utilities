/**
 * Reimplementation of nw_rename
 */

// include statements
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "common.h"
#include "compact_tree.h"

// definitions and constants
#define PROGRAM_DESCRIPTION "Renames nodes using a mapping"
#define PROGRAM_USAGE_ARGS_1 "[-hl] <newick trees filename|-> <map filename>"
#define PROGRAM_USAGE_ARGS_2 "[-hl] <newick trees filename|-> <old-label> <new-label>"
#define PROGRAM_USAGE_H "-h: print this message and exit"
#define PROGRAM_USAGE_L "-l: only replace leaf labels"

// show usage message
void show_usage(const char* const exe = "ct_rename") {
    std::cerr << "CompactTree-Utilities v" << CT_UTILS_VERSION << " (compiled with CompactTree v" << COMPACTTREE_VERSION << ')' << std::endl << std::endl;
    std::cerr << PROGRAM_DESCRIPTION << std::endl << std::endl;
    std::cerr << exe << ' ' << PROGRAM_USAGE_ARGS_1 << std::endl << "or" << std::endl << exe << ' ' << PROGRAM_USAGE_ARGS_2 << std::endl << std::endl;
    std::cerr << "  " << PROGRAM_USAGE_H << std::endl;
    std::cerr << "  " << PROGRAM_USAGE_L << std::endl;
}

// main program
int main(int argc, char** argv) {
    // check user args
    if(argc == 1) { show_usage(argv[0]); exit(1); }
    bool include_internal = true; char* fn = nullptr; std::vector<char*> pos_args;
    char* curr_arg; size_t curr_len;
    for(int i = 1; i < argc; ++i) {
        curr_arg = argv[i]; curr_len = strlen(curr_arg);
        if((curr_len != 1) && (curr_arg[0] == '-')) {
            for(size_t j = 1; j < curr_len; ++j) {
                switch(curr_arg[j]) {
                    case 'l': include_internal = false; break;
                    case 'h': default: show_usage(argv[0]); exit(1);
                };
            }
        } else { pos_args.emplace_back(curr_arg); }
    }
    fn = pos_args[0];

    // parse label mapping
    std::unordered_map<std::string, std::string> label_map; std::string line;
    if(pos_args.size() == 2) {
        std::ifstream infile(pos_args[1]); check_open(infile, pos_args[1]); std::istringstream ss; std::string first; std::string second;
        while(getline(infile, line)) { second = ""; ss = std::istringstream(line); ss >> first; ss >> second; label_map.emplace(first, second); }
    } else if(pos_args.size() == 3) {
        label_map.emplace(pos_args[1], pos_args[2]);
    } else { show_usage(argv[0]); exit(1); }

    // parse tree and rename labels
    std::istream* infile; if(strcmp(fn, "-") == 0) { infile = &(std::cin); } else { infile = new std::ifstream(fn); } check_open(*infile, fn);
    while(getline(*infile, line)) { compact_tree tree(line, false, true, true); tree.replace_labels(label_map, include_internal); tree.print_newick(std::cout); std::cout << std::endl; }
    if(strcmp(fn, "-") != 0) { delete infile; } return 0;
}
