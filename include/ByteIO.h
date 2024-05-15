#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <bitset>
#include "FreqQueue.h"

class FreqQueue;

typedef char byte;

template<typename FileType>
class ByteIO {
    protected:
    ByteIO(std::string fileName);
    virtual void close();
    virtual void open();

    std::string fileName;
    FileType file;
};

class ByteWriter : public ByteIO<std::ofstream> {
    public:
    ByteWriter(std::string fileName);
    ~ByteWriter();

    template<typename T, size_t size = sizeof(T)>
    void write(T data);
    void write(FreqQueue data);
};

class ByteReader : public ByteIO<std::ifstream> {
    public:
    ByteReader(std::string fileName);
    ~ByteReader();

    template<typename T, size_t size = sizeof(T)>
    T read();

    FreqQueue toFreqQueue();
    inline bool eof() {return this->file.eof();}
    void reset();
};

template<typename FileType>
ByteIO<FileType>::ByteIO(std::string fileName) {
    this->fileName = fileName;
    this->open();
}

template<typename FileType>
void ByteIO<FileType>::close() {
    if (this->file.is_open()) {
        this->file.close();
    }
}

// read data from the file of a specific size
template<typename T, size_t size = sizeof(T)>
T ByteReader::read() {
    T data;
    this->file.read((char*)(&data), size);
    return data;
}

// write data of a specific size to the file
template<typename T, size_t size = sizeof(T)>
void ByteWriter::write(T data) {
    const int dataBitsSize = size * 8;
    const int charBitsSize = sizeof(char) * 8;
    std::bitset<dataBitsSize> fullDataBits = std::bitset<dataBitsSize>(data);
    std::bitset<charBitsSize> byteBuffer;

    for (int i = 0, count = 0; i < dataBitsSize; ++i) {
        byteBuffer[count] = fullDataBits[i];
        if (count == charBitsSize-1) {
            char charByte = (char)std::stoi(byteBuffer.to_string().data(), nullptr, 2);
            this->file.write(&charByte, sizeof(char));
            count = 0;
        } else {
            ++count;
        }
    }
}