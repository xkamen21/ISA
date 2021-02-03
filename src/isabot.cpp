/**
  * soubor: isabot.cpp
  * autor: Daniel Kamenický (xkamen21)
  * detaily: Hlavni modul.
  */
#include "isabot.h"

int main(int argc, char const *argv[]) {
    Data *BotData = new Data;
    //kontrola a ziskani argumentu
    get_args(BotData, argc, argv);
    if(!BotData->paramT){
        error("ERROR: nebyl zadan bot_access_token", 1);
    }
    //zahajeni SSL spojeni
    if(startConnection(BotData->botToken, BotData->paramV)==-1){
        error("ERROR: selhalspatna inicializace SSL", 2);
    }
    return 0;
}
