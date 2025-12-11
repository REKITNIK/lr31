#ifndef FILEIO_H
#define FILEIO_H
#include <fstream>
#include <string>
#include <vector>
#if defined(_WIN32)
#  include <direct.h>
#  include <sys/stat.h>
#else
#  include <sys/stat.h>
#  include <sys/types.h>
#  include <unistd.h>
#endif
#include "Stack.h"
#include "Queue.h"
#include "ForwardList.h"
#include "DoubleList.h"
#include "Array.h"
#include "FullBinaryTree.h"

#include <map>
#include <string>
#include "Structure.h"

/**
 * @brief Обеспечивает кроссплатформное сохранение и загрузку структур данных из файлов.
 * 
 * Модуль FileIO поддерживает:
 *  - Операции на уровне отдельных структур (Array, ForwardList, etc.)
 *  - Операции на уровне базы данных (множество именованных структур)
 *  - Полиморфную сериализацию/десериализацию через базовый класс Structure
 * 
 * Формат файла:
 *  - Одна структура на одну строку
 *  - Формат: TYPE name count value1 value2 ...
 *  - Типы: 'M' (массив), 'F' (односвязный список), 'L' (двусвязный список),
 *           'S' (стек), 'Q' (очередь), 'T' (бинарное дерево)
 */

/**
 * @brief Создает директорию для файла, если она не существует.
 * 
 * Кроссплатформная функция, которая гарантирует наличие всех необходимых
 * директорий для сохранения файла.
 * 
 * @param filename Путь к файлу
 */
void ensureDirectoryExists(const std::string& filename);

/**
 * @brief Создает пустой файл.
 * @param filename Путь к файлу для создания
 */
void createFile(const std::string& filename);

/**
 * @brief Проверяет существование файла.
 * @param filename Путь к файлу
 * @return true если файл существует, false иначе
 */
bool fileExists(const std::string& filename);

/**
 * @brief Возвращает тип структуры данных из имени файла.
 * @param filename Путь к файлу
 * @return Строка с расширением файла (тип)
 */
std::string getStructureType(const std::string& filename);

/**
 * @brief Сохраняет массив в файл.
 * @param filename Путь к файлу
 * @param array Ссылка на массив для сохранения
 */
void saveArrayToFile(const std::string& filename, const Array& array);

/**
 * @brief Загружает массив из файла.
 * @param filename Путь к файлу
 * @return Указатель на загруженный массив (выделенный в heap)
 */
Array* loadArrayFromFile(const std::string& filename);

/**
 * @brief Сохраняет односвязный список в файл.
 * @param filename Путь к файлу
 * @param list Ссылка на список для сохранения
 */
void saveForwardListToFile(const std::string& filename, const ForwardList& list);

/**
 * @brief Загружает односвязный список из файла.
 * @param filename Путь к файлу
 * @return Указатель на загруженный список (выделенный в heap)
 */
ForwardList* loadForwardListFromFile(const std::string& filename);

/**
 * @brief Сохраняет двусвязный список в файл.
 * @param filename Путь к файлу
 * @param list Ссылка на список для сохранения
 */
void saveDoubleListToFile(const std::string& filename, const DFList& list);

/**
 * @brief Загружает двусвязный список из файла.
 * @param filename Путь к файлу
 * @return Указатель на загруженный список (выделенный в heap)
 */
DFList* loadDoubleListFromFile(const std::string& filename);

/**
 * @brief Сохраняет стек в файл.
 * @param filename Путь к файлу
 * @param mStack Ссылка на стек для сохранения
 */
void saveStackToFile(const std::string& filename, const Stack& mStack);

/**
 * @brief Загружает стек из файла.
 * @param filename Путь к файлу
 * @return Указатель на загруженный стек (выделенный в heap)
 */
Stack* loadStackFromFile(const std::string& filename);

/**
 * @brief Сохраняет очередь в файл.
 * @param filename Путь к файлу
 * @param mQueue Ссылка на очередь для сохранения
 */
void saveQueueToFile(const std::string& filename, const Queue& mQueue);

/**
 * @brief Загружает очередь из файла.
 * @param filename Путь к файлу
 * @return Указатель на загруженную очередь (выделенную в heap)
 */
Queue* loadQueueFromFile(const std::string& filename);

/**
 * @brief Сохраняет бинарное дерево в файл.
 * @param filename Путь к файлу
 * @param tree Ссылка на дерево для сохранения
 */
void saveTreeToFile(const std::string& filename, const BTree& tree);

/**
 * @brief Загружает бинарное дерево из файла.
 * @param filename Путь к файлу
 * @return Указатель на загруженное дерево (выделенное в heap)
 */
BTree* loadTreeFromFile(const std::string& filename);

/**
 * @brief Загружает всю базу данных структур из файла (уровень базы данных).
 * 
 * Читает файл и для каждой строки создает соответствующую структуру данных,
 * добавляя ее в map с ключом (именем структуры).
 * 
 * @param filename Путь к файлу с база данных структур
 * @param database Ссылка на map<name, Structure*> для заполнения
 */
void loadDatabaseFromFile(const std::string& filename, std::map<std::string, Structure*>& database);

/**
 * @brief Сохраняет всю базу данных структур в файл (уровень базы данных).
 * 
 * Итерирует по всем структурам в map и сохраняет каждую,
 * используя полиморфный метод serialize().
 * 
 * @param filename Путь к файлу для сохранения
 * @param database Ссылка на map<name, Structure*> для сохранения
 */
void saveDatabaseToFile(const std::string& filename, const std::map<std::string, Structure*>& database);

/**
 * @brief Загружает одну структуру из файла с определением типа.
 * @param filename Путь к файлу
 * @param type Ссылка на строку для вывода типа структуры
 * @return Указатель на загруженную структуру (void* для универсальности)
 */
void* loadStructureFromFile(const std::string& filename, std::string& type);

/**
 * @brief Сохраняет одну структуру в файл с указанием типа.
 * @param filename Путь к файлу
 * @param type Строка, обозначающая тип структуры
 * @param structure Указатель на структуру для сохранения
 */
void saveStructureToFile(const std::string& filename, const std::string& type, void* structure);

#endif
