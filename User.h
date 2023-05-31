#pragma once
#include <string>
#include <iostream>
using namespace std;

class User 
{
public : 
  
   // User() = default;
    User(const string& nickname, const string& password) : nickname_(nickname), password_(password) {}
    string getnickname() const;
    string getpassword() const;

protected : 

    string nickname_;
    string password_;  

};
