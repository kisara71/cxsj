#pragma once
//class Manager;
//class Database;
//#include "Manager.h"
//#include "Database.h"
#include <vector>
#include <string>
struct residentINFO {
	int	ID;
	int houseTypeID;
	std::string prePay;
	std::string debtAmount;
	std::string parkingID;
	std::string name;
	std::string loginName;
	std::string phoneNumber;
	std::string address;
};
//class CommunityINFO
//{
// 
// 
// 
// 
//public:
//	friend class Database;
//	friend class Manager;
//	CommunityINFO();
//	~CommunityINFO();
//private:
	struct communityINFO{
		int communityID;
		std::string communityName;
		std::string buildingAmounts;
	};
	struct chargeINFO{
		int chargeID;
		int houseTypeID;
		int now_charge_times;
		std::string toPay;
		std::string cycle;
		std::string chargeName;
		std::string status;
		std::string last_charge_date;
	};
	struct houseINFO {
		int houseTypeID;
		std::string area;
		std::string type;
	};
	
//
//};
//
