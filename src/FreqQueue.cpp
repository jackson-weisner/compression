#include "FreqQueue.h"
#include <iostream>
#include <algorithm>

FreqQueue::FreqQueue() : std::vector<Node*>() {
    this->max = 0;
}

// method to sort teh frequency queue
void FreqQueue::sort() {
    std::sort(this->begin(), this->end(), 
        [](const Node* n1, const Node* n2) {
            return n2->count < n1->count;
        }
    );
}

// method to check if the queue has a particular value
Node* FreqQueue::has(Node::value_t v) {
    for (Node* n : *this) {
        if (n->value == v) return n;
    }
    return nullptr;
}

// method to check if the queue has a particular node
Node* FreqQueue::has(const Node* n) {
    return this->has(n->value);
}

// push a node onto the frequency queue
void FreqQueue::push(Node* n) {
    Node* found = this->has(n);
    if (found == nullptr) {
       this->push_back(n);
    } else {
        found->count++;
        if (found->count > this->max) this->max = found->count;
    }
}

// pop the top of the queue
Node* FreqQueue::pop() {
    Node* n = this->back();
    this->pop_back();
    return n;
}