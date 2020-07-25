#include <iostream>
#include "Item.h"
#include <memory>

using namespace std;

int main() {
    DBMessage db_messages;
    db_messages.add(Message(1, 1, (Message::MESSAGE_TYPE)1));

}
