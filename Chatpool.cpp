#include "chatpool.h"
#include <string>
#include <iostream>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <mysql.h>
#define MESSAGE_BUFFER 4096 // Максимальный размер буфера для приема и передачи
#define PORT 7777 // Номер порта, который будем использовать для приема и передачи 
char buffer[MESSAGE_BUFFER];
int socket_descriptor, message_size;
struct sockaddr_in serveraddress;
using namespace std;

string saveResultAsString(MYSQL_RES* result) { 
    string resultString; 
    if (result) { MYSQL_ROW row; unsigned int numFields = mysql_num_fields(result); 
    // Iterate over rows 
        while ((row = mysql_fetch_row(result))) { 
            // Iterate over columns 
            for (unsigned int i = 0; i < numFields; ++i) { 
                 if (row[i]) { 
                    // Append the column value to the result string 
                    resultString += row[i]; 
                     }

                } 
        } 
                mysql_free_result(result); 
    } 
    return resultString; 
} 
                     
  
bool chatpool::showmap(string receiver, string nickname) {
    MYSQL mysql;
	MYSQL_RES* res;
    MYSQL_RES* res1;
    MYSQL_RES* nickname_id;
    MYSQL_RES* receiver_id;
    
	MYSQL_ROW row;
 
	int i = 0;
 
	// Получаем дескриптор соединения
	mysql_init(&mysql);
	if (&mysql == nullptr) {
		// Если дескриптор не получен — выводим сообщение об ошибке
		cout << "Error: can't create MySQL-descriptor" << endl;
	}
 
	// Подключаемся к серверу
	if (!mysql_real_connect(&mysql, "localhost", "root", "root", "testdb", 0, 0, 0)) {
		// Если нет возможности установить соединение с БД выводим сообщение об ошибке
		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
	}

    string check_receiver_id = "SELECT id FROM Users WHERE username LIKE  '" +  receiver +  "'";
	mysql_query(&mysql, check_receiver_id.c_str());
	receiver_id = mysql_store_result(&mysql);
    if (receiver_id != nullptr && mysql_num_rows(receiver_id) > 0){
        cout << "Chat history with " << receiver << ":" << endl;
    }

    else {
        cout << "Receiver does not exist. Chose another receiver." << endl;
        return 0;
    }

    string query_nickname_id = "SELECT id FROM Users WHERE username LIKE  '" +  nickname +  "'";
	mysql_query(&mysql, query_nickname_id.c_str());
	nickname_id = mysql_store_result(&mysql);
    string nickname_id_str;
    nickname_id_str = saveResultAsString(nickname_id);

     string query_receiver_id = "SELECT id FROM Users WHERE username LIKE '" + receiver + "'";
        if (mysql_query(&mysql, query_receiver_id.c_str()) != 0) {
            cout << "Error executing query1: " << mysql_error(&mysql) << endl;
        }

    receiver_id = mysql_store_result(&mysql);
    if (receiver_id == nullptr) {
        cout << "Error retrieving receiver ID: " << mysql_error(&mysql) << endl;
    }

    string receiver_id_str = saveResultAsString(receiver_id);
    string chat_id_opt1 = nickname_id_str + "#" + receiver_id_str;
    string chat_id_opt2 = receiver_id_str + "#" + nickname_id_str;

    string query_chat_id_opt1 = "SELECT sender, message FROM Messages WHERE chat_id LIKE '" + chat_id_opt1 + "' OR chat_id LIKE '" + chat_id_opt2 + "' ORDER BY mes_datetime";
    if (mysql_query(&mysql, query_chat_id_opt1.c_str()) != 0) {
        cout << "Error executing query: " << mysql_error(&mysql) << endl;
    }

    
      if (res1 = mysql_store_result(&mysql)) {
	    	while (row = mysql_fetch_row(res1)) {
		    	for (i = 0; i < mysql_num_fields(res1); i++) {
			    	cout << row[i] << "  ";
			    }
			    cout << endl;
		    }
	    }
	    else
		    cout << "Ошибка MySql номер " << mysql_error(&mysql);
    mysql_close(&mysql);
    return 1;
}



    void chatpool::sendmessage(string receiver, string nickname, string message) {
    MYSQL mysql;
	MYSQL_RES* res;
    MYSQL_RES* nickname_id;
    MYSQL_RES* receiver_id;
	MYSQL_ROW row;
 
	int i = 0;
 
	// Получаем дескриптор соединения
	mysql_init(&mysql);
	if (&mysql == nullptr) {
		// Если дескриптор не получен — выводим сообщение об ошибке
		cout << "Error: can't create MySQL-descriptor" << endl;
	}
 
	// Подключаемся к серверу
	if (!mysql_real_connect(&mysql, "localhost", "root", "root", "testdb", 0, 0, 0)) {
		// Если нет возможности установить соединение с БД выводим сообщение об ошибке
		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
	}

    string query_nickname_id = "SELECT id FROM Users WHERE username LIKE  '" +  nickname +  "'";
  	mysql_query(&mysql, query_nickname_id.c_str());
	nickname_id = mysql_store_result(&mysql);
	string nickname_id_str = saveResultAsString(nickname_id);
    string query_receiver_id = "SELECT id FROM Users WHERE username LIKE  '" +  receiver +  "'";
	mysql_query(&mysql, query_receiver_id.c_str());
	receiver_id = mysql_store_result(&mysql);
    string receiver_id_str = saveResultAsString(receiver_id);
    string chat_id = nickname_id_str + "#" + receiver_id_str;

  
    mysql_query(&mysql, "CREATE TABLE Messages(mes_id INT AUTO_INCREMENT PRIMARY KEY, sender VARCHAR(255), chat_id VARCHAR(10), message VARCHAR(255), mes_datetime datetime)");
    string query = "INSERT INTO Messages(mes_id, sender, chat_id, message, mes_datetime) values(default, '" + nickname + "', '"  + chat_id + "', '" +  message + "', now())";
   
    if (mysql_query(&mysql, query.c_str()) != 0) {
    // If an error occurs during the query execution
    cout << "Error executing query: " << mysql_error(&mysql) << endl;
}
      // Закрываем соединение с базой данных
	mysql_close(&mysql);
  
}


void chatpool::sendRequest(string nickname, string receiver) {
    
    cout << "SERVER IS LISTENING THROUGH THE PORT: " << PORT << " WITHIN A LOCAL SYSTEM" << endl;
    // Укажем адрес сервера
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    // Зададим номер порта для соединения с сервером
    serveraddress.sin_port = htons(PORT);
    // Используем IPv4
    serveraddress.sin_family = AF_INET;
    // Создадим сокет 
    socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    // Установим соединение с сервером
    if (connect(socket_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress)) < 0) {
        cout << endl << " Something went wrong Connection Failed" << endl;
        exit(1);
    }
// Запускаем функцию обработки сообщений от клиентов и ответа на эти сообщения
    cout << "CLIENT IS ESTABLISHING A CONNECTION WITH SERVER THROUGH PORT: " << PORT << endl;
//Converting nickname from string to char   
    int k;
    for (k = 0; k < sizeof(nickname); k++) {
        buffer[k] = nickname[k];
    }

    
    sendto(socket_descriptor, buffer, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));

    char answer[4096];

    recvfrom(socket_descriptor, answer, sizeof(buffer), 0, nullptr, nullptr);
 
    //Converting receiver from string to char for comparison in strcmp  
    int l;
    char receiver_char [4096];
    for (l = 0; l < sizeof(receiver); l++) {
        receiver_char[l] = receiver[l];
    }

      
    if (strcmp(answer, receiver_char) == 0)
    {    
       
    while (1) {
        cout << "Enter a message you want to send to the server: " << endl;
        cin >> buffer;

        if (strcmp(buffer, "end") == 0) {
            sendto(socket_descriptor, buffer, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
            cout << "Client work is done.!" << endl;
            close(socket_descriptor);
 
           return;
        }
        else {
            sendto(socket_descriptor, buffer, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
            cout << "Message sent successfully to the server: " << buffer << endl;
            cout << "Waiting for the Response from Server..." << endl;
        }
        
        this->sendmessage(receiver, nickname, buffer);
        cout << "Message Received From Server: " << endl;
        recvfrom(socket_descriptor, buffer, sizeof(buffer), 0, nullptr, nullptr);
        cout << buffer << endl;
        this->sendmessage(receiver, nickname, buffer);
    }
    // закрываем сокет, завершаем соединение
    close(socket_descriptor);
    return;
    
    }
    
    if (strcmp(answer, receiver_char) != 0) {
        cout << "Sorry, the user you want to chat it with is not online. Please try later." << endl;
        sendto(socket_descriptor, "end", MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
        close(socket_descriptor);
        return;
    }

}
    

