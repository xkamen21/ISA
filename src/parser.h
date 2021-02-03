/**
  * soubor: parser.h
  * autor: Daniel Kamenický (xkamen21)
  * detaily: Hlavickovy soubor pro parser.cpp.
  */
#include <cstdlib>
#include <cstdio>
#include <string>
#include "errorHandler.h"
#include "stack.h"

struct pData{
    std::string botToken;
    std::string IDOfGuild;
    std::string IDOfChannel;
    std::string IDOfLastMessage;
    std::string response;
    bool paramV = false;
    int reset = 0;
};

/*
 * vytvari GET pozadavek
 * parametry: getType - urcuje o jaky poazadvek se jedna
 *            data - struktura dat (bot token, ...) pro spravne zkonstruovani
 */
std::string getRequest(int getType, pData *data);

/*
 * Vytvari POST pozadavek
 * parametry: data - struktura dat (bot token, ...) pro spravne zkonstruovani
 *            prvek - struktura dat pro spravne zkonstruovani pozadavku
 */
std::string postRequest(pData *data, tItem *prvek);

/*
 * vybere jaka operace/funkce je potreba provest
 * parametry: getType - typ operace
 *            data - struktura dat (bot token, ...) pro predani funkci
 *            stack - struktura zasobniku pro predani funkci
 */
void doOperation(int getType, pData *data, tStack *stack);

/*
 * ziska ID guildy, ID channelu a ID posledni zpravy
 * parametry: getType - urcuje typ operace
 *            data - struktura dat (bot token, ...) pro ukladani ziskanych dat
 */
void guildIDChannelIDLastMessageID(int getType, pData *data);

/*
 * zpracovni dat z jednotlivych zprav
 * parametry: data - struktura dat (bot token, ...)
 *            stack - struktura zasobniku pro ukladani ziskanych dat ze zprav
 */
void parseData(pData *data, tStack *stack);

/*
 * kontrola x-ratelimit-remaining a x-ratelimit-reset-after
 * udava prikaz pro pripadny uspani, aby nedoslo k prehlceni
 * parametry: data - struktura dat pro ziskani odpovedi
 */
void rateHandler(pData* data);
