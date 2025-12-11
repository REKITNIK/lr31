#include "FullBinaryTree.h"
#include <sstream>
#include <functional>

BNode* findPlaceNode(BNode* currentNode, int key) {
    if (currentNode == nullptr) {
        return nullptr;
    }
    if (currentNode->key > key) {
        return currentNode->left == nullptr ? currentNode : findPlaceNode(currentNode->left, key);
    }
    if (currentNode->key < key) {
        return currentNode->right == nullptr ? currentNode : findPlaceNode(currentNode->right, key);
    }
    else {return currentNode;}
}

void addNode(BTree* tree, int key) {
    BNode* newNode = new BNode;
    newNode->key = key;
    if (tree->root == nullptr) {tree->root = newNode;}
    else {
        BNode* currentNode = findPlaceNode(tree->root, key);
        if (currentNode->key == key) {throw std::runtime_error("Ключ уже существует в дереве");}
        if (currentNode->key > key) {
            currentNode->left = newNode;
            newNode->parent = currentNode;
        }
        if (currentNode->key < key) {
            currentNode->right = newNode;
            newNode->parent = currentNode;
        }
    }
}

BNode* findNode(const BTree& tree, int key) {
    if (tree.root == nullptr) {
        throw std::runtime_error("Дерево пустое");
    }
    BNode* currentNode = findPlaceNode(tree.root, key);
    if (currentNode == nullptr || currentNode->key != key) {
        throw std::runtime_error("Ключ не найден в дереве");
    }
    return currentNode;
}

BNode* findMinNode(BNode* currentNode) {
    return currentNode->left == nullptr ? currentNode : findMinNode(currentNode->left);
}

BNode* findMaxNode(BNode* currentNode) {
    return currentNode->right == nullptr ? currentNode : findMaxNode(currentNode->right);
}

BNode* findInOrderPredecessor(BNode* node) {
    if (node->left != nullptr) {
        return findMaxNode(node->left);
    }
    BNode* successor = nullptr;
    BNode* current = node;
    
    while (current->parent != nullptr) {
        if (current->parent->right == current) {
            successor = current->parent;
            break;
        }
        current = current->parent;
    }
    return successor;
}

BNode* findInOrderSuccessor(BNode* node) {
    if (node->right != nullptr) {
        return findMinNode(node->right);
    }
    BNode* successor = nullptr;
    BNode* current = node;
    
    while (current->parent != nullptr) {
        if (current->parent->left == current) {
            successor = current->parent;
            break;
        }
        current = current->parent;
    }
    return successor;
}

void deleteNode(BTree* tree, int key) {
    if (tree == nullptr || tree->root == nullptr) {
        throw std::runtime_error("Дерево пустое");
    }
    
    BNode* currentNode = findNode(*tree, key);
    
    if (currentNode->left == nullptr && currentNode->right == nullptr) {
        if (currentNode->parent != nullptr) {
            if (currentNode->parent->left == currentNode) {
                currentNode->parent->left = nullptr;
            } else {
                currentNode->parent->right = nullptr;
            }
        } else {
            tree->root = nullptr;
        }
        delete currentNode;
    }
    else if (currentNode->left != nullptr && currentNode->right == nullptr) {
        if (currentNode->parent != nullptr) {
            if (currentNode->parent->left == currentNode) {
                currentNode->parent->left = currentNode->left;
            } else {
                currentNode->parent->right = currentNode->left;
            }
        } else {
            tree->root = currentNode->left;
        }
        currentNode->left->parent = currentNode->parent;
        delete currentNode;
    }
    else if (currentNode->left == nullptr && currentNode->right != nullptr) {
        if (currentNode->parent != nullptr) {
            if (currentNode->parent->left == currentNode) {
                currentNode->parent->left = currentNode->right;
            } else {
                currentNode->parent->right = currentNode->right;
            }
        } else {
            tree->root = currentNode->right;
        }
        currentNode->right->parent = currentNode->parent;
        delete currentNode;
    }
    else {
        BNode* successor = findMinNode(currentNode->right);
        currentNode->key = successor->key;
        
        if (successor->parent != nullptr) {
            if (successor->parent->left == successor) {
                successor->parent->left = successor->right;
            } else {
                successor->parent->right = successor->right;
            }
        }
        
        if (successor->right != nullptr) {
            successor->right->parent = successor->parent;
        }
        delete successor;
    }
}

int countInnerNodes(BNode* currentNode) {
    if (currentNode == nullptr || (currentNode->left == nullptr && currentNode->right == nullptr)) {return 0;}
    return countInnerNodes(currentNode->left) + countInnerNodes(currentNode->right) + 1;
}

int countLeavesNodes(BNode* currentNode) {
    if (currentNode == nullptr) {return 0;}
    if (currentNode->left == nullptr && currentNode->right == nullptr) {return 1;}
    return countLeavesNodes(currentNode->left) + countLeavesNodes(currentNode->right);
}


bool isFullTree(const BTree& tree) {
    return countInnerNodes(tree.root) + 1 == countLeavesNodes(tree.root);
}

int tGet(BTree* tree, int key) {
    if (tree == nullptr || tree->root == nullptr) {
        throw std::runtime_error("Ключ не найден"); 
    }
    return (findNode(*tree, key))->key;
}


std::string BTree::serialize() const {
    // Первый проход: считаем количество узлов
    int count = 0;
    std::ostringstream oss;
    std::function<void(BNode*)> pre = [&](BNode* node) {
        if (!node) return;
        ++count;
        pre(node->left);
        pre(node->right);
    };
    pre(root);

    // Второй проход: выводим значения узлов в порядке pre-order
    std::function<void(BNode*)> preOut = [&](BNode* node) {
        if (!node) return;
        oss << node->key << " ";
        preOut(node->left);
        preOut(node->right);
    };
    // Build full line: T <name> <count> <vals...>
    std::ostringstream line;
    line << "T " << name << " " << count;
    preOut(root);
    std::string values = oss.str();
    if (!values.empty()) {
        // preOut wrote to oss incorrectly earlier; use a fresh traversal
        oss.str(""); oss.clear();
        preOut(root);
        values = oss.str();
        // trim trailing space
        if (!values.empty() && values.back() == ' ') values.pop_back();
        line << " " << values;
    }
    return line.str();
}

void BTree::deserialize(const std::string& data) {
    // data expected like: T <name> <count> <vals...>
    std::istringstream iss(data);
    std::string type;
    iss >> type; // T
    iss >> name;
    int count = 0;
    iss >> count;
    // очистка существующего дерева
    std::function<void(BNode*)> del = [&](BNode* node) {
        if (!node) return;
        del(node->left);
        del(node->right);
        delete node;
    };
    del(root);
    root = nullptr;

    for (int i = 0; i < count; ++i) {
        int key;
        iss >> key;
        addNode(const_cast<BTree*>(this), key);
    }
}
