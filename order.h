
#ifndef HW1_ORDER_H
#define HW1_ORDER_H

#include "set.h"
#include "amount_set.h"
/*the structure of the order,each element of type Order represents a diffrent order
 * each order has an order id and an amount set witch contains a list of nodes
 * each node represents a diffrent produt in the cart and in the node is the id of the product and the amount asked for
 *
 */
typedef struct order_t{
    unsigned int order_id;
    AmountSet cart;
}*Order;

/*the three functions we provided for the generic set that holds the orders so it can handle the orders
 *
 */
/*static functions for set*/

/*
 * freeorder:frees the element of the set from type Order
 *
 * &param:Set element of type Order
 *
 */
static void freeOrder(SetElement void_order);
/*
 * copyOrder:copies an order of type Order and returns a pointer to the copied order
 *
 * &param:Set element of type Order
 *
 * &return:a pointer of type SetElement to the copied order if successful
 *          NULL in case of failure
 */
static SetElement copyOrder(SetElement void_order);
/*
 * compareOrder:compares between two orderes based on the orders id
 *
 * &param:two SetElements of type Order
 *
 * &return:positive number if order1 id is bigger
 *          negative if order2 is bigger
 *          0 if they are equal
 */
static int compareOrder(SetElement void_order1, SetElement void_order2);

/*
 * setFind:searches for an order in the set based on id,and returns a pointer to it
 *
 * &param:set of type Set,id of the order
 *
 * &return:a pointer to the element of the set in case of success
 *          NULL in case of failure
 */
static Order setFind(Set set, unsigned int id);
/*
 * newOrder:creates a new order of type Order
 *
 * &param:three functions copy,free,compare for the amount_set in the order
 *
 * &return: pointer to the order in case of success
 *          NULL in case of failure
 */
static Order newOrder(copySetElements copyElement,
                      freeSetElements freeElement,
                      compareSetElements compareElements);
/*
 * checkAmountForOrder:runs on the order and checks if an ordered product amount is bigger than the amount currently
 * in the warehouse
 *
 * &param:a warehouse of type Matamazom,order id
 *
 * &return:true of the amounts are valid
 *          false otherwise
 */
bool checkAmountForOrder(Matamazom matamazom ,unsigned int orderId);

/*
 * registerOrder:deals with an amount change of an order based on the amount in the inventory and the amount given
 */
static MatamazomResult registerOrder(Order order,double amount,double order_amount,unsigned int product_id);


#endif //HW1_ORDER_H
