#ifndef DFL_H
#define DFL_H

#include <stdexcept>
#include <algorithm>
#include <cstddef>
#include <string>

/**
 * @brief Узел двусвязного списка.
 *
 * Хранит строковое значение и указатели на предыдущий и следующий узлы,
 * что позволяет эффективно проходить список в обе стороны.
 */
struct DFNode {
    /** @brief Ключ/значение, хранящееся в узле */
    std::string key;
    /** @brief Указатель на следующий узел (nullptr если это последний узел) */
    DFNode* next = nullptr;
    /** @brief Указатель на предыдущий узел (nullptr если это первый узел) */
    DFNode* prev = nullptr;
};

#include "Structure.h"

/**
 * @brief Реализация структуры "Двусвязный список".
 *
 * Список, в котором каждый узел содержит ссылки на предыдущий и следующий узлы.
 * Позволяет эффективно удалять элементы с конца за O(1) благодаря сохранению
 * указателя на tail и доступу к prev от конца.
 * Поддерживает вставку/удаление в любую позицию и удаление диапазонов.
 */
struct DFList : public Structure {
    /** @brief Указатель на первый узел списка */
    DFNode* head = nullptr;
    /** @brief Указатель на последний узел списка (для O(1) операций с конца) */
    DFNode* tail = nullptr;
    /** @brief Количество узлов в списке */
    std::size_t length = 0;
    
    DFList() = default;
    ~DFList() override {
        DFNode* current = head;
        while (current) {
            DFNode* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        tail = nullptr;
        length = 0;
    }
    
    /**
     * @brief Сериализует список в формат: "L name count elem1 elem2 ..."
     * @return Строка с сохраненным состоянием списка (от head к tail)
     */
    std::string serialize() const override;
    
    /**
     * @brief Десериализует список из строки формата "L name count elem1 elem2 ..."
     * @param data Строка с сохраненными данными списка
     */
    void deserialize(const std::string& data) override;
};

/**
 * @brief Создает новый пустой двусвязный список.
 * @return Указатель на новый список (выделенный в heap)
 */
DFList* createDFList();

/**
 * @brief Удаляет весь список и все его узлы, освобождая память.
 * @param list Указатель на список для удаления
 */
void deleteListDFList(DFList* list);

/**
 * @brief Вставляет элемент после узла на заданной позиции.
 * @param list Указатель на список
 * @param key Значение вставляемого элемента
 * @param index Позиция, после которой произойдет вставка
 */
void addNodeAfterDFList(DFList* list, const std::string& key, int index);

/**
 * @brief Вставляет элемент перед узлом на заданной позиции.
 * @param list Указатель на список
 * @param key Значение вставляемого элемента
 * @param index Позиция, перед которой произойдет вставка
 */
void addNodeBeforeDFList(DFList* list, const std::string& key, int index);

/**
 * @brief Добавляет элемент в начало списка (к head).
 * @param list Указатель на список
 * @param key Значение добавляемого элемента
 */
void addNodeHeadDFList(DFList* list, const std::string& key);

/**
 * @brief Добавляет элемент в конец списка (к tail).
 * @param list Указатель на список
 * @param key Значение добавляемого элемента
 */
void addNodeTailDFList(DFList* list, const std::string& key);

/**
 * @brief Удаляет узел на заданной позиции.
 * @param list Указатель на список
 * @param index Позиция узла для удаления
 */
void deleteNodeAtDFList(DFList* list, int index);

/**
 * @brief Удаляет первый узел (head) списка.
 * @param list Указатель на список
 */
void deleteNodeHeadDFList(DFList* list);

/**
 * @brief Удаляет последний узел (tail) списка за O(1).
 * @param list Указатель на список
 */
void deleteNodeTailDFList(DFList* list);

/**
 * @brief Удаляет первый узел со значением key.
 * @param list Указатель на список
 * @param key Значение узла для удаления
 */
void deleteNodeByValueDFList(DFList* list, const std::string& key);

/**
 * @brief Получает значение элемента на заданной позиции.
 * @param list Указатель на список
 * @param index Позиция элемента
 * @return Значение элемента
 * @throw std::out_of_range если индекс вне границ
 */
std::string getElementDFList(const DFList* list, int index);

/**
 * @brief Получает и удаляет элемент на заданной позиции.
 * @param list Указатель на список
 * @param index Позиция элемента
 * @return Значение удаленного элемента
 */
std::string popElementDFList(DFList* list, int index);

/**
 * @brief Находит первый узел со значением key.
 * @param list Указатель на список
 * @param key Значение для поиска
 * @return Указатель на найденный узел или nullptr если не найден
 */
DFNode* findNodeByValueDFList(const DFList* list, const std::string& key);

/**
 * @brief Проверяет, пуст ли список.
 * @param list Указатель на список
 * @return true если список пуст, false иначе
 */
bool isEmptyDFList(const DFList* list);

/**
 * @brief Возвращает количество элементов в списке.
 * @param list Указатель на список
 * @return Количество узлов
 */
std::size_t getLengthDFList(const DFList* list);

/**
 * @brief Удаляет все узлы ДО указанного индекса (не включая сам индекс).
 * @param list Указатель на список
 * @param index Индекс, до которого происходит удаление
 */
void deleteNodesBeforeIndex(DFList* list, int index);

/**
 * @brief Удаляет все узлы ПОСЛЕ указанного индекса (не включая сам индекс).
 * @param list Указатель на список
 * @param index Индекс, после которого происходит удаление
 */
void deleteNodesAfterIndex(DFList* list, int index);

/**
 * @brief Удаляет все узлы в диапазоне [start, end] включительно.
 * @param list Указатель на список
 * @param start Начальный индекс диапазона
 * @param end Конечный индекс диапазона
 */
void deleteNodesFromTo(DFList* list, int start, int end);

/**
 * @brief Очищает весь список, удаляя все узлы.
 * @param list Указатель на список
 */
void clearDFList(DFList* list);

/**
 * @brief Удаляет только head узел, сохраняя остальной список.
 * @param list Указатель на список
 */
void deleteHeadOnlyDFList(DFList* list);

#endif
