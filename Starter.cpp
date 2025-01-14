#include "Starter.h"
#include <iostream>
#include <thread>
//#include <mutex>
//#include <Windows.h>
bool Starter::isCreated = false;
//std::mutex Starter::pageMtx;
//MenuTree* Starter::Menu = new MenuTree();
//MenuNode* Starter::root = Starter::Menu->nowMenu;
Logs* Starter::log;
Database* Starter::data;
Manager* Starter::manager;
User* Starter::user;

std::string Starter::loginName;
std::string Starter::loginPwd;
Starter::Starter()
{
    //std::thread changePage(Starter::change_page);
    Starter::isCreated = true;
    log = Logs::create_Logs();
    this->data = Database::createDB();
login:
    switch (this->data->login()) {
    case 0:
        this->manager = Manager::create_manager();
        while (true)
        {
            manager->manager_main_menu();
        }
        break;
    case 1:
        this->user = User::create_user();
        user->userINFO = data->get_user_info(data->loginNameIsExisted(Starter::loginName));
        user->repairMSG = data->get_user_repairMSG(user->userINFO->ID);
        user->complaintMSG = data->get_user_complaintMSG(user->userINFO->ID);
        while (true)
        {
          user->user_main_menu();
        }
        break;
    case 2:
    {
        system("cls");
        std::cout << "注册成功！" << std::endl;
        system("pause");
        goto login;
        break;
    }
    case -1:
        std::cout << "注册失败" << std::endl;
        system("pause");
        goto login;
        break;
    case -2:
        std::cout <<std::endl<< "密码错误，请检查账户名及密码" << std::endl;
        system("pause");
        goto login;
        break;

    }
    //Starter::display_page(Starter::Menu->nowMenu->menuName);

}

Starter* Starter::create_starter()
{
    if (!Starter::isCreated) {
        Starter* starter = new Starter();
        return starter;
    }
    return nullptr;
}


Starter::~Starter()
{
    if (log != nullptr)
        delete log;
    if (user != nullptr)
        delete user;
    if (manager != nullptr)
        delete manager;
}


//void Starter::change_page()
//{
//    while (true)
//    {
//        if (GetAsyncKeyState(VK_CONTROL) & 0x8000 && GetAsyncKeyState('B') & 0x8000)
//        {
//            {
//                std::unique_lock<std::mutex> lock(pageMtx);
//                if (Starter::Menu->nowMenu->father != nullptr)
//                {
//                    Starter::Menu->nowMenu = Starter::Menu->nowMenu->father;
//                }
//                lock.unlock();
//                Starter::display_page(Starter::Menu->nowMenu->menuName);
//            }
//            std::this_thread::sleep_for(std::chrono::milliseconds(500));
//        }
//        std::this_thread::sleep_for(std::chrono::milliseconds(10));
//    }
//}
//void Starter::display_page(std::string menuName)
//{
//    if (menuName == "login")
//    {
//        system("cls");
//        std::cout << "欢迎进入登陆界面" << std::endl;
//        Starter::data->login();
//   }
//    if (menuName == "user_register")
//    {
//        system("cls");
//        std::cout << "欢迎进入注册界面" << std::endl;
//    }
//}