#ifndef FL_H
#define FL_H

#include <stdexcept>
#include <algorithm>
#include <cstddef>
#include <string>

/**
 * @brief Узел односвязного списка.
 *
 * Хранит строковое значение и указатель на следующий узел.
 */
struct FNode {
    /** @brief Ключ/значение, хранящееся в узле */
    std::string key;
    /** @brief Указатель на следующий узел (nullptr если это последний узел) */
    FNode* next = nullptr;
};

#include "Structure.h"

/**
 * @brief Реализация структуры "Односвязный список".
 *
 * Список, в котором каждый узел содержит ссылку только на следующий узел.
 * Поддерживает вставку/удаление в начало, конец и по позиции.
 * Хранит оба указателя (head и tail) для оптимизации операций.
 */
struct ForwardList : public Structure {
    /** @brief Указатель на первый узел списка */
    FNode* head = nullptr;
    /** @brief Указатель на последний узел списка (для O(1) вставки в конец) */
    FNode* tail = nullptr;
    /** @brief Количество узлов в списке */
    std::size_t size = 0;
    
    ForwardList() = default;
    ~ForwardList() override {
        FNode* current = head;
        while (current) {
            FNode* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        tail = nullptr;
        size = 0;
    }
    
    /**
     * @brief Сериализует список в формат: "F name count elem1 elem2 ..."
     * @return Строка с сохраненным состоянием списка (от head к tail)
     */
    std::string serialize() const override;
    
    /**
     * @brief Десериализует список из строки формата "F name count elem1 elem2 ..."
     * @param data Строка с сохраненными данными списка
     */
    void deserialize(const std::string& data) override;
};

/**
 * @brief Создает новый пустой односвязный список.
 * @return Указатель на новый список (выделенный в heap)
 */
ForwardList* createFL();

/**
 * @brief Удаляет список и все его узлы, освобождая память.
 * @param list Указатель на список для удаления
 */
void deleteFL(ForwardList* list);

/**
 * @brief Добавляет элемент в конец списка.
 * @param list Указатель на список
 * @param key Значение добавляемого элемента
 */
void pushBackFL(ForwardList* list, const std::string& key);

/**
 * @brief Добавляет элемент в начало списка.
 * @param list Указатель на список
 * @param key Значение добавляемого элемента
 */
void pushFrontFL(ForwardList* list, const std::string& key);

/**
 * @brief Вставляет элемент перед узлом на заданной позиции.
 * @param list Указатель на список
 * @param key Значение вставляемого элемента
 * @param position Позиция, перед которой произойдет вставка
 */
void insertBeforeFL(ForwardList* list, const std::string& key, int position);

/**
 * @brief Вставляет элемент после узла на заданной позиции.
 * @param list Указатель на список
 * @param key Значение вставляемого элемента
 * @param position Позиция, после которой произойдет вставка
 */
void insertAfterFL(ForwardList* list, const std::string& key, int position);

/**
 * @brief Удаляет первый элемент списка.
 * @param list Указатель на список
 */
void popFrontFL(ForwardList* list);

/**
 * @brief Удаляет последний элемент списка.
 * @param list Указатель на список
 */
void popBackFL(ForwardList* list);

/**
 * @brief Удаляет узел, который находится после prevNode.
 * @param list Указатель на список
 * @param prevNode Узел, после которого нужно удалить элемент
 */
void removeAfterFL(ForwardList* list, FNode* prevNode);

/**
 * @brief Удаляет первый узел со значением value.
 * @param list Указатель на список
 * @param value Значение для поиска и удаления
 * @return true если узел был найден и удален, false если узел не найден
 */
bool removeByValueFL(ForwardList* list, const std::string& value);

/**
 * @brief Находит первый узел со значением value.
 * @param list Указатель на список
 * @param value Значение для поиска
 * @return Указатель на найденный узел или nullptr если не найден
 */
FNode* findByValueFL(ForwardList* list, const std::string& value);

/**
 * @brief Получает значение первого элемента списка.
 * @param list Указатель на список
 * @return Значение первого элемента
 * @throw std::underflow_error если список пуст
 */
std::string frontFL(const ForwardList* list);

/**
 * @brief Получает значение последнего элемента списка.
 * @param list Указатель на список
 * @return Значение последнего элемента
 * @throw std::underflow_error если список пуст
 */
std::string backFL(const ForwardList* list);

/**
 * @brief Получает значение элемента по индексу.
 * @param list Указатель на список
 * @param index Индекс элемента (0-based)
 * @return Значение элемента
 * @throw std::out_of_range если индекс вне границ
 */
std::string getAtFL(const ForwardList* list, std::size_t index);

/**
 * @brief Проверяет, пуст ли список.
 * @param list Указатель на список
 * @return true если список пуст, false иначе
 */
bool isEmptyFL(const ForwardList* list);

/**
 * @brief Возвращает количество элементов в списке.
 * @param list Указатель на список
 * @return Количество узлов
 */
std::size_t getSizeFL(const ForwardList* list);

void removeBeforeFL(ForwardList* list, FNode* nextNode);

#endif
