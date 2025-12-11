#ifndef ARRAY_H
#define ARRAY_H

#include <stdexcept>
#include <algorithm>
#include <string>
#include "Structure.h"

using namespace std;

/**
 * @brief Узел массива, содержащий одно значение.
 *
 * Простая структура для хранения строкового данные в массиве.
 */
struct ArNode {
    /** @brief Строковое значение, хранящееся в узле */
    string data;
};

/**
 * @brief Реализация структуры "Динамический массив".
 *
 * Массив с динамическим расширением: начинается с заданного размера,
 * автоматически увеличивается при необходимости.
 * Поддерживает операции вставки, удаления, получения элементов.
 */
struct Array : public Structure {
    /** @brief Указатель на начало массива элементов */
    ArNode* head = nullptr;
    /** @brief Количество заполненных элементов */
    int len = 0;
    /** @brief Выделенный размер массива */
    int size = 0;
    
    Array() = default;
    ~Array() override {
        if (head) delete[] head;
        head = nullptr;
        len = 0;
        size = 0;
    }
    
    /**
     * @brief Сериализует массив в формат: "M name count elem1 elem2 ..."
     * @return Строка с сохраненным состоянием массива
     */
    std::string serialize() const override;
    
    /**
     * @brief Десериализует массив из строки формата "M name count elem1 elem2 ..."
     * @param data Строка с сохраненными данными массива
     */
    void deserialize(const std::string& data) override;
};

/**
 * @brief Инициализирует пустой массив с заданным начальным размером.
 * @param emptyArray Указатель на структуру Array для инициализации
 * @param size Начальный размер массива
 */
void createArray(Array* emptyArray, int size);

/**
 * @brief Расширяет массив, увеличивая его вместимость в 2 раза.
 * @param mArray Указатель на массив для расширения
 */
void extendArray(Array* mArray);

/**
 * @brief Получает элемент массива по индексу.
 * @param mArray Указатель на массив
 * @param index Индекс элемента (0-based)
 * @return Строковое значение элемента
 * @throw std::out_of_range если индекс вне границ
 */
string getElementArray(const Array* mArray, int index);

/**
 * @brief Устанавливает значение элемента массива по индексу.
 * @param mArray Указатель на массив
 * @param key Новое значение элемента
 * @param index Индекс элемента
 */
void setKeyArray(Array* mArray, const string& key, int index);

/**
 * @brief Удаляет элемент массива по индексу, сдвигая остальные элементы.
 * @param mArray Указатель на массив
 * @param index Индекс элемента для удаления
 */
void deleteElementArray(Array* mArray, int index);

/**
 * @brief Вставляет элемент в массив по индексу, сдвигая остальные элементы.
 * @param mArray Указатель на массив
 * @param key Значение вставляемого элемента
 * @param index Индекс, где произойдет вставка
 */
void addElementIndexArray(Array* mArray, const string& key, int index);

/**
 * @brief Добавляет элемент в конец массива.
 * @param mArray Указатель на массив
 * @param key Значение добавляемого элемента
 */
void addElementEndArray(Array* mArray, const string& key);

/**
 * @brief Возвращает количество элементов в массиве.
 * @param mArray Указатель на массив
 * @return Количество заполненных элементов
 */
size_t getArrayLength(const Array* mArray);

#endif
