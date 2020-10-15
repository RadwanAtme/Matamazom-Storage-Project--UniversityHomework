
#ifndef HW1_PRODUCT_H
#define HW1_PRODUCT_H
#include "list.h"
/*the structure of our products(inventory),each element of type Product represents a diffrent produt in the warehouse
 * each product contains the fields:
 * element:a string holding the name of the product
 * element_id:the id of the product
 * amount:the amount of the product present in the warehouse
 * profit:how much profit the warehouse made from this product
 *
 *
 */
typedef struct product_t{
    char* element;
    unsigned int element_id;
    MtmProductData product_data;
    MatamazomAmountType measure_unit;
    double amount;
    double profit;
    MtmCopyData copyData;
    MtmFreeData freeData;
    MtmGetProductPrice productPrice;

}*Product;

/*the three functions we provided for the generic list that holds the products
 */

/*
 * copyProduct:copies a product of type Product and returns a pointer to it
 *
 * &param:list element of type Product
 *
 * &return:a pointer to the copied product in case of success
 *          NULL otherwise
 */
static ListElement copyProduct(ListElement void_product);

/*
 * freeProduct:frees a product of type Product
 *
 * &param:listelement of type Product
 */
static void freeProduct(ListElement void_product);
/*
 * compareProduct:compares between two listelements of type Product based on products id
 *
 * &param:two listelements of type Product
 *
 * &return:positive number if product1 id is bigger
 *          negative if product2 is bigger
 *          0 if they are equal
 */
static int compareProduct(ListElement void_product1, ListElement void_product2);

//other functions for the list


/*
 * listFind:sets the iterator of the list to the product that holds the given id
 *          the iterator is set on null in case the product dosent exist in the list
 *
 * &param:list of type List,the id of the product
 */
static void listFind(List list, unsigned int id);
/*
 * listContains:checks if a product is in the list
 *
 * &param:list of type List,id of the product
 *
 * &return:true if the product is in the warehouse,false otherwise
 */
bool listContains(unsigned int id, List list);

/*
 * createProduct:creates a new product of type Product
 *
 * &param:the details of the product
 *
 * &return:a pointer to the new product in case of success
 *          NULL otherwise
 *
 */
static Product createProduct(const char *name, unsigned int id, MtmProductData customData, MatamazomAmountType amountType,
                             double amount, MtmCopyData copyData, MtmFreeData freeData, MtmGetProductPrice prodPrice);

/*
 * changeProductsAmount:this function is used after an order has been shipped,the functions changes the amount
 * of the products in the warehouse base on the amount of product that has been shipped
 *
 * &param:warehoouse of type Matamazom,a set containing a list of the ordered products with the amount of type
 * AmountSet
 */
static void changeProductsAmount(Matamazom matamazom,AmountSet cart);

#endif //HW1_PRODUCT_H
