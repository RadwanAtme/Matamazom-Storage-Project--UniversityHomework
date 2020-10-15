
#ifndef HW1_STATIC_FUNCTIONS_H
#define HW1_STATIC_FUNCTIONS_H

#include "matamazom.h"
#include "amount_set.h"
/*static functions for amount_set*/
/*
 * copyInt:copies the id of an amountset and returns a copy of the copied id
 */
static ASElement copyInt(ASElement number);//copy element function for amount_set
/*
 * freeInt:frees the element(id) of the amountset
 */
static void freeInt(ASElement number);//free element function for amount_set
/*
 * compareInts:compares between two elements(id) of the amount_set
 *
 * &param:two elements of the amount set of type unsigned int*
 *
&return:positive number if id1 is bigger
 *          negative if id2 is bigger
 *          0 if they are equal */
static int compareInts(ASElement lhs, ASElement rhs);






/*static functions other*/

/*
 * compareId:compares between two numbers
 *
 * $param:two numbers of type unsigned int
 *
&return:positive number if id1 is bigger
 *          negative if id2 is bigger
 *          0 if they are equal */
static  int compareId(unsigned int id1, unsigned int id2);
/*
 * checkAmount:checks if the amount given and the amount type are compatible
 *
 * &param:amount of type double and amountType of type MatamazomAmountType
 *
 * &return:true if they are compatible
 *          false otherwise
 */
static bool checkAmount(double amount, MatamazomAmountType amountType);
/*
 *checkName:checks if a name is valid based on the instructions given by the course team
 *
 * &param:a name of type const char*
 *
 * &return:true if the name is valid
 *          false otherwise
 */
static bool checkName(const char* name);
/*
 * halfIntTest:checks if the amount is compatible with the amount type MATAMAZOM_HALF_INTEGER_AMOUNT
 *
 * &param:amount of type double
 *
 * &return:true if the amount is compatible
 *          false otherwise
 */
static bool halfIntTest(double amount);
/*
 * intTest:checks if the amount is compatible with the amount type MATAMAZOM_INTEGER_AMOUNT
 * * &param:amount of type double
 *
 * &return:true if the amount is compatible
 *          false otherwise
 */
static bool intTest(double amount);
/*
 * strLen:return the length of a string
 *
 * &param:string of type const char*
 *
 * &return:the length of the string(dosent count \0)
 */
static int strLen(const char* str);
/*
 * copyString:copies a string and returns a pointer to the copied string
 *
 * &param:string of type const char*
 *
 * &return:a pointer to the copied string in case of success
 *          NULL otherwise
 */
static char* copyString(const char* str2);
/*
 * profit:calculates the profit made for each product from the last order and updates the profit field in the inventory
 *
 * &param:warehouse of type Matamazom,set of type amount_set containing the ordered products
 */
static void Profit(Matamazom matamazom,AmountSet cart);

#endif //HW1_STATIC_FUNCTIONS_H
