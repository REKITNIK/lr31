
#include "ForwardList.h"
#include <sstream>
using namespace std;

ForwardList* createFL() {
    ForwardList* fl = new ForwardList();
    fl->head = nullptr;
    fl->tail = nullptr;
    fl->size = 0;
    return fl;
}

void deleteFL(ForwardList* list) {
    if (!list) return;
    
    FNode* current = list->head;
    while (current) {
        FNode* next = current->next;
        delete current;
        current = next;
    }
    delete list;
}

void validatePosition(const ForwardList* list, int position, bool allowEnd = false) {
    int maxPos = allowEnd ? list->size : list->size - 1;
    if (position < 0 || position > maxPos) {
        throw out_of_range("Index out of range");
    }
}

FNode* getNodeAt(const ForwardList* list, int position) {
    validatePosition(list, position, false);
    
    FNode* current = list->head;
    for (int i = 0; i < position; i++) {
        current = current->next;
    }
    return current;
}

void pushBackFL(ForwardList* list, const string& key) {
    FNode* newNode = new FNode{key, nullptr};
    
    if (!list->head) {
        list->head = list->tail = newNode;
    } else {
        list->tail->next = newNode;
        list->tail = newNode;
    }
    list->size++;
}

void pushFrontFL(ForwardList* list, const string& key) {
    FNode* newNode = new FNode{key, list->head};
    
    if (!list->head) {
        list->tail = newNode;
    }
    list->head = newNode;
    list->size++;
}

void insertBeforeFL(ForwardList* list, const string& key, int position) {
    if (position == 0) {
        pushFrontFL(list, key);
    }
    else if (position == list->size) {
        pushBackFL(list, key);
    }
    else {
        validatePosition(list, position, false);
        FNode* prev = getNodeAt(list, position - 1);
        FNode* newNode = new FNode{key, prev->next};
        prev->next = newNode;
        list->size++;
    }
}

void insertAfterFL(ForwardList* list, const string& key, int position) {
    validatePosition(list, position, false);
    
    if (position == list->size - 1) {
        pushBackFL(list, key);
        return;
    }
    
    FNode* current = getNodeAt(list, position);
    FNode* newNode = new FNode{key, current->next};
    current->next = newNode;
    if (!newNode->next) {
        list->tail = newNode;
    }
    
    list->size++;
}

void popFrontFL(ForwardList* list) {
    if (!list->head) {
        throw runtime_error("Список пустой");
    }
    
    FNode* temp = list->head;
    list->head = list->head->next;
    
    if (!list->head) {
        list->tail = nullptr;
    }
    
    delete temp;
    list->size--;
}

void popBackFL(ForwardList* list) {
    if (!list->head) {
        throw runtime_error("List is empty");
    }
    
    if (list->head == list->tail) {
        delete list->head;
        list->head = list->tail = nullptr;
    } else {
        FNode* current = list->head;
        while (current->next != list->tail) {
            current = current->next;
        }
        
        delete list->tail;
        current->next = nullptr;
        list->tail = current;
    }
    list->size--;
}

void removeBeforeFL(ForwardList* list, FNode* nextNode) {
    if (!nextNode) {
        throw invalid_argument("Нода не существует");
    }

    if (nextNode == list->head) {
        throw invalid_argument("Нельзя удалить перед головой списка");
    }
    
    if (list->head->next == nextNode) {
        FNode* toDelete = list->head;
        list->head = nextNode;
        delete toDelete;
        list->size--;
        return;
    }
  
    FNode* current = list->head;
    while (current && current->next && current->next->next != nextNode) {
        current = current->next;
    }
  
    if (!current || !current->next) {
        throw invalid_argument("Не удалось найти элемент для удаления");
    }
    
    FNode* toDelete = current->next;
    current->next = nextNode;
    
    delete toDelete;
    list->size--;
}

void removeAfterFL(ForwardList* list, FNode* prevNode) {
    if (!prevNode || !prevNode->next) {
        throw invalid_argument("Нода не имеет дочерней. Нечего удалять");
    }
    
    FNode* toDelete = prevNode->next;
    prevNode->next = toDelete->next;
    
    if (toDelete == list->tail) {
        list->tail = prevNode;
    }
    
    delete toDelete;
    list->size--;
}

bool removeByValueFL(ForwardList* list, const string& value) {
    if (!list->head) return false;
    
    if (list->head->key == value) {
        popFrontFL(list);
        return true;
    }
    
    FNode* current = list->head;
    while (current->next && current->next->key != value) {
        current = current->next;
    }
    
    if (current->next) {
        FNode* toDelete = current->next;
        current->next = toDelete->next;
        
        if (toDelete == list->tail) {
            list->tail = current;
        }
        
        delete toDelete;
        list->size--;
        return true;
    }
    
    return false;
}

FNode* findByValueFL(ForwardList* list, const string& value) {
    FNode* current = list->head;
    while (current) {
        if (current->key == value) return current;
        current = current->next;
    }
    return nullptr;
}

string frontFL(const ForwardList* list) {
    if (!list->head) {
        throw runtime_error("Список пустой");
    }
    return list->head->key;
}

string backFL(const ForwardList* list) {
    if (!list->tail) {
        throw runtime_error("Список пустой");
    }
    return list->tail->key;
}

string getAtFL(const ForwardList* list, size_t index) {
    return getNodeAt(list, index)->key;
}

bool isEmptyFL(const ForwardList* list) {
    return !list->head;
}

size_t getSizeFL(const ForwardList* list) {
    return list->size;
}

std::string ForwardList::serialize() const {
    std::ostringstream oss;
    oss << "F " << name << " " << size;
    FNode* cur = head;
    while (cur) {
        oss << " " << cur->key;
        cur = cur->next;
    }
    return oss.str();
}

void ForwardList::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string type;
    iss >> type; // F
    iss >> name;
    int count = 0; iss >> count;
    // очистка
    FNode* cur = head;
    while (cur) {
        FNode* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
    head = tail = nullptr; size = 0;
    for (int i = 0; i < count; ++i) {
        std::string v; iss >> v;
        pushBackFL(this, v);
    }
}
