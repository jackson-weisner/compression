#include "ByteIO.h"
#include <string>
#include <vector>

ByteWriter::ByteWriter(std::string fileName) : ByteIO<std::ofstream>(fileName) {}

ByteWriter::~ByteWriter() {
    this->close();
}

ByteReader::ByteReader(std::string fileName) : ByteIO<std::ifstream>(fileName) {}

ByteReader::~ByteReader() {
    this->close();
}

template<>
void ByteIO<std::ofstream>::open() {
    this->file.open(fileName, std::fstream::binary | std::fstream::out);
}

template<>
void ByteIO<std::ifstream>::open() {
    this->file.open(this->fileName, std::fstream::binary | std::fstream::in);
}

template<>
void ByteWriter::write<std::string>(std::string data) {
    std::string buffer = "";
    for (int i = 0; i < (int)data.length(); ++i) {
        if (i % 8 == 0 && i != 0) {
            write((char)std::stoi(buffer, nullptr, 2));
            buffer = "";
        }
        buffer += data[i];
    }
    while (buffer.length() < 8) {
        buffer.append("0");
    }
    write((char)std::stoi(buffer, nullptr, 2));
}

void ByteWriter::write(FreqQueue data) {
    char countByteSize = (data.max > INT8_MAX) ? 4 : 1; 
    std::string binString = "";
    this->write(countByteSize);
    this->write(data.size());
    char eofSymbol = *data.eofSymbol.data();
    this->write<char>(eofSymbol);
    this->write<char, 1>(1);
    for (auto n : data) {
        this->write<char>(*n->value.data());
        if (countByteSize == 1) {
            this->write<char, 1>(n->count);
        } else {
            this->write<int, 4>(n->count);
        }
    }
}

FreqQueue ByteReader::toFreqQueue() {
    FreqQueue fq;
    std::string s = "";
    while (true) {
        s += this->read<char>();
        fq.push(new Node(s, 1));
        if (this->file.eof()) {
            fq.eofSymbol = s;
            break;
        }
        s = "";
    }
    return fq;
}

void ByteReader::reset() {
    this->close();
    this->open();
}