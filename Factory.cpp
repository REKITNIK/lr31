#include "Factory.h"
#include "Array.h"
#include "ForwardList.h"
#include "DoubleList.h"
#include "Stack.h"
#include "Queue.h"
#include "FullBinaryTree.h"

Structure* createStructure(char type) {
    switch (type) {
        case 'M': return new Array();
        case 'F': return new ForwardList();
        case 'L': return new DFList();
        case 'S': return new Stack();
        case 'Q': return new Queue();
        case 'T': return new BTree();
        default: return nullptr;
    }
}
