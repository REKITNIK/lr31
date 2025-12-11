#include "DoubleList.h"
#include <sstream>

using namespace std;

DFList* createDFList() {
    DFList* l = new DFList();
    l->head = nullptr;
    l->tail = nullptr;
    l->length = 0;
    return l;
}

void deleteListDFList(DFList* list) {
    if (!list) return;
    
    DFNode* current = list->head;
    while (current) {
        DFNode* next = current->next;
        delete current;
        current = next;
    }
    delete list;
}

void validateIndex(const DFList* list, int index, bool allowEnd = false) {
    if (index < 0) {
        throw out_of_range("Index less than 0");
    }
    
    if (allowEnd) {
        if (index > list->length) {
            throw out_of_range("Index is greater than list length");
        }
    } else {
        if (index >= list->length) {
            throw out_of_range("Index out of range");
        }
    }
}

DFNode* getNodeAt(const DFList* list, int index) {
    validateIndex(list, index, false);
    
    if (index <= static_cast<int>(list->length / 2)) {
        DFNode* current = list->head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        return current;
    } else {
        DFNode* current = list->tail;
        for (int i = list->length - 1; i > index; i--) {
            current = current->prev;
        }
        return current;
    }
}

void addNodeAfterDFList(DFList* list, const string& key, int index) {
    validateIndex(list, index, true);
    
    if (index == list->length) {
        addNodeTailDFList(list, key);
        return;
    }
    
    if (list->length == 0) {
        addNodeHeadDFList(list, key);
        return;
    }
    
    DFNode* newNode = new DFNode{key, nullptr, nullptr};
    
    if (index == list->length - 1) {
        newNode->prev = list->tail;
        newNode->next = nullptr;
        list->tail->next = newNode;
        list->tail = newNode;
    } else {
        DFNode* current = getNodeAt(list, index);
        newNode->next = current->next;
        newNode->prev = current;
        
        if (current->next) {
            current->next->prev = newNode;
        }
        current->next = newNode;
    }
    
    list->length++;
}

void addNodeBeforeDFList(DFList* list, const string& key, int index) {
    validateIndex(list, index, false);
    
    if (index == 0) {
        addNodeHeadDFList(list, key);
    } else {
        DFNode* newNode = new DFNode{key, nullptr, nullptr};
        DFNode* current = getNodeAt(list, index);
        
        newNode->prev = current->prev;
        newNode->next = current;
        
        if (current->prev) {
            current->prev->next = newNode;
        } else {
            list->head = newNode;
        }
        current->prev = newNode;
        
        list->length++;
    }
}

void addNodeHeadDFList(DFList* list, const string& key) {
    DFNode* newNode = new DFNode{key, list->head, nullptr};
    
    if (list->head) {
        list->head->prev = newNode;
    } else {
        list->tail = newNode;
    }
    list->head = newNode;
    list->length++;
}

void addNodeTailDFList(DFList* list, const string& key) {
    DFNode* newNode = new DFNode{key, nullptr, list->tail};
    
    if (list->tail) {
        list->tail->next = newNode;
    } else {
        list->head = newNode;
    }
    list->tail = newNode;
    list->length++;
}

void deleteNodeAtDFList(DFList* list, int index) {
    validateIndex(list, index, false);
    
    if (index == 0) {
        deleteNodeHeadDFList(list);
    } else if (index == list->length - 1) {
        deleteNodeTailDFList(list);
    } else {
        DFNode* toDelete = getNodeAt(list, index);
        toDelete->prev->next = toDelete->next;
        toDelete->next->prev = toDelete->prev;
        delete toDelete;
        list->length--;
    }
}

void deleteNodeHeadDFList(DFList* list) {
    if (!list->head) return;
    
    DFNode* toDelete = list->head;
    list->head = toDelete->next;
    
    if (list->head) {
        list->head->prev = nullptr;
    } else {
        list->tail = nullptr;
    }
    
    delete toDelete;
    list->length--;
}

void deleteNodeTailDFList(DFList* list) {
    if (!list->tail) return;
    
    DFNode* toDelete = list->tail;
    list->tail = toDelete->prev;
    
    if (list->tail) {
        list->tail->next = nullptr;
    } else {
        list->head = nullptr;
    }
    
    delete toDelete;
    list->length--;
}

void deleteNodeByValueDFList(DFList* list, const string& key) {
    DFNode* current = list->head;
    int index = 0;
    
    while (current) {
        if (current->key == key) {
            deleteNodeAtDFList(list, index);
            return;
        }
        current = current->next;
        index++;
    }
    throw runtime_error("Ключ не найден");
}

void deleteNodesBeforeIndex(DFList* list, int index) {
    if (!list->head) {
        throw runtime_error("Список пустой");
    }
    if (index <= 0) return;
    if (index >= static_cast<int>(list->length)) {
        // удаление всех узлов
        DFNode* cur = list->head;
        while (cur) {
            DFNode* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
        list->head = list->tail = nullptr;
        list->length = 0;
        return;
    }

    // удаление первых 'index' узлов
    int toDelete = index;
    DFNode* cur = list->head;
    while (toDelete > 0 && cur) {
        DFNode* nxt = cur->next;
        delete cur;
        cur = nxt;
        toDelete--;
        list->length--;
    }
    list->head = cur;
    if (list->head) list->head->prev = nullptr;
}

// Удаление всех узлов ПОСЛЕ указанного индекса (не включая сам узел с индексом)
void deleteNodesAfterIndex(DFList* list, int index) {
    if (!list->head) {
        throw runtime_error("Список пустой");
    }
    if (index < 0) return;
    if (index >= static_cast<int>(list->length) - 1) {
        // нет узлов после индекса
        return;
    }

    // поиск узла по индексу
    DFNode* cur = list->head;
    for (int i = 0; i < index && cur; ++i) cur = cur->next;
    if (!cur) return;

    // удаление узлов после 'cur'
    DFNode* toDel = cur->next;
    while (toDel) {
        DFNode* nxt = toDel->next;
        delete toDel;
        list->length--;
        toDel = nxt;
    }
    cur->next = nullptr;
    list->tail = cur;
}

// Альтернативная версия: просто удаление головы без возврата значения
void deleteHeadOnlyDFList(DFList* list) {
    if (!list || !list->head) {
        throw runtime_error("Список пуст или не инициализирован");
    }
    
    deleteNodeHeadDFList(list);
}

// Удаление узлов в диапазоне [start, end] (включая оба конца)
void deleteNodesFromTo(DFList* list, int start, int end) {
    if (start < 0 || end >= static_cast<int>(list->length) || start > end) {
        throw out_of_range("Неверный диапазон индексов");
    }
    
    // Если нужно удалить с начала, используем deleteNodesBeforeIndex
    if (start == 0) {
        deleteNodesBeforeIndex(list, end + 1);
        return;
    }
    
    // Получаем узел, предшествующий диапазону
    DFNode* beforeStart = getNodeAt(list, start - 1);
    
    // Получаем узел, следующий за диапазоном
    DFNode* afterEnd = (end == static_cast<int>(list->length) - 1) 
        ? nullptr 
        : getNodeAt(list, end + 1);
    
    // Удаляем узлы в диапазоне
    DFNode* current = beforeStart->next;
    int deleteCount = end - start + 1;
    
    while (deleteCount > 0 && current) {
        DFNode* toDelete = current;
        current = current->next;
        delete toDelete;
        deleteCount--;
        list->length--;
    }
    
    // Связываем оставшиеся части списка
    beforeStart->next = afterEnd;
    if (afterEnd) {
        afterEnd->prev = beforeStart;
    } else {
        list->tail = beforeStart;
    }
}

// Функция очистки всего списка
void clearDFList(DFList* list) {
    deleteNodesBeforeIndex(list, list->length);
}

std::string DFList::serialize() const {
    std::ostringstream oss;
    oss << "L " << name << " " << length;
    DFNode* cur = head;
    while (cur) {
        oss << " " << cur->key;
        cur = cur->next;
    }
    return oss.str();
}

void DFList::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string type;
    iss >> type; // L
    iss >> name;
    int count = 0; iss >> count;
    // clear
    DFNode* cur = head;
    while (cur) {
        DFNode* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
    head = tail = nullptr; length = 0;
    for (int i = 0; i < count; ++i) {
        std::string v; iss >> v;
        addNodeTailDFList(this, v);
    }
}

string getElementDFList(const DFList* list, int index) {
    return getNodeAt(list, index)->key;
}

string popElementDFList(DFList* list, int index) {
    string value = getElementDFList(list, index);
    deleteNodeAtDFList(list, index);
    return value;
}

DFNode* findNodeByValueDFList(const DFList* list, const string& key) {
    DFNode* current = list->head;
    while (current) {
        if (current->key == key) return current;
        current = current->next;
    }
    return nullptr;
}

bool isEmptyDFList(const DFList* list) {
    return list->length == 0;
}

size_t getLengthDFList(const DFList* list) {
    return list->length;
}
