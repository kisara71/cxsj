#pragma once
class Manager;
class User;
class Logs;
// class MenuTree;
// class MenuNode;
#include "User.h"
#include "Manager.h"
#include "Database.h"
#include "Logs.h"
// #include "menu.h"
#include <mutex>
class Starter{
public:
    friend class User;
    friend class Manager;
    ~Starter();
    static Logs* log;
    // static MenuTree* Menu;
    // static MenuNode* root;

    static std::string loginName;
    static std::string loginPwd;
    
    //static std::mutex pageMtx;


    //static void change_page();
    //static void display_page(std::string menuName);
    static Starter* create_starter();
private:
    Starter();

    static bool isCreated;

    static Manager* manager;
    static User* user;
    static Database* data;
};
