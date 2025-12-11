#include "FileIO.h"
#include "Factory.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <vector>
#include <map>

static bool dirExists(const std::string& path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) return false;
#if defined(_WIN32)
    return (info.st_mode & _S_IFDIR) != 0;
#else
    return S_ISDIR(info.st_mode);
#endif
}

static void makeDir(const std::string& path) {
#if defined(_WIN32)
    _mkdir(path.c_str());
#else
    mkdir(path.c_str(), 0755);
#endif
}

static void mkdir_recursive(const std::string& dir) {
    if (dir.empty()) return;
    if (dirExists(dir)) return;
    std::string accum;
    for (size_t i = 0; i < dir.size(); ++i) {
        char c = dir[i];
        accum.push_back(c);
        if (c == '/' || c == '\\') {
            if (!accum.empty() && !dirExists(accum)) makeDir(accum);
        }
    }
    if (!accum.empty() && !dirExists(accum)) makeDir(accum);
}

void ensureDirectoryExists(const std::string& filename) {
    size_t pos1 = filename.find_last_of('/');
    size_t pos2 = filename.find_last_of('\\');
    size_t pos = std::string::npos;
    if (pos1 != std::string::npos && pos2 != std::string::npos) pos = std::max(pos1, pos2);
    else if (pos1 != std::string::npos) pos = pos1;
    else if (pos2 != std::string::npos) pos = pos2;
    if (pos == std::string::npos) return;
    std::string dir = filename.substr(0, pos);
    mkdir_recursive(dir);
}

bool fileExists(const std::string& filename) {
    std::ifstream f(filename);
    return f.good();
}

std::string getStructureType(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file");
    }

    std::string line;
    std::getline(file, line);
    file.close();

    if (line.empty()) {
        throw std::runtime_error("Empty file");
    }

    // Формат: M name count val1 val2 ...
    std::istringstream iss(line);
    std::string typeChar;
    iss >> typeChar;
    
    if (typeChar == "M") return "Array";
    if (typeChar == "F") return "ForwardList";
    if (typeChar == "L") return "DoubleList";
    if (typeChar == "S") return "Stack";
    if (typeChar == "Q") return "Queue";
    if (typeChar == "T") return "FBTree";
    
    throw std::runtime_error("Unknown type: " + typeChar);
}

// === ARRAY ===
void saveArrayToFile(const std::string& filename, const Array& array) {
    ensureDirectoryExists(filename);
    std::ofstream file(filename, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing");
    }

    // Формат: M name count val1 val2 val3...
    file << "M array " << array.len;
    for (std::size_t i = 0; i < array.len; ++i) {
        file << " " << getElementArray(const_cast<Array*>(&array), i);
    }
    file << std::endl;
    file.close();
}

Array* loadArrayFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file");
    }

    std::string line;
    std::getline(file, line);
    
    std::istringstream iss(line);
    std::string typeChar, name;
    int count;
    iss >> typeChar >> name >> count;
    
    if (typeChar != "M") {
        throw std::runtime_error("Not an array file");
    }

    Array* mArray = new Array;
    createArray(mArray, 10);

    for (int i = 0; i < count; ++i) {
        std::string value;
        iss >> value;
        if (i >= (int)mArray->size) {
            extendArray(mArray);
        }
        setKeyArray(mArray, value, i);
    }
    mArray->len = count;
    file.close();
    return mArray;
}

// === FORWARD LIST ===
void saveForwardListToFile(const std::string& filename, const ForwardList& list) {
    ensureDirectoryExists(filename);
    std::ofstream file(filename, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing");
    }

    int count = 0;
    FNode* cur = list.head;
    while (cur != nullptr) {
        count++;
        cur = cur->next;
    }

    file << "F list " << count;
    cur = list.head;
    while (cur != nullptr) {
        file << " " << cur->key;
        cur = cur->next;
    }
    file << std::endl;
    file.close();
}

ForwardList* loadForwardListFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file");
    }

    std::string line;
    std::getline(file, line);
    
    std::istringstream iss(line);
    std::string typeChar, name;
    int count;
    iss >> typeChar >> name >> count;
    
    if (typeChar != "F") {
        throw std::runtime_error("Not a forward list file");
    }

    ForwardList* list = createFL();
    for (int i = 0; i < count; ++i) {
        std::string value;
        iss >> value;
        pushBackFL(list, value);
    }
    file.close();
    return list;
}

// === DOUBLE LIST ===
void saveDoubleListToFile(const std::string& filename, const DFList& list) {
    ensureDirectoryExists(filename);
    std::ofstream file(filename, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing");
    }

    int count = 0;
    DFNode* cur = list.head;
    while (cur != nullptr) {
        count++;
        cur = cur->next;
    }

    file << "L dlist " << count;
    cur = list.head;
    while (cur != nullptr) {
        file << " " << cur->key;
        cur = cur->next;
    }
    file << std::endl;
    file.close();
}

DFList* loadDoubleListFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file");
    }

    std::string line;
    std::getline(file, line);
    
    std::istringstream iss(line);
    std::string typeChar, name;
    int count;
    iss >> typeChar >> name >> count;
    
    if (typeChar != "L") {
        throw std::runtime_error("Not a double list file");
    }

    DFList* mList = createDFList();
    for (int i = 0; i < count; ++i) {
        std::string value;
        iss >> value;
        addNodeTailDFList(mList, value);
    }
    file.close();
    return mList;
}

// === STACK ===
void saveStackToFile(const std::string& filename, const Stack& mStack) {
    ensureDirectoryExists(filename);
    std::ofstream file(filename, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing");
    }

    int count = 0;
    if (mStack.list && mStack.list->head) {
        FNode* cur = mStack.list->head;
        while (cur) { count++; cur = cur->next; }
    }

    file << "S st " << count;
    if (mStack.list && mStack.list->head) {
        FNode* cur = mStack.list->head;
        while (cur) {
            file << " " << cur->key;
            cur = cur->next;
        }
    }
    file << std::endl;
    file.close();
}

Stack* loadStackFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file");
    }

    std::string line;
    std::getline(file, line);
    std::istringstream iss(line);
    std::string typeChar;
    iss >> typeChar;
    if (typeChar != "S") {
        throw std::runtime_error("Not a stack file");
    }
    Stack* mStack = new Stack;
    mStack->deserialize(line);
    file.close();
    return mStack;
}

// === QUEUE ===
void saveQueueToFile(const std::string& filename, const Queue& mQueue) {
    ensureDirectoryExists(filename);
    std::ofstream file(filename, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing");
    }

    file << "Q qu " << mQueue.size;
    if (mQueue.list && mQueue.list->head) {
        FNode* cur = mQueue.list->head;
        while (cur) {
            file << " " << cur->key;
            cur = cur->next;
        }
    }
    file << std::endl;
    file.close();
}

Queue* loadQueueFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file");
    }

    std::string line;
    std::getline(file, line);
    std::istringstream iss(line);
    std::string typeChar;
    iss >> typeChar;
    if (typeChar != "Q") {
        throw std::runtime_error("Not a queue file");
    }
    Queue* mQueue = createQueue();
    mQueue->deserialize(line);
    file.close();
    return mQueue;
}

// === TREE (Pre-Order) ===
void saveTreeToFile(const std::string& filename, const BTree& tree) {
    ensureDirectoryExists(filename);
    std::ofstream file(filename, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing");
    }

    // Подсчитываем узлы
    int count = 0;
    std::function<void(BNode*)> countNodes = [&](BNode* node) {
        if (node) {
            count++;
            countNodes(node->left);
            countNodes(node->right);
        }
    };
    countNodes(tree.root);

    file << "T tree " << count;

    // Pre-order: Root -> Left -> Right
    std::function<void(BNode*)> preOrder = [&](BNode* node) {
        if (node) {
            file << " " << node->key;
            preOrder(node->left);
            preOrder(node->right);
        }
    };
    preOrder(tree.root);
    file << std::endl;
    file.close();
}

BTree* loadTreeFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file");
    }

    std::string line;
    std::getline(file, line);
    
    std::istringstream iss(line);
    std::string typeChar, name;
    int count;
    iss >> typeChar >> name >> count;
    
    if (typeChar != "T") {
        throw std::runtime_error("Not a tree file");
    }

    BTree* tree = new BTree;
    
    // Парсим значения в Pre-order (Root -> Left -> Right)
    for (int i = 0; i < count; ++i) {
        int key;
        iss >> key;
        addNode(tree, key);
    }
    
    file.close();
    return tree;
}

// === UNIVERSAL ===
void* loadStructureFromFile(const std::string& filename, std::string& type) {
    // Backwards-compatible loader (unused by new DB loader)
    type = getStructureType(filename);

    if (type == "Array") {
        return loadArrayFromFile(filename);
    } else if (type == "ForwardList") {
        return loadForwardListFromFile(filename);
    } else if (type == "DoubleList") {
        return loadDoubleListFromFile(filename);
    } else if (type == "Stack") {
        return loadStackFromFile(filename);
    } else if (type == "Queue") {
        return loadQueueFromFile(filename);
    } else if (type == "FBTree") {
        return loadTreeFromFile(filename);
    } else {
        throw std::runtime_error("Unknown structure: " + type);
    }
}

// New: load entire database from file (one structure per line)
void loadDatabaseFromFile(const std::string& filename, std::map<std::string, Structure*>& database) {
    std::ifstream file(filename);
    if (!file.is_open()) return;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        char typeChar; iss >> typeChar;
        std::string name; iss >> name;
        // create object
        Structure* obj = createStructure(typeChar);
        if (!obj) continue;
        obj->deserialize(line);
        obj->name = name;
        database[name] = obj;
    }
    file.close();
}

// New: save entire database to file (overwrite)
void saveDatabaseToFile(const std::string& filename, const std::map<std::string, Structure*>& database) {
    ensureDirectoryExists(filename);
    std::ofstream file(filename, std::ios::out | std::ios::trunc);
    if (!file.is_open()) return;
    for (const auto& kv : database) {
        Structure* obj = kv.second;
        if (!obj) continue;
        std::string line = obj->serialize();
        file << line << std::endl;
    }
    file.close();
}

void saveStructureToFile(const std::string& filename, const std::string& type, void* structure) {
    if (type == "Array") {
        saveArrayToFile(filename, *static_cast<Array*>(structure));
    } else if (type == "ForwardList") {
        saveForwardListToFile(filename, *static_cast<ForwardList*>(structure));
    } else if (type == "DoubleList") {
        saveDoubleListToFile(filename, *static_cast<DFList*>(structure));
    } else if (type == "Stack") {
        saveStackToFile(filename, *static_cast<Stack*>(structure));
    } else if (type == "Queue") {
        saveQueueToFile(filename, *static_cast<Queue*>(structure));
    } else if (type == "FBTree") {
        saveTreeToFile(filename, *static_cast<BTree*>(structure));
    } else {
        throw std::runtime_error("Unknown structure: " + type);
    }
}
