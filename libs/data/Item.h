//
// Created by puff on 13.06.2020.
//

#ifndef DATA_ITEM_H
#define DATA_ITEM_H

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class DBFile{
private:
    void to_begin_get(){
        file.seekg(ios_base::beg);
    }
    bool is_open;
    string filename;
    fstream file;
protected:
    bool truncate(){
        if(is_file_open())
            close();
        file.open(filename, ios_base::trunc | ios_base::out);
        close();
    }

    bool is_file_open(){ return is_open; }
    bool open(){
        if(!is_file_open()){
            file.open(filename, ios_base::out | ios_base::in | ios_base::app | ios_base::ate | ios_base::binary);
            is_open = true;
        }
        to_begin_get();
    }
    bool close(){
        file.close();
        is_open = false;
    }

    int write_string(const string& str){
        file << str;
        put_byte(0); //end of string
    }
    int write_int(long long num){ //int size = 8
        if(!is_file_open())
            throw runtime_error("file is not open in write");

        for(int i = 7; i >= 0; --i)
            file.put((num >> i * 8));
    }
    void put_byte(int byte){
        file.put(byte);
    }

    char peek_byte(){
        return file.peek();
    }

    string read_string(){
        string str;
        while(file.peek() != 0)
            str += file.get();
        file.get();
        return str;
    }
    long long read_int(){
        long long num = 0;
        for(int i = 0; i < 8; ++i){
            num = num << 8;
            num |= file.get();
        }
        return num;
    }
public:
    DBFile(const string& _filename) : filename(_filename), is_open(false){}
    ~DBFile(){
        close();
    }
};

struct Base{
    Base() : to_delete(false){}
    int id;
    bool to_delete;
    void mark_delete(){
        to_delete = true;
    }
};

struct Auction : Base{
    long long start_date;
    long long end_date;

    enum STATUS{
        EDIT,
        WAITING,
        ACTIVE,
        CLOSED,
    } status;

    int price;
    int min_step;
    int max_step;

    string winner_1;
    string winner_2;
    string winner_3;

    int winner_1_summ;
    int winner_2_summ;
    int winner_3_summ;

    string name;
    string desc;
    string photo_1;
    string photo_2;
    string photo_3;

    Auction(long long _start_date = 0, long long _end_date = 0, STATUS _status = STATUS::EDIT, int _price = 0, int _min_step = 25, int _max_step = 50,
            const string& _winner_1 = "", const string& _winner_2 = "", const string& _winner_3 = "", int _winner_1_summ = 0, int _winner_2_summ = 0, int _winner_3_summ = 0,
            const string& _name = "", const string& _desc = "", const string& _photo_1 = "", const string& _photo_2 = "", const string& _photo_3 = ""){
        start_date = _start_date;
        end_date = _end_date;
        status = _status;
        price = _price;
        min_step = _min_step;
        max_step = _max_step;
        winner_1 = _winner_1;
        winner_2 = _winner_2;
        winner_3 = _winner_3;
        winner_1_summ = _winner_1_summ;
        winner_2_summ = _winner_2_summ;
        winner_3_summ = _winner_3_summ;
        name = _name;
        desc = _desc;
        photo_1 = _photo_1;
        photo_2 = _photo_2;
        photo_3 = _photo_3;
    }
};

struct Message : Base{
    long long telegram_chat_id;
    int message_id;

    enum MESSAGE_TYPE{
        NULL_OPERATION = 0,
        NAME,
        DESC,
        PHOTO,
        PRICE,
        MIN_BET,
        MAX_BET,
        UNDEFINED_USERNAME
    } operation;

    Message(long long _telegram_chat_id, int _message_id, MESSAGE_TYPE _operation = NULL_OPERATION)
    : telegram_chat_id(_telegram_chat_id), message_id(_message_id), operation(_operation){
    }
};

template <class T> class ContainerInterface{
protected:
    vector<T> elems;
public:
    T* add(const T& elem){
        T buff = elem;
        buff.id = get_last_id() + 1;
        elems.push_back(buff);
        return &elems[elems.size() - 1];
    }
    vector<T>* get_elems(){
        return &elems;
    }
    T* find(int id){
        for(auto it = elems.begin(); it != elems.end(); ++it){
            if(it->id == id)
                return &(*it);
        }
        return nullptr;
    }
    void refresh(const T& elem){
        for(auto it = elems.begin(); it != elems.end(); ++it){
            if(it->id == elem.id) {
                *it = elem;
                return;
            }
        }
    }
    int get_last_id(){
        if(elems.size())
            return elems[elems.size() - 1].id;
        return 0;
    }

    T* get_last(){
        if(elems.size())
            return &elems[elems.size() - 1];
        return nullptr;
    }
    virtual bool save_all() = 0;
    virtual ~ContainerInterface(){}
};

class DBAuction : private DBFile, public ContainerInterface<Auction>{
public:
    DBAuction() : DBFile("dbfiles/auction.bin"){
        open();
        while(peek_byte() != EOF) {
            int id = read_int();
            long long start_date = read_int();
            long long end_date = read_int();
            int status = read_int();

            int price = read_int();
            int min_step = read_int();
            int max_step = read_int();

            string winner_1 = read_string();
            string winner_2 = read_string();
            string winner_3 = read_string();

            int winner_1_summ = read_int();
            int winner_2_summ = read_int();
            int winner_3_summ = read_int();

            string name = read_string();
            string desc = read_string();
            string photo_1 = read_string();
            string photo_2 = read_string();
            string photo_3 = read_string();

            elems.push_back(Auction(start_date, end_date, (Auction::STATUS)status, price, min_step, max_step, winner_1, winner_2, winner_3, winner_1_summ, winner_2_summ, winner_3_summ, name, desc, photo_1, photo_2, photo_3));
            elems[elems.size() - 1].id = id;
        }
        close();
    }

    bool save_all(){
        vector<Auction> new_elems;
        truncate();
        open();
        for(auto it = elems.begin(); it != elems.end(); ++it){
            if(it->to_delete)
                continue;
            Auction& auc = *it;
            write_int(auc.id);
            write_int(auc.start_date);
            write_int(auc.end_date);
            write_int(auc.status);
            write_int(auc.price);
            write_int(auc.min_step);
            write_int(auc.max_step);

            write_string(auc.winner_1);
            write_string(auc.winner_2);
            write_string(auc.winner_3);

            write_int(auc.winner_1_summ);
            write_int(auc.winner_2_summ);
            write_int(auc.winner_3_summ);

            write_string(auc.name);
            write_string(auc.desc);
            write_string(auc.photo_1);
            write_string(auc.photo_2);
            write_string(auc.photo_3);
            new_elems.push_back(auc);
        }
        close();
        elems = new_elems;
    }

    Auction* get_edit(){
        for(auto it = elems.begin(); it != elems.end(); ++it)
            if(!it->to_delete && it->status == Auction::STATUS::EDIT)
                return &(*it);
        return nullptr;
    }

    Auction* get_wait(){
        for(auto it = elems.begin(); it != elems.end(); ++it)
            if(!it->to_delete && it->status == Auction::STATUS::WAITING)
                return &(*it);
        return nullptr;
    }

    Auction* get_active(){
        for(auto it = elems.begin(); it != elems.end(); ++it)
            if(!it->to_delete && it->status == Auction::STATUS::ACTIVE)
                return &(*it);
        return nullptr;
    }
};

class DBMessage : private DBFile, public ContainerInterface<Message>{
public:
    DBMessage() : DBFile("dbfiles/message.bin"){
        open();
        while(peek_byte() != EOF) {
            int id = read_int();
            long long telegram_chat_id = read_int();
            int message_id = read_int();
            Message::MESSAGE_TYPE op = (Message::MESSAGE_TYPE)read_int();

            elems.push_back(Message(telegram_chat_id, message_id, op));
            elems[elems.size() - 1].id = id;
        }
        close();
    }

    Message* get_last_service_message(){
        for(int i = elems.size(); i > 0; --i)
            if(!elems[i - 1].to_delete && elems[i - 1].operation)
                return &elems[i - 1];
        return nullptr;
    }

    bool save_all(){
        vector<Message> new_elems;
        truncate();
        open();
        for(auto it = elems.begin(); it != elems.end(); ++it){
            if(it->to_delete)
                continue;
            write_int(it->id);
            write_int(it->telegram_chat_id);
            write_int(it->message_id);
            write_int(it->operation);
            new_elems.push_back(*it);
        }
        close();
        elems = new_elems;
    }

};

#endif //DATA_ITEM_H
