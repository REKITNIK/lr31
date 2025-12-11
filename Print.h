#ifndef PRINT_H
#define PRINT_H

#include <iostream>
#include <string>
#include "Stack.h"
#include "Queue.h"
#include "ForwardList.h"
#include "DoubleList.h"
#include "Array.h"
#include "FullBinaryTree.h"

inline void PRINT(const Stack& stack) {
    std::cout << "Stack (size: " << stack.size << "): [";
    if (stack.list) {
        FNode* current = stack.list->head;
        while (current != nullptr) {
            std::cout << current->key;
            if (current->next != nullptr) {
                std::cout << ", ";
            }
            current = current->next;
        }
    }
    std::cout << "]" << std::endl;
}

inline void PRINT(const Queue& queue) {
    std::cout << "Queue (size: " << queue.size << "): [";
    if (queue.list) {
        FNode* current = queue.list->head;
        while (current != nullptr) {
            std::cout << current->key;
            if (current->next != nullptr) {
                std::cout << ", ";
            }
            current = current->next;
        }
    }
    std::cout << "]" << std::endl;
}

inline void PRINT(const ForwardList& list) {
    std::cout << "ForwardList (size: " << list.size << "): [";
    FNode* current = list.head;
    while (current != nullptr) {
        std::cout << current->key;
        if (current->next != nullptr) {
            std::cout << ", ";
        }
        current = current->next;
    }
    std::cout << "]" << std::endl;
}

inline void PRINT(const DFList& list) {
    std::cout << "DoubleLinkedList (size: " << list.length << "): [";
    DFNode* current = list.head;
    while (current != nullptr) {
        std::cout << current->key;
        if (current->next != nullptr) {
            std::cout << ", ";
        }
        current = current->next;
    }
    std::cout << "]" << std::endl;
}

inline void PRINT(const Array& array) {
    std::cout << "Array (len: " << array.len << "): [";
    for (int i = 0; i < array.len; i++) {
        std::cout << (array.head + i)->data;
        if (i < array.len - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}

inline void printBTreeHelper(BNode* node) {
    if (node != nullptr) {
        printBTreeHelper(node->left);
        std::cout << node->key << " " << std::endl;
        printBTreeHelper(node->right);
    }
}

inline void PRINT(const BTree& tree) {
    std::cout << "FBTree: [";
    printBTreeHelper(tree.root);
    std::cout << "]" << std::endl;
}

#endif