// include statements
#include <cstring>
#include <fstream>
#include <iostream>
#include "compact_tree.h"

// definitions and constants
#define CT_UTILS_VERSION "0.0.1"
#define PROGRAM_DESCRIPTION "Extracts the tree's labels"
#define PROGRAM_USAGE_ARGS "[-hILrt] <newick trees filename|->"
#define PROGRAM_USAGE_H "-h: print this message and exit"
#define PROGRAM_USAGE_I "-I: don't print labels of inner nodes"
#define PROGRAM_USAGE_L "-L: don't print leaf labels"
#define PROGRAM_USAGE_R "-r: print only the root's label"
#define PROGRAM_USAGE_T "-t: TAB-separated - print on a single line, separated by tab stops."

// show usage message
void show_usage(const char* const exe = "nw_label") {
    std::cerr << "CompactTree-Utilities v" << CT_UTILS_VERSION << " (compiled with CompactTree v" << COMPACTTREE_VERSION << ')' << std::endl << std::endl;
    std::cerr << PROGRAM_DESCRIPTION << std::endl << std::endl;
    std::cerr << exe << ' ' << PROGRAM_USAGE_ARGS << std::endl << std::endl;
    std::cerr << "  " << PROGRAM_USAGE_H << std::endl;
    std::cerr << "  " << PROGRAM_USAGE_I << std::endl;
    std::cerr << "  " << PROGRAM_USAGE_L << std::endl;
    std::cerr << "  " << PROGRAM_USAGE_R << std::endl;
    std::cerr << "  " << PROGRAM_USAGE_T << std::endl;
}

// main program
int main(int argc, char** argv) {
    // check user args
    bool only_root = false; bool include_leaves = true; bool include_internal = true; char delim = '\n'; char* fn = nullptr;
    char* curr_arg; size_t curr_len;
    for(int i = 1; i < argc; ++i) {
        curr_arg = argv[i]; curr_len = strlen(curr_arg);

        // skip empty-string arguments (shouldn't happen)
        if(curr_len == 0) { continue; }

        // parse one-letter arguments
        else if((curr_len != 1) && (curr_arg[0] == '-')) {
            for(size_t j = 1; j < curr_len; ++j) {
                switch(curr_arg[j]) {
                    case 'I': include_internal = false; break;
                    case 'L': include_leaves = false; break;
                    case 'r': only_root = true; break;
                    case 't': delim = '\t'; break;
                    case 'h': default: show_usage(argv[0]); exit(1);
                };
            }
        }

        // filename
        else {
            if(fn != nullptr) { show_usage(argv[0]); exit(1); } fn = curr_arg;
        }
    }

    // parse tree and print labels
    std::string tree_string; std::istream* infile;
    if(strcmp(fn, "-") == 0) { infile = &(std::cin); } else { infile = new std::ifstream(fn); }
    while(getline(*infile, tree_string)) {
        compact_tree tree(tree_string, false, true, false);
        if(only_root) { std::cout << tree.get_label(ROOT_NODE) << std::endl; }
        else {
            const CT_NODE_T N = tree.get_num_nodes();
            for(CT_NODE_T node = 0; node < N; ++node) {
                if(tree.is_leaf(node) ? include_leaves : include_internal) { std::cout << tree.get_label(node) << delim; }
            }
        }
        if(delim != '\n') { std::cout << std::endl; }
    }
    if(strcmp(fn, "-") != 0) { delete infile; } return 0;
}
