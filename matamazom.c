#include "matamazom.h"
#include "matamazom_print.h"
#include "amount_set.h"
#include "product.h"
#include "order.h"
#include "list.h"
#include "set.h"
#include "static_functions.h"
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>



#define abs(x) (x>=-x ? x:-x)

struct Matamazom_t{//warehouse
    List inventory;
    Set orders;
};

Matamazom matamazomCreate(){
    //create the warehouse
    Matamazom warehouse=malloc(sizeof(*warehouse));
    if(warehouse==NULL){
        return NULL;
    }
    //create a list where each element is a struct of the type Product witch contains info about a specific element
    //of the warehouse
    warehouse->inventory=listCreate(copyProduct, freeProduct);
    if(warehouse->inventory == NULL){
        free(warehouse);
        return NULL;
    }
    //creates a set where each element is of type struct order_t witch is an order by it self
    warehouse->orders= setCreate(copyOrder, freeOrder, compareOrder);
    if(warehouse->orders==NULL){
        listDestroy(warehouse->inventory);
        free(warehouse);
        return NULL;
    }
    return warehouse;
}


void matamazomDestroy(Matamazom matamazom){
    if(matamazom==NULL){
        return ;
    }
    listDestroy(matamazom->inventory);
    setDestroy(matamazom->orders);
    free(matamazom);
}

MatamazomResult mtmNewProduct(Matamazom matamazom, const unsigned int id, const char *name,
                              const double amount, const MatamazomAmountType amountType,
                              const MtmProductData customData, MtmCopyData copyData,
                              MtmFreeData freeData, MtmGetProductPrice prodPrice){
    if(matamazom==NULL||name==NULL||customData==NULL||copyData==NULL||freeData==NULL||prodPrice==NULL){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    if(!checkName(name)){
        return MATAMAZOM_INVALID_NAME;
    }
    if(amount<0||!checkAmount(amount, amountType)){
        return MATAMAZOM_INVALID_AMOUNT;
    }
    if(listContains(id,matamazom->inventory)){//list contains returns true or false
        return MATAMAZOM_PRODUCT_ALREADY_EXIST;
    }
    //create a struct of type Product which represents an element and then add it to the inventory list
    Product product= createProduct(name, id, customData, amountType, amount, copyData, freeData, prodPrice);
    assert(product!=NULL);//*assert*
    listInsertFirst(matamazom->inventory, product);
    assert(listContains(id,matamazom->inventory));//*assert*
    freeProduct(product);
    return MATAMAZOM_SUCCESS;
}

MatamazomResult mtmChangeProductAmount(Matamazom matamazom, const unsigned int id, const double amount){
    if(matamazom==NULL){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    listFind(matamazom->inventory, id);
    Product product=listGetCurrent(matamazom->inventory);
    if(product==NULL){
        return MATAMAZOM_PRODUCT_NOT_EXIST;
    }
    if(!checkAmount(amount, product->measure_unit)){//
        return MATAMAZOM_INVALID_AMOUNT;
    }
    if(amount<0&&(-amount)>product->amount){
        return MATAMAZOM_INSUFFICIENT_AMOUNT;
    }
    product->amount+=amount;
    return MATAMAZOM_SUCCESS;
}

MatamazomResult mtmClearProduct(Matamazom matamazom, const unsigned int id){
    if(matamazom==NULL){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    listFind(matamazom->inventory, id);
    Product product=listGetCurrent(matamazom->inventory);
    if(product==NULL){
        return MATAMAZOM_PRODUCT_NOT_EXIST;
    }
    listRemoveCurrent(matamazom->inventory);
    return MATAMAZOM_SUCCESS;

}

/*order functions*/

unsigned int mtmCreateNewOrder(Matamazom matamazom){
    if(matamazom==NULL){
        return 0;
    }
    static int initial_id=1;
    Order order= newOrder(copyInt, freeInt, compareInts);
    if(order==NULL){
        return 0;
    }
    order->order_id=initial_id;
    if(setAdd(matamazom->orders,(SetElement)order)==SET_OUT_OF_MEMORY){
        free(order);
        return 0;
    }
    freeOrder(order);
    return initial_id++;
}

MatamazomResult mtmChangeProductAmountInOrder(Matamazom matamazom, const unsigned int orderId,
                                              const unsigned int productId, const double amount){
    if(matamazom==NULL){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    double order_amount=0;
    //declare two variables so we can send pointers of the values to our amount_set functions
    unsigned int order_id=orderId;
    unsigned int product_id=productId;
    Order order_to_check=setFind(matamazom->orders,order_id);
    if(order_to_check==NULL||!setIsIn(matamazom->orders,order_to_check)){
        return MATAMAZOM_ORDER_NOT_EXIST;
    }
    listFind(matamazom->inventory,product_id);
    Product product=listGetCurrent(matamazom->inventory);
    if(product==NULL){
        return MATAMAZOM_PRODUCT_NOT_EXIST;
    }
    if(!checkAmount(amount, product->measure_unit)) {
        return MATAMAZOM_INVALID_AMOUNT;
    }
    //order is a pointer to struct of type Order
    Order order= setFind(matamazom->orders,order_id);
    //we already checked the order exists via setIsIn so assert should not fail
    assert(order!=NULL);//*assert*
    //order.cart is the amount_set
    //if amount is 0 and the variables are valid
    return registerOrder(order,amount,order_amount,product_id);

}

MatamazomResult mtmShipOrder(Matamazom matamazom, const unsigned int orderId){
    if(matamazom==NULL){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    unsigned int shipped_order=orderId;
    Order order_to_check=setFind(matamazom->orders,orderId);
    if(order_to_check==NULL||!setIsIn(matamazom->orders,&shipped_order)){
        return MATAMAZOM_ORDER_NOT_EXIST;
    }
    if(!checkAmountForOrder(matamazom, orderId)){//function that checks if an amount in order is bigger than in the warehouse
        return MATAMAZOM_INSUFFICIENT_AMOUNT;
    }
    Order order= setFind(matamazom->orders, orderId);//setFind return a pointer to an element of the set
    //assert should not fail since we checked through setIsIn
    assert(order!=NULL);
    //change the profit for each element of the order
    Profit(matamazom,order->cart);
    //changes the amount for each element after the shipping is done
    changeProductsAmount(matamazom,order->cart);
    setRemove(matamazom->orders,&shipped_order);
    return MATAMAZOM_SUCCESS;
}

MatamazomResult mtmCancelOrder(Matamazom matamazom, const unsigned int orderId){
    if(matamazom==NULL){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    unsigned int order_id=orderId;
    Order order_to_check=setFind(matamazom->orders,order_id);
    if(order_to_check==NULL||!setIsIn(matamazom->orders,&order_id)){
        return MATAMAZOM_ORDER_NOT_EXIST;
    }
    setRemove(matamazom->orders,&order_id);
    return MATAMAZOM_SUCCESS;
}

/*
 * static functions
 */
static int compareId(unsigned int id1, unsigned int id2) {
    return id1-id2;
}



static void listFind(List list, unsigned int id){
    Product current_product=listGetFirst(list);
    while(current_product!=NULL){
        if(compareId(current_product->element_id, id) == 0){//if we find the item we exit,the iterator is standing on the element
            return ;
        }
        current_product=listGetNext(list);
    }
    //here the iterator should be NULL
}

static Product createProduct(const char *name, unsigned int id, MtmProductData customData, MatamazomAmountType amountType,
                             double amount, MtmCopyData copyData, MtmFreeData freeData, MtmGetProductPrice prodPrice) {
    Product product = malloc(sizeof(*product));
    if (product == NULL) {
        return NULL;
    }
    product->element=copyString(name);//mallocs a new string and copies the string into it
    assert(product->element!=NULL);//*assert*
    product->element_id=id;
    product->product_data=copyData(customData);
    product->measure_unit=amountType;
    product->amount=amount;
    product->profit=0;
    product->freeData=freeData;
    product->copyData=copyData;
    product->productPrice=prodPrice;
    return product;
}

//copies a product_t type struct,used for list
static ListElement copyProduct(ListElement void_product){
    Product product=void_product;
    assert(product!=NULL);//*assert*
    Product copied_product = malloc(sizeof(*copied_product));
    if (copied_product == NULL) {
        return NULL;
    }
    copied_product->element=copyString(product->element);//mallocs a new string and copies the string into it
    assert(copied_product->element!=NULL);//*assert*
    copied_product->element_id=product->element_id;
    copied_product->product_data=product->copyData(product->product_data);
    copied_product->measure_unit= product->measure_unit;
    copied_product->amount=product->amount;
    copied_product->profit= product->profit;
    copied_product->freeData=product->freeData;
    copied_product->copyData=product->copyData;
    copied_product->productPrice= product->productPrice;
    return (ListElement)copied_product;
}


/*freeProduct:frees a prouct of the list*/
static void freeProduct(ListElement void_product){
    Product product=void_product;
    if(product==NULL){
        return;
    }
    free(product->element);
    product->freeData(product->product_data);
    free(void_product);
}

static SetElement copyOrder(SetElement void_order){
    Order order=void_order;
    assert(order!=NULL);
    Order copied_order=malloc(sizeof(*copied_order));
    if(copied_order==NULL){
        return NULL;
    }
    copied_order->order_id=order->order_id;
    copied_order->cart=asCopy(order->cart);
    return (SetElement)copied_order;
}

static ASElement copyInt(ASElement number) {
    unsigned int *copy = malloc(sizeof(*copy));
    if (copy != NULL) {
        *copy = *(unsigned int *)number;
    }
    return (ASElement )copy;
}

static void freeInt(ASElement number) { free(number); }

static int compareInts(ASElement lhs, ASElement rhs) {
    unsigned int first_id=*((unsigned int*)lhs);
    unsigned int second_id=*((unsigned int*) rhs);
    return first_id-second_id;
}

static void freeOrder(SetElement void_order){
    Order order=void_order;
    if(order==NULL){
        return;
    }
    asDestroy(order->cart);
    free(order);
}

static int compareOrder(SetElement void_order1, SetElement void_order2){
    Order order1=void_order1;
    Order order2=void_order2;
    assert(order1!=NULL&&order2!=NULL);//*assert*
    return compareInts(&order1->order_id,&order2->order_id);
}

static int compareProduct(ListElement void_product1, ListElement void_product2){
    Product product1=void_product1;
    Product product2=void_product2;
    assert(product1!=NULL&&product2!=NULL);//*assert*
    return product1->element_id-product2->element_id;
}



static bool checkAmount(double amount, MatamazomAmountType amountType){
    if(amountType==MATAMAZOM_ANY_AMOUNT){
        return true;
    }
    if(amountType==MATAMAZOM_INTEGER_AMOUNT){
        return intTest(abs(amount));
    }
    if(amountType==MATAMAZOM_HALF_INTEGER_AMOUNT){
        return halfIntTest(abs(amount));
    }
    return true;

}

static bool intTest(double amount){
    double num=amount-(int)amount;
    if(num>0.5){
        num=1-num;
    }
    if(num<=0.001){
        return true;
    }
    return false;
}

static bool halfIntTest(double amount){
    if(intTest(amount) == true || intTest(amount + 0.5) == true){
        return true;
    }
    return false;
}

static bool checkName(const char* name){
    if(name==NULL){
        return false;
    }
    if(name[0]=='\0'){
        return false;
    }
    if((name[0]>='A')&&(name[0]<='Z')){
        return true;
    }
    if((name[0]>='a')&&(name[0]<='z')){
        return true;
    }
    if((name[0]>=0)&&(name[0]<=9)){
        return true;
    }
    return false;
}

static char* copyString(const char* str2){
    char *str1=malloc(sizeof(char)*(strLen(str2)+1));
    if(str1==NULL){
        return NULL;
    }
    int index=0;
    while(str2[index]){
        str1[index]=str2[index];
        index++;
    }
    str1[index]='\0';
    return str1;
}

static int strLen(const char* str){
    int counter=0;
    while(*str){
        str++;
        counter++;
    }
    return counter;
}

static Order newOrder(copySetElements copyElement,
                      freeSetElements freeElement,
                      compareSetElements compareElements){
    //mallocs a new order
    Order new_order=malloc(sizeof(*new_order));
    if(new_order==NULL){
        return NULL;
    }
    //mallocs a new empty amount set witch holds each product name,id,and amount in the order
    new_order->cart=asCreate(copyElement,freeElement,compareElements);
    if(new_order->cart==NULL){
        free(new_order);
        return NULL;
    }
    return new_order;
}

static Order setFind(Set set, unsigned int id){
    //get a pointer to the first order of the set
    Order current_order=setGetFirst(set);
    //searching for the order based on its id
    while(current_order!=NULL){
        if(compareId(current_order->order_id, id) == 0){//if we find the item we exit,the iterator is standing on the element
            return current_order;
        }
        current_order=setGetNext(set);
    }
    return NULL;
}

bool listContains(unsigned int id, List list){
    Product product=NULL;
    product=listGetFirst(list);
    while(product!=NULL){
        if(product->element_id==id){
            return true;
        }
        product=listGetNext(list);
    }
    return false;
}


bool checkAmountForOrder(Matamazom matamazom ,unsigned int orderId){
    Product product=NULL;//*
    double amount_order = 0;
    //find the order product in the set
    Order order=setFind(matamazom->orders,orderId);//
    //get a pointer to the order
    assert(order!=NULL);//*assert*
    unsigned int* product_id=asGetFirst(order->cart);//if returns null
    while(product_id!=NULL){
        //find the product in the warehouse
        listFind(matamazom->inventory, *product_id);
        //get the amount asked in the ordered product
        asGetAmount(order->cart,product_id,&amount_order);
        //get a pointer pointing at the product
        product=listGetCurrent(matamazom->inventory);
        //compare the amount in the order to the amount in the warehouse
        if(amount_order>(product->amount)){
            return false;
        }
        product_id=asGetNext(order->cart);
    }
    return true;
}

static void Profit(Matamazom matamazom,AmountSet cart){
    double amount=0;

    Product product=NULL;
    AS_FOREACH(unsigned int*,product_id,cart){
        listFind(matamazom->inventory, *product_id);
        asGetAmount(cart,product_id,&amount);
        if(amount<0){
            continue;
        }

        product=listGetCurrent(matamazom->inventory);
        product->profit+=product->productPrice(product->product_data,amount);
    }
}

static void changeProductsAmount(Matamazom matamazom,AmountSet cart){
    double amount=0;
    AS_FOREACH(unsigned int*,product_id,cart){
        listFind(matamazom->inventory, *product_id);
        asGetAmount(cart,product_id,&amount);
        mtmChangeProductAmount(matamazom,*product_id,-amount);
    }
}

MatamazomResult mtmPrintInventory(Matamazom matamazom, FILE *output){
    if(matamazom==NULL||output==NULL){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    fprintf(output,"Inventory Status:\n");
    Product  product=listGetFirst(matamazom->inventory);
    if(product==NULL){//if the list is empty(the inventory is empty)
        return 0;
    }
    //sort the list of products
    listSort(matamazom->inventory, compareProduct);
    //get a pointer to the first element of the sorted inventory and print its details
    product=listGetFirst(matamazom->inventory);
    mtmPrintProductDetails(product->element,product->element_id,product->amount,
                           product->productPrice(product->product_data,1),output);
    //jump to the next product in the inventory
    product=listGetNext(matamazom->inventory);

    while (product!=NULL){//iterate over the inventory and print each products details
        mtmPrintProductDetails(product->element,product->element_id,product->amount,
                               product->productPrice(product->product_data,1),output);
        product=listGetNext(matamazom->inventory);
    }
    return MATAMAZOM_SUCCESS;
}

MatamazomResult mtmPrintOrder(Matamazom matamazom, const unsigned int orderId, FILE *output){
    double total_price=0,ordered_product_amount=0;
    Product product=NULL;
    if(matamazom==NULL||output==NULL){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    //find the order based on its id
    Order order=setFind(matamazom->orders, orderId);
    if(order==NULL){
        return MATAMAZOM_ORDER_NOT_EXIST;
    }
    //print the heading
    mtmPrintOrderHeading(orderId,output);
    //get the first product of the order
    ASElement order_product_id=asGetFirst(order->cart);
    if(order_product_id==NULL){
        mtmPrintOrderSummary(0,output);
        return 0;
    }
    while (order_product_id!=NULL){
        //find the product in the inventory
        listFind(matamazom->inventory, *(unsigned int*)order_product_id);
        //get a pointer to the product
        product=listGetCurrent(matamazom->inventory);
        //we reached the end of the list
        if(product==NULL){
            break;
        }
        //get the amount of the ordered product
        asGetAmount(order->cart,order_product_id,&ordered_product_amount);
        //calculate the total profit of the product from the order
        double product_profit=product->productPrice(product->product_data,ordered_product_amount);
        total_price+=product_profit;
        //print the ordered product details
        mtmPrintProductDetails(product->element,product->element_id,ordered_product_amount,product_profit,output);
        //jump to the next product in the order
        order_product_id=asGetNext(order->cart);
    }
    mtmPrintOrderSummary(total_price,output);
    return MATAMAZOM_SUCCESS;
}

MatamazomResult mtmPrintBestSelling(Matamazom matamazom, FILE *output){
    if(matamazom==NULL||output==NULL){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    fprintf(output,"Best Selling Product:\n");
    Product best_selling_product=NULL;
    //get a pointer to the first product of the inventory
    Product product=listGetFirst(matamazom->inventory);
    if(product==NULL){
        fprintf(output,"none\n");
        return 0;
    }
    //mark it as the current best selling
    best_selling_product=product;
    //jump to the next product
    product=listGetNext(matamazom->inventory);
    while(product!=NULL){
        //if the current product is the highest selling so far
        if((product->profit)>(best_selling_product->profit)){
            best_selling_product=product;
        }else{
            //if they are equal we compare based on id
            if(product->profit==best_selling_product->profit){
                //if the id of our current product is smalled
                if(compareProduct(best_selling_product,product)>0){
                    best_selling_product=product;
                }
            }
        }
        product=listGetNext(matamazom->inventory);
    }
    if(best_selling_product->profit==0){
        fprintf(output,"none\n");
        return 0;
    }
    mtmPrintIncomeLine(best_selling_product->element,best_selling_product->element_id,
                       best_selling_product->profit,output);
    return MATAMAZOM_SUCCESS;
}

MatamazomResult mtmPrintFiltered(Matamazom matamazom, MtmFilterProduct
customFilter, FILE *output){
    if(matamazom==NULL||output==NULL||customFilter==NULL){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    Product product=listGetFirst(matamazom->inventory);
    while (product!=NULL){
        //if the product passes the filter print its details
        if(customFilter(product->element_id,product->element,product->amount,product->product_data)==true){
            mtmPrintProductDetails(product->element,product->element_id,product->amount,
                                   product->productPrice(product->product_data,1),output);
        }
        product=listGetNext(matamazom->inventory);
    }
    return MATAMAZOM_SUCCESS;
}

static MatamazomResult registerOrder(Order order,double amount,double order_amount,unsigned int product_id){
    if(amount==0){
        return MATAMAZOM_SUCCESS;
    }
    //if the product dosent exist in the cart
    if(!asContains(order->cart,&product_id)){
        if(amount>0){
            asRegister(order->cart,&product_id);
            assert(asContains(order->cart,&product_id));//*assert*
            asChangeAmount(order->cart,&product_id,amount);
            return MATAMAZOM_SUCCESS;
        }
        else{
            return MATAMAZOM_SUCCESS;
        }

    }
    if(amount<0){
        asGetAmount(order->cart,&order->order_id,&order_amount);
        if(order_amount+amount<0){//checks if the first is smaller than the second
            asDelete(order->cart,order);
            return MATAMAZOM_SUCCESS;
        }
    }
    //if the product exists but the amount is negative and greater than the current amount
    //if the product exist and we dont have to delete it or add it
    asChangeAmount(order->cart,&product_id,amount);
    return MATAMAZOM_SUCCESS;
}
