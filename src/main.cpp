#include <iostream>
#include "ByteIO.h"
#include "Tree.h"
#include <algorithm>
#include <string>

const bool DEBUG = false;

inline std::string readSymbol(ByteReader& fileInput) {
    return std::string(1, fileInput.read<char>()); 
}

void compress(std::string from, std::string to) {
    ByteReader fileInput(from);
    ByteWriter fileOutput(to);

    FreqQueue fq = fileInput.toFreqQueue();
    Tree t(fq);
    BinMap m = t.symbolCodes();

    fileInput.reset();
    fileOutput.write(fq);
    if (DEBUG) {
        for (auto a : fq) {
            std::cout << a->value << " " << a->count << std::endl;
        }
    }

    const short outputBufferLength = 8;
    std::string symbol, outputString = "";

    while (!fileInput.eof()) {
        symbol = readSymbol(fileInput); 
        outputString += m[symbol];
        if (outputString.length() > outputBufferLength) {
            fileOutput.write(outputString.substr(0, outputBufferLength));
            outputString = outputString.substr(outputBufferLength, outputString.length()-outputBufferLength);
        }
    }
    symbol = readSymbol(fileInput);
    outputString += m[symbol];
    fileOutput.write(outputString);
}

void decompress(std::string from, std::string to) {
    ByteReader fileInput(from);
    FreqQueue fq;

    int countByteSize = (int)fileInput.read<char>();
    int symbolCount = fileInput.read<int>();
    fq.eofSymbol = readSymbol(fileInput);

    fileInput.read<char>();
    Node::count_t nodeCount;
    Node::value_t value;
    for (int i = 0; i < symbolCount; ++i) {
        value = readSymbol(fileInput);
        if (countByteSize == 1) {
            nodeCount = (Node::count_t)fileInput.read<char>();
        } else {
            nodeCount = (Node::count_t)fileInput.read<int>();
        }
        fq.push(new Node(value, nodeCount));
    }
    if (DEBUG) {
        for (auto a : fq) {
            std::cout << a->value << " " << a->count << std::endl;
        }
    }

    Tree t(fq);
    BinMap m = t.symbolCodes();
    
    std::bitset<8> charBits;
    std::string binString, searchString = "";
    std::cout << "\n\n============\n";
    while (!fileInput.eof()) {
        charBits = std::bitset<8>(fileInput.read<char>());
        for (int j = 7; j >= 0; --j) {
            binString += ((charBits[j] == 0) ? '0' : '1');
            if (binString == m[t.eofBits]) {
                return;
            }
            Node::value_t found = t.search(binString);
            if (found != "") {
                std::cout << found;
                binString = "";
            }
        }
    }
}

int main(int argc, char** argv) {
    compress(argv[1], argv[2]);
    decompress(argv[2], "test");
    return 0;
}