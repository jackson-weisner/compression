#include <iostream>
#include "ByteIO.h"
#include "Tree.h"
#include <algorithm>
#include <string>

const bool DEBUG = false;

inline std::string readSymbol(ByteReader& fileInput) {
    return std::string(1, fileInput.read<char>()); 
}

// function to run the necessary steps for compression
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

    // read in all symbols from the input file
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

// steps for decompression
void decompress(std::string from, std::string to) {
    ByteReader fileInput(from);
    FreqQueue fq;

    int countByteSize = (int)fileInput.read<char>();
    int symbolCount = fileInput.read<int>();
    fq.eofSymbol = readSymbol(fileInput);

    fileInput.read<char>();
    Node::count_t nodeCount;
    Node::value_t value;
    // read all symbols from the file
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
    std::ofstream outFile(to);
    std::bitset<8> charBits;
    std::string binString, searchString = "";
    // output the characters to the decompressed file
    while (!fileInput.eof()) {
        charBits = std::bitset<8>(fileInput.read<char>());
        for (int j = 7; j >= 0; --j) {
            binString += ((charBits[j] == 0) ? '0' : '1');
            if (binString == m[t.eofBits]) {
                return;
            }
            Node::value_t found = t.search(binString);
            if (found != "") {
                outFile << found;
                binString = "";
            }
        }
    }
    outFile.close();
}

int main(int argc, char** argv) {
    std::string mode = argv[1];
    std::string from = argv[2];
    std::string to = argv[3];
    if (mode == "-c") {
        compress(from, to);
    } else if (mode == "-d") {
        decompress(from, to);
    }
    return 0;
}