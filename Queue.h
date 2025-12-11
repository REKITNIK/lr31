#ifndef QUEUE_H
#define QUEUE_H

#include <string>
#include <cstddef>
#include <stdexcept>
#include "Structure.h"
#include "ForwardList.h"

/**
 * @brief Реализация структуры "Очередь" (Queue) с использованием адаптера.
 *
 * Очередь реализована как адаптер над ForwardList согласно паттерну Adapter.
 * Обеспечивает FIFO (First-In-First-Out) порядок работы с элементами.
 * Использует front (head) списка как фронт очереди и back (tail) как конец.
 *
 * @note Адаптер нужен для:
 *  - Переиспользования проверенной логики ForwardList
 *  - Обеспечения единого интерфейса сериализации/десериализации
 *  - Разделения абстракций: очередь как интерфейс, ForwardList как реализация
 */
struct Queue : public Structure {
    /** @brief Внутренний однозвязный список: head (фронт) -> tail (конец) */
    ForwardList* list = nullptr;
    /** @brief Количество элементов в очереди */
    std::size_t size = 0;
    /** @brief Максимальное количество элементов, которые может содержать очередь */
    static const std::size_t MAX_SIZE = 1000;
    
    /** @brief Конструктор инициализирует внутренний список */
    Queue() { list = createFL(); }
    
    /** @brief Деструктор освобождает внутренний список */
    ~Queue() override { delete list; }
    
    /**
     * @brief Сериализует очередь в формат: "Q name count front ... back"
     * 
     * Формат: первый элемент - фронт очереди, последний - конец.
     * @return Строка с сохраненным состоянием очереди
     */
    std::string serialize() const override;
    
    /**
     * @brief Десериализует очередь из строки формата "Q name count front ... back"
     * @param data Строка с сохраненными данными очереди
     */
    void deserialize(const std::string& data) override;
};

/**
 * @brief Создает новую пустую очередь.
 * @return Указатель на новую очередь (выделенная в heap)
 */
Queue* createQueue();

/**
 * @brief Удаляет очередь и все ее элементы, освобождая память.
 * @param queue Указатель на очередь для удаления
 */
void deleteQueue(Queue* queue);

/**
 * @brief Добавляет элемент в конец очереди (enqueue).
 * @param queue Указатель на очередь
 * @param value Значение добавляемого элемента
 * @throw std::overflow_error если очередь переполнена
 */
void enqueue(Queue* queue, const std::string& value);

/**
 * @brief Удаляет и возвращает элемент с фронта очереди (dequeue).
 * @param queue Указатель на очередь
 * @return Значение удаленного элемента
 * @throw std::underflow_error если очередь пуста
 */
std::string dequeue(Queue* queue);

/**
 * @brief Возвращает значение элемента на фронте очереди без удаления.
 * @param queue Указатель на очередь
 * @return Значение элемента на фронте
 * @throw std::underflow_error если очередь пуста
 */
std::string frontQueue(const Queue* queue);

/**
 * @brief Проверяет, пуста ли очередь.
 * @param queue Указатель на очередь
 * @return true если очередь пуста, false иначе
 */
bool isQueueEmpty(const Queue* queue);

bool isQueueFull(const Queue* queue);

std::size_t getQueueSize(const Queue* queue);

void clearQueue(Queue* queue);

#endif