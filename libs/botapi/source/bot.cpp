#include <bot.h>

string urlencode(const char*);

TelegramBot::TelegramBot(const string& tok) : WEB_API(tok, "api.telegram.org"), offset(0){
    json result = method("getMe");
    if(result["ok"] == false)
        throw runtime_error("Invalid bot token");
}

long long get_msk_time(){
    return time(nullptr) + 3 * 60 * 60;
}

string get_formatted_time(long long epoch){
    string str;
    if(epoch / 60 / 60 % 24 < 10)
        str += '0';
    str += to_string(epoch / 60 / 60 % 24);
    str += ':';
    if(epoch / 60 % 60 < 10)
        str += '0';
    str += to_string(epoch / 60 % 60);
    return str;
}


string markdown_free(const string src){
    const char* str = src.c_str();
    string ret;
    while(*str) {
        if (*str == '_' || *str == '*' || *str == '`' || *str == '[' || *str == ']' || *str == '(' ||
            *str == ')' || *str == '~' || *str == '>' || *str == '#' || *str == '+' || *str == '-' ||
            *str == '=' || *str == '|' || *str == '{' || *str == '}' || *str == '.' || *str == '!')
            ret += "\\";
        ret += *str++;
    }

    //'_‘, ’*‘, ’[‘, ’]‘, ’(‘, ’)‘, ’~‘, ’`‘, ’>‘, ’#‘, ’+‘, ’-‘, ’=‘, ’|‘, ’{‘, ’}‘, ’.‘, ’!‘
    return ret;
}

string remove_markdown(const string src){
    const char* str = src.c_str();
    string ret;
    while(*str){
        if(*str == '*' || *str == '_' || *str == '`' || *str == '\"')
            str++;
        else
            ret += *str++;

    }
    return ret;
}


char int_to_hex_ansii(int num){
    switch (num) {
    case 0:
        return '0';
    case 1:
        return '1';
    case 2:
        return '2';
    case 3:
        return '3';
    case 4:
        return '4';
    case 5:
        return '5';
    case 6:
        return '6';
    case 7:
        return '7';
    case 8:
        return '8';
    case 9:
        return '9';
    case 10:
        return 'A';
    case 11:
        return 'B';
    case 12:
        return 'C';
    case 13:
        return 'D';
    case 14:
        return 'E';
    case 15:
        return 'F';
    default:
        return num;
    }
}

int hex_ansii_to_int(char num){
    switch (num) {
    case '0':
        return 0;
    case '1':
        return 1;
    case '2':
        return 2;
    case '3':
        return 3;
    case '4':
        return 4;
    case '5':
        return 5;
    case '6':
        return 6;
    case '7':
        return 7;
    case '8':
        return 8;
    case '9':
        return 9;
    case 'A':
        return 10;
    case 'B':
        return 11;
    case 'C':
        return 12;
    case 'D':
        return 13;
    case 'E':
        return 14;
    case 'F':
        return 15;
    default:
        return num;
    }
}

string urlencode(const char* raw){
    string encoded;
    while(*raw){
        if(*raw != '_' &&(*raw < '0' || *raw > '9') && (*raw < 'a' || *raw > 'z') && (*raw < 'A' || *raw > 'Z')){
            if(*raw == ' '){
                encoded.push_back('+');
            }else{
                encoded.push_back('%');
                if(uint(*raw) < 16){
                    encoded.push_back('0');
                    encoded.push_back(int_to_hex_ansii(*raw));
                }else{
                    encoded.push_back(int_to_hex_ansii((unsigned char)*raw / 16));
                    encoded.push_back(int_to_hex_ansii((unsigned char)*raw % 16));
                }
            }
        }else
            encoded.push_back(*raw);
        ++raw;
    }
    return encoded;
}

string urldecode(const char* raw){
    string decoded;
    while(*raw){
        if(*raw == '%'){
            char new_value = hex_ansii_to_int(*(++raw)) * 16 + hex_ansii_to_int(*(++raw));
            decoded.push_back(new_value);
        }else if(*raw == '+'){
            decoded.push_back(' ');
        }
        else
            decoded.push_back(*raw);
        ++raw;
    }
    return decoded;
}
