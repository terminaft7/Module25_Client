#include "User.h"
#include <string>
#include <iostream>
#include <vector>
#include "Userpool.h"
#include <mysql.h>
#include <string>
using namespace std;

void Userpool::addUser(const User& username) {

 	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;
	unsigned int num_fields;
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
    mysql_query(&mysql, "CREATE TABLE Users(id INT AUTO_INCREMENT PRIMARY KEY, username VARCHAR(255), password VARCHAR(255))");

	string query1 = "SELECT id FROM Users WHERE username LIKE  '" +  username.getnickname() +  "'";
	mysql_query(&mysql, query1.c_str());
	res = mysql_store_result(&mysql);
	//row = mysql_fetch_row(res);
	if (res != nullptr && mysql_num_rows(res) > 0)  {
		cout << "User with name " << username.getnickname() << " already exists. Try another name!" << endl;
	}
	else {
		string query = "INSERT INTO Users(id, username, password) values(default,  '" +  username.getnickname() +  "', SHA1('" + username.getpassword() + "')) ";
		cout << "User " << username.getnickname()  << " was created." << endl;
		mysql_query(&mysql, query.c_str());
	}
   	
	// Закрываем соединение с базой данных
	mysql_free_result(res);
	mysql_close(&mysql);
}

bool Userpool::checkUser(const string& nickname, const string& password) {
	
	// Получаем дескриптор соединения
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;
	mysql_init(&mysql);
	
	if (&mysql == nullptr) {
		// Если дескриптор не получен — выводим сообщение об ошибке
		cout << "Error: can't create MySQL-descriptor" << endl;
	}
 
	// Подключаемся к серверу
	if (!mysql_real_connect(&mysql, "localhost", "root", "root", "testdb", 0, NULL, 0)) {
		// Если нет возможности установить соединение с БД выводим сообщение об ошибке
		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
	}

	
    string query = "SELECT id FROM Users WHERE username LIKE  '" +  nickname +  "' AND password LIKE SHA1('" + password + "')";
	mysql_query(&mysql, query.c_str());
	res = mysql_store_result(&mysql);
	if (res != nullptr && mysql_num_rows(res) > 0) {
	cout << "Login successful. " << endl;
	mysql_close(&mysql);
	return true;}
	else {
		cout << "Login failed. Try again." << endl;
	return false;
	}

return true;
}

void Userpool::showUsers() {
	
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;
 	int i = 0;
 
	// Получаем дескриптор соединения
	mysql_init(&mysql);
	if (&mysql == nullptr) {
		// Если дескриптор не получен — выводим сообщение об ошибке
		cout << "Error: can't create MySQL-descriptor" << endl;
	}
 
	// Подключаемся к серверу
	if (!mysql_real_connect(&mysql, "localhost", "root", "root", "testdb", 0, NULL, 0)) {
		// Если нет возможности установить соединение с БД выводим сообщение об ошибке
		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
	}
 	mysql_query(&mysql, "SELECT id, username FROM Users"); //Делаем запрос к таблице
 
	//Выводим все что есть в таблице через цикл
	if (res = mysql_store_result(&mysql)) {
		while (row = mysql_fetch_row(res)) {
			for (i = 0; i < mysql_num_fields(res); i++) {
				cout << row[i] << "  ";
			}
			cout << endl;
		}
	}
	else
		cout << "Ошибка MySql номер " << mysql_error(&mysql);
 
	// Закрываем соединение с базой данных
	mysql_close(&mysql);
 
}
