#include "Tree.h"
#include <string>

// constructor that populated the tree based on the frequency queue
Tree::Tree(FreqQueue fq) {
    Node *n1, *n2 = nullptr;
    while (fq.size() > 1) {
        n1 = fq.pop();
        n2 = fq.pop();
        fq.push(new Node(n1->value + n2->value, n1->count + n2->count, n1, n2));
        fq.sort();
    }
    this->eofBits = fq.eofSymbol;
    this->root = fq.pop();
}

// return a map of the symbol codes from the tree
BinMap Tree::symbolCodes() {
    BinMap m = {};
    return Tree::symbolCodes(this->root, "", m);
}

// recursive function to get the codes from the tree
BinMap Tree::symbolCodes(Node* n, std::string bits, BinMap& m) {
    if (n->left == nullptr && n->right == nullptr) {
        m[n->value] = bits;
        return m;
    }
    symbolCodes(n->left, bits+"0", m);
    symbolCodes(n->right, bits+"1", m);
    return m;
}

Node::value_t Tree::search(const std::string binString) {
    return this->search(this->root, binString);
}

Node::value_t Tree::search(Node* n, const std::string binString) {
    if (n->left == nullptr || n->right == nullptr) {
        return n->value;
    } else if (binString.length() == 0) {
        return "";
    }
    std::string chopped = binString.substr(1, binString.length()-1);
    switch (binString[0]) {
        case '0':
            return this->search(n->left, chopped);
            break;
        case '1':
            return this->search(n->right, chopped);
            break;
        default:
            return "";
            break;
    }
}