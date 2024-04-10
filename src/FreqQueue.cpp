#include "FreqQueue.h"
#include <iostream>
#include <algorithm>

FreqQueue::FreqQueue() : std::vector<Node*>() {
    this->max = 0;
}

void FreqQueue::sort() {
    std::sort(this->begin(), this->end(), 
        [](const Node* n1, const Node* n2) {
            return n2->count < n1->count;
        }
    );
}

Node* FreqQueue::has(Node::value_t v) {
    for (Node* n : *this) {
        if (n->value == v) return n;
    }
    return nullptr;
}

Node* FreqQueue::has(const Node* n) {
    return this->has(n->value);
}

void FreqQueue::push(Node* n) {
    Node* found = this->has(n);
    if (found == nullptr) {
       this->push_back(n);
    } else {
        found->count++;
        if (found->count > this->max) this->max = found->count;
    }
}

Node* FreqQueue::pop() {
    Node* n = this->back();
    this->pop_back();
    return n;
}