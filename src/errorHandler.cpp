/**
  * soubor: errorHandler.cpp
  * autor: Daniel Kamenický (xkamen21)
  * detaily: Modul pro rizeni chybovych hlaseni a ukonceni.
  */
#include "errorHandler.h"

/*
 * vypise chybovou zpravu na stderr a ukonci program
 * parametry: msg - chybova zprava
 *            retval - navratova hodnota
 */
void error(std::string msg, int retval)
{
    fprintf(stderr, "%s\n", msg.c_str());
    exit(retval);
}
