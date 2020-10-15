#include <stdio.h>
#include <stdlib.h>
#include "amount_set.h"
#include<assert.h>
#include "Node.h"






struct AmountSet_t{
    Node head;
    Node iterator;
    CopyASElement N_copy;
    FreeASElement N_free;
    CompareASElements N_compare;
};


AmountSet asCreate(CopyASElement copyElement,
                   FreeASElement freeElement,
                   CompareASElements compareElements){
    if(!copyElement||!freeElement||!compareElements){
        return NULL;
    }
    AmountSet set=malloc(sizeof(*set));
    if(set==NULL){
        return NULL;
    }
    set->head=NULL;
    set->iterator=NULL;
    set->N_compare=compareElements;
    set->N_copy=copyElement;
    set->N_free=freeElement;
    return set;

}
void asDestroy(AmountSet set){
    asClear(set);
    free(set);
}

AmountSet asCopy(AmountSet set){
    if(set==NULL){
        return NULL;
    }
    AmountSet new_set=asCreate(set->N_copy,set->N_free,set->N_compare);
   if(set->head==NULL){
       return new_set;
   }
    new_set->head=CopyNode(set,set->head);
    Node TempIterator1=set->head;
    Node TempIterator2=new_set->head;
    TempIterator1=TempIterator1->next;
//runs until we get to the last node
    while(TempIterator1!=NULL){
        //we make a new copy
        Node new_copy=CopyNode(set,TempIterator1);
        //we link the new copy to the set
        TempIterator2->next=new_copy;
        TempIterator2=TempIterator2->next;
        TempIterator1=TempIterator1->next;
    }

    return new_set;
}

int asGetSize(AmountSet set){
    if(set==NULL){
        return -1;
    }
    int amount=0;
    AS_FOREACH(Node,iterator_t,set){
        amount++;
    }
    return amount;
}
bool asContains(AmountSet set, ASElement element){
    Node iterator_t=set->head;
    for(;iterator_t;iterator_t=iterator_t->next){
        if(set->N_compare(iterator_t->element,element)==0){
            return true;
        }
    }
    return false;
}

AmountSetResult asGetAmount(AmountSet set, ASElement element, double *outAmount){
    //if we recive null pointer
    if(set==NULL){
        return AS_NULL_ARGUMENT;
    }
    //if we recive null pointer
    if(outAmount==NULL){
        return AS_NULL_ARGUMENT;
    }
    if(asContains(set,element)==false){
        return AS_ITEM_DOES_NOT_EXIST;
    }
    //find the struct that contains the element and return a pointer to it
    Node TempIterator=FindElementNoChange(set,element);
    *outAmount=TempIterator->amount;
    return AS_SUCCESS;
}
AmountSetResult asRegister(AmountSet set, ASElement element){
    //if we recive a null pointer
    if(set==NULL){
        return AS_NULL_ARGUMENT;
    }
    //if the item already exists
    if(asContains(set,element)==true){
        return AS_ITEM_ALREADY_EXISTS;
    }
    //create a new node
    Node temp=malloc(sizeof(*temp));
    if(temp==NULL) {
        return AS_NULL_ARGUMENT;
    }
    //fill up node
    temp->element=set->N_copy(element);
    temp->amount=0;
    temp->next=NULL;
    //if the set is empty
    if(set->head==NULL){
        set->head=temp;
        asGetFirst(set);
        set->iterator->next=NULL;
        return AS_SUCCESS;
    }
    //if the node is smaller than the head
    if(set->N_compare(element,(set->head->element))<0){
        temp->next=set->head;
        set->head=temp;
        return AS_SUCCESS;
    }
    //iterator for jumping over the list
    asGetFirst(set);
    while(set->iterator->next!=NULL){
        //if the new node is bigger than the next element
        if(set->N_compare(element,set->iterator->next->element)>0){
            assert(set->N_compare(element,set->iterator->next->element)!=0);
            asGetNext(set);
        }
            //we found the right place for the node
        else{
            temp->next=set->iterator->next;
            set->iterator->next=temp;
            break;
        }

    }
    //we are in the last place
    if(set->iterator){
        set->iterator->next=temp;
    }
    return AS_SUCCESS;
}
AmountSetResult asChangeAmount(AmountSet set, ASElement element, const double amount){
    if(set==NULL){
        return AS_NULL_ARGUMENT;
    }
    if(!asContains(set,element)){
        return AS_ITEM_DOES_NOT_EXIST;
    }
    Node iterator_t=FindElementNoChange(set,element);{
        if(iterator_t->amount+amount<0){
            return AS_INSUFFICIENT_AMOUNT;
        }
    }
    iterator_t->amount=iterator_t->amount+amount;
    return AS_SUCCESS;

}
AmountSetResult asDelete(AmountSet set, ASElement element){
    if(set==NULL){
        return AS_NULL_ARGUMENT;
    }
    if(asContains(set,element)==false){
        return AS_ITEM_DOES_NOT_EXIST;
    }
    asGetFirst(set);
    Node element_iterator=FindElementNoChange(set,element);
    if(element_iterator==set->head){
        //we make the second node as the head
        set->head=set->head->next;
        set->N_free(element_iterator->element);
        free(element_iterator);
    }
    else{
        while(set->iterator->next!=element_iterator){
            set->iterator=set->iterator->next;
        }
        linkAfterNextNode(set);
        set->N_free(element_iterator->element);
        free(element_iterator);
    }
    return AS_SUCCESS;
}
AmountSetResult asClear(AmountSet set){
    if(set==NULL){
        return AS_NULL_ARGUMENT;
    }
    Node temp=NULL;
    for(Node iterator_t=set->head;iterator_t;){
        temp=iterator_t;
        iterator_t=iterator_t->next;
        set->N_free(temp->element);
        free(temp);
    }
    set->head=NULL;
    set->iterator=NULL;
    return AS_SUCCESS;
}

ASElement asGetFirst(AmountSet set){
    if(set==NULL||set->head==NULL){
        return NULL;
    }
    set->iterator=set->head;
    return set->iterator->element;
}

ASElement asGetNext(AmountSet set){
    if(set==NULL){
        return NULL;
    }
    if(set->iterator->next==NULL){
        return NULL;
    }
    set->iterator=set->iterator->next;
    return set->iterator->element;
}

//the function recives a set and a pointer to a node and returns a copy of the node
 Node CopyNode(AmountSet set,Node original_node){
    Node new_copy=malloc(sizeof(*original_node));
    if(new_copy==NULL){
        return NULL;
    }
    new_copy->amount=original_node->amount;
    new_copy->element=set->N_copy(original_node->element);
    new_copy->next=NULL;
    return new_copy;
}

//function recives a set and an element and returns a pointer to the node that contains the element
 Node FindElementNoChange(AmountSet set,ASElement element){
    Node temp=set->head;
    while(temp){
        if(set->N_compare(temp->element,element)==0){
            return temp ;
        }
        temp=temp->next;
    }
    return NULL;
}

static void linkAfterNextNode(AmountSet set){
    set->iterator->next=set->iterator->next->next;
}