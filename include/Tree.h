#pragma once
#include <iostream>
#include <string>
#include "FreqQueue.h"
#include <map>

typedef std::map<std::string, std::string> BinMap;

class Tree {
    public:
    Tree(FreqQueue fq);
    BinMap symbolCodes();
    std::string search(const std::string binString);
    std::string eofBits;

    private:
    std::string search(Node* n, const std::string binString);
    BinMap symbolCodes(Node* n, std::string bits, BinMap& m);
    Node* root;
};