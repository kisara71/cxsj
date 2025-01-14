#pragma once
class Starter;
class User;
class Manager;
#include <string>
#include <vector>
#include <optional>
#include "include/mysql.h"
#include "CommunityINFO.h"
#include "User.h"
#include "Manager.h"


class Database{
public:
  ~Database();
   friend class User;
   friend class Manager;
   static Database* createDB(const std::string& baseName = "manager");
   static std::string inputPWD();
   static void log_error(const std::string& message);

   int login();
   bool user_regist();
   int loginNameIsExisted(std::string loginName);

   std::optional<residentINFO> get_user_info(int ID);
   std::string get_pwd(std::string loginName);

   bool repair_info_existed(int repairID);
   bool complaint_info_existed(int complaintID);

   std::vector<std::string> get_user_repairMSG(int id);
   std::vector<std::string> get_user_complaintMSG(int id);

   std::vector<std::string> get_all_unaccepted_repairMSG();
   std::vector<std::string> get_all_unaccepted_complaintMSG();

   std::vector<std::string> get_all_solved_repairMSG();
   std::vector<std::string> get_all_solved_complaintMSG();
   
   std::vector<residentINFO> get_all_user_by_name(std::string inputName);
   std::vector<residentINFO> get_all_user();

   std::vector<houseINFO> get_all_house_info();
   std::vector<communityINFO> get_all_community_info();
   std::vector<chargeINFO> get_all_charge_info();
   
   
private:
    Database(const std::string& baseName);
    MYSQL* dat;
};


