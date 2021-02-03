/**
  * soubor: argHandler.cpp
  * autor: Daniel Kamenický (xkamen21)
  * detaily: Modul pro zpracvani argumentu.
  */
#include "argHandler.h"

/*
 * zpracovani parametru a osetreni spatnych vstupu
 * parametry: data - struktura pro ulozeni vysktu parametru a jejich dat
 *            argc - pocet parametru
 *            argv - pole parametru
 */
void get_args(Data *data, int argc, char const *argv[])
{
    if(argc == 1){
        error("ERROR: nebyl zadan zadny paramter. zadejte -h|--help pro napovedu", 1);
    }
    else{
        bool dataAfterParam = false; //promenna pro kontrolu dat za parametrem
        std::string tmp; //pomocna promenna
        for (int i = 1; i < argc; i++) {
            //parametr --help
            if (!strcmp("--help", argv[i]) || !strcmp("-h", argv[i])) {
                if(argc == 2)
                {
                    printf(" ._______________________________________ HELP ________________________________________________.\n");
                    printf(" | Vypis vsech parametru a jejich funkcnost:                                                   |\n");
                    printf(" | Seznam parametru: -h|--help, -v|--verbose, -t                                               |\n");
                    printf(" | POZOR: parametr -t <bot_access_token> musi byt vzdy zadan                                   |\n");
                    printf(" |‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|\n");
                    printf(" | Parametr -h|--help, help, vypise pomocnou zpravu                                            |\n");
                    printf(" | Parametr -v|--verbose, zobrazovani zprav na standartni vystup (stdout)                      |\n");
                    printf(" | Parametr -t <bot_access_token>, predani autentizacniho tokenu pro pristup bota na discord   |\n");
                    printf(" *‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾*\n");
                    exit(0);
                }
                else
                {
                    error("ERROR: parametr --help nemuze byt kombinovany s jinymi parametry", 1);
                }
            }
            //parametr -t <bot_access_token>
            else if(!strcmp("-t", argv[i])){
                //kontrola duplicity parametru
                if(data->paramT)
                {
                    error("ERROR: duplicitni parametr -t", 1);
                }
                else if (dataAfterParam) {
                    tmp = "ERROR: chybejici data parametru " + tmp;
                    error(tmp, 1);
                }
                else{
                    data->paramT = true;
                    dataAfterParam = true;
                    tmp = argv[i];
                }
            }

            //parametr --verbose nebo -v
            else if(!strcmp("--verbose", argv[i]) || !strcmp("-v", argv[i])){
                //kontrola duplicity parametru
                if(data->paramV)
                {
                    error("ERROR: duplicitni parametr -v|--verbose", 1);
                }
                else if (dataAfterParam) {
                    tmp = "ERROR: chybejici data parametru " + tmp;
                    error(tmp, 1);
                }
                else{
                    data->paramV = true;
                    tmp = argv[i];
                }
            }

            else{
                //data po parametru
                if(dataAfterParam)
                {
                    data->botToken = argv[i];
                    dataAfterParam = false;
                }
                else
                {
                    tmp = argv[i];
                    tmp = "ERROR: neznamy parametr " + tmp;
                    error(tmp, 1);
                }
            }

        }
        //kontrola vyskty parametru -t na konci bez dat
        if (dataAfterParam) {
            error("ERROR: chybejici token", 1);
        }
    }
}
