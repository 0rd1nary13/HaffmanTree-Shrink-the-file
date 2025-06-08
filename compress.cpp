#include "HCTree.hpp"
#include <vector>
#include <iostream>

int main(int argc, char** argv) {
    if(argc != 3) {
        error("Usage: ./compress infile outfile");
    }
    FancyInputStream in(argv[1]);
    FancyOutputStream out(argv[2]);
    if(!in.good() || !out.good()) {
        error("Failed to open files");
    }
    std::vector<int> freqs(256,0);
    int byte;
    while((byte = in.read_byte()) != -1) {
        freqs[byte]++;
    }
    for(int f : freqs) {
        out.write_int(f);
    }
    HCTree tree;
    tree.build(freqs);
    in.reset();
    while((byte = in.read_byte()) != -1) {
        tree.encode((unsigned char)byte, out);
    }
    out.flush();
    return 0;
}
