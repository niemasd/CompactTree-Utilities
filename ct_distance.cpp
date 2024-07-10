/**
 * Reimplementation of nw_distance
 */

// include statements
#include <cstring>
#include <fstream>
#include <iostream>
#include "common.h"
#include "compact_tree.h"

// definitions and constants
#define PROGRAM_DESCRIPTION "Prints distances between nodes, following tree branches, in various ways"
#define PROGRAM_USAGE_ARGS "[-hmnst] <tree file|-> [label]*"
#define PROGRAM_USAGE_H "-h: print this message and exit"
#define PROGRAM_USAGE_M "-m <mode>: selects mode. Mode is determined by the first letter of the argument: 'r' for root mode (default), 'l' for LCA, 'p' for parent, and 'm' for matrix. Thus, '-mm', '-m matrix', and '-m mat' all select matrix mode"
#define PROGRAM_USAGE_N "-n: prints labels (or '' if empty) in addition to distances"
#define PROGRAM_USAGE_S "-s <selection>, where selection is determined by the first letter of the argument: 'd' for labeled leaves (default), 'l' for labeled nodes, 'a' for all nodes, 'i' for inner nodes, 'f' for leaves. E.g. '-s a' and '-s all' both select all nodes"
#define PROGRAM_USAGE_T "-t: in matrix mode, print a triangular matrix. In other modes, print values on a line, separated by TABs"

// show usage message
void show_usage(const char* const exe = "ct_distance") {
    std::cerr << "CompactTree-Utilities v" << CT_UTILS_VERSION << " (compiled with CompactTree v" << COMPACTTREE_VERSION << ')' << std::endl << std::endl;
    std::cerr << PROGRAM_DESCRIPTION << std::endl << std::endl;
    std::cerr << exe << ' ' << PROGRAM_USAGE_ARGS << std::endl << std::endl;
    std::cerr << "  " << PROGRAM_USAGE_H << std::endl;
    std::cerr << "  " << PROGRAM_USAGE_M << std::endl;
    std::cerr << "  " << PROGRAM_USAGE_N << std::endl;
    std::cerr << "  " << PROGRAM_USAGE_S << std::endl;
    std::cerr << "  " << PROGRAM_USAGE_T << std::endl;
}

// main program
int main(int argc, char** argv) {
    // check user args
    if(argc == 1) { show_usage(argv[0]); exit(1); }
    char mode = 'r'; bool print_labels = false; char selection = 'd'; bool t_arg = false; char* fn = nullptr;
    char* curr_arg; size_t curr_len;
    for(int i = 1; i < argc; ++i) {
        curr_arg = argv[i]; curr_len = strlen(curr_arg);

        // skip empty-string arguments (shouldn't happen)
        if(curr_len == 0) { continue; }

        // parse one-letter arguments
        else if((curr_len != 1) && (curr_arg[0] == '-')) {
            switch(curr_arg[1]) {
                case 'm': if(curr_len == 2) { mode = argv[++i][0]; } else { mode = curr_arg[2]; } break;
                case 'n': print_labels = true; break;
                case 's': if(curr_len == 2) { selection = argv[++i][0]; } else { selection = curr_arg[2]; } break;
                case 't': t_arg = true; break;
                case 'h': default: show_usage(argv[0]); exit(1);
            };
        }

        // filename
        else {
            if(fn != nullptr) { show_usage(argv[0]); exit(1); } fn = curr_arg;
        }
    }

    // parse selection variable
    bool include_leaves = false; bool include_internal = false; bool include_unlabeled = false;
    switch(selection) {
        case 'd': include_leaves = true;  include_internal = false; include_unlabeled = false; break;
        case 'l': include_leaves = true;  include_internal = true;  include_unlabeled = false; break;
        case 'a': include_leaves = true;  include_internal = true;  include_unlabeled = true;  break;
        case 'i': include_leaves = false; include_internal = true;  include_unlabeled = true;  break;
        case 'f': include_leaves = true;  include_internal = false; include_unlabeled = true;  break;
        default: throw std::invalid_argument(std::string("Invalid selection: ") + selection);
    };
    bool load_labels = print_labels || (!include_unlabeled);

    // parse tree and print lengths
    std::string tree_string; std::istream* infile; CT_NODE_T curr; CT_NODE_T num_nodes; CT_NODE_T num_nodes_minus_1;
    if(strcmp(fn, "-") == 0) { infile = &(std::cin); } else { infile = new std::ifstream(fn); } check_open(*infile, fn);
    while(getline(*infile, tree_string)) {
        compact_tree tree(tree_string, false, load_labels, true); num_nodes = tree.get_num_nodes(); num_nodes_minus_1 = num_nodes - 1;
        if(mode == 'm') {
            throw std::invalid_argument("Matrix mode is not yet implemented"); // TODO IMPLEMENT MATRIX MODE
        } else {
            // calculate distances
            std::vector<double> dists(num_nodes, 0.);
            if       (mode == 'p') { // parent mode
                for(curr = ROOT_NODE; curr < num_nodes; ++curr) { dists[curr] = tree.get_edge_length(curr); }
            } else if(mode == 'r') { // root mode
                for(curr = (CT_NODE_T)1; curr < num_nodes; ++curr) { dists[curr] = dists[tree.get_parent(curr)] + tree.get_edge_length(curr); }
            } else if(mode == 'l') { // LCA mode
                throw std::invalid_argument("LCA mode is not yet implemented"); // TODO IMPLEMENT LCA MODE
            } else { throw std::invalid_argument(std::string("Invalid mode: ") + mode); }

            // if printing labels AND tabular output, print tab-delimited labels first
            if(print_labels && t_arg) {
                for(curr = ROOT_NODE; curr < num_nodes; ++curr) {
                    if((include_leaves && include_internal) ? true : (tree.is_leaf(curr) ? include_leaves : include_internal)) {
                        std::cout << tree.get_label(curr) << ((curr != num_nodes_minus_1) ? '\t' : '\n');
                    }
                }
            }

            // print distances
            for(curr = ROOT_NODE; curr < num_nodes; ++curr) {
                if((include_leaves && include_internal) ? true : (tree.is_leaf(curr) ? include_leaves : include_internal)) {
                    if(print_labels && (!t_arg)) { std::cout << tree.get_label(curr) << '\t'; }
                    std::cout << dists[curr] << ((t_arg && (curr != num_nodes_minus_1)) ? '\t' : '\n');
                }
            }
        }
    }
    if(strcmp(fn, "-") != 0) { delete infile; } return 0;
}
