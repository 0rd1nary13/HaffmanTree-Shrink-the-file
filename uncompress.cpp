#include "HCTree.hpp"
#include <vector>
#include <iostream>

int main(int argc, char** argv) {
    if(argc != 3) {
        error("Usage: ./uncompress infile outfile");
    }
    FancyInputStream in(argv[1]);
    FancyOutputStream out(argv[2]);
    if(!in.good() || !out.good()) {
        error("Failed to open files");
    }
    std::vector<int> freqs(256);
    for(int i=0;i<256;i++) {
        freqs[i] = in.read_int();
    }
    HCTree tree;
    tree.build(freqs);
    int total = 0;
    for(int f : freqs) total += f;
    for(int i=0;i<total;i++) {
        unsigned char sym = tree.decode(in);
        out.write_byte(sym);
    }
    out.flush();
    return 0;
}
