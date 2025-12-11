#include "Array.h"
#include <sstream>

using namespace std;

void createArray(Array* emptyArray, int size) {
    if (size < 1) {
        throw runtime_error("Невозможно создать массив без выделения памяти");
    }
    if (emptyArray->head != nullptr) {
        delete[] emptyArray->head;
    }
    
    emptyArray->head = new ArNode[size]{};
    emptyArray->size = size;
    emptyArray->len = 0;
}

void extendArray(Array* mArray) {
    if (mArray->size == 0) {
        createArray(mArray, 10);
        return;
    }
    
    int newSize = mArray->size * 2;
    ArNode* newHead = new ArNode[newSize]{};
    
    for (int i = 0; i < mArray->len; i++) {
        newHead[i].data = std::move(mArray->head[i].data);
    }
    
    delete[] mArray->head;
    mArray->head = newHead;
    mArray->size = newSize;
}

string getElementArray(const Array* mArray, int index) {
    return mArray->head[index].data;
}

void setKeyArray(Array* mArray, const string& key, int index) {
    mArray->head[index].data = key;
}

void deleteElementArray(Array* mArray, int index) {
    for (int i = index; i < mArray->len - 1; i++) {
        mArray->head[i].data = std::move(mArray->head[i + 1].data);
    }
    mArray->len--;
}

void addElementIndexArray(Array* mArray, const string& key, int index) {
    if (index < 0 || index > mArray->len) {
        throw std::out_of_range("Index out of range");
    }
    
    if (mArray->len >= mArray->size) {
        extendArray(mArray);
    }
    
    for (int i = mArray->len; i > index; i--) {
        mArray->head[i].data = std::move(mArray->head[i - 1].data);
    }
    
    mArray->head[index].data = key;
    mArray->len++;
}

void addElementEndArray(Array* mArray, const string& key) {
    addElementIndexArray(mArray, key, mArray->len);
}

size_t getArrayLength(const Array* mArray) {
    return mArray->len;
}

std::string Array::serialize() const {
    std::ostringstream oss;
    oss << "M " << name << " " << len;
    for (int i = 0; i < len; ++i) {
        oss << " " << (head + i)->data;
    }
    return oss.str();
}

void Array::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string type;
    iss >> type; // Тип M
    iss >> name;
    int count = 0;
    iss >> count;
    // очистка существующих данных
    if (head) { delete[] head; head = nullptr; }
    createArray(const_cast<Array*>(this), std::max(10, count));
    for (int i = 0; i < count; ++i) {
        std::string v; iss >> v;
        setKeyArray(const_cast<Array*>(this), v, i);
    }
    len = count;
}
