/**
  * soubor: stack.h
  * autor: Daniel Kamenický (xkamen21)
  * detaily: Hlavickovy soubor pro modul stack.cpp.
  */
#include <cstddef>
#include <cstdio>
#include <string>

struct tItem{
    std::string IDOfMessage;
    std::string nameOfAuthor;
    std::string content;
    tItem *next;
};

struct tStack{
    tItem *first;
};

/*
 * inicializace zasobniku
 * parametry: stack - zasobnik ktery je inicializovan
 */
void initStack(tStack *stack);

/*
 * vlozeni prvku do zasobniku
 * parametry: stack - zasobnik do ktereho je vkladan prvek
 *            new_item - prvek ktery je vkladan
 */
void push(tStack *stack, tItem *new_item);

/*
 * ziskani prvku ze zasobniku
 * parametry: stack - zasobnik ze ktereho je bran prvek
 */
tItem* pop(tStack *stack);

/*
 * inicializace prvku
 * parametry: prvek - prvek ktery je inicializovan
 */
void initItem(tItem *prvek);
