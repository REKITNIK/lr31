#ifndef FBT_H
#define FBT_H

#include <iostream>
#include <string>
#include <functional>

#include "Structure.h"

/**
 * @brief Узел бинарного дерева поиска.
 *
 * Хранит целочисленный ключ и указатели на левого, правого потомков и родителя.
 */
struct BNode {
    /** @brief Целочисленный ключ, хранящийся в узле */
    int key = -1;
    /** @brief Указатель на левого потомка (nullptr если отсутствует) */
    BNode* left = nullptr;
    /** @brief Указатель на правого потомка (nullptr если отсутствует) */
    BNode* right = nullptr;
    /** @brief Указатель на родительский узел (nullptr для корня) */
    BNode* parent = nullptr;
};

/**
 * @brief Реализация структуры "Полное бинарное дерево поиска".
 *
 * Бинарное дерево, в котором все внутренние узлы имеют ровно двух потомков
 * (или это полное дерево по определению задания). Поддерживает операции поиска,
 * вставки, удаления, и различные варианты обхода (pre-order, in-order, post-order, BFS).
 */
struct BTree : public Structure {
    /** @brief Указатель на корень дерева (nullptr если дерево пусто) */
    BNode* root = nullptr;
    
    BTree() = default;
    ~BTree() override {
        std::function<void(BNode*)> deleteTree = [&](BNode* node) {
            if (!node) return;
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        };
        deleteTree(root);
        root = nullptr;
    }
    
    /**
     * @brief Сериализует дерево в формат: "T name count val1 val2 ..."
     * 
     * Формат: предпорядковый обход дерева (pre-order: корень, левое поддерево, правое поддерево).
     * @return Строка с сохраненным состоянием дерева
     */
    std::string serialize() const override;
    
    /**
     * @brief Десериализует дерево из строки формата "T name count val1 val2 ..."
     * 
     * Восстанавливает дерево путем вставки значений в порядке их появления.
     * @param data Строка с сохраненными данными дерева
     */
    void deserialize(const std::string& data) override;
};

/**
 * @brief Находит позицию для вставки нового узла с заданным ключом.
 * 
 * Рекурсивно спускается по дереву в соответствии с правилами BST,
 * возвращая узел, где должен быть вставлен новый элемент.
 * 
 * @param currentNode Текущий узел при поиске
 * @param key Ключ для вставки
 * @return Указатель на узел, под которым должен быть вставлен новый узел
 */
BNode* findPlaceNode(BNode* currentNode, int key);

/**
 * @brief Добавляет новый узел с заданным ключом в дерево.
 * 
 * Вставляет узел в соответствии с правилами бинарного дерева поиска.
 * 
 * @param tree Указатель на дерево
 * @param key Ключ для вставки
 */
void addNode(BTree* tree, int key);

/**
 * @brief Находит узел с заданным ключом в дереве.
 * 
 * Выполняет поиск в дереве, используя свойство BST.
 * 
 * @param tree Ссылка на дерево
 * @param key Ключ для поиска
 * @return Указатель на найденный узел
 * @throw std::runtime_error если узел не найден
 */
BNode* findNode(const BTree& tree, int key);

/**
 * @brief Находит узел с минимальным ключом в поддереве.
 * 
 * Рекурсивно спускается в левое поддерево для нахождения минимального элемента.
 * 
 * @param currentNode Корень поддерева
 * @return Указатель на узел с минимальным ключом
 */
BNode* findMinNode(BNode* currentNode);

/**
 * @brief Находит узел с максимальным ключом в поддереве.
 * 
 * Рекурсивно спускается в правое поддерево для нахождения максимального элемента.
 * 
 * @param currentNode Корень поддерева
 * @return Указатель на узел с максимальным ключом
 */
BNode* findMaxNode(BNode* currentNode);

/**
 * @brief Находит ближайшего предшественника узла в порядке in-order.
 * 
 * Предшественник - это узел с наибольшим ключом, который меньше ключа текущего узла.
 * 
 * @param node Указатель на узел
 * @return Указатель на предшественника или nullptr если его нет
 */
BNode* findInOrderPredecessor(BNode* node);

/**
 * @brief Находит ближайшего преемника узла в порядке in-order.
 * 
 * Преемник - это узел с наименьшим ключом, который больше ключа текущего узла.
 * 
 * @param node Указатель на узел
 * @return Указатель на преемника или nullptr если его нет
 */
BNode* findInOrderSuccessor(BNode* node);

/**
 * @brief Удаляет узел с заданным ключом из дерева.
 * 
 * Обрабатывает три случая:
 *  1. Узел - лист: просто удаляется
 *  2. Узел имеет одного потомка: потомок заменяет узел
 *  3. Узел имеет двух потомков: заменяется на предшественника/преемника
 * 
 * @param tree Указатель на дерево
 * @param key Ключ узла для удаления
 */
void deleteNode(BTree* tree, int key);

/**
 * @brief Считает количество внутренних узлов в поддереве.
 * 
 * Внутренний узел - это узел, который не является листом.
 * 
 * @param currentNode Корень поддерева
 * @return Количество внутренних узлов
 */
int countInnerNodes(BNode* currentNode);

/**
 * @brief Считает количество листовых узлов в поддереве.
 * 
 * Листовой узел - это узел без потомков.
 * 
 * @param currentNode Корень поддерева
 * @return Количество листовых узлов
 */
int countLeavesNodes(BNode* currentNode);

/**
 * @brief Проверяет, является ли дерево полным (full tree).
 * 
 * Полное дерево - это дерево, в котором каждый узел имеет либо 0, либо 2 потомков.
 * Используется для проверки команды TCHECK.
 * 
 * @param tree Ссылка на дерево
 * @return true если дерево полное, false иначе
 */
bool isFullTree(const BTree& tree);

/**
 * @brief Возвращает ключ узла по его значению (используется для отладки).
 * 
 * @param tree Указатель на дерево
 * @param key Ключ узла для получения
 * @return Ключ узла
 */
int tGet(BTree* tree, int key);

#endif