/**
  * soubor: argHandler.h
  * autor: Daniel Kamenický (xkamen21)
  * detaily: Hlavickovy soubor pro modul argHandler.cpp.
  */
#include <string>
#include <string.h>
#include "errorHandler.h"

// struktura pro zpracovani parametru
struct Data{
    bool paramV = false; //parametr filtrovani paketu
    bool paramT = false;
    std::string botToken;
};

/*
 * zpracovani parametru a osetreni spatnych vstupu
 * parametry: data - struktura pro ulozeni vysktu parametru a jejich dat
 *            argc - pocet parametru
 *            argv - pole parametru
 */
void get_args(Data *data, int argc, char const *argv[]);
