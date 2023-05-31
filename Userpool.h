#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "User.h"
using namespace std;

class Userpool {
public:
	void addUser(const User& username);
	vector<User> Userpool_vect;
	bool checkUser(const string& nickname, const string& password);
	void showUsers();

protected:
	
};

