/**
  * soubor: SSLClient.h
  * autor: Daniel Kamenický (xkamen21)
  * detaily: Hlavickovy soubor pro modul SSLClient.cpp.
  */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <cstring>
#include <netdb.h>
#include <thread>
#include <unistd.h>
#include <mutex>
#include <condition_variable>
#include "parser.h"
#include "errorHandler.h"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

/*
* zahajeni SSL spojeni a spusteni vlaken pro reciever a packetsHandler
* parametry: botToken - token bota pro GET/POST
*            paramV - verbose, pro vypisovani na stdout
*/
int startConnection(std::string botToken, bool paramV);

/*
* inicializace ssl spojeni s discord.com
*/
int sslInit();

/*
 * ridi odpovedi na dotazy
 * pri ztraceni spojeni, zkousi obnovit
 */
int reciever();

/*
 * odeslani paketu
 * parametry: buf - paket pro odeslani (GET, POST)
 */
int SendPacket(const char* buf);

/*
 * stara se o spravne predavani packetu GET a POST
 * parametry: botToken - token bota pro GET/POST
 */
void packetsHandler(std::string botToken);

/*
 * kontroluje zdali se v odpovedi nachazi Connection: close
 */
int connectionCloseHandler();
