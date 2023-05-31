#include <iostream>
#include "User.h"
#include "Userpool.h"
#include <vector>
#include <map>
#include "chatpool.h"
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
using namespace std;


int main()
{
    char action('a');
    Userpool Userpool_vect;
    chatpool chatsinmap;

    while (action != 'q')
    {
        cout << "Press 1 to login with your nickname and password " << endl;
        cout << "Press 2 to create new user and send message " << endl;
        cout << "Press 3 to see all users." << endl;
        cout << "Press q to quit" << endl;
        cin >> action;

        if (action == 'q')
            break;
        string nickname;
        string password;
        string name;
        string receiver;
        string message;
        int ind(0);
        int ind_showmap(0);

        switch (action)
        {
        case '1':

            cout << "Enter your nickname" << endl;
            cin >> nickname;
            cout << "Enter your password" << endl;
            cin >> password;

            ind = Userpool_vect.checkUser(nickname, password);
            if (ind == true) {
                cout << "Enter receiver" << endl;
                cin >> receiver;
                if (receiver == nickname){
                    cout << "You cannot enter yourself as a receiver. Please try again." << endl;
                    break;
                }
                ind_showmap = chatsinmap.showmap(receiver, nickname);
                if (ind_showmap == true){
                cin.ignore();
                                      
                // Запускаем функцию обработки сообщений от клиентов и ответа на эти сообщения
                chatsinmap.sendRequest(nickname, receiver);
                }
                else break;
                
                break;
            }
            else break;

        case '2': {

            cout << "Enter your nickname" << endl;
            cin >> nickname;
            if (nickname == "all") {
                cout << "This nickname is not allowed. Please try again." << endl;
                break;
            }
            cout << "Enter your password" << endl;
            cin >> password;

            Userpool_vect.addUser(User(nickname, password));
       
            break; }

        case '3':{

      
            Userpool_vect.showUsers();
            break;

        default: break;
        }
        
        }
    }
}
