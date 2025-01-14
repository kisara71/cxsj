#pragma once
class Database;
#include <vector>
#include <optional>
#include "CommunityINFO.h"
#include "Starter.h"
#include "Database.h"

class CommunityINFO;
class Starter;

class Manager
{
public:
	friend class Starter;
	Manager();
	~Manager();

	static Manager* create_manager();

	static bool is_number(std::string& input);
	static bool is_exit(std::string content);
	bool is_default(std::string content);
	bool not_phone(std::string& str);
	bool not_double_str(std::string& str);

	bool user_is_exist(int id);
	bool house_is_exist(int id);
	bool community_is_exist(int id);
	bool charge_is_exist(int id);
	bool parking_is_exist(std::string parkingID);

	bool add_user_info();
	bool update_user_info(int sig);
	bool delete_user_info(int sig);


	bool display_all_user_by_debt();
	bool display_all_user();

	bool display_unaccepted_repairmsg();
	bool display_solved_repairmsg();
	bool delete_solved_repairmsg();
	bool target_unaccepted_repairmsg();

	bool display_unaccepted_complaintmsg();
	bool display_solved_complaintmsg();
	bool delete_solved_complaintmsg();
	bool target_unaccepted_complaintmsg();

	bool add_house_info();
	bool display_all_house();
	bool update_house_info();
	bool delete_house_info();

	bool add_community_info();
	bool display_all_community();
	bool update_community_info();
	bool delete_community_info();

	bool add_charge_info();
	bool display_all_charge();
	bool update_charge_info();
	bool delete_charge_info();
	bool synchronize_charge_info();
	

	void manager_main_menu();
	void manager_user_menu();
	void manager_repair_menu();
	void manager_complaint_menu();
	void manager_house_menu();
	void manager_community_menu();
	void manager_charge_menu();
private:
	Database* data;
	static bool isCreated;
	std::optional<residentINFO> resident;
	std::vector<residentINFO> residents;

	std::vector<std::string> repairmsg;
	std::vector<std::string> complaintmsg;

	std::vector<houseINFO> house;
	
	std::vector<communityINFO> community;

	std::vector<chargeINFO> charges;
	//std::vector<CommunityINFO> Community;
};
