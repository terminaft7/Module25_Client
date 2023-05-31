#include "User.h"
#include <string>
#include <iostream>
using namespace std;

string User::getnickname() const {
	return nickname_;
}

string User::getpassword() const {
	return password_;
}
