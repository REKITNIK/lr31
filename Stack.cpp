#include "Stack.h"
#include <sstream>
using namespace std;

void initializeStack(Stack* stack) {
    if (!stack->list) stack->list = createFL();
    stack->size = 0;
}

void pushStack(Stack* stack, const string& data) {
    if (stack->size >= Stack::MAX_SIZE) {
        throw overflow_error("Переполнение стека");
    }
    if (!stack->list) stack->list = createFL();
    pushFrontFL(stack->list, data);
    stack->size++;
}

string popStack(Stack* stack) {
    if (!stack->list || !stack->list->head) {
        throw underflow_error("Стек пустой");
    }
    string val = frontFL(stack->list);
    popFrontFL(stack->list);
    stack->size--;
    return val;
}

string peekStack(const Stack* stack) {
    if (!stack->list || !stack->list->head) {
        throw underflow_error("Стек пустой");
    }
    return frontFL(stack->list);
}

bool isStackEmpty(const Stack* stack) {
    return !stack->list || isEmptyFL(stack->list);
}

bool isStackFull(const Stack* stack) {
    return stack->size >= Stack::MAX_SIZE;
}

size_t getStackSize(const Stack* stack) {
    return stack->size;
}

void clearStack(Stack* stack) {
    if (!stack->list) return;
    while (!isEmptyFL(stack->list)) popFrontFL(stack->list);
    stack->size = 0;
}

/**
 * Сериализация стека в строковое представление.
 * 
 * Формат: "S name count elem1 elem2 ..."
 * где elem1 - вершина стека (top), elemN - дно стека (bottom)
 * 
 * В ForwardList head указывает на первый элемент, который в контексте стека
 * является вершиной (top of stack).
 */
std::string Stack::serialize() const {
    std::ostringstream oss;
    oss << "S " << name << " " << size;
    if (list && list->head) {
        // В ForwardList head указывает на вершину стека
        FNode* cur = list->head;
        while (cur) {
            oss << " " << cur->key;
            cur = cur->next;
        }
    }
    return oss.str();
}

/**
 * Десериализация стека из строкового представления.
 * 
 * Читает строку вида "S name count elem1 elem2 ..." и восстанавливает состояние.
 * 
 * Ключевой момент: мы читаем элементы в порядке от вершины к дну (top->bottom),
 * а затем добавляем их в очередь с помощью pushBackFL. Это сохраняет правильный порядок,
 * потому что pushBackFL добавляет элемент в конец, а head указывает на первый элемент,
 * который был добавлен первым (вершина стека).
 */
void Stack::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string type;
    iss >> type; // Прочитываем тип 'S'
    iss >> name; // Прочитываем имя стека
    int count = 0;
    iss >> count; // Прочитываем количество элементов
    if (!list) list = createFL();
    
    // Очищаем существующий список
    while (!isEmptyFL(list)) popFrontFL(list);
    size = 0;
    
    // Читаем значения в порядке top->bottom, pushBackFL сохраняет правильный порядок:
    // 1. pushBackFL(elem1) -> list = [elem1]
    // 2. pushBackFL(elem2) -> list = [elem1, elem2]
    // 3. pushBackFL(elem3) -> list = [elem1, elem2, elem3]
    // Результат: head = elem1 (вершина), tail = elem3 (дно)
    for (int i = 0; i < count; ++i) {
        std::string v; iss >> v;
        pushBackFL(list, v);
        size++;
    }
}