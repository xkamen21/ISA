/**
  * soubor: SSLClient.cpp
  * autor: Daniel Kamenický (xkamen21)
  * detaily: Modul pro inicializaci a zahajeni SSL spojeni.
  *          Ridi posilani paketu a prijamni jejich odpovedi.
  */
#include "SSLClient.h"

// semafor pro rizeni odesilani a prijimani paketu
std::mutex mtx;

//pro wait na unlock
std::condition_variable cv;

//typ GET pozadavku
int getType = 0;
// promenna pro urceni POST pozadavku
bool post = false;
// struktura pro ulozeni (tokenu, ID Guildy, odpovedi, ..)
pData *data;
// struktura pro ukladani zprav
tStack *stack;
SSL *ssl;

/*
 * zahajeni SSL spojeni a spusteni vlaken pro reciever a packetsHandler
 * parametry: botToken - token bota pro GET/POST
 *            paramV - verbose, pro vypisovani na stdout
 */
int startConnection(std::string botToken, bool paramV){
    // inicializace SSL
    if(sslInit()==-1){
        return -1;
    }
    // inicializace struktury pData
    data = new pData();
    data->paramV = paramV;
    // inicializace struktury tStack
    stack = new tStack();
    initStack(stack);
    //zahajeni vlaken
    std::thread th2(reciever);
    std::thread th1(packetsHandler, botToken);
    th1.join();
    th2.join();
    return 0;
}

/*
 * inicializace ssl spojeni s discord.com
 */
int sslInit()
{
    // vytvareni soketu
    int Socket;
    Socket = socket(AF_INET, SOCK_STREAM, 0);
    if (Socket < 0) {
        fprintf(stderr,"ERROR: spatne vytvoreny soket.\n");
        return -1;
    }


    struct sockaddr_in serverAddress;
    memset (&serverAddress, 0, sizeof(serverAddress));

    // ziskani ip adresy domeny discord.com
    struct hostent *remoteHost = gethostbyname("discord.com");
    struct in_addr in;
    bcopy(*remoteHost->h_addr_list++, (char *) &in, sizeof(in));
    if(inet_pton(AF_INET, inet_ntoa(in), &serverAddress.sin_addr)<=0){
        fprintf(stderr, "ERROR: nevalidni adresa.\n");
        return -1;
    }

    // pripojeni soketu k serveru
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(443);
    if (connect(Socket, (struct sockaddr *)&serverAddress, sizeof(serverAddress))) {
        fprintf(stderr, "ERROR: spatne pripojeni k serveru.\n");
        return -1;
    }

    // inicializace SSL spojeni, kod prevzat od uzivatele "O.logN" na foru stackoverflow.com.
    // url: https://stackoverflow.com/questions/41229601/openssl-in-c-socket-connection-https-client
    SSL_library_init();
    SSLeay_add_ssl_algorithms();
    SSL_load_error_strings();
    const SSL_METHOD *meth = TLSv1_2_client_method();
    SSL_CTX *ctx = SSL_CTX_new (meth);
    ssl = SSL_new (ctx);
    if (!ssl) {
        fprintf(stderr, "ERROR: spatne vytvoreni SSL.\n");
        return -1;
    }
    // int fd = SSL_get_fd(ssl);
    SSL_set_fd(ssl, Socket);
    int err = SSL_connect(ssl);
    if (err <= 0) {
        fprintf(stderr, "Error spatne vytvoreni SSL pripojeni.\n");
        return -1;
    }
    return 0;
}

/*
 * ridi odpovedi na dotazy
 * pri ztraceni spojeni, zkousi obnovit
 */
int reciever()
{
    //maximalni delka retezce
    int len=100;
    //buffer pro ukladani odpovedi
    char buffer[1000000];
    do {
        // ziskani odpovedi
        len=SSL_read(ssl, buffer, 100);
        buffer[len]=0;
        // ukladani odpovedi do globalni promenne
        data->response += buffer;

        // kontrola autorizace bota
        if(strstr(buffer, "401 Unauthorized")!=NULL){
            // vnitrni kontrola zda nebyla poslana zprava s "401 Unauthorized"
            std::string tmp;
            for(long unsigned int i = 0; i<data->response.length()-1; i++){
                if(data->response[i]=='\n'){
                    break;
                }
                tmp += data->response[i];
            }
            if(tmp.find("401 Unauthorized")!=std::string::npos){
                error("ERROR: bot token Unauthorized", 4);
            }
        }

        // kontrola konce odpovedi
        if(!strcmp(buffer, "0\r\n\r\n") || strstr(buffer, "\r\n\r\n[]")!=NULL){
            // zpracovani GET odpovedi
            if(!post){
                doOperation(getType, data, stack);
                sleep(1 + data->reset);
            }
            // kontrola rate-limit v odpovedi, pripadne cekani
            else{
                rateHandler(data);
                sleep(data->reset);
            }
            // pri ukonceni spojeni, nova inicializace a nove SSL pripojeni
            if(connectionCloseHandler()==1){
                if(sslInit() == -1){
                    error("ERROR: selhalspatna inicializace SSL", 2);
                }
            }
            data->response = "";
            //odmknuti semaforu pro prijem dalsich odpovedi
            cv.notify_one();
            continue;
        }
    } while (len > 0);
    cv.notify_one();
    // ukonceni spojeni pri spatnem cteni v SSL_reader
    if (len < 0) {
        error("ERROR: spatne cteni v SSL_reader", 3);
    }
    return 0;
}

/*
 * odeslani paketu
 * parametry: buffer - paket pro odeslani (GET, POST)
 */
int SendPacket(const char *buffer)
{
    int len = SSL_write(ssl, buffer, strlen(buffer));
    if (len < 0)
        return -1;
    return 0;
}

/*
 * stara se o spravne predavani packetu GET a POST
 * parametry: botToken - token bota pro GET/POST
 */
void packetsHandler(std::string botToken){
    // pomocna struktura pro brani prvku ze zasobniku zprav
    tItem *tmp = new tItem;
    initItem(tmp);
    data->response = "";
    data->botToken = botToken;
    std::string request;
    std::string Prequest;
    //nekonecny while cyklus pro odesilani dotazu
    while(1){
        post = true;
        //cyklus pro POST dotazy, cykli dokud neni zasobnik prazdny
        while(1){
            if(getType != 3){
                break;
            }
            Prequest = "";
            //ziskani prvku ze zasobniku
            tmp = pop(stack);
            if(tmp == NULL){
                break;
            }else if(data->paramV){
                // pokud je nastaven parametr -v|--verbose, vypisuje zpravy na stdout
                printf("echo: %s - %s\n", tmp->nameOfAuthor.c_str(), tmp->content.c_str());
            }
            // ziskani POST dotazu
            Prequest = postRequest(data, tmp);

            // zablokovani semaforu, dokud neni zpracovan dotaz, neodesle se dalsi
            std::unique_lock<std::mutex> lock2(mtx);

            // odeslani POST dotazu
            SendPacket(Prequest.c_str());
            cv.wait(lock2);
            lock2.unlock();
        }
        post = false;
        request = "";
        if(getType!=3){
            getType++;
        }
        // ziskani GET dotazu
        request = getRequest(getType, data);

        // zablokovani semaforu, dokud neni zpracovan dotaz, neodesle se dalsi
        std::unique_lock<std::mutex> lock1(mtx);

        // odeslani GET dotazu
        SendPacket(request.c_str());
        cv.wait(lock1);
        lock1.unlock();
    }
}

/*
 * kontroluje zdali se v odpovedi nachazi Connection: close
 */
int connectionCloseHandler(){
    bool found_from = false;
    bool found_to = false;
    bool connection_found = false;
    std::string buffer = "";
    // for cyklus pro prochazeni odpovedi
    for(long unsigned int i = 0; i<data->response.length(); i++){
        // pokud byl nalezen podretezec Connection:
        if(connection_found){
            buffer="";
            // kontrola vysktytu "close"
            for(long unsigned int j = i;j<=i+4;j++){
                buffer+=data->response[j];
            }
            // jedna-li se o close, potreba reinicializovat SSL pripojeni
            if(!buffer.compare("close")){
                return 1;
            }
            else{
                return 0;
            }
        }

        // rozdeleni hledanyho retezce na casti
        // musi zacinat na novym radku a s 'C'
        if(data->response[i] == 'C' && !found_from){
            found_from = true;
        }
        // nalezeni dalsiho 'C' znamena nespravny podretezec
        else if(data->response[i] == 'C' && found_from){
            found_from = false;
            buffer="";
        }
        else if(!found_from){
            buffer="";
            continue;
        }

        //pokud byl nalezen podretezec Connection: nastaveni flagu connection_found
        if(found_to){
            if(!buffer.compare("Connection:")){
                connection_found = true;
            }
            else{
                found_to = false;
                found_from = false;
                buffer="";
            }
        }
        else{
            found_to = false;
        }

        //kontrola ukonceni podretezce znakem ':'
        if(data->response[i] == ':' && found_from){
            found_to = true;
        }
        buffer += data->response[i];
    }
    return 0;
}
