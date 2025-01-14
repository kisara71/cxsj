#include "User.h"
#include "Database.h"
#include <iostream>
bool User::isCreated = false;

User::User()
{
    User::isCreated = true;
    this->data = Database::createDB();
}

bool User::repair_info_existed(int repairID)
{
   return data->repair_info_existed(repairID);
}
bool User::complaint_info_existed(int complaintID)
{
   return data->complaint_info_existed(complaintID);
}
User::~User()
{
}
User* User::create_user()
{
    if (!User::isCreated)
    {
        User* user = new User();
        return user;
    }
    return nullptr;
}

bool User::display_repair_info()
{
    if (this->repairMSG.size()!=0){
        std::cout<<"查询到"<<repairMSG.size()<<"条数据"<<std::endl;
        for(auto msg:repairMSG)
        {
            std::cout<<"----------------------------------------------------------------"<<std::endl<<msg<<std::endl;;
        }
        return true;
    }
    else{
        std::cout<<"未查询到报修信息"<<std::endl;
        return false;
    }
}

bool User::add_repair_info()
{
    std::string newMSG;
    std::cout<<"请输入新建报修信息"<<std::endl;
    std::cin>>newMSG;
    if(Manager::is_exit(newMSG))
    {
        user_main_menu();
        return false;
    }
    char toQuery[2056];
    sprintf(toQuery,"INSERT INTO repairmsg (ID,MSG,deliver_date) VALUES(%d, '%s','%s');", userINFO->ID, newMSG.c_str(),Logs::get_now_time().c_str());
    if (mysql_query(data->dat, toQuery)!=0)
    {
        std::string error = mysql_error(data->dat);
        Database::log_error("INSERT repair msg error" + error);
        return false;
    }
    else{
            std::cout<<"插入完成"<<std::endl;
            this->repairMSG = data->get_user_repairMSG(userINFO->ID);
            system("pause");
            return true;
        }
    return false;
}

bool User::update_repair_info(std::string updatedMSG = "")
{
    if(repairMSG.size() == 0)
    {
        std::cout<<"未查询到报修信息"<<std::endl;
        system("pause");
        user_main_menu();
        return false;
    }
    this->display_repair_info();
    std::cout<<"请输入要修改的报修编号： ";
    std::string inputid;
    std::cin>>inputid;
    if(Manager::is_exit(inputid))
    {
        user_main_menu();
        return false;
    }
    int repairID;
    if(Manager::is_number(inputid))
    {
        repairID = std::stoi(inputid);
    }else{
        std::cout<<"不合法的报修id"<<std::endl;
        system("pause");
        user_main_menu();
        return false;
    }
    if(repair_info_existed(repairID)){
        std::cout<<"请输入修改后的报修信息: ";
        std::cin>>updatedMSG;
        char mysqlUpdate[128];
        sprintf(mysqlUpdate, "UPDATE repairmsg SET MSG = '%s' ,deliver_date = '%s', status = 'not accepted' WHERE repairID = %d;",updatedMSG.c_str(),Logs::get_now_time().c_str(),repairID);
        if (mysql_query(data->dat, mysqlUpdate)!=0)
        {
            std::string error = mysql_error(data->dat);
            Database::log_error("mysql update user repiar error" + error);
            std::cout<<error<<std::endl;
            return false;
        }
        else{
            std::cout<<"修改报修信息成功"<<std::endl;
            this->repairMSG = data->get_user_repairMSG(userINFO->ID);
            system("pause");
            user_main_menu();
            return true;
        }
    }else{
        std::cout<<"不存在该报修信息，请检查后重试"<<std::endl;
        system("pause");
        user_main_menu();
        return false;
    }
    return false;
}

void User::user_main_menu()
{
    system("cls");
    std::cout << "欢迎" << userINFO->name << "使用" << std::endl;
    std::cout <<"others:退出系统\n1.报修信息管理" << std::endl << "2.投诉信息管理\n请输入操作：";
    if (std::cin.peek() == '\n') {
        std::cin.ignore(); 
    }
    std::string input;
    int choice = 0;
    getline(std::cin,input);
    if(Manager::is_number(input))
    {
        choice = std::stoi(input);
    }else{
        std::cout<<"不明确的选项"<<std::endl;
        system("pause");
        user_main_menu();
        return;
    }
    switch(choice)
    {
        case 1:
            system("cls");
            this->display_repair_menu();
            break;
        case 2:
            system("cls");
            this->display_complaint_menu();
            break;
        default:
            exit(0);
            break;
    }
    return;
}
bool User::delete_repair_info()
{
    if(repairMSG.size()==0)
    {
        std::cout<<"未查询到报修信息"<<std::endl;
        system("pause");
        user_main_menu();
        return false;
    }else
    {
        display_repair_info();
        std::cout<<"请输入要删除的报修信息ID: ";
        std::string input;
        std::cin>>input;
        if(Manager::is_exit(input))
        {
            user_main_menu();
            return false;
        }
        int checkID;
        if(Manager::is_number(input))
        {
            checkID = std::stoi(input);
        }else{
            std::cout<<"不合法的报修id"<<std::endl;
            system("pause");
            user_main_menu();
            return false;
        }
        if(repair_info_existed(checkID)){
            std::cout<<"警告: 该操作会删除所有报修信息，请慎用，输入back返回,输入其他任意字符继续操作: "<<std::endl;
            std::string operate;
            std::cin>>operate;
            if (operate=="back\0")
            {
                return false;
                user_main_menu();
                return false;
            }
            else{
                std::string makeSure;
                std::cout<<"确认要删除吗？ y/n：";
                std::cin>>makeSure;
                    if (makeSure == "y\0")
                    {
                        char toQuery[128];
                        sprintf(toQuery, "DELETE FROM repairmsg WHERE repairID = %d;", checkID);
                        if (mysql_query(data->dat,toQuery)!=0)
                        {
                            std::cout<<"因内部原因，删除失败，请联系管理员处理"<<std::endl;
                            system("pause");
                            std::string error = mysql_error(data->dat);
                            Database::log_error("delete user repairmsg error" + error);

                            return false;
                        }
                        else{
                            this->repairMSG = data->get_user_repairMSG(userINFO->ID);
                            std::cout<<"删除成功"<<std::endl;
                            system("pause");
                            return true;
                        }
                }
            }
        }else{
            std::cout<<"未查询到该条报修信息"<<std::endl;
            system("pause");
            user_main_menu();
            return false;
        }
    }
    return false;
}

bool User::add_complaint_info()
{
    std::string newMSG;
    std::cout<<"请输入投诉信息"<<std::endl;
    std::cin>>newMSG;
    if(Manager::is_exit(newMSG))
    {
        user_main_menu();
        return false;
    }
    char toQuery[2056];
    sprintf(toQuery,"INSERT INTO complaintmsg (ID,MSG,deliver_date) VALUES(%d, '%s','%s');", userINFO->ID,newMSG.c_str(),Logs::get_now_time().c_str());
    if (mysql_query(data->dat, toQuery)!=0)
    {
        std::string error = mysql_error(data->dat);
        Database::log_error("INSERT complaint msg error" + error);
        return false;
    }else{
        std::cout<<"插入完成"<<std::endl;
        this->complaintMSG = data->get_user_complaintMSG(userINFO->ID);
        system("pause");
        user_main_menu();
        return true;
    }
    return false;
}

bool User::update_complaint_info(std::string updatedMSG = "")
{
    if(this->complaintMSG.size()==0)
    {
        std::cout<<"未查询到投诉信息"<<std::endl;
        system("pause");
        user_main_menu();
        return false;
    }
    display_complaint_info();
    std::cout<<"请输入要修改的报修编号： ";
    std::string inputid;
    std::cin>>inputid;
    if(Manager::is_exit(inputid))
    {
        user_main_menu();
        return false;
    }
    int complaintID;
    if(Manager::is_number(inputid))
    {
        complaintID = std::stoi(inputid);
    }else{
        std::cout<<"不合法的投诉信息id"<<std::endl;
        system("pause");
        user_main_menu();
        return false;
    }
    if (complaint_info_existed(complaintID))
    { 
        std::cout<<"请输入修改后的投诉信息: ";
        std::cin>>updatedMSG;
        char mysqlUpdate[2048];
        sprintf(mysqlUpdate, "UPDATE complaintmsg SET MSG = '%s' ,deliver_date = '%s', status = 'not accepted' WHERE complaintID = %d;",updatedMSG.c_str(),Logs::get_now_time().c_str(),complaintID);
        if (mysql_query(data->dat, mysqlUpdate)!=0)
        {
            std::string error = mysql_error(data->dat);
            Database::log_error("mysql update user repiar error" + error);
            std::cout<<error<<std::endl;
            return false;
        }
        else{
            std::cout<<"修改投诉信息成功"<<std::endl;
            this->complaintMSG = data->get_user_complaintMSG(userINFO->ID);
            system("pause");
            user_main_menu();
            return true;
        }
    }
    else
    {
        std::cout<<"不存在该投诉信息，请检查后重试"<<std::endl;
        system("pause");
        user_main_menu();
        return false;
    }
return false;
}
bool User::display_complaint_info()
{
   if (this->complaintMSG.size()!=0){
        std::cout<<"查询到"<<complaintMSG.size()<<"条数据"<<std::endl;
        for(auto msg:complaintMSG)
        {
            std::cout<<"----------------------------------------------------------------"<<std::endl<<msg<<std::endl;
        }
        return true;
    }
    else{
        std::cout<<"未查询到投诉信息"<<std::endl;
        return false;
    }
}
bool User::delete_complaint_info()
{
    if (this->complaintMSG.size()==0)
    {
        std::cout<<"未查询到投诉信息"<<std::endl;
        system("pause");
        user_main_menu();
        return false;
    }
    else{
            display_complaint_info();
            std::cout<<"请输入要删除的投诉信息ID: ";
            std::string input;
            std::cin>>input;
            if(Manager::is_exit(input))
            {
                user_main_menu();
                return false;
            }
            int checkID;
            if(Manager::is_number(input))
            {
                checkID = std::stoi(input);
            }else{
                std::cout<<"不合法的投诉信息id"<<std::endl;
                system("pause");
                user_main_menu();
                return false;
            }
            if(complaint_info_existed(checkID)){
                std::cout<<"警告: 该操作会删除所有报修信息，请慎用，输入back返回,输入其他任意字符继续操作: "<<std::endl;
                std::string operate;
                std::cin>>operate;
                if (operate=="back\0")
                {
                    return false;
                }
                else{
                    std::string makeSure;
                    std::cout<<"确认要删除吗？ y/n：";
                    std::cin>>makeSure;
                        if (makeSure == "y\0")
                        {
                            char toQuery[128];
                            sprintf(toQuery, "DELETE FROM complaintmsg WHERE complaintID = %d;", checkID);
                            if (mysql_query(data->dat,toQuery)!=0)
                            {
                                std::cout<<"因内部原因，删除失败，请联系管理员处理"<<std::endl;
                                system("pause");
                                std::string error = mysql_error(data->dat);
                                Database::log_error("delete user repairmsg error" + error);
                                exit(0);
                                return false;
                            }
                            else{
                                std::cout<<"删除成功"<<std::endl;
                                this->complaintMSG = data->get_user_complaintMSG(userINFO->ID);
                                system("pause");
                                user_main_menu();
                                return true;
                            }
                        }
                    }
  
              }else{
            std::cout<<"未查询到该条报修信息"<<std::endl;
            system("pause");
            user_main_menu();
            return false;
        }
}
    return false;
}
void User::display_complaint_menu()
{
    std::cout<<"0.返回上级菜单\n1.添加投诉信息\n2.修改投诉信息\n3.删除投诉信息\n4.显示投诉信息"<<std::endl;
    int choice;
    std::string input;
    if (std::cin.peek() == '\n') {
        std::cin.ignore(); 
    }
    getline(std::cin,input);
    if(Manager::is_exit(input))
    {
        user_main_menu();
        return;
    }
    if(Manager::is_number(input))
    {
        choice = std::stoi(input);
    }else{
        std::cout<<"不明确的选项"<<std::endl;
        system("pause");
        user_main_menu();
        return;
    }
    switch (choice)
    {
        case 1:
            system("cls");
            this->add_complaint_info();
            break;
        case 2:
            system("cls");
            this->update_complaint_info();
            break;
        case 3:
            system("cls");
            this->delete_complaint_info();
            break;
        case 4:
            system("cls");
            this->display_complaint_info();
            system("pause");
            break;
        default:
            system("cls");
            this->user_main_menu();
            break;
    }
    return;
}

void User::display_repair_menu()
{
    system("cls");
    std::cout<<"0.返回上级菜单\n1.添加报修信息\n2.修改报修信息\n3.删除报修信息\n4.显示报修信息"<<std::endl;
    int choice;
    std::string input;
    if (std::cin.peek() == '\n') {
        std::cin.ignore(); 
    }
    getline(std::cin,input);
    if(Manager::is_exit(input))
    {
        user_main_menu();
        return;
    }
  
    if(!Manager::is_number(input))
    {
        std::cout<<"不明确的选项"<<std::endl;
        system("pause");
        display_repair_menu();
        return;
    }  
    choice = std::stoi(input);
    switch (choice)
    {
        exit:
        case 1:
            system("cls");
            this->add_repair_info();
            break;
        case 2:
            system("cls");
            this->update_repair_info();
            break;
        case 3:
            system("cls");
            this->delete_repair_info();
            break;
        case 4:
            system("cls");
            this->display_repair_info();
            system("pause");
            break;
        default:
            this->user_main_menu();
            break;
    }
    return;
}