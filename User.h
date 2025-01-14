#pragma once
class Database;
#include <string>
#include "Database.h"
#include "Starter.h"
#include "CommunityINFO.h"

class User
{
public:
 	friend class Starter;
	~User();

	static User* create_user();

	bool add_repair_info();
	bool update_repair_info(std::string updateMSG);
	bool delete_repair_info();
	bool display_repair_info();
	bool repair_info_existed(int repairID);


	bool add_complaint_info();
	bool update_complaint_info(std::string updatedMSG);
	bool delete_complaint_info();
	bool display_complaint_info();
	bool complaint_info_existed(int complaintID);

	void display_repair_menu();
	void display_complaint_menu();
	
	void user_main_menu();
	std::optional<residentINFO> userINFO;
private:
	User();
	Database* data;
	std::vector<std::string> repairMSG;
	std::vector<std::string> complaintMSG;
	static bool isCreated;
};
