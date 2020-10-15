
#ifndef HW1_NODE_H
#define HW1_NODE_H

typedef struct node{
    ASElement element;
    double amount;
    struct node* next;
}*Node;
/*
 * CopyNode:copies a node of type Node and returns a pointer to it
 *
 * &param:set of type AmountSet,node we want to copy,element of the
 *
 * &return:pointer to the copied node in case of success
 *          NULL otherwise
 */
static Node CopyNode(AmountSet set,Node original_node);
/*
 * FindElementNoChange:finds an element in the set based on the compare and the element provided by the user and
 * returns a pointer to the element without changing the set iterator
 *
 * &param:set of type AmountSet,element of type ASElement
 *
 * &return:a pointer to the element if found
 *          NULL otherwise
 */
static Node FindElementNoChange(AmountSet set,ASElement element);

static void linkAfterNextNode(AmountSet set);


#endif //HW1_NODE_H
