#define LINUX

#ifdef LINUX
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <sys/stat.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <unistd.h>
#else
    #include <windows.h>
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #define close closesocket
#endif

#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <ssl.h>
#include <err.h>
#include <json.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>

#include <func.h>

using json = nlohmann::json;
using namespace std;

namespace Telegram {

    struct InlineKeyboardButton{
        string text;
        string callback_data;
        string url;
        InlineKeyboardButton(const string& _text, const string& _callback_data, const string& _url = "") : text(_text), callback_data(_callback_data), url(_url){}
        InlineKeyboardButton(){}
    };

    class Update{
    public:
        json data;
        int selector;
        enum select{
            MESSAGE,
            MESSAGE_FROM,
            MESSAGE_CHAT,
            CALLBACK_QUERY,
            CALLBACK_QUERY_FROM,
            CALLBACK_QUERY_MESSAGE,
            CALLBACK_QUERY_MESSAGE_CHAT,
            CALLBACK_QUERY_MESSAGE_FROM,
            PHOTO,
            VIDEO,
            BAD_VALUE
        };
    public:

        int sizes[3] = {1, 8, 64};
        enum callback_field{
            DELETE,
            TYPE,
            DATA
        };

        enum CALLBACK_TYPE{
            NULL_CALLBACK,
            EDIT_MENU = 1,
            NEW_BET,
            EDIT_NAME,
            EDIT_DESC,
            EDIT_PHOTO,
            EDIT_PRICE,
            EDIT_MIN_BET,
            EDIT_MAX_BET,
            EDIT_TIME,
            EDIT_TIME_CHOOSE,
            SEND_LOT,
            PREVIEW,
        };


        long long get_callback_encoded(CALLBACK_TYPE type, bool is_delete_service, long long data = 0) const{
            long long ret = 0;
            ret |= data;
            ret = (ret << sizes[callback_field::TYPE]) | type;
            ret = (ret << sizes[callback_field::DELETE]) | is_delete_service;
            return ret;
        }

        CALLBACK_TYPE get_callback_type(){
            int callback = atoi(get_callback_query().get_data().c_str());
            return (CALLBACK_TYPE)((callback >> sizes[callback_field::DELETE]) & 0b11111111);
        }

        bool get_callback_is_delete(){
            return atoi(get_callback_query().get_data().c_str()) & 1;
        }

        long long get_callback_data(){
            long long callback = atoll(get_callback_query().get_data().c_str());
            return callback >> (sizes[callback_field::DELETE] + sizes[callback_field::TYPE]);
        }

        Update(const json& obj) : data(obj), selector(BAD_VALUE){}
        Update() {}
        bool operator!(){
            if(data.is_null() || selector == BAD_VALUE)
                return true;
            return false;
        }
        operator bool(){
            return !(operator!());
        }
        bool operator||(Telegram::Update& obj){
            if(!obj && !*this)
                return false;
            return true;
        }
        string get_username(){
            switch (selector) {
            case MESSAGE_FROM:
                if(!data["message"]["from"]["username"].is_null())
                    return (string)(data["message"]["from"]["username"]);
                break;
            case MESSAGE_CHAT:
                if(!data["message"]["chat"]["username"].is_null())
                    return (string)(data["message"]["chat"]["username"]);
                break;
            case CALLBACK_QUERY_FROM:
                if(!data["callback_query"]["from"]["username"].is_null())
                    return (string)(data["callback_query"]["from"]["username"]);
                break;
            case CALLBACK_QUERY_MESSAGE_FROM:
                if(!data["callback_query"]["message"]["from"]["username"].is_null())
                    return (string)(data["callback_query"]["message"]["from"]["username"]);
                break;
            case CALLBACK_QUERY_MESSAGE_CHAT:
                if(!data["callback_query"]["message"]["chat"]["username"].is_null())
                    return (string)(data["callback_query"]["message"]["chat"]["username"]);
                break;
            }
            return "";
        }
        string get_first_name(){
            switch (selector) {
            case MESSAGE_FROM:
                if(!data["message"]["from"]["first_name"].is_null())
                    return (string)(data["message"]["from"]["first_name"]);
                break;
            case MESSAGE_CHAT:
                if(!data["message"]["chat"]["first_name"].is_null())
                    return (string)(data["message"]["chat"]["first_name"]);
                break;
            case CALLBACK_QUERY_FROM:
                if(!data["callback_query"]["from"]["first_name"].is_null())
                    return (string)(data["callback_query"]["from"]["first_name"]);
                break;
            case CALLBACK_QUERY_MESSAGE_FROM:
                if(!data["callback_query"]["message"]["from"]["last_name"].is_null())
                    return (string)(data["callback_query"]["message"]["from"]["last_name"]);
                break;
            case CALLBACK_QUERY_MESSAGE_CHAT:
                if(!data["callback_query"]["message"]["chat"]["last_name"].is_null())
                    return (string)(data["callback_query"]["message"]["chat"]["last_name"]);
                break;
            }
            return "";
        }
        string get_last_name(){
            switch (selector) {
            case MESSAGE_FROM:
                if(!data["message"]["from"]["last_name"].is_null())
                    return (string)(data["message"]["from"]["last_name"]);
                break;
            case MESSAGE_CHAT:
                if(!data["message"]["chat"]["last_name"].is_null())
                    return (string)(data["message"]["chat"]["last_name"]);
                break;
            case CALLBACK_QUERY_FROM:
                if(!data["callback_query"]["from"]["last_name"].is_null())
                    return (string)(data["callback_query"]["from"]["last_name"]);
                break;
            case CALLBACK_QUERY_MESSAGE_FROM:
                if(!data["callback_query"]["message"]["from"]["last_name"].is_null())
                    return (string)(data["callback_query"]["message"]["from"]["last_name"]);
                break;
            case CALLBACK_QUERY_MESSAGE_CHAT:
                if(!data["callback_query"]["message"]["chat"]["last_name"].is_null())
                    return (string)(data["callback_query"]["message"]["chat"]["last_name"]);
                break;
            }
            return "";
        }
        long long get_id(){
            switch (selector) {
                case MESSAGE:
                    return (long long)(data["message"]["message_id"]);
                case CALLBACK_QUERY_MESSAGE:
                    return (long long)(data["callback_query"]["message"]["message_id"]);
                case MESSAGE_FROM:
                    return (long long)(data["message"]["from"]["id"]);
                case MESSAGE_CHAT:
                    return (long long)(data["message"]["chat"]["id"]);
                case CALLBACK_QUERY:
                    return (long long)(data["callback_query"]["id"]);
                case CALLBACK_QUERY_FROM:
                    return (long long)(data["callback_query"]["from"]["id"]);
                case CALLBACK_QUERY_MESSAGE_FROM:
                    return (long long)(data["callback_query"]["message"]["from"]["id"]);
                case CALLBACK_QUERY_MESSAGE_CHAT:
                    return (long long)(data["callback_query"]["message"]["chat"]["id"]);
                default:
                    return 0;
            }
        }
        string get_text(){
            switch (selector){
                case MESSAGE:
                    if(!data["message"]["text"].is_null())
                        return string(data["message"]["text"]);
                    break;
                case CALLBACK_QUERY_MESSAGE:
                    if(!data["callback_query"]["message"]["text"].is_null())
                        return string(data["callback_query"]["message"]["text"]);
                    break;
            }
            return "";
        }

        Update& get_message(){
            switch (selector){
                case CALLBACK_QUERY:
                    if(data["callback_query"]["message"].is_null())
                        selector = BAD_VALUE;
                    else
                        selector = CALLBACK_QUERY_MESSAGE;
                    break;

                default:
                    if(data["message"].is_null())
                        selector = BAD_VALUE;
                    else
                        selector = MESSAGE;
                    break;

            }
            return *this;
        }

        bool is_message(){
            if(selector == MESSAGE)
                return true;
            return false;
        }

        bool is_callback(){
            if(selector == CALLBACK_QUERY)
                return true;
            return false;
        }

        string get_data(){
            if(selector != CALLBACK_QUERY)
                return "";
            return string(data["callback_query"]["data"]);
        }
        Update& get_callback_query(){
            if(data["callback_query"].is_null())
                selector = BAD_VALUE;
            else
                selector = CALLBACK_QUERY;
            return *this;
        }
        Update& get_from(){
            switch (selector) {
            case MESSAGE:
                if(!data["message"]["from"].is_null())
                    selector = MESSAGE_FROM;
                break;
            case CALLBACK_QUERY:
                if(!data["callback_query"]["from"].is_null())
                    selector = CALLBACK_QUERY_FROM;
                break;
            case CALLBACK_QUERY_MESSAGE:
                if(!data["callback_query"]["message"]["from"].is_null())
                    selector = CALLBACK_QUERY_MESSAGE_FROM;
                break;
            default:
                selector = BAD_VALUE;
                break;
            }
            return *this;
        }
        Update& get_chat(){
            switch(selector){
                case MESSAGE:
                    if(data["message"]["chat"].is_null())
                        selector = BAD_VALUE;
                    else
                        selector = MESSAGE_CHAT;
                    break;
                case CALLBACK_QUERY_MESSAGE:
                    if(data["callback_query"]["message"]["chat"].is_null())
                        selector = BAD_VALUE;
                    else
                        selector = CALLBACK_QUERY_MESSAGE_CHAT;
                    break;
            }
            return *this;
        }
        Update& get_photo(){
            if(selector != MESSAGE || data["message"]["photo"].is_null())
                selector = BAD_VALUE;
            else
                selector = PHOTO;
            return *this;
        }

        Update& get_video(){
            if(selector != MESSAGE || data["message"]["video"].is_null())
                selector = BAD_VALUE;
            else
                selector = VIDEO;
            return *this;
        }
        string get_file_id(){
            switch (selector){
                case PHOTO:{
                    int photos = data["message"]["photo"].size();
                    return string(data["message"]["photo"][photos - 1]["file_id"]);
                }
                case VIDEO: {
                    int videos = data["message"]["video"].size();
                    return string(data["message"]["video"][videos - 1]["file_id"]);
                }
                default:
                    return "";
            }
        }
    };
}

static SSL_METHOD *meth;

struct SSLCONTEXT{
    static void init(){
        SSL_library_init();
        SSLeay_add_ssl_algorithms();
        SSL_load_error_strings();
        meth = const_cast<SSL_METHOD*>(TLSv1_2_client_method());
    }
};

class HTTPResponse{
	string response;
public:
    HTTPResponse(string msg) : response(msg){}
    string findBody(){
        const char* str = response.c_str();
            while(*str != 0)
                   if(*str++ == '\r')
                       if(*str++ == '\n')
                           if(*str++ == '\r')
                               if(*str++ == '\n')
                                   return str;
        return "";
    }
    const string& getText(){
        return response;
    }
    json toJSON(){
        return json::parse(findBody());
    }
};

class HTTPSocket{
    sockaddr_in sin;
protected:
    int nSocket;
public:
    enum HTTP_methods{
        GET,
        POST,
        PATCH,
        PUT
    };
    enum HTTP_content_type{
        URLENCODE,
        JSON
    };
    HTTPSocket(const string& domain, int port = 80){
        hostent* h = gethostbyname(domain.c_str());
        sin.sin_family = AF_INET;
        sin.sin_port = htons(port);
        memcpy(&sin.sin_addr.s_addr, h->h_addr, sizeof(int));
        open();
    }
    virtual ~HTTPSocket(){
        close();
    }
    virtual void open(){
        if((nSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
            throw runtime_error("Error creating HTTP socket");
        if(connect(nSocket, reinterpret_cast<sockaddr*>(&sin), sizeof (sin)))
            throw runtime_error("Error creating HTTP connection");
    }
    virtual void close(){
        ::close(nSocket);
    }
};

class HTTPSSocket : public HTTPSocket{
    SSL_CTX* ctx;
    SSL* sslSocket;
public:
    HTTPSSocket(const string& domain, int port = 443) : HTTPSocket(domain, port), ctx(0), sslSocket(0){
        open();
    }
    void refresh(){
        close();
	HTTPSocket::close();
        HTTPSocket::open();
	open();
    }
    void open(){
        SSLCONTEXT::init();
        ctx = SSL_CTX_new(meth);
        if(!ctx){
            cout << ERR_get_error()  << endl;
            throw runtime_error("Error creating SLL context");
        }
        sslSocket = SSL_new(ctx);
        if(!sslSocket)
            throw runtime_error("Error creating SSL socket");
        SSL_set_fd(sslSocket, nSocket);
        if(SSL_connect(sslSocket) != 1)
            throw runtime_error("Error creating SSL connection");
    }
    ~HTTPSSocket(){
        close();
    }
    void close(){
        SSL_shutdown(sslSocket);
        SSL_CTX_free(ctx);
    }
    virtual HTTPResponse query(const string& sendBytes) {
        int toShip = sendBytes.size();
        int sended = 0;
        do
            sended += SSL_write(sslSocket, sendBytes.c_str() + sended, toShip);
        while (sended != toShip);

        constexpr int block_size = 10000;
        int recv_buff_offset = 0;
        char *recv_buffer = new char[1024 * 1024 * 10]{0}; // 10 mb
        int readed = 0;
        do{
            readed = SSL_read(sslSocket, recv_buffer + recv_buff_offset, block_size);
            recv_buff_offset += readed;
        }
        while(readed == block_size || (recv_buff_offset % (1024 * 16) == 0 && recv_buff_offset > 0));
        cout << "readed: " << strlen(recv_buffer) << endl;
//        do {
//            received_now = SSL_read(sslSocket, buffer + received, sizeBuff);
//            received += received_now;
//            cout << "received now: " << received_now << endl;
//        } while (received_now == 16384);
//        buffer[received] = 0;
        string buff(recv_buffer);
        delete[] recv_buffer;
        return HTTPResponse(buff);
    }
};

class WEB_API{
protected:
    HTTPSSocket httpsSocket;
    string token, host;
public:
    WEB_API(const string& tok, const string& _host) : httpsSocket(_host), token(tok), host(_host){}
    json method(const string& path, vector<string> headers = {},
                const string& content = "",
                HTTPSocket::HTTP_methods method_type = HTTPSocket::HTTP_methods::GET){
        string query;
        if(method_type == HTTPSocket::HTTP_methods::GET)
            query += "GET ";
        else if(method_type == HTTPSocket::HTTP_methods::POST)
            query += "POST ";
        else if(method_type == HTTPSocket::HTTP_methods::PATCH)
            query += "PATCH ";
        else if(method_type == HTTPSocket::HTTP_methods::PUT)
            query += "PUT ";
        query += path;
        if(method_type == HTTPSocket::HTTP_methods::GET)
            query += "?" + content;
        query += string(" HTTP/1.1\r\n")
                 + "HOST:" + host + "\r\n";

        for(auto it = headers.begin(); it != headers.end(); ++it)
            query += *it + "\r\n";
        if(method_type == HTTPSocket::HTTP_methods::POST || method_type == HTTPSocket::HTTP_methods::PUT){
            query += "Content-Length: " + to_string(content.size()) + "\r\n\r\n";
            query += content;
        }else
            query += "\r\n";

        cout << "\nQuery: " << query << "\n";
        HTTPResponse answer = httpsSocket.query(query);
        cout << "Answer: " << answer.getText() << endl;
        if(answer.findBody().size()){
            json janswer = answer.toJSON();
            return janswer;
        }
        return json();
    }
};

class QIWI : public WEB_API{
    string number;
public:
    enum withdraw_type{
        QIWI_RF = 99,
        VISA_RF = 1963,
        VISA_SNG = 1960,
        MIR = 31652,
        VIRTUAL_SNG = 22351,
        MASTERCARD_RF = 21013,
        MASTERCARD_SNG = 21012,
    };

    struct input_transaction{
        unsigned long long id;
        int summ;
        string comment;
    };
    QIWI(const string& token, const string& _number) : WEB_API(token, "edge.qiwi.com"), number(_number){}
    json getProfile(){
        return method("/person-profile/v1/profile/current", {"Authorization: Bearer " + token});
    }

    double getCommission(withdraw_type id, string account, int amount){
        string content = "{\"account\":\"" + account + "\", "
                         + "\"purchaseTotals\": {\"total\":{\"amount\" : " + to_string(amount) + ", \"currency\" : \"643\"}},"
                         + "\"paymentMethod\":{\"type\":\"Account\", \"accountId\":\"643\"}"
                           "}";
        json ret = method("/sinap/providers/" + to_string(id) + "/onlineCommission",
                      {"Accept: application/json", "Authorization: Bearer " + token,
                       "Content-type: application/json"},
                      content, HTTPSocket::HTTP_methods::POST);
        if(!ret["qwCommission"].is_null())
            if(!ret["qwCommission"]["amount"].is_null())
                return double(ret["qwCommission"]["amount"]);
        return -1;
    }

    vector<input_transaction>* getSuccessPayments(int count = 50){
        auto ptrans = new vector<input_transaction>(0);
        json ret = method("/payment-history/v2/persons/" + number + "/payments",
                          {"Accept: application/json", "Authorization: Bearer " + token},
                          "rows="
                          + to_string(count)
                          + "&operation=IN&sources[0]=QIWI_RUB");
        if(!ret["data"].is_null()){
            for(int i = 0; i < ret["data"].size(); ++i){
                if(string(ret["data"][i]["status"]) != "SUCCESS")
                    continue;
                string comment;
                if(!ret["data"][i]["comment"].is_null())
                    comment = string(ret["data"][i]["comment"]);
                ptrans->push_back({(unsigned long long)ret["data"][i]["txnId"], int(ret["data"][i]["sum"]["amount"]), comment});
            }
        }
        return ptrans;
    }

    string sendQiwi(int id, withdraw_type type, const string& account, int amount, const string& comment = " "){
        string content = "{\"id\":\"" + to_string(id) + "\", "
                            + "\"sum\": {\"amount\" : " + to_string(amount) + ", \"currency\" : \"643\"},"
                            + "\"fields\" : {\"account\":\"" + account + "\"},"
                            + "\"paymentMethod\":{\"type\":\"Account\", \"accountId\":\"643\"}"
                            "}";
        json ret = method("/sinap/api/v2/terms/"+ to_string(type) +"/payments",
                        {"Accept: application/json", "Authorization: Bearer " + token,
                         "Content-type: application/json"},
                      content, HTTPSocket::HTTP_methods::POST);
        if(ret["message"].is_null())
            return "SUCCESS";
        else return string(ret["message"]);
    }
};

class QIWI_BILLING : public WEB_API{
public:
    QIWI_BILLING(const string& token) : WEB_API(token, "api.qiwi.com"){}
    string get_url(int billId, int amount, int comment){
        string content = "{\"amount\": {\"currency\":\"RUB\", \"value\":" + to_string(amount) + ".00}, \"comment\":\"" + to_string(comment) + "\", \"expirationDateTime\":\"2050-04-13T14:30:00+03:00\"}";
        json ret = method("/partner/bill/v1/bills/" + to_string(billId),
                          {"Accept: application/json", "Authorization: Bearer " + token,
                           "Content-type: application/json"},
                          content, HTTPSocket::HTTP_methods::PUT);
        if(!ret["payUrl"].is_null())
            return string(ret["payUrl"]);
        return "";
    }
};

class TelegramBot : public WEB_API{
    int offset;
public:

    json removeMessages(long long chat_id, const vector<int>& elems){
        for(auto it = elems.begin(); it != elems.end(); ++it){
            deleteMessage(chat_id, *it);
        }
    }
    json method(const string& method, const string& args = ""){
        return WEB_API::method("/bot" + token + "/" + method, {"Content-Type: application/x-www-form-urlencoded"}, args, HTTPSocket::HTTP_methods::POST);
    }
    TelegramBot(const string& tok);
    json sendMessage(const string& chat_id, const string& text){
        return method("sendMessage", "parse_mode=MarkdownV2&text=" + urlencode(text.c_str()) + "&chat_id=" + chat_id);
    }

    json pinChatMessage(long long chat_id, int message_id){
        return method("pinChatMessage", "chat_id=" + to_string(chat_id) + "&message_id=" + to_string(message_id));
    }

    json unpinChatMessage(long long chat_id){
        return method("unpinChatMessage", "chat_id=" + to_string(chat_id));
    }

    json sendInlineKeyboard(const string& chat_id, const string& text,
                            const vector<vector<Telegram::InlineKeyboardButton>>& inline_buttons = vector<vector<Telegram::InlineKeyboardButton>>(0)){
        if(!inline_buttons.size())
            return sendMessage(chat_id, text);


        string inline_keyboard = "{\"inline_keyboard\" : [";
        for(vector<vector<Telegram::InlineKeyboardButton>>::const_iterator it = inline_buttons.begin(); it != inline_buttons.end(); ++it){
            inline_keyboard += '[';
            for(vector<Telegram::InlineKeyboardButton>::const_iterator jt = it->begin(); jt != it->end(); jt++){
                string temp;
                for(int i = 0; i < jt->text.size(); ++i){
                    if(jt->text[i] == '"')
                        temp += '\\';
                    temp += jt->text[i];
                }

                if(!jt->url.size())
                    inline_keyboard += "{\"text\" : \"" + temp + "\", \"callback_data\" : \"" + jt->callback_data + "\"}";
                else
                    inline_keyboard += "{\"text\" : \"" + temp + "\", \"url\" : \"" + jt->url + "\"}";

                if((jt + 1) != it->end())
                    inline_keyboard += ',';
            }
            inline_keyboard += ']';
            if((it + 1) != inline_buttons.end())
                inline_keyboard += ',';
        }
        inline_keyboard += "]}";
        return method("sendMessage", "parse_mode=MarkdownV2&text=" + urlencode(text.c_str()) + "&chat_id=" + chat_id + "&reply_markup=" + urlencode(inline_keyboard.c_str()));
    }
    json sendPhoto(const string& chat_id, const string& photo){
        return method("sendPhoto", "photo=" + photo + "&chat_id=" + chat_id);
    }

    json sendVideo(const string& chat_id, const string& video){
        return method("sendVideo", "video=" + video + "&chat_id=" + chat_id);
    }
    json deleteMessage(long long chat_id, int message_id){
        return method("deleteMessage", "message_id=" + to_string(message_id) + "&chat_id=" + to_string(chat_id));
    }

    bool isChatMember(string chat_id, string user_id){
        json ret = method("getChatMember", "chat_id=" + chat_id + "&user_id=" + user_id);
        if (ret["ok"] == true)
            if(ret["result"]["status"] == "member" || ret["result"]["status"] == "creator" || ret["result"]["status"] == "administrator")
                return true;
        return false;
    }
    bool isChatMembers(const vector<string>& chat_ids, const string& user_id){
        for(int i = 0; i < chat_ids.size(); ++i){
            json ret = method("getChatMember", "chat_id=" + chat_ids[i] + "&user_id=" + user_id);
            if (ret["ok"] == true)
                if(!(ret["result"]["status"] == "member") && !(ret["result"]["status"] == "creator") && !(ret["result"]["status"] == "administrator"))
                    return false;
        }
        return true;
    }

    vector<Telegram::Update>* getUpdates(int delay = 300){
        string arg = "limit=100&offset=";
        arg.append(to_string(offset));
        arg.append("&timeout=");
        arg.append(to_string(delay));
        json response = method("getUpdates", arg);

        if(!response["result"].size())
            return new vector<Telegram::Update>(0);

        int updates = response["result"].size();
        bool first = true;
        if(offset)
            first = false;
        offset = response["result"][updates - 1]["update_id"];
        offset++;
        if(first)
            getUpdates(0);
        vector<Telegram::Update>* upd = new vector<Telegram::Update>(updates);
        for(int i = 0; i < updates; ++i)
            (*upd)[i] = response["result"][i];
        return upd;
    }
    json peekUpdates(int delay = 0){
        string arg = "limit=1&offset=";
        arg.append(to_string(offset));
        arg.append("&timeout=");
        arg.append(to_string(delay));
        json response = method("getUpdates", arg);
        return response;
    }

    json getChat(const string& chat_id){
        return method("getChat", "chat_id="+chat_id);
    }
    json forwardMessage(const string& chat_id, const string& from_chat_id, const string& message_id){
        return method("forwardMessage", "message_id=" + message_id + "&chat_id=" + chat_id + "&from_chat_id=" + from_chat_id);
    }
};

class User{
    long long user_telegram_id;
    long long chat_telegram_id;
    string username;
    Telegram::Update update;

public:
    string get_username(){
        return username;
    }

    long long get_chat_telegram_id(){
        return chat_telegram_id;
    }

    int user_sendMessage(TelegramBot& bot, const string& text){
        json result = bot.sendMessage(to_string(user_telegram_id), text);
        if(!result["result"]["message_id"].is_null()){
            int message_id = int(result["result"]["message_id"]);
            return message_id;
        }
        return 0;
    }

    int chat_sendMessage(TelegramBot& bot, const string& text){
        json result = bot.sendMessage(to_string(chat_telegram_id), text);
        if(!result["result"]["message_id"].is_null()){
            int message_id = int(result["result"]["message_id"]);
            return message_id;
        }
        return 0;
    }

    int user_send_keyboard(TelegramBot& bot, const string& text, const vector<vector<Telegram::InlineKeyboardButton>>& inline_buttons){
        json result = bot.sendInlineKeyboard(to_string(user_telegram_id), text, inline_buttons);
        if(!result["result"]["message_id"].is_null())
            return int(result["result"]["message_id"]);
        return 0;
    }
    int chat_send_keyboard(TelegramBot& bot, const string& text, const vector<vector<Telegram::InlineKeyboardButton>>& inline_buttons){
        json result = bot.sendInlineKeyboard(to_string(chat_telegram_id), text, inline_buttons);
        if(!result["result"]["message_id"].is_null())
            return int(result["result"]["message_id"]);
        return 0;
    }

    User(const Telegram::Update& _update){
        update = _update;

        if(update.get_message()){
            user_telegram_id = update.get_message().get_from().get_id();
            chat_telegram_id = update.get_message().get_chat().get_id();
            username = update.get_message().get_from().get_username();
        }else if(update.get_callback_query()){
            user_telegram_id = update.get_callback_query().get_from().get_id();
            chat_telegram_id = update.get_callback_query().get_message().get_chat().get_id();
            username = update.get_callback_query().get_from().get_username();
        }
    }

    long long get_user_telegram_id() const{
        return user_telegram_id;
    }
};

