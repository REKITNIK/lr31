
#include "Queue.h"
#include <sstream>

using namespace std;

Queue* createQueue() {
    Queue* q = new Queue;
    if (!q->list) q->list = createFL();
    q->size = 0;
    return q;
}

void deleteQueue(Queue* queue) {
    if (queue) {
        if (queue->list) deleteFL(queue->list);
        delete queue;
    }
}

void enqueue(Queue* queue, const string& value) {
    if (queue->size >= Queue::MAX_SIZE) {
        throw overflow_error("Переполнение очереди");
    }
    if (!queue->list) queue->list = createFL();
    pushBackFL(queue->list, value);
    queue->size++;
}

string dequeue(Queue* queue) {
    if (!queue->list || queue->list->head == nullptr) {
        throw underflow_error("Очередь пустая");
    }
    string val = frontFL(queue->list);
    popFrontFL(queue->list);
    queue->size--;
    return val;
}

string frontQueue(const Queue* queue) {
    if (!queue->list || queue->list->head == nullptr) {
        throw underflow_error("Очередь пустая");
    }
    return frontFL(queue->list);
}

bool isQueueEmpty(const Queue* queue) {
    return !queue->list || isEmptyFL(queue->list);
}

bool isQueueFull(const Queue* queue) {
    return queue->size >= Queue::MAX_SIZE;
}

size_t getQueueSize(const Queue* queue) {
    return queue->size;
}

void clearQueue(Queue* queue) {
    if (!queue->list) return;
    while (!isEmptyFL(queue->list)) popFrontFL(queue->list);
    queue->size = 0;
}

std::string Queue::serialize() const {
    std::ostringstream oss;
    oss << "Q " << name << " " << size;
    if (list && list->head) {
        FNode* cur = list->head;
        while (cur) {
            oss << " " << cur->key;
            cur = cur->next;
        }
    }
    return oss.str();
}

void Queue::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string type;
    iss >> type; // Q
    iss >> name;
    int count = 0;
    iss >> count;
    if (!list) list = createFL();
    while (!isEmptyFL(list)) popFrontFL(list);
    size = 0;
    for (int i = 0; i < count; ++i) {
        std::string v; iss >> v;
        pushBackFL(list, v);
        size++;
    }
}