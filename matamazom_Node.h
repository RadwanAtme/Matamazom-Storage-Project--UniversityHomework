
#ifndef HW1_MATAMAZOM_NODE_H
#define HW1_MATAMAZOM_NODE_H
typedef struct node
{
    ASElement element;
    double amount;
    struct node* next;
}*Node;

struct AmountSet_t
{
    Node head;
    Node iterator;
    CopyASElement N_copy;
    FreeASElement N_free;
    CompareASElements N_compare;
};
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
#endif //HW1_MATAMAZOM_NODE_H
