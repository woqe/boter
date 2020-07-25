#include <string>
//
// Created by puff on 22.03.2020.
//

#ifndef AUK_FUNC_H
#define AUK_FUNC_H

using namespace std;
char int_to_hex_ansii(int num);
int hex_ansii_to_int(char byte);
string urlencode(const char* raw);
string urldecode(const char* raw);
long long get_msk_time();
string get_formatted_time(long long epoch);
string markdown_free(const string src);
string get_cat_name(int cat_id);

#endif //AUK_FUNC_H
