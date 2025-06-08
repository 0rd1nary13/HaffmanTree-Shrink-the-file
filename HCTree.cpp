#include "HCTree.hpp"
#include <stack>

HCTree::~HCTree() {
    // Recursively delete all nodes
    if(!root) return;
    std::stack<HCNode*> stk;
    stk.push(root);
    while(!stk.empty()) {
        HCNode* node = stk.top();
        stk.pop();
        if(node->c0) stk.push(node->c0);
        if(node->c1) stk.push(node->c1);
        delete node;
    }
}

void HCTree::build(const vector<int>& freqs) {
    // clear any existing tree
    if(root){
        std::stack<HCNode*> st; st.push(root); while(!st.empty()){ HCNode* n = st.top(); st.pop(); if(n->c0) st.push(n->c0); if(n->c1) st.push(n->c1); delete n; } }
    root = nullptr;
    leaves.assign(256, nullptr);
    std::priority_queue<HCNode*, std::vector<HCNode*>, HCNodePtrComp> pq;
    for(int i=0;i<256;i++) {
        if(freqs[i] > 0) {
            HCNode* n = new HCNode(freqs[i], (unsigned char)i);
            leaves[i] = n;
            pq.push(n);
        }
    }
    if(pq.empty()) return;
    if(pq.size()==1) {
        root = pq.top();
        return;
    }
    while(pq.size() > 1) {
        HCNode* n1 = pq.top(); pq.pop();
        HCNode* n2 = pq.top(); pq.pop();
        unsigned char sym = n1->symbol < n2->symbol ? n1->symbol : n2->symbol;
        HCNode* parent = new HCNode(n1->count + n2->count, sym);
        parent->c0 = n1; parent->c1 = n2;
        n1->p = parent; n2->p = parent;
        pq.push(parent);
    }
    root = pq.top();
}

void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const {
    HCNode* node = leaves[(int)symbol];
    if(!node) return; // should not happen
    std::vector<int> bits;
    while(node->p) {
        if(node->p->c0 == node) bits.push_back(0);
        else bits.push_back(1);
        node = node->p;
    }
    for(auto it = bits.rbegin(); it != bits.rend(); ++it) {
        out.write_bit(*it);
    }
}

unsigned char HCTree::decode(FancyInputStream & in) const {
    if(!root) return 0;
    HCNode* node = root;
    if(!node->c0 && !node->c1) {
        return node->symbol;
    }
    int bit;
    while(node->c0 || node->c1) {
        bit = in.read_bit();
        if(bit == -1) break; // should not happen
        node = bit==0 ? node->c0 : node->c1;
        if(!node->c0 && !node->c1) break;
    }
    return node->symbol;
}

