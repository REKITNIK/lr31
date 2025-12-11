#ifndef ST_H
#define ST_H

#include <string>
#include <cstddef>
#include <stdexcept>
#include "Structure.h"
#include "ForwardList.h"

/**
 * @brief Реализация структуры "Стек" (Stack) с использованием адаптера.
 *
 * Стек реализован как адаптер над ForwardList согласно паттерну Adapter.
 * Обеспечивает LIFO (Last-In-First-Out) порядок работы с элементами.
 * Использует передний конец (head) однозвязного списка как вершину стека.
 *
 * @note Адаптер нужен для:
 *  - Переиспользования проверенной логики ForwardList
 *  - Обеспечения единого интерфейса сериализации/десериализации
 *  - Разделения абстракций: стек как интерфейс, ForwardList как реализация
 */
struct Stack : public Structure {
    /** @brief Внутренний однозвязный список, используемый для хранения элементов */
    ForwardList* list = nullptr;
    /** @brief Количество элементов в стеке */
    std::size_t size = 0;
    /** @brief Максимальное количество элементов, которые может содержать стек */
    static const std::size_t MAX_SIZE = 1000;
    
    /** @brief Конструктор инициализирует внутренний список */
    Stack() { list = createFL(); }
    
    /** @brief Деструктор освобождает внутренний список */
    ~Stack() override { delete list; }
    
    /**
     * @brief Сериализует стек в формат: "S name count top ... bottom"
     * 
     * Формат: первый элемент - вершина стека (top), последний - дно (bottom).
     * @return Строка с сохраненным состоянием стека
     */
    std::string serialize() const override;
    
    /**
     * @brief Десериализует стек из строки формата "S name count top ... bottom"
     * @param data Строка с сохраненными данными стека
     */
    void deserialize(const std::string& data) override;
};

/**
 * @brief Инициализирует пустой стек.
 * @param stack Указатель на структуру Stack
 */
void initializeStack(Stack* stack);

/**
 * @brief Добавляет элемент на вершину стека.
 * @param stack Указатель на стек
 * @param data Значение добавляемого элемента
 * @throw std::overflow_error если стек переполнен
 */
void pushStack(Stack* stack, const std::string& data);

/**
 * @brief Удаляет и возвращает элемент с вершины стека.
 * @param stack Указатель на стек
 * @return Значение удаленного элемента
 * @throw std::underflow_error если стек пуст
 */
std::string popStack(Stack* stack);

/**
 * @brief Возвращает значение элемента на вершине стека без удаления.
 * @param stack Указатель на стек
 * @return Значение элемента на вершине
 * @throw std::underflow_error если стек пуст
 */
std::string peekStack(const Stack* stack);

/**
 * @brief Проверяет, пуст ли стек.
 * @param stack Указатель на стек
 * @return true если стек пуст, false иначе
 */
bool isStackEmpty(const Stack* stack);

/**
 * @brief Проверяет, переполнен ли стек.
 * @param stack Указатель на стек
 * @return true если достигнут MAX_SIZE, false иначе
 */
bool isStackFull(const Stack* stack);

/**
 * @brief Возвращает количество элементов в стеке.
 * @param stack Указатель на стек
 * @return Количество элементов
 */
std::size_t getStackSize(const Stack* stack);

/**
 * @brief Очищает стек, удаляя все элементы.
 * @param stack Указатель на стек
 */
void clearStack(Stack* stack);

#endif