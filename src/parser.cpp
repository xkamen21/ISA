/**
  * soubor: parser.cpp
  * autor: Daniel Kamenický (xkamen21)
  * detaily: Modul pro ziskani dat z odpovedi.
  */
#include "parser.h"

bool first_init = true;

/*
 * vytvari GET pozadavek
 * parametry: getType - urcuje o jaky poazadvek se jedna
 *            data - struktura dat (bot token, ...)
 */
std::string getRequest(int getType, pData *data){
    std::string get = "";
    // pozadavek pro ziskani guildy v ktere se nachazi bot
    if(getType == 1){
        get += "GET /api/users/@me/guilds HTTP/1.1\r\nHost: discord.com\r\nAuthorization: Bot ";
        get += data->botToken;
        get += "\r\nContent-Type: application/json\r\n\r\n";
    }

    // pozadavek pro ziskani channelu do kterych bot vidi v dane guilde
    else if(getType == 2){
        if(data->IDOfGuild.length() == 0){
            error("ERROR: nebylo ziskano ID guildy", 5);
        }
        get += "GET /api/guilds/";
        get += data->IDOfGuild;
        get += "/channels HTTP/1.1\r\nHost: discord.com\r\nAuthorization: Bot ";
        get += data->botToken;
        get += "\r\nContent-Type: application/json\r\n\r\n";
    }

    // pozadavek pro ziskani zprav v channelu
    else if(getType >= 3){
        // jedna-li se o novy channel kde nejsou zadne zpravy
        if(data->IDOfLastMessage.length()==0 && first_init){
            printf("TADYY\n");
            first_init = false;
            get += "GET /api/channels/";
            get += data->IDOfChannel;
            get += "/messages HTTP/1.1\r\nHost: discord.com\r\nAuthorization: Bot ";
            get += data->botToken;
            get += "\r\nContent-Type: application/json\r\n\r\n";
        }
        // ziskani zprav z channelu pomoci parametru "after"
        else{
            first_init = false;
            get += "GET /api/channels/";
            get += data->IDOfChannel;
            get += "/messages?after=";
            get += data->IDOfLastMessage;
            get += " HTTP/1.1\r\nHost: discord.com\r\nAuthorization: Bot ";
            get += data->botToken;
            get += "\r\nContent-Type: application/json\r\n\r\n";

        }
    }
    return get;
}

/*
 * Vytvari POST pozadavek
 * parametry: data - struktura dat (bot token, ...) pro spravne zkonstruovani
 *            prvek - struktura dat pro spravne zkonstruovani pozadavku
 */
std::string postRequest(pData *data, tItem *prvek){
    // nastaveni ID posledni zpravy
    data->IDOfLastMessage = prvek->IDOfMessage;
    //vytvorenu contentu pro POST
    std::string content = "{\"content\": \"echo: ";
    content += prvek->nameOfAuthor;
    content += " - ";
    content += prvek->content;
    content += "\"}";

    // zkonstruovani vysledne podoby POST dotazu
    std::string post = "";
    post += "POST /api/channels/";
    post += data->IDOfChannel;
    post += "/messages HTTP/1.1\r\nHost: discord.com\r\nAuthorization: Bot ";
    post += data->botToken;
    post += "\r\nContent-Type: application/json\r\nContent-Length: ";
    post += std::to_string(content.length());
    post += "\r\n\r\n";
    post += content;
    post += "\r\n\r\n";

    return post;
}

/*
 * vybere jaka operace/funkce je potreba provest
 * parametry: getType - typ operace
 *            data - struktura dat (bot token, ...) pro predani funkci
 *            stack - struktura zasobniku pro predani funkci
 */
void doOperation(int getType, pData *data, tStack *stack){
    if(getType == 1 || getType == 2){
        guildIDChannelIDLastMessageID(getType, data);
    }
    else if(getType >= 3){
        parseData(data, stack);
    }
}

/*
 * ziska ID guildy, ID channelu a ID posledni zpravy
 * parametry: getType - urcuje typ operace
 *            data - struktura dat (bot token, ...) pro ukladani ziskanych dat
 */
void guildIDChannelIDLastMessageID(int getType, pData *data){
    bool found_to = false;
    bool found_from = false;
    bool id_found = false;
    bool id_copy = false;
    bool lastMessageID_found = false;
    bool lastMessageID_copy = false;
    int to_index = 0;
    int from_index = 0;

    // ziskani json dat z odpovedi, useknuti hlavicky
    // ulozi index od kud data zacinaji a index kde data konci
    for(int i = data->response.length()-1; i>0;i--){
        if(data->response[i] == ']' && !found_to){
            to_index = i;
            found_to = true;
        }
        if(found_from && data->response[i] == '\n'){
            from_index = i+1;
            break;
        }
        else{
            found_from = false;
        }
        if(data->response[i] == '['){
            found_from = true;
        }
    }


    bool isTextChannel = false;
    std::string tmp = "";
    // projde json data
    while(from_index<=to_index){
        // potrebne data jsou v ""
        // do tmp ukladame podretezec "tmp"
        if(data->response[from_index] == '"'){
            from_index++;
            tmp = "";
            while(data->response[from_index]!='"'){
                tmp += data->response[from_index];
                from_index++;
            }
            if(!tmp.compare("id")){
                //pokud bylo jednou id nalezeno, ale nenasel se nazev isa-bot, bylo to id jineho channelu
                if(id_copy){
                    //vycisti pole
                    if(getType == 1){
                        data->IDOfGuild = "";
                        id_copy = false;
                    }
                    else{
                        data->IDOfChannel = "";
                        data->IDOfLastMessage = "";
                        id_copy = false;
                    }
                }
                //nastavi flag nalezeni ID
                id_found = true;
            }
            else if(id_found){
                //zkopiruje ID guildy
                if(getType == 1){
                    data->IDOfGuild = tmp;
                    return;
                }
                //zkopiruje ID channelu
                else{
                    data->IDOfChannel = tmp;
                }
                id_copy = true;
                id_found = false;
            }
            else if(!tmp.compare("last_message_id")){
                if(lastMessageID_copy){
                    //vycisti pole
                    data->IDOfLastMessage = "";
                    lastMessageID_copy = false;
                }
                tmp = "";
                // kontrola zda-li se nejedna o novy channel a posledni zprava je rovna "null"
                for(int j = from_index+3; j<from_index+7; j++){
                    tmp+=data->response[j];
                }
                if(!tmp.compare("null")){
                    data->IDOfLastMessage = "";
                    lastMessageID_copy = true;
                }
                else{
                    //nastavi flag nalezeni ID
                    lastMessageID_found = true;
                }
            }
            else if(lastMessageID_found){
                //zkopiruje ID guildy
                data->IDOfLastMessage = tmp;
                lastMessageID_copy = true;
                lastMessageID_found = false;
            }
            // kontrola typu, hleda text channel -> type: 0
            else if(!tmp.compare("type")){
                if(data->response[from_index+3]%48 == 0){
                    isTextChannel = true;
                }else{
                    isTextChannel = false;
                }
            }
            // text channel ma jmeno isa-bot => byl nalezen spravny channel
            else if(!tmp.compare("isa-bot")&&isTextChannel){
                return;
            }
        }
        from_index++;
    }
    // chybove vypisy pro jednotlive typy operaci
    if(getType==1){
        error("ERROR: nebyla nalezena guilda", 6);
    }
    else{
        error("ERROR: nebyl nalezen text channel s nazvem isa-bot", 7);
    }
}

/*
 * zpracovni dat z jednotlivych zprav
 * parametry: data - struktura dat (bot token, ...)
 *            stack - struktura zasobniku pro ukladani ziskanych dat ze zprav
 */
void parseData(pData *data, tStack *stack){
    // kontrola rate-limitu
    rateHandler(data);
    bool found_to = false;
    bool found_from = false;

    bool isAuthor = false;
    bool content_found = false;
    bool userNameFound = false;

    bool id_found = false;
    int to_index = 0;
    int from_index = 0;

    // ziskani json dat z odpovedi, useknuti hlavicky
    // ulozi index od kud data zacinaji a index kde data konci
    for(int i = data->response.length()-1; i>0;i--){
        if(data->response[i] == ']' && !found_to){
            to_index = i;
            found_to = true;
        }
        if(found_from && data->response[i] == '\n'){
            from_index = i+1;
            break;
        }
        else{
            found_from = false;
        }
        if(data->response[i] == '['){
            found_from = true;
        }
    }
    //pomocna promenna pro podretezce
    std::string tmp;
    //struktura jednotlive zpravy
    tItem *new_item = new tItem();
    initItem(new_item);
    // pocitadlo zkontrolovanych dat v jednotlive zprave
    int counter = 0;

    bool tryAttachments = false;
    bool attachmentsFound = false;
    bool urlFound = false;
    bool doNotTakeID = false;

    // projde json data
    while(from_index<=to_index){
        // potrebne data jsou v ""
        // do tmp ukladame podretezec "tmp"
        if(data->response[from_index] == '"'){
            from_index++;
            tmp = "";
            while(data->response[from_index]!='"'||data->response[from_index+2]!=' '){
                tmp += data->response[from_index];
                from_index++;
            }
            // id zpravy, nejedna-li se o autora nebo prilohu
            if(!tmp.compare("id") && !isAuthor && !tryAttachments && !doNotTakeID){
                id_found = true;
            }
            //ulozeni ID zpravy
            else if(id_found){
                new_item->IDOfMessage = tmp;
                id_found = false;
                counter++;
            }
            // obsah zpravy
            else if(!tmp.compare("content")){
                content_found = true;
            }
            // ulozeni obsahu zpravy
            else if(content_found){
                new_item->content = tmp;
                content_found = false;
                counter++;
            }
            // nalezeni pole autora
            else if(!tmp.compare("author")){
                isAuthor = true;
            }
            // pole jmena autora, nastaveni flagu
            else if(!tmp.compare("username") && !attachmentsFound){
                userNameFound = true;
            }
            // ulozeni jmena autora dane zpravy
            else if(userNameFound){
                new_item->nameOfAuthor = tmp;
                userNameFound = false;
                counter++;
                isAuthor = false;
                tryAttachments = true;
            }
            else if(!tmp.compare("embeds")){
                doNotTakeID = true;
            }
            else if(!tmp.compare("flags")){
                doNotTakeID = false;
            }
            // ma-li zprava prilohu
            if(tryAttachments){
                //nastaveni falgu prilohy
                if(!tmp.compare("attachments")){
                    attachmentsFound = true;
                }
                //priloha nebyla nalezena zjistime pomoci nalezeni dalsiho prvku jsona, tim je "embeds"
                else if(!tmp.compare("embeds")){
                    // zkontrolovani, zda nemela zprava nejaky obsah, pokud ano counter nastavi na 4 pro ulozeni zpravy
                    if(!new_item->content.compare("")){
                        counter = 0;
                    }
                    else{
                        counter = 4;
                    }
                    tryAttachments = false;
                    attachmentsFound = false;
                    urlFound = false;
                }
                // nalezeni proxy url prilohy
                else if(attachmentsFound && !tmp.compare("proxy_url")){
                    urlFound = true;
                }
                //ulozeni proxy url prilohy a pripojeni ji k obsahu zpravy s pridanim noveho radku
                else if(urlFound){
                    if(new_item->content.compare("")){
                        tmp = " \\n " + tmp;
                    }
                    new_item->content += tmp;
                    counter = 4;
                    tryAttachments = false;
                    attachmentsFound = false;
                    urlFound = false;

                }
            }
            // "counter == 4" => znaci nalezeni vsech potrebnych dat pro ulozeni zpravy
            if(counter == 4 && new_item->nameOfAuthor.find("bot")==std::string::npos){
                counter = 0;
                push(stack, new_item);
            }
        }
        from_index++;
    }
}

/*
 * kontrola x-ratelimit-remaining a x-ratelimit-reset-after
 * udava prikaz pro pripadny uspani, aby nedoslo k prehlceni
 * parametry: data - struktura dat pro ziskani odpovedi
 */
void rateHandler(pData* data){
    int rate_limit = -1;
    int rate_reset = -1;
    bool found_to = false;
    bool found_from = false;
    std::string buf = "";
    // prochazi odpoved
    for(long unsigned int i = 0; i<data->response.length(); i++){
        // nalezeni zacatku podretezce
        if(data->response[i] == 'x'){
            if(found_from){
                buf="";
            }
            found_from = true;
        }
        // nalezen konec i zacatek podretezce
        if(found_to){
            //jedna-li se o hledany podretezec
            if(!buf.compare("x-ratelimit-remaining:")){
                //ulozeni x-ratelimit-remaining
                rate_limit = (data->response[i+1])%48;
            }
            else if(!buf.compare("x-ratelimit-reset-after:")){
                //ulozeni x-ratelimit-reset-after
                rate_reset = (data->response[i+1])%48;
            }
            // nalezeny oba typy ratelimit -> ukonci hledani
            if(rate_limit != -1 && rate_reset != -1){
                break;
            }
        }
        else{
            found_to = false;
        }
        // nalezeni konce podretezce
        if(data->response[i] == ':'){
            found_to = true;
        }
        buf += data->response[i];
    }
    // je-li limit roven 0, nastavime dobu cekani, nez se resetuje
    if(rate_limit == 0){
        data->reset = rate_reset;
    }
    else{
        data->reset = 0;
    }
}
