#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <functional>
#include <cstdlib>
#include "Array.h"
#include "ForwardList.h"
#include "DoubleList.h"
#include "Stack.h"
#include "Queue.h"
#include "FullBinaryTree.h"
#include "FileIO.h"
#include "Print.h"
#include "Factory.h"
#include <map>

using namespace std;

int safeStoi(const string &str)
{
    try
    {
        return stoi(str);
    }
    catch (...)
    {
        cerr << "ERROR 30: Invalid index/argument" << endl;
        exit(1);
    }
}

class StructureManager
{
private:
    std::string currentFilename;
    std::map<std::string, Structure *> database;

public:
    ~StructureManager() { cleanup(); }

    void setFilename(const std::string &filename) { currentFilename = filename; }
    std::string getFilename() const { return currentFilename; }

    void cleanup()
    {
        for (auto &kv : database)
            delete kv.second;
        database.clear();
    }

    void saveCurrentStructure()
    {
        if (currentFilename.empty())
            return;
        try
        {
            saveDatabaseToFile(currentFilename, database);
        }
        catch (...)
        {
            cerr << "ERROR 30: Invalid index/argument" << endl;
            exit(1);
        }
    }

    bool loadStructuresFromFile(const std::string &filename)
    {
        try
        {
            cleanup();
            loadDatabaseFromFile(filename, database);
            currentFilename = filename;
            return true;
        }
        catch (...)
        {
            cerr << "ERROR 10: Unknown command" << endl;
            exit(1);
            return false;
        }
    }

    template <typename T>
    T *get(const std::string &name)
    {
        auto it = database.find(name);
        if (it == database.end())
            return nullptr;
        return dynamic_cast<T *>(it->second);
    }

    void printCurrentStructure(const std::string &name)
    {
        if (!database.count(name))
        {
            cerr << "ERROR 20: Structure not found" << endl;
            exit(1);
        }
        Structure *s = database[name];
        if (Array *a = dynamic_cast<Array *>(s))
        {
            PRINT(*a);
            return;
        }
        if (ForwardList *fl = dynamic_cast<ForwardList *>(s))
        {
            PRINT(*fl);
            return;
        }
        if (DFList *dl = dynamic_cast<DFList *>(s))
        {
            PRINT(*dl);
            return;
        }
        if (Stack *st = dynamic_cast<Stack *>(s))
        {
            PRINT(*st);
            return;
        }
        if (Queue *q = dynamic_cast<Queue *>(s))
        {
            PRINT(*q);
            return;
        }
        if (BTree *t = dynamic_cast<BTree *>(s))
        {
            PRINT(*t);
            return;
        }
        cerr << "ERROR 10: Unknown command" << endl;
        exit(1);
    }

    void handleMCommand(const std::vector<std::string> &tokens)
    {
        try
        {
            // Специальная логика для CREATE: берем имя из tokens[1], если оно явно указано
            if (tokens[0] == "MCREATE")
            {
                std::string name = "default";
                if (tokens.size() > 1)
                {
                    name = tokens[1];
                }
                if (database.count(name))
                {
                    cerr << "ERROR 21: Structure already exists" << endl;
                    exit(1);
                }
                Array *arr = new Array();
                createArray(arr, 10);
                arr->name = name;
                database[name] = arr;
                return;
            }

            // Для других команд: определяем имя структуры и начальный индекс параметров (paramStart)
            std::string name = "default";
            int paramStart = 1;
            if (tokens.size() > 1 && database.count(tokens[1]) > 0)
            {
                name = tokens[1];
                paramStart = 2;
            }

            Array *arr = get<Array>(name);
            if (!arr)
            {
                cerr << "ERROR 20: Structure not found" << endl;
                exit(1);
            }

            // Обработка операций над массивом
            if (tokens[0] == "MPUSH")
            {
                // MPUSH value - добавляет элемент в конец массива
                if (tokens.size() <= paramStart)
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
                addElementEndArray(arr, tokens[paramStart]);
            }
            else if (tokens[0] == "MPUSHAT")
            {
                // MPUSHAT value index - вставляет элемент в указанную позицию
                if (tokens.size() <= paramStart + 1)
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
                std::size_t idx = static_cast<std::size_t>(safeStoi(tokens[paramStart + 1]));
                addElementIndexArray(arr, tokens[paramStart], idx);
            }
            else if (tokens[0] == "MGET")
            {
                if (tokens.size() <= paramStart)
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
                std::size_t idx = static_cast<std::size_t>(safeStoi(tokens[paramStart]));
                cout << getElementArray(arr, idx) << endl;
            }
            else if (tokens[0] == "MDEL")
            {
                if (tokens.size() <= paramStart)
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
                std::size_t idx = static_cast<std::size_t>(safeStoi(tokens[paramStart]));
                deleteElementArray(arr, idx);
            }
            else if (tokens[0] == "MSET")
            {
                if (tokens.size() <= paramStart + 1)
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
                std::size_t idx = static_cast<std::size_t>(safeStoi(tokens[paramStart]));
                setKeyArray(arr, tokens[paramStart + 1], idx);
            }
            else if (tokens[0] == "MLEN")
            {
                cout << getArrayLength(arr) << endl;
            }
            else
            {
                cerr << "ERROR 10: Unknown command" << endl;
                exit(1);
            }
        }
        catch (...)
        {
            cerr << "ERROR 30: Invalid index/argument" << endl;
            exit(1);
        }
    }

    void handleFCommand(const std::vector<std::string> &tokens)
    {
        try
        {
            // Специальная логика для CREATE: берем имя из tokens[1], если оно явно указано
            if (tokens[0] == "FCREATE")
            {
                std::string name = "default";
                if (tokens.size() > 1)
                {
                    name = tokens[1];
                }
                if (database.count(name))
                {
                    cerr << "ERROR 21: Structure already exists" << endl;
                    exit(1);
                }
                ForwardList *fl = createFL();
                fl->name = name;
                database[name] = fl;
                return;
            }

            // Для других команд: определяем имя структуры и начальный индекс параметров
            std::string name = "default";
            int paramStart = 1;
            if (tokens.size() > 1 && database.count(tokens[1]) > 0)
            {
                name = tokens[1];
                paramStart = 2;
            }

            // Auto-create if doesn't exist
            ForwardList *fl = get<ForwardList>(name);
            if (!fl && tokens[0] != "FCREATE")
            {
                fl = createFL();
                fl->name = name;
                database[name] = fl;
            }
            if (!fl)
            {
                cerr << "ERROR 20: Structure not found" << endl;
                exit(1);
            }

            if (tokens[0] == "FPUSH")
            {
                if (tokens.size() < paramStart + 2)
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
                std::string value = tokens[paramStart];
                int mode = safeStoi(tokens[paramStart + 1]);
                if (mode == 0)
                    pushFrontFL(fl, value);
                else if (mode == 1)
                    pushBackFL(fl, value);
                else if (mode == 2)
                {
                    if (fl->head)
                        insertAfterFL(fl, value, 0);
                    else
                        pushFrontFL(fl, value);
                }
                else if (mode == 3)
                {
                    if (fl->head)
                    {
                        int len = 0;
                        FNode *cur = fl->head;
                        while (cur)
                        {
                            len++;
                            cur = cur->next;
                        }
                        if (len > 0)
                            insertBeforeFL(fl, value, len - 1);
                        else
                            pushFrontFL(fl, value);
                    }
                    else
                        pushFrontFL(fl, value);
                }
                else
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
            }
            else if (tokens[0] == "FDEL")
            {
                if (tokens.size() < paramStart + 1)
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
                int mode = safeStoi(tokens[paramStart]);

                if (mode == 0)
                {
                    popFrontFL(fl);
                }
                else if (mode == 1)
                {
                    popBackFL(fl);
                }
                else if (mode == 2)
                {
                    if (fl->head && fl->head->next)
                    {
                        removeAfterFL(fl, fl->head);
                    }
                }
                else if (mode == 3)
                {
                    if (fl->tail && fl->head != fl->tail)
                    {

                        FNode *current = fl->head;
                        while (current && current->next && current->next != fl->tail)
                        {
                            current = current->next;
                        }
                        if (current)
                        {
                            removeAfterFL(fl, current);
                        }
                    }
                }
                else
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
            }
            else if (tokens[0] == "FDELVAL")
            {
                if (tokens.size() < paramStart + 1)
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
                std::string value = tokens[paramStart];
                if (!removeByValueFL(fl, value))
                {
                    cerr << "ERROR 20: Structure not found" << endl;
                    exit(1);
                }
            }
            else if (tokens[0] == "FDELVALBEF")
            {
                if (tokens.size() < paramStart + 1)
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
                std::string value = tokens[paramStart];
                FNode *targetNode = findByValueFL(fl, value);
                if (!targetNode)
                {
                    cerr << "ERROR 20: Structure not found" << endl;
                    exit(1);
                }
                try
                {
                    removeBeforeFL(fl, targetNode);
                }
                catch (const invalid_argument &e)
                {
                    cerr << "ERROR 30: " << e.what() << endl;
                    exit(1);
                }
            }
            else if (tokens[0] == "FDELVALAFT")
            {
                if (tokens.size() < paramStart + 1)
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
                std::string value = tokens[paramStart];
                FNode *targetNode = findByValueFL(fl, value);
                if (!targetNode)
                {
                    cerr << "ERROR 20: Structure not found" << endl;
                    exit(1);
                }
                try
                {
                    removeAfterFL(fl, targetNode);
                }
                catch (const invalid_argument &e)
                {
                    cerr << "ERROR 30: " << e.what() << endl;
                    exit(1);
                }
            }
            else if (tokens[0] == "FSEARCH")
            {
                if (tokens.size() < paramStart + 1)
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
                std::string value = tokens[paramStart];
                FNode *res = findByValueFL(fl, value);
                cout << (res ? "TRUE" : "FALSE") << endl;
            }
            else if (tokens[0] == "FGET")
            {
                if (tokens.size() < paramStart + 1)
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
                int idx = safeStoi(tokens[paramStart]);
                cout << getAtFL(fl, idx) << endl;
            }
            else if (tokens[0] == "FLEN")
            {
                int len = 0;
                FNode *cur = fl->head;
                while (cur)
                {
                    len++;
                    cur = cur->next;
                }
                cout << len << endl;
            }
            else
            {
                cerr << "ERROR 10: Unknown command" << endl;
                exit(1);
            }
        }
        catch (...)
        {
            cerr << "ERROR 30: Invalid index/argument" << endl;
            exit(1);
        }
    } // Конец handleFCommand

    void handleLCommand(const std::vector<std::string> &tokens)
    {
        try
        {
            // Специальная логика для CREATE: берем имя из tokens[1], если оно явно указано
            if (tokens[0] == "LCREATE")
            {
                std::string name = "default";
                if (tokens.size() > 1)
                {
                    name = tokens[1];
                }
                if (database.count(name))
                {
                    cerr << "ERROR 21: Structure already exists" << endl;
                    exit(1);
                }
                DFList *dl = createDFList();
                dl->name = name;
                database[name] = dl;
                return;
            }

            // Для других команд: определяем имя структуры и начальный индекс параметров
            std::string name = "default";
            int paramStart = 1;
            if (tokens.size() > 1 && database.count(tokens[1]) > 0)
            {
                name = tokens[1];
                paramStart = 2;
            }

            // Auto-create if doesn't exist
            DFList *dl = get<DFList>(name);
            if (!dl && tokens[0] != "LCREATE")
            {
                dl = createDFList();
                dl->name = name;
                database[name] = dl;
            }
            if (!dl)
            {
                cerr << "ERROR 20: Structure not found" << endl;
                exit(1);
            }

            if (tokens[0] == "LPUSH")
            {
                if (tokens.size() < paramStart + 2)
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
                std::string value = tokens[paramStart];
                int mode = safeStoi(tokens[paramStart + 1]);
                if (mode == 0)
                    addNodeHeadDFList(dl, value);
                else if (mode == 1)
                    addNodeTailDFList(dl, value);
                else if (mode == 2)
                {
                    if (dl->head)
                        addNodeAfterDFList(dl, value, 0);
                    else
                        addNodeHeadDFList(dl, value);
                }
                else if (mode == 3)
                {
                    if (dl->head)
                    {
                        int len = 0;
                        DFNode *cur = dl->head;
                        while (cur)
                        {
                            len++;
                            cur = cur->next;
                        }
                        if (len > 0)
                            addNodeBeforeDFList(dl, value, len - 1);
                        else
                            addNodeHeadDFList(dl, value);
                    }
                    else
                        addNodeHeadDFList(dl, value);
                }
                else
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
            }
            else if (tokens[0] == "LDEL")
            {
                if (tokens.size() < paramStart + 1)
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
                int mode = safeStoi(tokens[paramStart]);
                if (mode == 0)
                    deleteNodeHeadDFList(dl);
                else if (mode == 1)
                    deleteNodeTailDFList(dl);
                else if (mode == 2)
                {
                    if (dl->head && dl->head->next)
                    {
                        DFNode *temp = dl->head->next;
                        dl->head->next = temp->next;
                        if (temp->next)
                            temp->next->prev = dl->head;
                        else
                            dl->tail = dl->head;
                        delete temp;
                        dl->length--;
                    }
                }
                else if (mode == 3)
                {
                    if (dl->tail && dl->tail->prev)
                    {
                        DFNode *temp = dl->tail->prev;
                        dl->tail->prev = temp->prev;
                        if (temp->prev)
                            temp->prev->next = dl->tail;
                        else
                            dl->head = dl->tail;
                        delete temp;
                        dl->length--;
                    }
                }
                else
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
            }
            else if (tokens[0] == "LGET")
            {
                if (tokens.size() < paramStart + 1)
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
                int idx = safeStoi(tokens[paramStart]);
                cout << getElementDFList(dl, idx) << endl;
            }
            else if (tokens[0] == "LSEARCH")
            {
                if (tokens.size() < paramStart + 1)
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
                DFNode *found = findNodeByValueDFList(dl, tokens[paramStart]);
                cout << (found ? "TRUE" : "FALSE") << endl;
            }
            else if (tokens[0] == "LDELVAL")
            {
                if (tokens.size() < paramStart + 1)
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
                deleteNodeByValueDFList(dl, tokens[paramStart]);
            }
            else if (tokens[0] == "LLEN")
            {
                cout << dl->length << endl;
            }
            else
            {
                cerr << "ERROR 10: Unknown command" << endl;
                exit(1);
            }
        }
        catch (...)
        {
            cerr << "ERROR 30: Invalid index/argument" << endl;
            exit(1);
        }
    }

    void handleSCommand(const std::vector<std::string> &tokens)
    {
        try
        {
            // Специальная логика для CREATE: берем имя из tokens[1], если оно явно указано
            if (tokens[0] == "SCREATE")
            {
                std::string name = "default";
                if (tokens.size() > 1)
                {
                    name = tokens[1];
                }
                if (database.count(name))
                {
                    cerr << "ERROR 21: Structure already exists" << endl;
                    exit(1);
                }
                Stack *s = new Stack();
                s->name = name;
                database[name] = s;
                return;
            }

            // Для других команд: определяем имя структуры и начальный индекс параметров
            std::string name = "default";
            int paramStart = 1;
            if (tokens.size() > 1 && database.count(tokens[1]) > 0)
            {
                name = tokens[1];
                paramStart = 2;
            }
            // Auto-create if doesn't exist
            Stack *s = get<Stack>(name);
            if (!s && tokens[0] != "SCREATE")
            {
                s = new Stack();
                s->name = name;
                database[name] = s;
            }
            if (!s)
            {
                cerr << "ERROR 20: Structure not found" << endl;
                exit(1);
            }
            if (tokens[0] == "SPUSH")
            {
                if (tokens.size() <= paramStart)
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
                pushStack(s, tokens[paramStart]);
            }
            else if (tokens[0] == "SPOP")
            {
                try
                {
                    cout << popStack(s) << endl;
                }
                catch (...)
                {
                    cerr << "ERROR 40: Empty structure" << endl;
                    exit(1);
                }
            }
            else if (tokens[0] == "SLEN")
            {
                cout << s->size << endl;
            }
            else
            {
                cerr << "ERROR 10: Unknown command" << endl;
                exit(1);
            }
        }
        catch (...)
        {
            cerr << "ERROR 40: Empty structure" << endl;
            exit(1);
        }
    }

    void handleQCommand(const std::vector<std::string> &tokens)
    {
        try
        {
            // Специальная логика для CREATE: берем имя из tokens[1], если оно явно указано
            if (tokens[0] == "QCREATE")
            {
                std::string name = "default";
                if (tokens.size() > 1)
                {
                    name = tokens[1];
                }
                if (database.count(name))
                {
                    cerr << "ERROR 21: Structure already exists" << endl;
                    exit(1);
                }
                Queue *q = new Queue();
                q->name = name;
                database[name] = q;
                return;
            }

            // Для других команд: определяем имя структуры и начальный индекс параметров
            std::string name = "default";
            int paramStart = 1;
            if (tokens.size() > 1 && database.count(tokens[1]) > 0)
            {
                name = tokens[1];
                paramStart = 2;
            }
            // Auto-create if doesn't exist
            Queue *q = get<Queue>(name);
            if (!q && tokens[0] != "QCREATE")
            {
                q = new Queue();
                q->name = name;
                database[name] = q;
            }
            if (!q)
            {
                cerr << "ERROR 20: Structure not found" << endl;
                exit(1);
            }
            if (tokens[0] == "QPUSH")
            {
                if (tokens.size() <= paramStart)
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
                enqueue(q, tokens[paramStart]);
            }
            else if (tokens[0] == "QPOP")
            {
                try
                {
                    cout << dequeue(q) << endl;
                }
                catch (...)
                {
                    cerr << "ERROR 40: Empty structure" << endl;
                    exit(1);
                }
            }
            else if (tokens[0] == "QLEN")
            {
                cout << q->size << endl;
            }
            else
            {
                cerr << "ERROR 10: Unknown command" << endl;
                exit(1);
            }
        }
        catch (...)
        {
            cerr << "ERROR 40: Empty structure" << endl;
            exit(1);
        }
    }

    void handleTCommand(const std::vector<std::string> &tokens)
    {
        try
        {
            // Специальная логика для CREATE: берем имя из tokens[1], если оно явно указано
            if (tokens[0] == "TCREATE")
            {
                std::string name = "default";
                if (tokens.size() > 1)
                {
                    name = tokens[1];
                }
                if (database.count(name))
                {
                    cerr << "ERROR 21: Structure already exists" << endl;
                    exit(1);
                }
                BTree *t = new BTree();
                t->name = name;
                database[name] = t;
                return;
            }

            // Для других команд: определяем имя структуры и начальный индекс параметров
            std::string name = "default";
            int paramStart = 1;
            if (tokens.size() > 1 && database.count(tokens[1]) > 0)
            {
                name = tokens[1];
                paramStart = 2;
            }
            // Auto-create if doesn't exist
            BTree *t = get<BTree>(name);
            if (!t && tokens[0] != "TCREATE")
            {
                t = new BTree();
                t->name = name;
                database[name] = t;
            }
            if (!t)
            {
                cerr << "ERROR 20: Structure not found" << endl;
                exit(1);
            }
            if (tokens[0] == "TINSERT")
            {
                if (tokens.size() <= paramStart)
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
                int key = safeStoi(tokens[paramStart]);
                addNode(t, key);
            }
            else if (tokens[0] == "TSEARCH")
            {
                if (tokens.size() <= paramStart)
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
                int key = safeStoi(tokens[paramStart]);
                try
                {
                    findNode(*t, key);
                    cout << "TRUE" << endl;
                }
                catch (...)
                {
                    cout << "FALSE" << endl;
                }
            }
            else if (tokens[0] == "TCHECK")
            {
                cout << (t->root == nullptr ? "TRUE" : (isFullTree(*t) ? "TRUE" : "FALSE")) << endl;
            }
            else if (tokens[0] == "TDEL")
            {
                if (tokens.size() <= paramStart)
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
                int key = safeStoi(tokens[paramStart]);
                deleteNode(t, key);
            }
            else if (tokens[0] == "TGET")
            {
                if (tokens.size() <= paramStart)
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
                string mode = tokens[paramStart];
                if (t->root == nullptr)
                {
                    cerr << "ERROR 40: Empty structure" << endl;
                    exit(1);
                }
                if (mode == "PRE")
                {
                    function<void(BNode *)> pre = [&](BNode *n)
                    { if(n){ cout<<n->key<<" "; pre(n->left); pre(n->right);} };
                    pre(t->root);
                    cout << endl;
                }
                else if (mode == "IN")
                {
                    function<void(BNode *)> in = [&](BNode *n)
                    { if(n){ in(n->left); cout<<n->key<<" "; in(n->right);} };
                    in(t->root);
                    cout << endl;
                }
                else if (mode == "POST")
                {
                    function<void(BNode *)> post = [&](BNode *n)
                    { if(n){ post(n->left); post(n->right); cout<<n->key<<" ";} };
                    post(t->root);
                    cout << endl;
                }
                else if (mode == "BFS")
                {
                    vector<BNode *> q;
                    q.push_back(t->root);
                    size_t i = 0;
                    while (i < q.size())
                    {
                        BNode *n = q[i++];
                        cout << n->key << " ";
                        if (n->left)
                            q.push_back(n->left);
                        if (n->right)
                            q.push_back(n->right);
                    }
                    cout << endl;
                }
                else
                {
                    cerr << "ERROR 10: Unknown command" << endl;
                    exit(1);
                }
            }
            else if (tokens[0] == "TGETNODES")
            {
                if (tokens.size() <= paramStart + 1)
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
                int key = safeStoi(tokens[paramStart]);
                string mode = tokens[paramStart + 1];
                try
                {
                    BNode *node = findNode(*t, key);
                    BNode *res = nullptr;
                    if (mode == "PREV")
                        res = findInOrderPredecessor(node);
                    else if (mode == "NEXT")
                        res = findInOrderSuccessor(node);
                    else
                    {
                        cerr << "ERROR 10: Unknown command" << endl;
                        exit(1);
                    }
                    if (!res)
                        cout << endl;
                    else
                        cout << res->key << endl;
                }
                catch (...)
                {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
            }
            else
            {
                cerr << "ERROR 10: Unknown command" << endl;
                exit(1);
            }
        }
        catch (...)
        {
            cerr << "ERROR 10: Unknown command" << endl;
            exit(1);
        }
    }
}; // Конец класса StructureManager

// processQuery: split query and dispatch to manager
void processQuery(const std::string &query, StructureManager &manager)
{
    // Разбор запроса: разбиваем на токены
    std::istringstream iss(query);
    std::vector<std::string> tokens;
    std::string tok;
    while (iss >> tok)
        tokens.push_back(tok);
    if (tokens.empty())
    {
        cerr << "ERROR 10: Unknown command" << endl;
        exit(1);
    }

    std::string cmd = tokens[0];
    char c = cmd[0]; // Первый символ определяет тип структуры

    // Специальная команда PRINT: выводит содержимое структуры
    if (cmd == "PRINT")
    {
        if (tokens.size() < 2)
        {
            cerr << "ERROR 30: Invalid index/argument" << endl;
            exit(1);
        }
        manager.printCurrentStructure(tokens[1]);
        return;
    }

    // Диспетчеризация команд по первому символу:
    // M - Array, F - ForwardList, L - DoubleList, S - Stack, Q - Queue, T - BTree
    if (c == 'M')
        manager.handleMCommand(tokens);
    else if (c == 'F')
        manager.handleFCommand(tokens);
    else if (c == 'L')
        manager.handleLCommand(tokens);
    else if (c == 'S')
        manager.handleSCommand(tokens);
    else if (c == 'Q')
        manager.handleQCommand(tokens);
    else if (c == 'T')
        manager.handleTCommand(tokens);
    else
    {
        cerr << "ERROR 10: Unknown command" << endl;
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    string filename;
    string query;
    StructureManager manager;
    bool helpRequested = false;

    // === Этап 1: Парсинг аргументов командной строки ===
    for (int i = 1; i < argc; i++)
    {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc)
        {
            filename = argv[++i];
            manager.setFilename(filename);
        }
        else if (arg == "--query" && i + 1 < argc)
        {
            query = argv[++i];
        }
        else if (arg == "--help")
        {
            helpRequested = true;
        }
    }

    // === Этап 2: STATELESS режим ===
    // Каждый вызов программы выполняет цикл: Load → Execute → Save → Exit
    try
    {
        // Обработка --help
        if (helpRequested)
        {
            cout << "Usage: ./lab1 --file <path> --query '<COMMAND> <ARGS...>'" << endl;
            return 0;
        }

        // Этап A: ЗАГРУЗКА (Десериализация)
        // Если файл существует, загружаем всю базу данных структур из файла
        if (!filename.empty())
        {
            if (fileExists(filename))
            {
                // Загружаем ВСЕ структуры из файла в map<name, Structure*>
                if (!manager.loadStructuresFromFile(filename))
                {
                    cerr << "ERROR 10: Unknown command" << endl;
                    return 1;
                }
            }
        }

        // Этап B: ВЫПОЛНЕНИЕ
        // Парсим и выполняем одну команду из --query
        if (!query.empty())
        {
            processQuery(query, manager);
        }
        else
        {
            // Если ни query ни help не указаны - ошибка
            cerr << "ERROR 10: Unknown command" << endl;
            return 1;
        }

        // Этап C: СОХРАНЕНИЕ (Сериализация)
        // Сохраняем ВСЮ базу данных в файл (все структуры с их новым состоянием)
        if (!filename.empty())
        {
            manager.saveCurrentStructure();
        }
    }
    catch (...)
    {
        cerr << "ERROR 10: Unknown command" << endl;
        return 1;
    }

    // === ЗАВЕРШЕНИЕ ===
    return 0;
}
