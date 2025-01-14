#include "Database.h"
#include <iostream>
#include <string>
#include <conio.h>
#include <optional>
#include <Windows.h>
#include "Starter.h"


void Database::log_error(const std::string& message) {
    std::unique_lock<std::mutex> lock(Starter::log->mtx);
    Starter::log->logMSG.push_back(message);
    Starter::log->getMSG.notify_one();
}
Database::Database(const std::string& baseName)
{

    bool isOK = true;
    char toQueryData[128];
    sprintf(toQueryData, "SHOW DATABASES LIKE '%s';", baseName.c_str());
    this->dat = mysql_init(NULL);
    if (dat == NULL)
    {
        std::string error = mysql_error(dat);
        log_error("mysql init error :" + error);
        return;
    }

    if (mysql_real_connect(this->dat, "localhost", "root", "123456", baseName.c_str(), 3306, nullptr, 0) == NULL)
    {

        std::string error = mysql_error(dat);
        log_error("mysql init error :" + error);
        mysql_close(dat);
        dat = mysql_init(NULL);
        if (mysql_query(dat, toQueryData) != 0)
        {
            std::cout << "未检测到数据库,自动创建" << std::endl;
            log_error("no database found,create database");
            if (mysql_real_connect(this->dat, "localhost", "root", "123456", NULL, 3306, nullptr, 0) == NULL)
            {
                std::string error = mysql_error(dat);
                log_error("mysql connect error :" + error);
            }
            if (mysql_query(dat, "CREATE DATABASE manager;") != 0)
            {
                std::string error = mysql_error(dat);
                log_error("create database manager error:" + error);
                isOK = false;
                return;
            }
            else
            {
                mysql_close(dat);
                dat = mysql_init(NULL);

                if (mysql_real_connect(dat, "localhost", "root", "123456", "manager", 3306, nullptr, 0) == NULL)
                {
                    std::string error = mysql_error(dat);
                    log_error("connect new database error :" + error);
                    isOK = false;
                    return;
                }

                if (mysql_query(dat, "USE manager") != 0)
                {
                    std::string error = mysql_error(dat);
                    log_error("use new database :" + error);
                    isOK = false;
                }
                if (mysql_query(dat, "CREATE TABLE communityinfo(\
                                              communityID   INT PRIMARY KEY AUTO_INCREMENT,\
                                              communityName     varchar(10) DEFAULT 'unknown',\
                                              buildingAmounts          varchar(10) DEFAULT '0');") != 0) {
                    std::string error = mysql_error(dat);
                    log_error("table communityinfo created error :" + error);
                    isOK = false;
                }
                else {
                    log_error("table houseINFO created");
                }
                if (mysql_query(dat, "CREATE TABLE houseinfo(\
                                              houseTypeID   INT PRIMARY KEY AUTO_INCREMENT,\
                                              houseType     varchar(10) DEFAULT 'unknown',\
                                              area          varchar(10) DEFAULT 'unknown');") != 0) {
                    std::string error = mysql_error(dat);
                    log_error("table houseINFO created error :" + error);
                    isOK = false;
                }
                else {
                    log_error("table houseINFO created");
                }
                if (mysql_query(dat, "CREATE TABLE residents(\
                                            ID              INT PRIMARY KEY AUTO_INCREMENT,\
                                            name            varchar(10) DEFAULT 'unknown',\
                                            address         varchar(20) DEFAULT 'unknown',\
                                            parkingID       varchar(10) DEFAULT 'none',\
                                            prePay          varchar(10) DEFAULT '0',\
                                            debtAmount      varchar(20) DEFAULT '0',\
                                            phoneNumber     varchar(15) DEFAULT 'unknown',\
                                            houseTypeID     INT         DEFAULT 1 ,\
                                            FOREIGN KEY (houseTypeID) REFERENCES houseinfo(houseTypeID)\
                                                ON DELETE CASCADE\
                                                ON UPDATE CASCADE\
                                                                                       );") != 0) {
                    std::string error = mysql_error(dat);
                    log_error("create table residents error :" + error);
                    isOK = false;
                }
                else
                {
                    log_error("table residents created :");
                }
                if (mysql_query(dat, "CREATE TABLE login(\
                                             ID             INT ,\
                                             loginName      varchar(10) PRIMARY KEY,\
                                             pwd            varchar(20),  \
                                             FOREIGN KEY (ID) REFERENCES residents(ID)\
                                                ON DELETE CASCADE\
                                                ON UPDATE CASCADE);") != 0)
                {
                    std::string error = mysql_error(dat);
                    log_error("create table login error :" + error);
                    isOK = false;
                }
                else {
                    log_error("table login created");
                    if (mysql_query(dat, "INSERT INTO login(loginname, pwd) VALUES('root', '8888');") != 0)
                    {
                        std::string error = mysql_error(dat);
                        log_error("manager init error :" + error);
                        isOK = false;
                    }
                    else
                    {
                        log_error("mysql init ");
                    }
                }
                if (mysql_query(dat, "CREATE TABLE chargeINFO(\
                                             chargeID                   INT             AUTO_INCREMENT PRIMARY  KEY,\
                                             chargeName                 varchar(20)     NOT NULL             , \
                                             toPay                      varchar(10)     DEFAULT 'unknown',\
                                             cycle                      varchar(10)     DEFAULT '每1个月',\
                                             status                     varchar(50)     DEFAULT '长期收费',\
                                             now_charge_times           INT             DEFAULT 0,\
                                             last_charge_date           DATETIME        DEFAULT CURRENT_TIMESTAMP,\
                                             houseTypeID                INT DEFAULT 0,\
                                             FOREIGN KEY (houseTypeID) REFERENCES houseinfo(houseTypeID)\
                                                ON DELETE CASCADE\
                                                ON UPDATE CASCADE\
                                                            );") != 0) {
                    std::string error = mysql_error(dat);
                    log_error("create chargeINFO error :" + error);
                    isOK = false;
                }
                else {
                    log_error("chargeINFO created ");
                }
                if (mysql_query(dat, "CREATE TABLE repairMSG(\
                                             repairID       INT AUTO_INCREMENT PRIMARY KEY,\
                                             ID             INT ,\
                                             MSG            varchar(300) DEFAULT '',\
                                             deliver_date   DATETIME,\
                                             status         varchar(50) DEFAULT 'not accepted',\
                                             FOREIGN KEY (ID) REFERENCES residents(ID)\
                                                ON DELETE CASCADE\
                                                ON UPDATE CASCADE);") != 0)
                {
                    std::string error = mysql_error(dat);
                    log_error("create table repairMSG error :" + error);
                    isOK = false;
                }
                if (mysql_query(dat, "CREATE TABLE complaintMSG(\
                                             complaintID    INT AUTO_INCREMENT PRIMARY KEY,\
                                             ID             INT ,\
                                             MSG            varchar(300) DEFAULT '',\
                                             deliver_date   DATETIME,\
                                             status         varchar(50) DEFAULT 'not accepted',\
                                             FOREIGN KEY (ID) REFERENCES residents(ID)\
                                                ON DELETE CASCADE\
                                                ON UPDATE CASCADE);") != 0)
                {
                    std::string error = mysql_error(dat);
                    log_error("create table compalintMSG error :" + error);
                    isOK = false;
                }
                if (isOK) {
                    std::cout << "已创建数据库" << std::endl;
                    log_error("database created");
                }
                else
                {
                    std::cout << "创建数据库出现错误，请查看日志联系管理员" << std::endl;
                    log_error("database creating failed");
                }
            }
        }
    }
    else {
        log_error("connect to database");
    }


}

std::string Database::inputPWD()
{
    char ch;
    std::string pwd;
    while ((ch = _getch()) != '\r')
    {
        if (ch == '\b')
        {
            if (!pwd.empty())
            {
                std::cout << "\b \b";
                pwd.pop_back();
            }
        }
        else if (pwd.size() < 20) {
            std::cout << "*";
            pwd.push_back(ch);
        }
    }
    return pwd;
}


Database* Database::createDB(const std::string& baseName)
{
  
    Database* dataBase = new Database(baseName);
    return dataBase;
}

Database::~Database()
{
    mysql_close(this->dat);
}

std::string Database::get_pwd(std::string loginName)
{
    std::string toQuery = "SELECT * FROM login WHERE loginName = '" + loginName + "';";
    log_error("get login pwd");
    if (mysql_query(dat, toQuery.c_str()) != 0)
    {
        std::string error = mysql_error(dat);
        log_error("check pwd error: " + error);
        return "";
    }
    MYSQL_RES* res = mysql_store_result(dat);
    if (res != nullptr)
    {
        MYSQL_ROW row;
        row = mysql_fetch_row(res);
        std::string pwd = row[2];
        mysql_free_result(res);
        return pwd;
    }
    else
    {
        std::string error = mysql_error(dat);
        log_error("check pwd get null res error: " + error);
        return "";
    };
}

int Database::loginNameIsExisted(std::string loginName)
{
    log_error("query loginName");
    std::string toQuery = "SELECT * FROM login WHERE loginName = '" + loginName + "';";
    if (mysql_query(dat, toQuery.c_str()) != 0)
    {
        std::string error = mysql_error(dat);
        log_error("loginName querying error :" + error);
        return -1;
    }
    MYSQL_RES* res;
    res = mysql_store_result(dat);
    if (mysql_num_rows(res) > 0)
    {
        MYSQL_ROW row = mysql_fetch_row(res);
        if (row != nullptr)
        {
            int id = std::stoi(row[0]);
            mysql_free_result(res);
            log_error("user existed");
            return id;
        }
        else {
            log_error("judge user exist error when get id");
            return -1;
        }
    }
    log_error(" user not found");
    return -2;
}

std::optional<residentINFO> Database::get_user_info(int ID)
{
    log_error("get user info");
    char toQuery[128];
    sprintf(toQuery, "SELECT * FROM residents WHERE ID = %d;", ID);
    if (mysql_query(dat, toQuery) == 0)
    {
        MYSQL_RES* res;
        res = mysql_store_result(dat);
        MYSQL_ROW row = mysql_fetch_row(res);
        if (res != NULL && row != nullptr) {
            residentINFO ri;
            ri.ID = std::stoi(row[0]);
            ri.name = row[1];
            ri.address = row[2];
            ri.houseTypeID = std::stoi(row[7]);
            ri.parkingID = row[3];
            ri.prePay = row[4];
            ri.debtAmount = row[5];
            ri.phoneNumber = row[6];
            mysql_free_result(res);
            return ri;
        }
    }
    log_error("get empty user info");
    return std::nullopt;
}
bool Database::user_regist()
{
    log_error("user registing");
    int id;
    std::string phoneNumber;
    std::string name;
    std::cout << "请输入你的住户ID： " << std::endl;
    std::string inputID;
    std::cin >> inputID;
    if(Manager::is_exit(inputID))
    {
        return false;
    }
    if(!Manager::is_number(inputID))
    {
        std::cout<<"不合法的ID "<<std::endl;
        return false;
    }
    id = std::stoi(inputID);
    // if (std::cin.fail())
    // {
    //     std::cout << "不明id" << std::endl;
    //     std::cin.clear();
    //     std::cin.ignore(10000, '\n');
    //     return false;
    // }
    std::cout << "请输入你的姓名： " << std::endl;
    std::cin >> name;
    std::cout << "请输入你的电话号码： " << std::endl;
    std::cin >> phoneNumber;
    std::optional<residentINFO> ri = get_user_info(id);
    if (ri)
    {
        if (name == ri->name && phoneNumber == ri->phoneNumber)
        {
        rgs:
            std::cout << "请输入你的用户名（上限为10）： ";
            std::string loginName;
            std::cin >> loginName;
            if (loginNameIsExisted(loginName) < 0)
            {
                std::cout << "请输入你的密码（上限为20）： ";
                std::string userPWD1 = Database::inputPWD();
                std::cout << std::endl << "请确认你的密码： ";
                std::string userPWD2 = Database::inputPWD();
                if (userPWD1 == userPWD2)
                {
                    log_error("user regist successfully");
                    std::string add = "INSERT INTO login(ID, loginName, pwd) VALUES(" + std::to_string(id) + ", '" + loginName + "', '" + userPWD1 + "');";
                    if (mysql_query(dat, add.c_str()) == 0) {
                        return true;
                    }
                    else {
                        std::string error = mysql_error(dat);
                        log_error("user regist error :" + error);
                        return false;
                    }

                }
                else {
                    std::cout << "两次密码输入不一致，请重试。" << std::endl;
                    goto rgs;
                }
            }
        }
    }
    else
    {
        std::cout << "未识别的身份" << std::endl;
        log_error("undefined register");
        return false;
    }
    return false;

}

int Database::login()
{
    system("cls");
    /* {
         std::unique_lock<std::mutex> lock(Starter::pageMtx);
         Starter::Menu->nowMenu = Starter::root;
         lock.unlock();
     }*/
    std::cout<<"****欢迎使用物业管理系统****"<<std::endl;
    std::cout << "输入用户名(没有账号直接输入regist):  ";
    std::cin >> Starter::loginName;
    if(Starter::loginName=="regist\0")
    {
        if(user_regist())
        {
            return 2;
        }else{
            return -1;
        }
    }
    std::cout << "请输入密码： ";
    Starter::loginPwd = Database::inputPWD();
    if (strcmp("root\0", Starter::loginName.c_str()) == 0)
    {
        std::string realPWD = get_pwd(Starter::loginName);
        if (Starter::loginPwd == realPWD) {
            return 0;
        }
        else {
            return -2;
        }
    }
    else {
        if (loginNameIsExisted(Starter::loginName) < 0)
        {
            std::cout << std::endl << "未查询到该用户，是否要注册用户？（仅小区居民可注册）,输入y/n： ";
            log_error("no loginName found");
            std::string result;
            std::cin >> result;
            if (strcmp(result.c_str(), "y\0") == 0)
            {
                /* {
                     std::unique_lock<std::mutex> lock(Starter::pageMtx);
                     Starter::Menu->nowMenu = Starter::root->child[2];
                     Starter::display_page(Starter::Menu->nowMenu->menuName);
                 }*/
                if (user_regist()) {
                    return 2;
                }
                else {
                    return -1;
                }
            }
            else {
                return -1;
            }
        }
        else {
            std::string realPWD = get_pwd(Starter::loginName);
            if (strcmp(Starter::loginPwd.c_str(), realPWD.c_str()) == 0) {
                return 1;
            }
            else {
                std::cout << "密码错误，请检查密码" << std::endl;
                return -2;
            }
        }
    }
    return -3;
}

std::vector<std::string> Database::get_user_repairMSG(int id)
{
   char toQuery[128];
   std::vector<std::string> repairmsgs;
   sprintf(toQuery,"SELECT * FROM repairmsg WHERE ID = %d;",id);
   if (mysql_query(dat,toQuery)!=0)
   {
        std::string error = mysql_error(dat);
        log_error("get user repairMSG error" + error);
        return repairmsgs;
   }
   else{
        MYSQL_RES* res = mysql_store_result(dat);
        if(res!=nullptr)
        {
            MYSQL_ROW row;
            while ((row=mysql_fetch_row(res))!=NULL){
                std::string repairmsg;
                repairmsg = "RepairID: ";
                repairmsg += row[0];
                repairmsg += " |  [";
                repairmsg += row[3];
                repairmsg += "] (";
                repairmsg += row[4];
                repairmsg += "): ";
                repairmsg += row[2];
                repairmsgs.push_back(repairmsg);
            }
        }
        mysql_free_result(res);
        return repairmsgs;
    }
     return repairmsgs;
}
std::vector<std::string> Database::get_user_complaintMSG(int id)
{
   char toQuery[128];
   std::vector<std::string> complaintmsgs;
   sprintf(toQuery,"SELECT * FROM complaintmsg WHERE ID = %d;",id);
   if (mysql_query(dat,toQuery)!=0)
   {
        std::string error = mysql_error(dat);
        log_error("get user repairMSG error" + error);
        return complaintmsgs;
   }
   else{
        MYSQL_RES* res = mysql_store_result(dat);
        if(res!=nullptr)
        {
           MYSQL_ROW row;
           while((row=mysql_fetch_row(res))!=NULL)
           {
            std::string complaintmsg;
                complaintmsg = "ComplaintID: ";
                complaintmsg += row[0];
                complaintmsg += " |  [";
                complaintmsg += row[3];
                complaintmsg += "] (";
                complaintmsg += row[4];
                complaintmsg += "): ";
                complaintmsg += row[2];
                complaintmsgs.push_back(complaintmsg);
           }
        }
        mysql_free_result(res);
        return complaintmsgs;
    }
     return complaintmsgs;
}

std::vector<residentINFO> Database::get_all_user_by_name(std::string inputName)
{
    std::vector<residentINFO> result;
    char toQuery[128];
    sprintf(toQuery,"SELECT * FROM residents WHERE name = '%s';",inputName.c_str());
    if (mysql_query(dat,toQuery)!=0)
    {
        std::cout<<"获取用户"<<inputName<<"失败"<<std::endl;
        std::string error = mysql_error(dat);
        log_error("get userinfo by name failed: "+error);
        return result;
    }
    MYSQL_RES* res = mysql_store_result(dat);
    if(res!=nullptr)
    {
        MYSQL_ROW row;
        while((row=mysql_fetch_row(res))!=NULL)
        {
            residentINFO ri;
            ri.ID = std::stoi(row[0]);
            ri.name = row[1];
            ri.address = row[2];
            ri.houseTypeID = std::stoi(row[7]);
            ri.parkingID = row[3];
            ri.prePay = row[4];
            ri.debtAmount = row[5];
            ri.phoneNumber = row[6];
            result.push_back(ri);
        }
        mysql_free_result(res);
        return result;
    }
    return result;
}

std::vector<residentINFO> Database::get_all_user()
{
  std::string toQuery = "SELECT * FROM residents";
  std::vector<residentINFO> result;
  if(mysql_query(dat,toQuery.c_str())!=0)
  {
    std::string error = mysql_error(dat);
    log_error("get all user info error: "+error);
    return result;
  }
  MYSQL_RES* res = mysql_store_result(dat);
  MYSQL_ROW row;
  while((row=mysql_fetch_row(res))!=NULL)
  {
    residentINFO ri;
    ri.ID = std::stoi(row[0]);
    ri.name = row[1];
    ri.address = row[2];
    ri.houseTypeID = std::stoi(row[7]);
    ri.parkingID = row[3];
    ri.prePay = row[4];
    ri.debtAmount = row[5];
    ri.phoneNumber = row[6];
    result.push_back(ri);
  }
  mysql_free_result(res);
  return result;
}

std::vector<std::string> Database::get_all_unaccepted_repairMSG()
{
    std::string toQuery = "SELECT * FROM repairmsg WHERE status = 'not accepted';";
    std::vector<std::string> result;
    if(mysql_query(dat,toQuery.c_str())!=0)
    {
        std::string error = mysql_error(dat);
        std::cout<<"获取未受理报修信息失败: "+error;
        log_error("get unaccepted repairmsg error :"+error);
        return result;
    }
    MYSQL_RES* res = mysql_store_result(dat);
    if(res!=nullptr)
    {
        MYSQL_ROW row;
        while((row=mysql_fetch_row(res))!=NULL)
        {
            std::string repairmsg;
            repairmsg = "RepairID: ";
            repairmsg += row[0];
            repairmsg += " | UserID: ";
            repairmsg += row[1];
            repairmsg += " |  [";
            repairmsg += row[3];
            repairmsg += "] (";
            repairmsg += row[4];
            repairmsg += "): ";
            repairmsg += row[2];
            result.push_back(repairmsg);
        }
        mysql_free_result(res);
        return result;
    }
    else
    {
        std::string error = mysql_error(dat);
        std::cout<<"获取未受理报修信息失败: "+error;
        log_error("get unaccepted repairmsg error :"+error);
        return result;
    }
}
std::vector<std::string> Database::get_all_solved_repairMSG()
{
    std::string toQuery = "SELECT * FROM repairmsg WHERE status = 'solved';";
    std::vector<std::string> result;
    if(mysql_query(dat,toQuery.c_str())!=0)
    {
        std::string error = mysql_error(dat);
        std::cout<<"获取已受理报修信息失败: "+error;
        log_error("get solved msg error :"+error);
        return result;
    }
    MYSQL_RES* res = mysql_store_result(dat);
    if(res!=nullptr)
    {
        MYSQL_ROW row;
        while((row=mysql_fetch_row(res))!=NULL)
        {
                std::string repairmsg;
                repairmsg = "RepairID: ";
                repairmsg += row[0];
                repairmsg += " | UserID: ";
                repairmsg += row[1];
                repairmsg += " |  [";
                repairmsg += row[3];
                repairmsg += "] (";
                repairmsg += row[4];
                repairmsg += "): ";
                repairmsg += row[2];
                result.push_back(repairmsg);
        }
        mysql_free_result(res);
        return result;
    }else
    {
        std::string error = mysql_error(dat);
        std::cout<<"获取已完成报修信息失败: "+error;
        log_error("get solved msg error :"+error);
        return result;
    }
}

std::vector<std::string> Database::get_all_solved_complaintMSG()
{
    std::string toQuery = "SELECT * FROM complaintmsg WHERE status = 'solved';";
    std::vector<std::string> result;
    if(mysql_query(dat,toQuery.c_str())!=0)
    {
        std::string error = mysql_error(dat);
        std::cout<<"获取已受理投诉信息失败: "+error;
        log_error("get solved complaintmsg error :"+error);
        return result;
    }
    MYSQL_RES* res = mysql_store_result(dat);
    if(res!=nullptr)
    {
        MYSQL_ROW row;
        while((row=mysql_fetch_row(res))!=NULL)
        {
                std::string complaintmsg;
               complaintmsg = "ComplaintID: ";
               complaintmsg += row[0];
               complaintmsg += " | UserID: ";
               complaintmsg += row[0];
               complaintmsg += " |  [";
               complaintmsg += row[3];
               complaintmsg += "] (";
               complaintmsg += row[4];
               complaintmsg += "): ";
               complaintmsg += row[2];
                result.push_back(complaintmsg);
        }
        mysql_free_result(res);
        return result;
    }else
    {
        std::string error = mysql_error(dat);
        std::cout<<"获取已完成投诉信息失败: "+error;
        log_error("get solved complaintmsg error :"+error);
        return result;
    }
}
bool Database::repair_info_existed(int repairID)
{
    char toQuery[128];
    sprintf(toQuery,"SELECT * FROM repairmsg WHERE repairID = %d;",repairID);
    if (mysql_query(dat,toQuery)!=0)
    {
        std::string error = mysql_error(dat);
        Database::log_error("check repair exist error" + error);
        std::cout<<error<<std::endl;
        system("pause");
        exit(-1);
        return false;
    }
    MYSQL_RES* res = mysql_store_result(dat);
    if(res!=nullptr)
    {
        MYSQL_ROW row = mysql_fetch_row(res);
        mysql_free_result(res);
        return row!=NULL;
    }
    return false;
}

bool Database::complaint_info_existed(int complaintID)
{
    char toQuery[128];
    sprintf(toQuery,"SELECT * FROM complaintmsg WHERE complaintID = %d;",complaintID);
    if (mysql_query(dat,toQuery)!=0)
    {
        std::string error = mysql_error(dat);
        Database::log_error("check complaint exist error" + error);
        std::cout<<error<<std::endl;
        system("pause");
        exit(-1);
        return false;
    }
    MYSQL_RES* res = mysql_store_result(dat);
    if(res!=nullptr)
    {
        MYSQL_ROW row = mysql_fetch_row(res);
        mysql_free_result(res);
        return row!=NULL;
    }
    return false;
}

std::vector<std::string> Database::get_all_unaccepted_complaintMSG()
{
    std::string toQuery = "SELECT * FROM complaintmsg WHERE status = 'not accepted';";
    std::vector<std::string> result;
    if(mysql_query(dat,toQuery.c_str())!=0)
    {
        std::string error = mysql_error(dat);
        std::cout<<"获取未受理投诉信息失败: "+error;
        log_error("get unaccepted complaintmsg error :"+error);
        return result;
    }
    MYSQL_RES* res = mysql_store_result(dat);
    if(res!=nullptr)
    {
        MYSQL_ROW row;
        while((row=mysql_fetch_row(res))!=NULL)
        {
           std::string complaintmsg;
           complaintmsg = "ComplaintID: ";
           complaintmsg += row[0];
           complaintmsg += " | UserID: ";
           complaintmsg += row[1];
           complaintmsg += " |  [";
           complaintmsg += row[3];
           complaintmsg += "] (";
           complaintmsg += row[4];
           complaintmsg += "): ";
           complaintmsg += row[2];
            result.push_back(complaintmsg);
        }
        mysql_free_result(res);
        return result;
    }
    else
    {
        std::string error = mysql_error(dat);
        std::cout<<"获取未受理投诉信息失败: "+error;
        log_error("get unaccepted complaintmsg error :"+error);
        return result;
    }
}

std::vector<houseINFO> Database::get_all_house_info()
{
    std::string toQuery = "SELECT * FROM houseinfo";
    std::vector<houseINFO> result;
    if(mysql_query(dat,toQuery.c_str())!=0)
    {
        std::string error = mysql_error(dat);
        std::cout<<"获取房型信息时出错: "+error<<std::endl;
        log_error("get all house info error: "+error);
        return result;
    }
    MYSQL_RES* res = mysql_store_result(dat);
    if(res!=nullptr)
    {
        MYSQL_ROW row;
        while((row=mysql_fetch_row(res))!=NULL)
        {
            houseINFO hs;
            hs.area = row[2];
            hs.type = row[1];
            hs.houseTypeID = std::stoi(row[0]);
            result.push_back(hs);
        }
        mysql_free_result(res);
        return result;
    }else{
        log_error("get nullptr of house info ");
        return result;
    }
    return result;
}
std::vector<communityINFO> Database::get_all_community_info()
{
    std::string toQuery = "SELECT * FROM communityinfo";
    std::vector<communityINFO> result;
    if(mysql_query(dat,toQuery.c_str())!=0)
    {
        std::string error = mysql_error(dat);
        std::cout<<"获取小区信息时出错: "+error<<std::endl;
        log_error("get all community info error: "+error);
        return result;
    }
    MYSQL_RES* res = mysql_store_result(dat);
    if(res!=nullptr)
    {
        MYSQL_ROW row;
        while((row=mysql_fetch_row(res))!=NULL)
        {
            communityINFO cm;
            cm.communityID = std::stoi(row[0]);
            cm.communityName = row[1];
            cm.buildingAmounts = row[2];
            result.push_back(cm);
        }
        mysql_free_result(res);
        return result;
    }else{
        log_error("get nullptr of community info ");
        return result;
    }
    return result;
}

std::vector<chargeINFO> Database::get_all_charge_info()
{
    std::vector<chargeINFO> result;
    std::string toQuery = "SELECT * FROM chargeinfo;";
    if(mysql_query(dat,toQuery.c_str())!=0)
    {
        std::string error = mysql_error(dat);
        std::cout<<"获取收费信息时出错: "+error<<std::endl;
        log_error("get all charge info error: "+error);
        return result;
    }
    MYSQL_RES* res = mysql_store_result(dat);
    if(res!=nullptr)
    {
        MYSQL_ROW row;
        while((row=mysql_fetch_row(res))!=NULL)
        {
            chargeINFO ci;
            ci.chargeID = std::stoi(row[0]);
            ci.chargeName = row[1];
            ci.toPay = row[2];
            ci.cycle = row[3];
            ci.status = row[4];
            ci.now_charge_times = std::stoi(row[5]);
            ci.last_charge_date = row[6];
            ci.houseTypeID = std::stoi(row[7]);
            result.push_back(ci);
        }
        mysql_free_result(res);
        return result;
    }else{
        log_error("nullptr of res when get all charge info");
        return result;
    }
    return result;
}