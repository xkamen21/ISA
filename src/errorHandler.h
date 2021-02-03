/**
  * soubor: errorHandler.h
  * autor: Daniel Kamenický (xkamen21)
  * detaily: Hlavickovy soubor pro modul errorHandler.cpp.
  */
#include <cstdlib>
#include <cstdio>
#include <string>

/*
 * vypise chybovou zpravu na stderr a ukonci program
 * parametry: msg - chybova zprava
 *            retval - navratova hodnota
 */
void error(std::string msg, int retval);
