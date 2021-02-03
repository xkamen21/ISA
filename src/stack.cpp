/**
  * soubor: stack.cpp
  * autor: Daniel Kamenický (xkamen21)
  * detaily: Modul pro praci s zasobnikem.
  */
#include "stack.h"

/*
 * inicializace zasobniku
 * parametry: stack - zasobnik ktery je inicializovan
 */
void initStack(tStack *stack){
    if(stack==NULL){
        fprintf(stderr, "Spatne vytvoreny zasobnik\n");
        exit(8);
    }
    stack->first = NULL;
}

/*
 * vlozeni prvku do zasobniku
 * parametry: stack - zasobnik do ktereho je vkladan prvek
 *            new_item - prvek ktery je vkladan
 */
void push(tStack *stack, tItem *new_item){
    tItem *tmp = new tItem();
    tmp->IDOfMessage = new_item->IDOfMessage;
    tmp->nameOfAuthor = new_item->nameOfAuthor;
    tmp->content = new_item->content;
    tmp->next = stack->first;
    stack->first = tmp;
}

/*
 * ziskani prvku ze zasobniku
 * parametry: stack - zasobnik ze ktereho je bran prvek
 */
tItem* pop(tStack *stack){
    tItem *tmp = new tItem();
    tmp = stack->first;
    if(tmp!=NULL){
        stack->first = stack->first->next;
    }
    return tmp;
}

/*
 * inicializace prvku
 * parametry: prvek - prvek ktery je inicializovan
 */
void initItem(tItem *prvek){
    if(prvek==NULL){
        fprintf(stderr, "Spatne vytvoreny prvek\n");
        exit(9);
    }
    prvek->next = NULL;
    prvek->IDOfMessage = "";
    prvek->nameOfAuthor = "";
    prvek->content = "";

}
