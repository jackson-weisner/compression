#pragma once
#include <vector>
#include <string>
#include "ByteIO.h"

struct Node {
    typedef std::string value_t;
    typedef unsigned long long count_t;

    value_t value;
    count_t count;
    Node *left, *right;

    Node(value_t value, count_t count) {
        this->value = value;
        this->count = count;
        this->left = nullptr;
        this->right = nullptr;
    }
    Node(value_t value, count_t count, Node* left, Node* right) : Node(value, count) {
        this->left = left;
        this->right = right;
    }
    Node() {}
};

struct FreqQueue : public std::vector<Node*> {
    FreqQueue();
    inline int size() {return std::vector<Node*>::size();}
    void push(Node* n);
    void sort();
    Node* pop();
    Node* has(Node::value_t v);
    Node* has(const Node* n);
    Node::count_t max;
    Node::value_t eofSymbol;
}; 