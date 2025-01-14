#include "Manager.h"
#include <iostream>
#include <cstring>
#include <algorithm>
bool Manager::isCreated = false;

Manager::Manager()
{
	Manager::isCreated = true;
	this->data = Database::createDB();
}

Manager::~Manager()
{
}
bool Manager::house_is_exist(int id)
{
	char toQuery[128];
	sprintf(toQuery,"SELECT * FROM houseinfo WHERE houseTypeID = %d;",id);
	if(mysql_query(data->dat,toQuery)!=0)
	{
		std::string error = mysql_error(data->dat);
		std::cout<<"检测房型信息时出错: "+error;
		Database::log_error("check house exist error: "+error);
		return false;
		system("pause");
	}else{
		MYSQL_RES* res = mysql_store_result(data->dat);
		MYSQL_ROW row = mysql_fetch_row(res);
		mysql_free_result(res);
		return row!=NULL;
	}
	return false;
}
bool Manager::is_number(std::string& input)
{
	try{
		std::size_t pos;
        int num = std::stoi(input, &pos);
        return pos == input.size();
	}catch(const std::invalid_argument& e){
		return false;
	}
	return true;
}
Manager* Manager::create_manager()
{
	if (!Manager::isCreated)
	{
		Manager* manager = new Manager();
		return manager;
	}
	return nullptr;
}

bool Manager::user_is_exist(int id)
{
		char checkIsExist[128];
		sprintf(checkIsExist,"SELECT * FROM residents WHERE ID = %d;",id);
		if (mysql_query(data->dat,checkIsExist)!=0)
		{
			std::string error = mysql_error(data->dat);
			Database::log_error("check user exist error: "+error);
			system("pause");
			return false;
		}
		else{
			MYSQL_RES* res = mysql_store_result(data->dat);
			if (res==nullptr)
			{
				std::string error = mysql_error(data->dat);
				Database::log_error("check user exist error: "+error);
				return false;
			}else{
				MYSQL_ROW row = mysql_fetch_row(res);
				mysql_free_result(res);
				return row!=NULL;
			}
			}
			return false;
}
bool Manager::update_charge_info()
{
	system("cls");
	display_all_charge();
	std::cout<<"请输入要修改的收费ID: ";
	std::string inputID;
	std::cin>>inputID;
	if(is_exit(inputID))
	{
		manager_charge_menu();
		return false;
	}
	if(!is_number(inputID))
	{
		std::cout<<"不合法的收费ID"<<std::endl;
		system("pause");
		manager_charge_menu();
		return false;
	}
	int choice = std::stoi(inputID);
	if(!charge_is_exist(choice))
	{
		std::cout<<"不存在的收费ID,请检查后重试"<<std::endl;
		system("pause");
		manager_charge_menu();
		return false;
	}
	int toUpdate;
	std::string input;
	std::cout<<"请输入要修改的信息: \n0.返回上级菜单\n1.收费项目名称\n2.收费金额\n3.收费周期\n4.目标收费次数\n5.已收费次数\n6.上次收费日期\n7.房型ID\nchoice: ";
	std::cin>>input;
	if(is_exit(input))
	{
		manager_charge_menu();
		return false;
	}
	if(!is_number(input))
	{
		std::cout<<"未知输入"<<std::endl;
		system("pause");
		manager_charge_menu();
		return false;
	}
	toUpdate = std::stoi(input);
	std::string secondInput;
	char toQuery[128];
	int newHouseID;
	switch (toUpdate)
	{
		case 1:
			std::cout<<"请输入修改后的收费项目名称: ";
			std::cin>>secondInput;
			if(is_exit(secondInput))
			{
				manager_charge_menu();
				return false;
			}
			sprintf(toQuery,"UPDATE chargeinfo SET chargeName = '%s' WHERE chargeID = %d;",secondInput.c_str(),choice);
			if(mysql_query(data->dat,toQuery)!=0)
			{
				std::string error = mysql_error(data->dat);
				std::cout<<"更新收费项目名称失败: "+error<<std::endl;
				Database::log_error("update chargeName error: "+error);
				system("pause");
				manager_charge_menu();
				return false;
			}
			std::cout<<"更新收费项目名称成功"<<std::endl;
			system("pause");
			return true;
			break;
		case 2:
			std::cout<<"请输入修改后的收费金额: ";
			std::cin>>secondInput;
			if(is_exit(secondInput))
			{
				manager_charge_menu();
				return false;
			}
			if(not_double_str(secondInput))
			{
				std::cout<<"请输入正确的收费金额"<<std::endl;
				system("pause");
				manager_charge_menu();
				return false;
			}
			secondInput+="元";
			sprintf(toQuery,"UPDATE chargeinfo SET toPay = '%s' WHERE chargeID = %d;",secondInput.c_str(),choice);
			if(mysql_query(data->dat,toQuery)!=0)
			{
				std::string error = mysql_error(data->dat);
				std::cout<<"更新收费金额失败: "+error<<std::endl;
				Database::log_error("update toPay error: "+error);
				system("pause");
				manager_charge_menu();
				return false;
			}
			std::cout<<"更新收费金额成功"<<std::endl;
			system("pause");
			return true;
			break;
		case 3:
			std::cout<<"请输入修改后的收费周期,按月输入,只输入数字即可: ";
			std::cin>>secondInput;
			if(is_exit(secondInput))
			{
				manager_charge_menu();
				return false;
			}
			if(!is_number(secondInput))
			{
				std::cout<<"无法识别的月数"<<std::endl;
				system("pause");
				manager_charge_menu();
			}
			secondInput= "每"+secondInput+"个月";
			sprintf(toQuery,"UPDATE chargeinfo SET cycle = '%s' WHERE chargeID = %d;",secondInput.c_str(),choice);
			if(mysql_query(data->dat,toQuery)!=0)
			{
				std::string error = mysql_error(data->dat);
				std::cout<<"更新收费周期失败: "+error<<std::endl;
				Database::log_error("update cycle error: "+error);
				system("pause");
				manager_charge_menu();
				return false;
			}
			std::cout<<"更新收费周期成功"<<std::endl;
			system("pause");
			return true;
			break;
		case 4:
			std::cout<<"请输入修改后的收费期数(目标收费次数,长期收费输入0即可,反正输入对应期数): ";
			std::cin>>secondInput;
			if(is_exit(secondInput))
			{
				manager_charge_menu();
				return false;
			}
			if(!is_number(secondInput))
			{
				std::cout<<"无法识别的次数"<<std::endl;
				system("pause");
				manager_charge_menu();
				return false;
			}
			if(input!="0\0")
			{
				secondInput = "收费"+secondInput+"次";
			}else{
				secondInput = "长期收费";
			}
			sprintf(toQuery,"UPDATE chargeinfo SET status = '%s' WHERE chargeID = %d;",secondInput.c_str(),choice);
			if(mysql_query(data->dat,toQuery)!=0)
			{
				std::string error = mysql_error(data->dat);
				std::cout<<"更新目标收费失败: "+error<<std::endl;
				Database::log_error("update status error: "+error);
				system("pause");
				manager_charge_menu();
				return false;
			}
			std::cout<<"更新目标收费次数成功"<<std::endl;
			system("pause");
			return true;
			break;
		case 5:
			std::cout<<"请输入已收费的次数: ";
			std::cin>>secondInput;
			if(!is_number(secondInput))
			{
				std::cout<<"无法识别的收费次数";
				system("pause");
				manager_charge_menu();
				return false;
			}
			sprintf(toQuery,"UPDATE chargeinfo SET now_charge_times = %d WHERE chargeID = %d;",std::stoi(secondInput),choice);
			if(mysql_query(data->dat,toQuery)!=0)
			{
				std::string error = mysql_error(data->dat);
				std::cout<<"更新已收费次数失败: "+error<<std::endl;
				Database::log_error("update now charge times error: "+error);
				system("pause");
				manager_charge_menu();
				return false;
			}
			std::cout<<"更新已收费次数成功"<<std::endl;
			system("pause");
			return true;
			break;
		case 6:
			std::cout<<"请按xxxx-xx-xx的格式输入年月日: ";
			std::cin>>secondInput;
			secondInput = secondInput +" 00:00:00";
			sprintf(toQuery,"UPDATE chargeinfo SET last_charge_date = '%s' WHERE chargeID = %d;",secondInput.c_str(),choice);
			if(mysql_query(data->dat,toQuery)!=0)
			{
				std::string error = mysql_error(data->dat);
				std::cout<<"更新上次收费日期失败: "+error<<std::endl;
				Database::log_error("update last charge date error: "+error);
				system("pause");
				manager_charge_menu();
				return false;
			}
			std::cout<<"更新上次收费日期成功"<<std::endl;
			system("pause");
			return true;
			break;
		case 7:
			std::cout<<"请输入修改后的房型ID: ";
			std::cin>>secondInput;
			if(is_exit(secondInput))
			{
				manager_charge_menu();
				return false;
			}
			if(!is_number(secondInput))
			{
				std::cout<<"无法识别的房型ID"<<std::endl;
				system("pause");
				manager_charge_menu();
				return false;
			}
			newHouseID = std::stoi(secondInput);
			if(!house_is_exist(newHouseID))
			{
				std::cout<<"不存在的房型,请检查后重试"<<std::endl;
				system("pause");
				manager_charge_menu();
				return false;
			}
			sprintf(toQuery,"UPDATE chargeinfo SET houseTypeID = %d WHERE chargeID = %d;",newHouseID,choice);
			if(mysql_query(data->dat,toQuery)!=0)
			{
				std::string error = mysql_error(data->dat);
				std::cout<<"更新房型ID失败: "+error<<std::endl;
				Database::log_error("update charge houseTypeID error: "+error);
				system("pause");
				manager_charge_menu();
				return false;
			}
			std::cout<<"更新房型ID成功"<<std::endl;
			system("pause");
			return true;
			break;
	}
	return false;
	
}
bool Manager::update_community_info()
{
	system("cls");
	display_all_community();
	std::cout<<"请输入要修改的小区ID: ";
	std::string inputID;
	std::cin>>inputID;
	if(is_exit(inputID))
	{
		manager_community_menu();
		return false;
	}
	if(!is_number(inputID))
	{
		std::cout<<"不合法的小区ID"<<std::endl;
		system("pause");
		manager_community_menu();
		return false;
	}
	int choice = std::stoi(inputID);
	if(!community_is_exist(choice))
	{
		std::cout<<"不存在的小区ID,请检查后重试"<<std::endl;
		system("pause");
		manager_community_menu();
		return false;
	}
	int toUpdate;
	std::string input;
	std::cout<<"请输入要修改的信息: \n0.返回上级菜单\n1.小区ID\n2.小区名\n3.小区楼宇数\nchoice: ";
	std::cin>>input;
	if(is_exit(input))
	{
		manager_community_menu();
		return false;
	}
	if(!is_number(input))
	{
		std::cout<<"未知输入"<<std::endl;
		system("pause");
		manager_community_menu();
		return false;
	}
	toUpdate = std::stoi(input);
	std::string secondInput;
	char toQuery[128];
	int newID;
	switch(toUpdate)
	{
		case 1:
			std::cout<<"请输入修改后的小区ID: ";
			std::cin>>secondInput;
			if(is_exit(secondInput))
			{
				manager_community_menu();
				return false;
			}
			if(!is_number(secondInput))
			{
				std::cout<<"不合法的小区ID"<<std::endl;
				system("pause");
				manager_community_menu();
				return false;
			}
			newID = std::stoi(secondInput);
			if(community_is_exist(newID))
			{
				std::cout<<"小区ID已存在,请检查后重试"<<std::endl;
				system("pause");
				manager_community_menu();
				return false;
			}
			sprintf(toQuery,"UPDATE communityinfo SET communityID = %d WHERE communityID = %d;",newID,choice);
			if(mysql_query(data->dat,toQuery)!=0)
			{
				std::string error = mysql_error(data->dat);
				std::cout<<"更新小区ID失败: "+error<<std::endl;
				Database::log_error("update community id error: "+error);
				manager_community_menu();
				return false;
			}else{
				std::cout<<"修改成功"<<std::endl;
				system("pause");
				return true;
			}
			break;
		case 2:
			std::cout<<"请输入修改后的小区名: ";
			std::cin>>secondInput;
			if (is_exit(secondInput))
			{
				this->manager_community_menu();
				return false;
			}
			sprintf(toQuery,"UPDATE communityinfo SET communityName = '%s' WHERE communityID = %d;",secondInput.c_str(),choice);
			if(mysql_query(data->dat,toQuery)!=0)
			{
				std::string error = mysql_error(data->dat);
				std::cout<<"更新小区名失败: "+error<<std::endl;
				Database::log_error("update community name error: "+error);
				manager_community_menu();
				return false;
			}else{
				std::cout<<"修改成功"<<std::endl;
				system("pause");
				return true;
			}
			break;
		case 3:
			std::cout<<"请输入修改后的楼宇数: ";
			std::cin>>secondInput;
			if (is_exit(secondInput))
			{
				this->manager_community_menu();
				return false;
			}
			if(!is_number(secondInput))
			{
				std::cout<<"请输入正确的楼宇数"<<std::endl;
				system("pause");
				manager_community_menu();
				return false;
			}
			secondInput+="栋";
			sprintf(toQuery,"UPDATE communityinfo SET buildingAmounts = '%s' WHERE communityID = %d;",secondInput.c_str(),choice);
			if(mysql_query(data->dat,toQuery)!=0)
			{
				std::string error = mysql_error(data->dat);
				std::cout<<"更新楼宇失败: "+error<<std::endl;
				Database::log_error("update community buildingAmounts error: "+error);
				manager_community_menu();
				return false;
			}else{
				std::cout<<"修改成功"<<std::endl;
				system("pause");
				return true;
			}
			break;
		default:
			manager_main_menu();
			break;
	}
	return false;
}
bool Manager::delete_community_info()
{
	system("cls");
	display_all_community();
	std::cout<<"请输入要删除的小区ID: ";
	std::string input;
	std::cin>>input;
	if(is_exit(input))
	{
		manager_community_menu();
		return false;
	}
	if(!is_number(input))
	{
		std::cout<<"不合法的小区ID"<<std::endl;
		system("pause");
		manager_community_menu();
		return false;
	}
	int choice = std::stoi(input);
	if(!community_is_exist(choice))
	{
		std::cout<<"不存在的小区ID,请检查后重试"<<std::endl;
		system("pause");
		manager_community_menu();
		return false;
	}
	std::cout<<"确认要删除吗? y/n: ";
	std::string makeSure;
	std::cin>>makeSure;
	if(makeSure!="y\0")
	{
		manager_community_menu();
		return false;
	}
	char toQuery[128];
	sprintf(toQuery,"DELETE FROM communityinfo WHERE communityID = %d;",choice);
	if(mysql_query(data->dat,toQuery)!=0)
	{
		std::string error = mysql_error(data->dat);
		std::cout<<"删除小区信息时出错: "+error<<std::endl;
		Database::log_error("delet communityinfo error: "+error);
		manager_community_menu();
		return false;
	}
	std::cout<<"删除成功"<<std::endl;
	system("pause");
	manager_community_menu();
	return true;
}
bool Manager::community_is_exist(int id)
{
	char toQuery[128];
	sprintf(toQuery,"SELECT * FROM communityinfo WHERE communityID = %d;",id);
	if(mysql_query(data->dat,toQuery)!=0)
	{
		std::string error = mysql_error(data->dat);
		std::cout<<"检测小区信息时出错: "+error;
		Database::log_error("check community exist error: "+error);
		return false;
		system("pause");
	}else{
		MYSQL_RES* res = mysql_store_result(data->dat);
		MYSQL_ROW row = mysql_fetch_row(res);
		mysql_free_result(res);
		return row!=NULL;
	}
	return false;
}
bool Manager::add_community_info()
{
	system("cls");
	std::cout<<"已为您分配小区ID,是否引用?y/n: ";
	std::string use;
	std::cin>>use;
	if(is_exit(use))
	{
		manager_community_menu();
		return false;
	}
	int ID = -1;
	std::string communityName;
	std::string buildingAmounts;
	if(use!="y\0")
	{
		std::cout<<"请输入自定ID: ";
		std::string inputID;
		std::cin>>inputID;
		if(is_exit(inputID))
		{
			manager_community_menu();
			return false;
		}
		if(!is_number(inputID))
		{
			std::cout<<"不合法的小区id"<<std::endl;
			system("pause");
			manager_community_menu();
			return false;
		}
		ID = std::stoi(inputID);
		if(community_is_exist(ID))
		{
			std::cout<<"小区ID已存在,请检查后重试"<<std::endl;
			system("pause");
			manager_community_menu();
			return false;
		}
	}
	std::cout<<"请输入小区名: ";
	std::string inputCommunity;
	std::cin>>inputCommunity;
	if(is_exit(inputCommunity))
	{
		manager_community_menu();
		return false;
	}
	communityName = inputCommunity;
	std::cout<<"请输入小区楼宇数: ";
	std::string inputAmounts;
	std::cin>>inputAmounts;
	if(is_exit(inputAmounts))
	{
		manager_community_menu();
		return false;
	}
	if(!is_number(inputAmounts))
	{
		std::cout<<"请输入正确的楼宇数"<<std::endl;
		system("pause");
		manager_community_menu();
		return false;
	}
	buildingAmounts = inputAmounts;
	buildingAmounts+="栋";
	char toQuery[128];
	if(ID==-1)
	{
		sprintf(toQuery,"INSERT INTO communityinfo (communityName,buildingAmounts) VALUES('%s','%s');",communityName.c_str(),buildingAmounts.c_str());
	}else{
		sprintf(toQuery,"INSERT INTO communityinfo (communityID,communityName,buildingAmounts) VALUES(%d,'%s','%s');",ID,communityName.c_str(),buildingAmounts.c_str());
	}
	if (mysql_query(data->dat,toQuery)!=0)
		{
			std::string error = mysql_error(data->dat);
			Database::log_error("insert community info error: "+ error);
			std::cout<<"insert community info error: "+ error;
			system("pause");
			manager_community_menu();
			return false;
			
		}else{
			std::cout<<"新建小区信息完成"<<std::endl;
			system("pause");
			return true;
		}
	return false;
}
void Manager::manager_community_menu()
{
	system("cls");
	std::cout<<"0.返回上级菜单\n1.查看小区信息\n2.新增小区信息\n3.修改小区信息\n4.删除小区信息\n请输入操作: ";
	int choice;
    std::string input;
    if (std::cin.peek() == '\n') {
        std::cin.ignore(); 
    }
    getline(std::cin,input);
    if(Manager::is_exit(input))
    {
        manager_main_menu();
        return;
    }
    if(Manager::is_number(input))
    {
        choice = std::stoi(input);
    }else{
        std::cout<<"不明确的选项"<<std::endl;
        system("pause");
        manager_community_menu();
        return;
    }
	switch(choice)
	{
		case 1:
			system("cls");
			this->display_all_community();
			break;
		case 2:
			this->add_community_info();
			break;
		case 3:
			this->update_community_info();
			break;
		case 4:
			this->delete_community_info();
			break;
		default:
			this->manager_main_menu();
			break;
	}
}
void Manager::manager_main_menu()
{
	system("cls");
    std::cout << "欢迎使用管理员系统" << std::endl;
	std::cout<<"0.退出\n1.用户基础信息管理\n2.用户报修管理\n3.用户投诉管理\n4.房型信息管理\n5.小区资料管理\n6.物业收费管理\n";
	std::cout<<"要进行的操作: ";
	int choice;
    std::string input;
    if (std::cin.peek() == '\n') {
        std::cin.ignore(); 
    }
    getline(std::cin,input);
    if(Manager::is_exit(input))
    {
        manager_main_menu();
        return;
    }
    if(Manager::is_number(input))
    {
        choice = std::stoi(input);
    }else{
        std::cout<<"不明确的选项"<<std::endl;
        system("pause");
       	manager_main_menu();
        return;
    }

	switch(choice)
	{
		case 1:
			this->manager_user_menu();
			break;
		case 2:
			this->manager_repair_menu();
			break;
		case 3:
			this->manager_complaint_menu();
			break;
		case 4:
			this->manager_house_menu();
			break;
		case 5:
			this->manager_community_menu();
			break;
		case 6:
			this->manager_charge_menu();
			break;
		default:
			exit(0);
			break;
	}
	return;
}

bool Manager::is_default(std::string content)
{
	return content=="default\0";
}
bool Manager::is_exit(std::string content)
{
	return content=="exit\0";
}
bool Manager::not_phone(std::string& str)
{
	for(auto ch: str)
	{
		if(ch!='-'&&!std::isdigit(ch))
		{
			return true;
		}
	}
	return false;
}
bool Manager::add_user_info()
{
	system("cls");
	std::cout<<"如用户信息不明确或者没有,输入default录入默认信息,在录入任一信息时,输入exit退出。"<<std::endl;
	std::cout<<"已分配id,是否引用 y/n? ";
	std::string use;
	std::cin>>use;
	if(is_exit(use))
	{
		manager_user_menu();
		return false;
	}
	int realID = -2005;
	std::string inputID;
	idput:
	if(use!="y\0")
	{
		std::cout<<"输入自定id: ";
		std::cin>>inputID;
		if (is_exit(inputID))
		{
			this->manager_user_menu();
			return false;
		}
		if (is_number(inputID))
		{
			realID = std::stoi(inputID);
		}else{
			std::cout<<"不合法的id"<<std::endl;
			system("pause");
			goto idput;
		}
		
	}
	std::cout<<"输入用户姓名: ";
	std::string inputUserName;
	std::cin>>inputUserName;
	if (is_exit(inputUserName))
	{
		this->manager_user_menu();
		return false;
	}
	std::string realUserName = "";
	if (!is_default(inputUserName))
	{
		realUserName = inputUserName;
	}

	std::cout<<"输入用户预付款: ";
	std::string inputPrePay;
	std::cin>>inputPrePay;
	if (is_exit(inputPrePay))
	{
		this->manager_user_menu();
		return false;
	}
	
	if(not_double_str(inputPrePay))
	{
		std::cout<<"请输入正确的金额"<<std::endl;
		system("pause");
		manager_user_menu();
		return false;
	}
	std::string realPrePay = "0";
	if (!is_default(inputPrePay)){		
		realPrePay = inputPrePay;
		realPrePay+="元";
	}

	std::cout<<"输入用户欠款: ";
	std::string inputDebt;
	std::cin>>inputDebt;
	if(not_double_str(inputDebt))
	{
		std::cout<<"请输入正确的金额"<<std::endl;
		system("pause");
		manager_user_menu();
		return false;
	}
	if (is_exit(inputDebt))
	{
		this->manager_user_menu();
		return false;
	}
	std::string realDebt = "";
	if(!is_default(inputDebt))
	{
		realDebt = inputDebt;
		realDebt+="元";
	}
	park:
	std::cout<<"输入用户停车位: ";
	std::string inputParkingID;
	std::cin>>inputParkingID;
	if (is_exit(inputParkingID))
	{
		this->manager_user_menu();
		return false;
	}
	if(parking_is_exist(inputParkingID))
	{
		std::cout<<"该停车位已被占用，请检查后输入"<<std::endl;
		system("pause");
		goto park;
	}
	std::string realParkingID = "none";
	if(!is_default(inputParkingID))
	{
		realParkingID = inputParkingID;
	}

	std::cout<<"输入用户住址: ";
	std::string inputAddress;
	std::cin>>inputAddress;
	if (is_exit(inputAddress))
	{
		this->manager_user_menu();
		return false;
	}
	std::string realAddress = "unknown";
	if (!is_default(inputAddress))
	{
		realAddress = inputAddress;
	}
	houseput:
	std::cout<<"输入用户房型ID: ";
	std::string inputHouse;
	std::cin>>inputHouse;
	if (is_exit(inputHouse))
	{
		this->manager_user_menu();
		return false;
	}
	int realHouse = 0;
	if(!is_default(inputHouse))
	{
		if(is_number(inputHouse)){
			realHouse = std::stoi(inputHouse);
			if(!house_is_exist(realHouse))
			{
				std::cout<<"房型不存在"<<std::endl;
				system("pause");
				goto houseput;
			}
		} else {
			std::cout<<"不合法的房型ID"<<std::endl;
			system("pause");
			goto houseput;
		}
	}

	std::cout<<"输入用户电话号码: ";
	std::string inputPhone;
	std::cin>>inputPhone;
	if(not_phone(inputPhone))
	{
		std::cout<<"请输入正确的手机号"<<std::endl;
		system("pause");
		manager_user_menu();
		return false;
	}
	if (is_exit(inputPhone))
	{
		this->manager_user_menu();
		return false;
	}
	std::string realPhone = "unknown";
	if (!is_default(inputPhone))
	{
		realPhone = inputPhone;
	}
	mysql_start:
	if(realID==-2005)
	{
		char toQuery[512];
		sprintf(toQuery,"INSERT INTO residents (name,address,houseTypeID,parkingID,prePay,debtAmount,phoneNumber) VALUES('%s','%s',%d,'%s','%s','%s','%s');",realUserName.c_str(), realAddress.c_str() ,realHouse,realParkingID.c_str(), realPrePay.c_str(), realDebt.c_str(), realPhone.c_str());
		if (mysql_query(data->dat,toQuery)!=0)
		{
			std::string error = mysql_error(data->dat);
			Database::log_error("insert user info error: "+ error);
			std::cout<<"insert user info error: "+ error;
			system("pause");
			return false;
			
		}else{
			std::cout<<"新建用户完成"<<std::endl;
			system("pause");
			return true;
		}
	}
else{
			if (user_is_exist(realID))
			{
				secondid:
				std::cout<<"检测到用户id重复\n1.修改已存用户\n2.覆盖已存用户\n3.重新选择id\nothers.回到用户菜单";
				std::cout<<"输入选项: ";
				int choice;
				std::cin>>choice;
				if (std::cin.fail())
				{	
					std::cout << "不明序号" << std::endl;
					system("pause");
					std::cin.clear();
					std::cin.ignore(10000, '\n');
					manager_user_menu();
					return false;
				}
				switch(choice)
				{
					case 1:
						update_user_info(realID);
						break;
					case 2:
						if(delete_user_info(realID))
						{
							goto mysql_start;
						}
						break;
					case 3:
						std::cout<<"输入自定id: ";
						std::cin>>inputID;
						if (is_exit(inputID))
						{
							this->manager_user_menu();
							return false;
						}	
						if(is_number(inputID)){
							realID = std::stoi(inputID);
							} 
							else {
								std::cout<<"不合法的id"<<std::endl;
								system("pause");
								goto secondid;
							}
						goto mysql_start;
						break;
					default:
						manager_user_menu();
						return false;
						break;
				}
			}
			else{
					char toQuery[512];
					sprintf(toQuery,"INSERT INTO residents (ID,name,address,houseTypeID,parkingID,prePay,debtAmount,phoneNumber) VALUES(%d,'%s','%s',%d,'%s','%s','%s','%s');",realID,realUserName.c_str(), realAddress.c_str() ,realHouse ,realParkingID.c_str(), realPrePay.c_str(), realDebt.c_str(), realPhone.c_str());
					if (mysql_query(data->dat,toQuery)!=0)
					{
						std::string error = mysql_error(data->dat);
						Database::log_error("insert user info error: "+ error);
						std::cout<<"insert user info error: "+ error;
						system("pause");
						return false;
						
					}
					else{
						std::cout<<"新建用户完成"<<std::endl;
						system("pause");
						return true;
						}
				}
		
	}
return false;
}


bool Manager::update_user_info(int sig = 0)
{
	system("cls");
	std::string input;
	int checkID = -1;
	if(sig==0)
	{
		display_all_user();
		std::cout<<"输入任何数据时可输入exit返回上级菜单"<<std::endl;
		std::cout<<"请输入要更改的住户id/姓名: ";
		std::cin>>input;
	}else{
		checkID = sig;
		goto start;
	}
	if(is_exit(input))
	{
		manager_user_menu();
		return false;
	}
	if(is_number(input)){
		checkID = std::stoi(input);
		this->resident = data->get_user_info(checkID);
		if (!resident)
		{
			std::cout<<"住户id不存在"<<std::endl;
			system("pause");
			manager_user_menu();
			return false;
		}
		std::cout<<"为您查询到该用户: "<<std::endl;
		std::cout<<"id: "<<resident->ID<<"\t姓名: "<<resident->name<<"\t住址: "<<resident->address<<"\t房型ID: "<<resident->houseTypeID<<"\t预付款: "<<resident->prePay<<"\t欠费: "<<resident->prePay<<"\t停车位: "<<resident->parkingID<<"\t电话号: "<<resident->phoneNumber<<std::endl;
		startUpdate:
		this->resident = data->get_user_info(checkID);
		if (!resident)
		{
			std::cout<<"住户id不存在"<<std::endl;
			system("pause");
			manager_user_menu();
			return false;
		}
		start:
		if(sig!=0)
		{
			this->resident = data->get_user_info(checkID);
			std::cout<<"id: "<<resident->ID<<"\t姓名: "<<resident->name<<"\t住址: "<<resident->address<<"\t房型ID: "<<resident->houseTypeID<<"\t预付款: "<<resident->prePay<<"\t欠费: "<<resident->prePay<<"\t停车位: "<<resident->parkingID<<"\t电话号: "<<resident->phoneNumber<<std::endl;
		}
		std::string inputid;
		std::string newPhone;
		std::string newName;
		std::string newAddress;
		std::string inputHouse;
		std::string newPrePay;
		std::string newParking;
		std::string newDebt;
		int newHouseID;
		std::cout<<"选择修改项: \n0.返回上级菜单\n1.id\n2.姓名\n3.住址\n4.房型\n5.预付款\n6.欠费\n7.停车位\n8.电话号\n: ";
		std::string inputchoice;
		std::cin>>inputchoice;
		if(is_exit(inputchoice))
		{
			manager_user_menu();
			return false;
		}
		if(!is_number(inputchoice))
		{
			std::cout << "不明选项" << std::endl;
			system("pause");
			manager_user_menu();
			return false;
		}
		int choice = std::stoi(inputchoice);
		char toQuery[128];
		switch(choice)
		{
			case 1:
				std::cout<<"请输入要修改的id: ";
				int newID;
				std::cin>>inputid;
				if (is_exit(inputid))
				{
					this->manager_user_menu();
					return false;
				}
				if(is_number(inputid)){
					newID = std::stoi(inputid);
				} else {
					std::cout<<"不合法的id"<<std::endl;
					system("pause");
					manager_user_menu();
					return false;
				}
				sprintf(toQuery,"UPDATE residents SET ID = %d WHERE ID = %d;",newID,checkID);
				if (mysql_query(data->dat,toQuery)!=0)
				{
					std::string error = mysql_error(data->dat);
					std::cout<<"update user ID error: "+error<<std::endl;
					Database::log_error("update user ID error: "+error);
					this->manager_user_menu();
					return false;
				}
				else{
					std::cout<<"住户ID修改成功"<<std::endl;
					system("pause");
					this->manager_user_menu();
					return false;
				}
			case 2:
				std::cout<<"请输入修改后的住户姓名： ";
				std::cin>>newName;
				if (is_exit(newName))
				{
					this->manager_user_menu();
					return false;
				}
				sprintf(toQuery,"UPDATE residents SET name = '%s' WHERE ID = %d;",newName.c_str(),checkID);
				if (mysql_query(data->dat,toQuery)!=0)
				{
					std::string error = mysql_error(data->dat);
					std::cout<<"update user name error: "+error<<std::endl;
					Database::log_error("update user name error: "+error);
					this->manager_user_menu();
					return false;
				}
				else{
					std::cout<<"住户名字修改成功"<<std::endl;
					system("pause");
					this->manager_user_menu();
					return true;
				}
				break;
			case 3:
				std::cout<<"请输入修改后的住户住址： ";
				std::cin>>newAddress;
				if(is_exit(newAddress))
				{
					manager_user_menu();
					return false;
				}
				sprintf(toQuery,"UPDATE residents SET address = '%s' WHERE ID = %d;",newAddress.c_str(),checkID);
				if (mysql_query(data->dat,toQuery)!=0)
				{
					std::string error = mysql_error(data->dat);
					std::cout<<"update user address error: "+error<<std::endl;
					Database::log_error("update user address error: "+error);
					this->manager_user_menu();
					return false;
				}else{
					std::cout<<"住户地址修改成功"<<std::endl;
					system("pause");
					this->manager_user_menu();
					return false;
				}
				break;
			case 4:
				std::cout<<"请输入新的房型id: ";
				std::cin>>inputHouse;
				if(is_exit(inputHouse))
				{
					manager_user_menu();
					return false;
				}
				if(!is_number(inputHouse))
				{
					std::cout<<"不合法的房型id"<<std::endl;
					system("pause");
					manager_user_menu();
					return false;
				}
				newHouseID = std::stoi(inputHouse);
				if(!house_is_exist(newHouseID))
				{
					std::cout<<"不存在的房型"<<std::endl;
					system("pause");
					manager_user_menu();
					return false;
				}
				sprintf(toQuery,"UPDATE residents SET houseTypeID = %d WHERE ID = %d;",newHouseID,checkID);
				if (mysql_query(data->dat,toQuery)!=0)
				{
					std::string error = mysql_error(data->dat);
					std::cout<<"update user houseTypeID error: "+error<<std::endl;
					Database::log_error("update user houseTypeID error: "+error);
					this->manager_user_menu();
					return false;
				}else{
					std::cout<<"住户房型修改成功"<<std::endl;
					system("pause");
					this->manager_user_menu();
					return false;
				}
				break;
			case 5:
				std::cout<<"请输入修改后的预付款: ";
				std::cin>>newPrePay;
				if (is_exit(newPrePay))
				{
					this->manager_user_menu();
					return false;
				}
				if(not_double_str(newPrePay))
				{
					std::cout<<"请输入正确的金额"<<std::endl;
					system("pause");
					manager_user_menu();
					return false;
				}
				newPrePay+="元";
				sprintf(toQuery,"UPDATE residents SET prePay = '%s' WHERE ID = %d;",newPrePay.c_str(),checkID);
				if (mysql_query(data->dat,toQuery)!=0)
				{
					std::string error = mysql_error(data->dat);
					std::cout<<"update user prePay error: "+error<<std::endl;
					Database::log_error("update user prePay error: "+error);
					this->manager_user_menu();
					return false;
				}else{
					std::cout<<"住户地址预付款修改成功"<<std::endl;
					system("pause");
					this->manager_user_menu();
					return false;
				}
				break;
			case 6:
				std::cout<<"请输入修改后的欠费金额： ";
				std::cin>>newDebt;
				if (is_exit(newDebt))
				{
					this->manager_user_menu();
					return false;
				}
				if(not_double_str(newDebt))
				{
					std::cout<<"请输入正确的金额"<<std::endl;
					system("pause");
					manager_user_menu();
					return false;
				}
				newDebt+="元";
				sprintf(toQuery,"UPDATE residents SET debtAmount = '%s' WHERE ID = %d;",newDebt.c_str(),checkID);
				if (mysql_query(data->dat,toQuery)!=0)
				{
					std::string error = mysql_error(data->dat);
					std::cout<<"update user debt error: "+error<<std::endl;
					Database::log_error("update user debt error: "+error);
					this->manager_user_menu();
					return false;
				}
				else{
					std::cout<<"住户欠费修改成功"<<std::endl;
					system("pause");
					this->manager_user_menu();
					return false;
				}
				break;
			case 7:
				std::cout<<"请输入修改后的停车位: ";
				std::cin>>newParking;
				if (is_exit(newParking))
				{
					this->manager_user_menu();
					return false;
				}
				if(parking_is_exist(newParking))
				{
					std::cout<<"该停车位已被占用，请检查"<<std::endl;
					system("pause");
					manager_user_menu();
					return false;
				}
				sprintf(toQuery,"UPDATE residents SET parkingID = '%s' WHERE ID = %d;",newParking.c_str(),checkID);
				if (mysql_query(data->dat,toQuery)!=0)
				{
					std::string error = mysql_error(data->dat);
					std::cout<<"update user parkingID error: "+error<<std::endl;
					Database::log_error("update user parkingID error: "+error);
					this->manager_user_menu();
					return false;
				}else{
					std::cout<<"住户停车位修改成功"<<std::endl;
					system("pause");
					this->manager_user_menu();
					return false;
				}
				break;
			case 8:
				std::cout<<"请输入修改后的手机号: ";
				std::cin>>newPhone;
				if(not_phone(newPhone))
				{
					std::cout<<"请输入正确的手机号"<<std::endl;
					system("pause");
					manager_user_menu();
					return false;
				}
				sprintf(toQuery,"UPDATE residents SET phoneNumber = '%s' WHERE ID = %d;",newPhone.c_str(),checkID);
				if (mysql_query(data->dat,toQuery)!=0)
				{
					std::string error = mysql_error(data->dat);
					std::cout<<"update user phonenumber error: "+error<<std::endl;
					Database::log_error("update user phonenumber error: "+error);
					this->manager_user_menu();
					return false;
				}else{
					std::cout<<"住户手机号修改成功"<<std::endl;
					system("pause");
					this->manager_user_menu();
					return false;
				}
		}	
	}else
	{
		this->residents = data->get_all_user_by_name(input);
		if(residents.size()==0)
		{
			std::cout<<"未查询到 '"<<input<<"' "<<std::endl;
			system("pause");
			manager_user_menu();
			return false;
		}else{
			std::cout<<"为您查询到以下用户: "<<std::endl;
			for (auto r:this->residents)
			{
				std::cout<<"id: "<<r.ID<<"\t姓名: "<<r.name<<"\t住址: "<<r.address<<"\t房型ID: "<<r.houseTypeID<<"\t预付款: "<<r.prePay<<"\t欠费: "<<r.prePay<<"\t停车位: "<<r.parkingID<<"\t电话号: "<<r.phoneNumber<<std::endl;
			}
			std::cout<<"请输入要修改的用户ID: ";
			std::string secondInput;
			std::cin>>secondInput;
			if(is_exit(secondInput))
			{
				manager_user_menu();
				return false;
			}
			if(is_number(secondInput))
			{
				checkID = std::stoi(secondInput);
				goto startUpdate;
			}else{
				std::cout<<"不合法的id"<<std::endl;
				system("pause");
				manager_user_menu();
				return false;
			}
		}
	}
	return false;
}

bool Manager::delete_user_info(int sig=0)
{
	if(sig==0)
	{	

		system("cls");
		display_all_user();
		std::cout<<"请输入要删除用户的id/姓名: ";
		std::string input;
		std::cin>>input;
		if(is_exit(input))
		{
			this->manager_user_menu();
			return false;
		}
		int id = -1;
		if(is_number(input)){
			id = std::stoi(input);
			resident = data->get_user_info(id);
			if(resident)
			{
				std::cout<<"为您查询到该用户： "<<std::endl;
				std::cout<<"id: "<<resident->ID<<"\t姓名: "<<resident->name<<"\t住址: "<<resident->address<<"\t房型ID: "<<resident->houseTypeID<<"\t预付款: "<<resident->prePay<<"\t欠费: "<<resident->prePay<<"\t停车位: "<<resident->parkingID<<"\t电话号: "<<resident->phoneNumber<<std::endl;
				startdelete:
				resident = data->get_user_info(id);
				if (!resident)
				{
				std::cout<<"住户id不存在"<<std::endl;
				system("pause");
				manager_user_menu();
				return false;
				}
				std::cout<<"是否要删除 y/n? : ";
				std::string de;std::cin>>de;
				if (de=="y\0")
				{
					std::cout<<"输入 y 确认您的操作：";
					std::string makeSure;
					std::cin>>makeSure;
					if(makeSure=="y\0")
					{
						char toQuery[128];
						sprintf(toQuery,"DELETE FROM residents WHERE ID = %d;",id);
						if(mysql_query(data->dat,toQuery)!=0)
						{
							std::string error = mysql_error(data->dat);
							Database::log_error("delete user info error: "+ error);
							std::cout<<"delete user info error"+error<<std::endl;
							system("pause");
							this->manager_main_menu();
							return false;
						}
						else{
							std::cout<<"删除成功!"<<std::endl;
							system("pause");
							this->manager_user_menu();
							return true;
						}
					}
					else{
						this->manager_user_menu();
						return false;
					}
				}
			}
			else{
				std::cout<<"未查询到该用户"<<std::endl;
				system("pause");
				this->manager_user_menu();
				return false;
			}
			this->manager_user_menu();
			return false;
		}else
		{
			this->residents = data->get_all_user_by_name(input);
			if(residents.size()==0)
			{
				std::cout<<"未查询到 '"<<input<<"' "<<std::endl;
				system("pause");
				manager_user_menu();
				return false;
			}else{
				std::cout<<"为您查询到以下用户: "<<std::endl;
				for (auto r:this->residents)
				{
					std::cout<<"id: "<<r.ID<<"\t姓名: "<<r.name<<"\t住址: "<<r.address<<"\t房型ID: "<<r.houseTypeID<<"\t预付款: "<<r.prePay<<"\t欠费: "<<r.prePay<<"\t停车位: "<<r.parkingID<<"\t电话号: "<<r.phoneNumber<<std::endl;
				}
				std::cout<<"请输入要删除的用户ID: ";
				std::string secondInput;
				std::cin>>secondInput;
				if(is_exit(secondInput))
				{
					manager_user_menu();
					return false;
				}
				if(is_number(secondInput))
				{
					id = std::stoi(secondInput);
					goto startdelete;
				}else{
					std::cout<<"不合法的id"<<std::endl;
					system("pause");
					manager_user_menu();
					return false;
				}
			}
		}
	return false;
}else{
	std::cout<<"确认要覆盖?y/n: ";
	std::string makeSure;
	std::cin>>makeSure;
	if(makeSure=="y\0")
	{
		char toQuery[128];
		sprintf(toQuery,"DELETE FROM residents WHERE ID = %d;",sig);
		if(mysql_query(data->dat,toQuery)!=0)
		{
			std::string error = mysql_error(data->dat);
			Database::log_error("delete user info error: "+ error);
			std::cout<<"delete user info error"+error<<std::endl;
			return false;
		}
		else{
			return true;
		}
	}else{
			return false;
		}
	}
}


bool Manager::display_all_user()
{
	this->residents = data->get_all_user();
	if(residents.size()==0)
	{
		std::cout<<"未查询到任何用户信息"<<std::endl;
		system("pause");
		manager_user_menu();
		return false;
	}else{
		std::cout<<"为您查询到以下信息："<<std::endl;
		for (auto r:this->residents)
		{
			std::cout<<"--------------------------------------------------------------------------------------------------------------------------------------------\n"<<"id: "<<r.ID<<" \t姓名: "<<r.name<<" \t住址: "<<r.address<<" \t房型ID: "<<r.houseTypeID<<" \t预付款: "<<r.prePay<<" \t欠费: "<<r.debtAmount<<" \t停车位: "<<r.parkingID<<" \t电话号: "<<r.phoneNumber<<std::endl;
		}
		system("pause");
		return true;
	}
	return false;
}
bool Manager::not_double_str(std::string& str)
{
	for(auto ch:str)
	{
		if(ch!='.'&&!std::isdigit(ch))
		{
			return true;
		}
	}
	return false;
}
bool not_double(char ch)
{
	return !std::isdigit(ch)&&ch!='.';
}
bool compare_by_debt(const residentINFO& r1,const residentINFO& r2)
{
	return std::stod(r1.debtAmount)>std::stod(r2.debtAmount);
}
bool Manager::display_all_user_by_debt()
{
	this->residents = data->get_all_user();
	for(auto r:residents)
	{
		r.debtAmount.erase(std::remove_if(r.debtAmount.begin(),r.debtAmount.end(),not_double),r.debtAmount.end());
	}
	std::sort(residents.begin(),residents.end(),compare_by_debt);
	std::cout<<"按欠费降序展示用户信息： "<<std::endl;
	for(auto r:residents)
	{
			std::cout<<"--------------------------------------------------------------------------------------------------------------------------------------------\n"<<"id: "<<r.ID<<" \t姓名: "<<r.name<<" \t住址: "<<r.address<<" \t房型ID: "<<r.houseTypeID<<" \t预付款: "<<r.prePay<<" \t欠费: "<<r.debtAmount<<" \t停车位: "<<r.parkingID<<" \t电话号: "<<r.phoneNumber<<std::endl;
	}
	system("pause");
	manager_user_menu();
	return true;
}
void Manager::manager_user_menu()
{
	system("cls");
	std::cout<<"用户基础信息管理系统"<<std::endl;
	std::cout<<"0.返回上级菜单\n1.新建用户信息\n2.修改用户信息\n3.删除用户信息\n4.展示所有用户信息\n5.按欠费排序展示用户\n";
	std::cout<<"要进行的操作:  ";
	int choice;
    std::string input;
    if (std::cin.peek() == '\n') {
        std::cin.ignore(); 
    }
    getline(std::cin,input);
    if(Manager::is_exit(input))
    {
        manager_main_menu();
        return;
    }
    if(Manager::is_number(input))
    {
        choice = std::stoi(input);
    }else{
        std::cout<<"不明确的选项"<<std::endl;
        system("pause");
        manager_user_menu();
        return;
    }
	switch(choice)
	{
		case 1:
			system("cls");
			this->add_user_info();
			break;
		case 2:
			system("cls");
			this->update_user_info();
			break;
		case 3:
			system("cls");
			this->delete_user_info();
			break;
		case 4:
			system("cls");
			this->display_all_user();
			break;
		case 5:
			system("cls");
			this->display_all_user_by_debt();
			break;
		default:
			manager_main_menu();
			break;
	}
	return;
}

// int Manager::manager_parking_menu()
// {
// 	std::cout<<"0.返回上级菜单\n1.查看所有用户车位信息\n2.修改用户车位信息\n3.删除用户车位信息\n";
// 	std::cout<<"请输入你的操作: ";
// 	std::string input;
// 	if(is_exit(input))
// 	{
// 		manager_user_menu();
// 		return -1;
// 	}
// 	if(!is_number(input))
// 	{
// 		manager_user_menu();
// 		return -1;
// 	}
// 	int choice = std::stoi(input);
// 	switch(choice)
// 	{
// 		case 1:
// 			break;
// 		default:
// 			manager_user_menu();
// 			break;
// 	}
// }
bool Manager::display_unaccepted_repairmsg()
{
	this->repairmsg = data->get_all_unaccepted_repairMSG();
	if(repairmsg.size()==0)
	{
		std::cout<<"当前没有未受理的信息"<<std::endl;
		system("pause");
		manager_repair_menu();
		return false;
	}else{
		std::cout<<"查询到 "<<repairmsg.size()<<" 条未受理的报修信息: "<<std::endl;
		for (auto r:repairmsg)
		{
			std::cout<<"--------------------------------------------------------------------------------------\n"<<r<<std::endl;
		}
		system("pause");
		return true;
	}
}
bool Manager::display_solved_repairmsg()
{
	this->repairmsg = data->get_all_solved_repairMSG();
	if(repairmsg.size()==0)
	{
		std::cout<<"当前没有已解决的报修信息"<<std::endl;
		system("pause");
		manager_repair_menu();
		return false;
	}else{
		std::cout<<"查询到 "<<repairmsg.size()<<" 条已解决的报修信息: "<<std::endl;
		for (auto r:repairmsg)
		{
			std::cout<<"--------------------------------------------------------------------------------------\n"<<r<<std::endl;
		}
		system("pause");
		return true;
	}
}
bool Manager::delete_solved_repairmsg()
{
	display_solved_repairmsg();
	std::cout<<"请选择要删除的报修id: ";
	std::string input;
	std::cin>>input;
	int choice;
	if(is_exit(input))
	{
		manager_repair_menu();
		return false;
	}
	if(!is_number(input))
	{
		std::cout<<"请输入正确的报修信息id"<<std::endl;
		system("pause");
		manager_repair_menu();
		return false;
	}
	choice = std::stoi(input);
	if(data->repair_info_existed(choice))
	{
		std::cout<<"确认要删除吗? y/n: ";
		std::string makeSure;
		std::cin>>makeSure;
		if(makeSure!="y\0")
		{
			manager_repair_menu();
			return false;
		}
		choice = std::stoi(input);
		char toQuery[128];
		sprintf(toQuery,"DELETE FROM repairmsg WHERE repairID = %d;",choice);
		if (mysql_query(data->dat,toQuery)!=0)
		{
			std::string error = mysql_error(data->dat);
			std::cout<<"删除已完成报修信息错误: "<<error<<std::endl;
			Database::log_error("delete solved repairmsg error: "+ error);
			system("pause");
			manager_repair_menu();
			return false;
		}else{
			std::cout<<"该报修信息已被删除"<<std::endl;
			system("pause");
			manager_repair_menu();
			return true;
		}
	}else{
		std::cout<<"该报修信息不存在"<<std::endl;
		system("pause");
		manager_repair_menu();
		return false;
	}
}
bool Manager::target_unaccepted_repairmsg()
{
	display_unaccepted_repairmsg();
	std::cout<<"请选择要标记的报修信息id: ";
	std::string input;
	std::cin>>input;
	int choice;
	if(is_exit(input))
	{
		manager_repair_menu();
		return false;
	}
	if(!is_number(input))
	{
		std::cout<<"请输入正确的报修信息id"<<std::endl;
		system("pause");
		manager_repair_menu();
		return false;
	}
	choice = std::stoi(input);
	if (!data->repair_info_existed(choice))
	{
		std::cout<<"报修id不存在"<<std::endl;
		system("pause");
		return false;
	}
	std::cout<<"确认要标记吗? y/n: ";
	std::string makeSure;
	std::cin>>makeSure;
	if(makeSure!="y\0")
	{
		manager_repair_menu();
		return false;
	}
	char toQuery[128];
	sprintf(toQuery,"UPDATE repairmsg SET status = 'solved' WHERE repairID = %d;",choice);
	if (mysql_query(data->dat,toQuery)!=0)
	{
		std::string error = mysql_error(data->dat);
		std::cout<<"标记报修信息错误: "<<error<<std::endl;
		Database::log_error("target repairmsg error: "+ error);
		system("pause");
		manager_repair_menu();
		return false;
	}else{
		std::cout<<"标记成功"<<std::endl;
		system("pause");
		return true;
	}
	return false;
}
bool Manager::display_unaccepted_complaintmsg()
{
	this->complaintmsg = data->get_all_unaccepted_complaintMSG();
	if(complaintmsg.size()==0)
	{
		std::cout<<"当前没有未受理的信息"<<std::endl;
		system("pause");
		manager_complaint_menu();
		return false;
	}else{
		std::cout<<"查询到 "<<complaintmsg.size()<<" 条未受理的投诉信息: "<<std::endl;
		for (auto r:complaintmsg)
		{
			std::cout<<"--------------------------------------------------------------------------------------\n"<<r<<std::endl;
		}
		system("pause");
		return true;
	}
}
bool Manager::display_solved_complaintmsg()
{
	this->complaintmsg = data->get_all_solved_complaintMSG();
	if(complaintmsg.size()==0)
	{
		std::cout<<"当前没有已解决的报修信息"<<std::endl;
		system("pause");
		return false;
	}else{
		std::cout<<"查询到 "<<complaintmsg.size()<<" 条已解决的投诉信息: "<<std::endl;
		for (auto r:complaintmsg)
		{
			std::cout<<"--------------------------------------------------------------------------------------\n"<<r<<std::endl;
		}
		system("pause");
		return true;
	}
}
void Manager::manager_complaint_menu(){
	system("cls");
	std::cout<<"0.返回上级菜单\n1.查看未受理的投诉信息\n2.查看已解决的投诉信息\n3.删除已解决的投诉信息\n4.标记已解决投诉信息"<<std::endl;
	std::cout<<"请输入你的操作: ";
	int choice;
    std::string input;
    if (std::cin.peek() == '\n') {
        std::cin.ignore(); 
    }
    getline(std::cin,input);
    if(Manager::is_exit(input))
    {
        manager_user_menu();
        return;
    }
    if(Manager::is_number(input))
    {
        choice = std::stoi(input);
    }else{
        std::cout<<"不明确的选项"<<std::endl;
        system("pause");
        manager_complaint_menu();
        return;
    }
	switch(choice)
	{
		case 1:
			system("cls");
			display_unaccepted_complaintmsg();
			break;
		case 2:
			system("cls");
			display_solved_complaintmsg();
			break;
		case 3:
			system("cls");
			delete_solved_complaintmsg();
			break;
		case 4:
			system("cls");
			target_unaccepted_complaintmsg();
		default:
			manager_main_menu();
			break;
	}
	return;
}
bool Manager::target_unaccepted_complaintmsg()
{
	display_unaccepted_complaintmsg();
	std::cout<<"请选择要标记的投诉信息id: ";
	std::string input;
	std::cin>>input;
	int choice;
	if(is_exit(input))
	{
		manager_complaint_menu();
		return false;
	}
	if(!is_number(input))
	{
		std::cout<<"请输入正确的投诉信息id"<<std::endl;
		system("pause");
		manager_complaint_menu();
		return false;
	}
	choice = std::stoi(input);
	if (!data->complaint_info_existed(choice))
	{
		std::cout<<"投诉id不存在"<<std::endl;
		system("pause");
		return false;
	}
	std::cout<<"确认要标记吗? y/n: ";
	std::string makeSure;
	std::cin>>makeSure;
	if(makeSure!="y\0")
	{
		manager_complaint_menu();
		return false;
	}
	char toQuery[128];
	sprintf(toQuery,"UPDATE complaintmsg SET status = 'solved' WHERE complaintID = %d;",choice);
	if (mysql_query(data->dat,toQuery)!=0)
	{
		std::string error = mysql_error(data->dat);
		std::cout<<"标记投诉信息错误: "<<error<<std::endl;
		Database::log_error("target complaintmsg error: "+ error);
		system("pause");
		manager_complaint_menu();
		return false;
	}else{
		std::cout<<"标记成功"<<std::endl;
		system("pause");
		return true;
	}
	return false;
}
bool Manager::delete_solved_complaintmsg()
{
	display_solved_complaintmsg();
	std::cout<<"请选择要删除的投诉id: ";
	std::string input;
	std::cin>>input;
	int choice;
	if(is_exit(input))
	{
		manager_complaint_menu();
		return false;
	}
	if(!is_number(input))
	{
		std::cout<<"请输入正确的投诉信息id"<<std::endl;
		system("pause");
		manager_complaint_menu();
		return false;
	}
	choice = std::stoi(input);
	if(data->complaint_info_existed(choice))
	{
		std::cout<<"确认要删除吗? y/n: ";
		std::string makeSure;
		std::cin>>makeSure;
		if(makeSure!="y\0")
		{
			manager_complaint_menu();
			return false;
		}
		choice = std::stoi(input);
		char toQuery[128];
		sprintf(toQuery,"DELETE FROM complaintmsg WHERE complaintID = %d;",choice);
		if (mysql_query(data->dat,toQuery)!=0)
		{
			std::string error = mysql_error(data->dat);
			std::cout<<"删除已完成投诉信息错误: "<<error<<std::endl;
			Database::log_error("delete solved complaintmsg error: "+ error);
			system("pause");
			manager_complaint_menu();
			return false;
		}else{
			std::cout<<"该报修信息已被删除"<<std::endl;
			system("pause");
			manager_complaint_menu();
			return true;
		}
	}else{
		std::cout<<"该投诉信息不存在"<<std::endl;
		system("pause");
		manager_complaint_menu();
		return false;
	}
}
bool Manager::display_all_community()
{
	this->community = data->get_all_community_info();
	if(community.size()==0)
	{
		std::cout<<"未查询到小区信息"<<std::endl;
		system("pause");
		manager_community_menu();
		return false;
	}
	std::cout<<"查询到' "<<community.size()<<" '条小区信息: "<<std::endl;
	for(auto cm:community)
	{
		std::cout<<"-----------------------------------------------------------------\n"<<"小区ID: "<<cm.communityID<<" \t小区名称: "<<cm.communityName<<" \t楼宇数:  "<<cm.buildingAmounts<<std::endl;
	}
	system("pause");
	return true;
}
bool Manager::display_all_house()
{
	this->house = data->get_all_house_info();
	if(house.size()==0)
	{
		std::cout<<"未查询到房型信息"<<std::endl;
		system("pause");
		manager_house_menu();
		return false;
	}
	std::cout<<"查询到' "<<house.size()<<" '条房型信息: "<<std::endl;
	for(auto hs:house)
	{
		std::cout<<"-----------------------------------------------------------------\n"<<"房型ID: "<<hs.houseTypeID<<" \t房型: "<<hs.type<<" \t面积: "<<hs.area<<std::endl;
	}
	system("pause");
	return true;
}
bool Manager::delete_house_info()
{
	system("cls");
	display_all_house();
	std::cout<<"请输入要删除的房型ID: ";
	std::string input;
	std::cin>>input;
	if(is_exit(input))
	{
		manager_house_menu();
		return false;
	}
	if(!is_number(input))
	{
		std::cout<<"不合法的房型ID"<<std::endl;
		system("pause");
		manager_house_menu();
		return false;
	}
	int choice = std::stoi(input);
	if(!house_is_exist(choice))
	{
		std::cout<<"不存在的房型ID,请检查后重试"<<std::endl;
		system("pause");
		manager_house_menu();
		return false;
	}
	std::cout<<"确认要删除吗? y/n: ";
	std::string makeSure;
	std::cin>>makeSure;
	if(makeSure!="y\0")
	{
		manager_house_menu();
		return false;
	}
	char toQuery[128];
	sprintf(toQuery,"DELETE FROM houseinfo WHERE houseTypeID = %d;",choice);
	if(mysql_query(data->dat,toQuery)!=0)
	{
		std::string error = mysql_error(data->dat);
		std::cout<<"删除房型信息时出错: "+error<<std::endl;
		Database::log_error("delet house info error: "+error);
		manager_house_menu();
		return false;
	}
	std::cout<<"删除成功"<<std::endl;
	system("pause");
	manager_house_menu();
	return true;
}
void Manager::manager_house_menu()
{
	system("cls");
	std::cout<<"0.返回上级菜单\n1.查看房型信息\n2.新增房型信息\n3.修改房型信息\n4.删除房型信息\n请输入操作: ";
	int choice;
    std::string input;
    if (std::cin.peek() == '\n') {
        std::cin.ignore(); 
    }
    getline(std::cin,input);
    if(Manager::is_exit(input))
    {
        manager_main_menu();
        return;
    }
    if(Manager::is_number(input))
    {
        choice = std::stoi(input);
    }else{
        std::cout<<"不明确的选项"<<std::endl;
        system("pause");
        manager_house_menu();
        return;
    }
	switch(choice)
	{
		case 1:
			system("cls");
			this->display_all_house();
			break;
		case 2:
			this->add_house_info();
			break;
		case 3:
			this->update_house_info();
			break;
		case 4:
			this->delete_house_info();
			break;
		default:
			this->manager_main_menu();
			break;
	}
}
bool Manager::update_house_info()
{
	system("cls");
	display_all_house();
	std::cout<<"请输入要修改的房型ID: ";
	std::string inputID;
	std::cin>>inputID;
	if(is_exit(inputID))
	{
		manager_house_menu();
		return false;
	}
	if(!is_number(inputID))
	{
		std::cout<<"不合法的房型ID"<<std::endl;
		system("pause");
		manager_house_menu();
		return false;
	}
	int choice = std::stoi(inputID);
	if(!house_is_exist(choice))
	{
		std::cout<<"不存在的房型ID,请检查后重试"<<std::endl;
		system("pause");
		manager_house_menu();
		return false;
	}
	int toUpdate;
	std::string input;
	std::cout<<"请输入要修改的信息: \n0.返回上级菜单\n1.房型ID\n2.房型名\n3.房型面积\nchoice: ";
	std::cin>>input;
	if(is_exit(input))
	{
		manager_house_menu();
		return false;
	}
	if(!is_number(input))
	{
		std::cout<<"未知输入"<<std::endl;
		system("pause");
		manager_house_menu();
		return false;
	}
	toUpdate = std::stoi(input);
	std::string secondInput;
	char toQuery[128];
	int newID;
	switch(toUpdate)
	{
		case 1:
			std::cout<<"请输入修改后的房型ID: ";
			std::cin>>secondInput;
			if(is_exit(secondInput))
			{
				manager_house_menu();
				return false;
			}
			if(!is_number(secondInput))
			{
				std::cout<<"不合法的房型ID"<<std::endl;
				system("pause");
				manager_house_menu();
				return false;
			}
			newID = std::stoi(secondInput);
			if(house_is_exist(newID))
			{
				std::cout<<"房型ID已存在,请检查后重试"<<std::endl;
				system("pause");
				manager_house_menu();
				return false;
			}
			sprintf(toQuery,"UPDATE houseinfo SET houseTypeID = %d WHERE houseTypeID = %d;",newID,choice);
			if(mysql_query(data->dat,toQuery)!=0)
			{
				std::string error = mysql_error(data->dat);
				std::cout<<"更新房型ID失败: "+error<<std::endl;
				Database::log_error("update house id error: "+error);
				manager_house_menu();
				return false;
			}else{
				std::cout<<"修改成功"<<std::endl;
				system("pause");
				return true;
			}
			break;
		case 2:
			std::cout<<"请输入修改后的房型类名: ";
			std::cin>>secondInput;
			sprintf(toQuery,"UPDATE houseinfo SET houseType = '%s'WHERE houseTypeID = %d;",secondInput.c_str(),choice);
			if(mysql_query(data->dat,toQuery)!=0)
			{
				std::string error = mysql_error(data->dat);
				std::cout<<"更新房型类名失败: "+error<<std::endl;
				Database::log_error("update house name error: "+error);
				manager_house_menu();
				return false;
			}else{
				std::cout<<"修改成功"<<std::endl;
				system("pause");
				return true;
			}
			break;
		case 3:
			std::cout<<"请输入修改后的房型面积: ";
			std::cin>>secondInput;
			if(not_double_str(secondInput))
			{
				std::cout<<"请输入正确的面积"<<std::endl;
				system("pause");
				manager_house_menu();
				return false;
			}
			secondInput+="m^2";
			sprintf(toQuery,"UPDATE houseinfo SET area = '%s'WHERE houseTypeID = %d;",secondInput.c_str(),choice);
			if(mysql_query(data->dat,toQuery)!=0)
			{
				std::string error = mysql_error(data->dat);
				std::cout<<"更新房型面积失败: "+error<<std::endl;
				Database::log_error("update house area error: "+error);
				manager_house_menu();
				return false;
			}else{
				std::cout<<"修改成功"<<std::endl;
				system("pause");
				return true;
			}
			break;
		default:
			manager_main_menu();
			break;
	}
	return false;
}
bool Manager::display_all_charge()
{
	this->charges = data->get_all_charge_info();
	if(charges.size()==0)
	{
		std::cout<<"未查询到收费信息"<<std::endl;
		system("pause");
		manager_charge_menu();
		return false;
	}
	std::cout<<"查询到' "<<charges.size()<<" '条收费信息: "<<std::endl;
	for(auto ch:charges)
	{
		std::cout<<"-----------------------------------------------------------------\n"<<"收费ID: "<<ch.chargeID<<" \t收费项目: "<<ch.chargeName<<" \t收费金额:  "<<ch.toPay<<" \t收费周期: "<<ch.cycle<<" \t收费属性: "<<ch.status<<" \t已收费次数: "<<ch.now_charge_times<<" \t上次收费日期: "<<ch.last_charge_date<<" \t收费房型ID: "<<ch.houseTypeID<<std::endl;
	}
	system("pause");
	return true;
}
bool Manager::add_charge_info()
{
	system("cls");
	std::string input;
	std::cout<<"请输入收费项目名称: "<<std::endl;
	std::cin>>input;
	if(is_exit(input))
	{
		manager_charge_menu();
		return false;
	}
	std::string chargeName = input;
	std::cout<<"请输入收费金额: ";
	std::cin>>input;
	if(is_exit(input))
	{
		manager_charge_menu();
		return false;
	}
	if(not_double_str(input))
	{
		std::cout<<"请输入正确的收费金额"<<std::endl;
		manager_charge_menu();
		return false;
	}
	std::string toPay = input+"元";
	std::cout<<"请输入收费周期,按月输入,只输入数字即可: ";
	std::cin>>input;
	if(is_exit(input))
	{
		manager_charge_menu();
		return false;
	}
	if(!is_number(input))
	{
		std::cout<<"无法识别的月数"<<std::endl;
		system("pause");
		manager_charge_menu();
	}
	std::string cycle = "每"+input+"个月";
	std::cout<<"请输入收费期数(目标收费次数,长期收费输入0即可,反正输入对应期数): ";
	std::cin>>input;
	if(is_exit(input))
	{
		manager_charge_menu();
		return false;
	}
	if(!is_number(input))
	{
		std::cout<<"无法识别的次数"<<std::endl;
		system("pause");
		manager_charge_menu();
		return false;
	}
	std::string status = "长期收费";
	if(input!="0\0")
	{
		status = "收费"+input+"次";
	}
	int houseTypeID;
	std::cout<<"请输入对应的房型ID: ";
	std::cin>>input;
	if(is_exit(input))
	{
		manager_charge_menu();
		return false;
	}
	if(!is_number(input))
	{
		std::cout<<"无法识别的房型ID"<<std::endl;
		system("pause");
		manager_charge_menu();
		return false;
	}
	houseTypeID = std::stoi(input);
	if(!house_is_exist(houseTypeID))
	{
		std::cout<<"不存在的房型,请检查后重试"<<std::endl;
		system("pause");
		manager_charge_menu();
		return false;
	}
	std::cout<<"是否要自定上次收费时间？(默认为今天）y/n：  ";
	std::cin>>input;
	if(is_exit(input))
	{
		manager_charge_menu();
		return false;
	}
	char toQuery[256];
	if(input=="y\0")
	{
		std::cout<<"请按xxxx-xx-xx的格式输入年月日: ";
		std::cin>>input;
		std::string last_charge_date = input +" 00:00:00";
		std::cout<<"是否自定已收费次数?(默认为0),y/n?: ";
		std::cin>>input;
		int now_charge_times;
		if(input=="y")
		{
			std::cout<<"请输入已收费的次数: ";
			std::cin>>input;
			if(!is_number(input))
			{
				std::cout<<"无法识别的收费次数";
				manager_charge_menu();
				return false;
			}
			now_charge_times = std::stoi(input);
			sprintf(toQuery,"INSERT INTO chargeinfo(chargeName,toPay,cycle,status,houseTypeID,last_charge_date,now_charge_times) VALUES('%s','%s','%s','%s',%d,'%s',%d);",chargeName.c_str(),toPay.c_str(),cycle.c_str(),status.c_str(),houseTypeID,last_charge_date.c_str(),now_charge_times);

		}else{
			sprintf(toQuery,"INSERT INTO chargeinfo(chargeName,toPay,cycle,status,houseTypeID,last_charge_date) VALUES('%s','%s','%s','%s',%d,'%s');",chargeName.c_str(),toPay.c_str(),cycle.c_str(),status.c_str(),houseTypeID,last_charge_date.c_str());
		}
	}else{
			sprintf(toQuery,"INSERT INTO chargeinfo(chargeName,toPay,cycle,status,houseTypeID) VALUES('%s','%s','%s','%s',%d);",chargeName.c_str(),toPay.c_str(),cycle.c_str(),status.c_str(),houseTypeID);
	}
	if (mysql_query(data->dat,toQuery)!=0)
	{
		std::string error = mysql_error(data->dat);
		std::cout<<"插入收费信息失败: "+error<<std::endl;
		Database::log_error("insert charge info error: "+error);
		system("pause");
		manager_charge_menu();
		return false;
	}else{
		std::cout<<"插入成功"<<std::endl;
		system("pause");
		return true;
	}
	return false;
}
bool Manager::synchronize_charge_info()
{
	system("cls");
	this->charges = data->get_all_charge_info();
	if(charges.size()==0)
	{
		std::cout<<"没有要同步的收费信息"<<std::endl;
		system("pause"); 
		return false;
	}
	this->residents = data->get_all_user();
	for (auto ch: charges)
	{
		int target;
		std::string ts = ch.status;
		if(ts.size()==10)
		{
			target = ts[6]-'0';
		}else{
			if(ts.size()==11)
			{
				target = (ts[6]-'0')*10+(ts[7]-'0');
			}
		}
		if(ch.now_charge_times<target)
		{
			int duration;
			std::string now = Logs::get_now_time();
			duration = (std::stoi(ch.last_charge_date.substr(0,4))-std::stoi(now.substr(0,4)))*12+std::stoi(ch.last_charge_date.substr(5,2))-std::stoi(now.substr(5,2));
			duration = abs(duration);
			if (duration>0)
			{
				if(ch.status!="长期收费\0")
				{
					if(duration>target)
					{
						duration = target;
					}
				}
				std::vector<residentINFO> temp;
				for(auto ri:residents)
				{
					if(ri.houseTypeID==ch.houseTypeID)
					{
						std::string tempAmount = std::to_string(std::stod(ri.debtAmount.substr(0,ri.debtAmount.size()-1)) + std::stod(ch.toPay.substr(0,ch.toPay.size()-1))*duration);
						ri.debtAmount = tempAmount.substr(0,tempAmount.size()-5)+"元";
						char riQuery[128];
						sprintf(riQuery,"UPDATE residents SET debtAmount = '%s' WHERE ID = %d;",ri.debtAmount.c_str(),ri.ID);
						if(mysql_query(data->dat,riQuery)!=0)
						{
							std::string error = mysql_error(data->dat);
							std::cout<<"同步住户欠费出错: "+error<<std::endl;
							system("pause");
							manager_charge_menu();
							return false;
						}
						temp.push_back(ri);
					}
				}
				ch.now_charge_times+=duration;
				std::cout<<"该项目已收费: "<<ch.chargeName<<"收费ID: "<<ch.chargeID<<"已收费次数: "<<ch.now_charge_times<<std::endl;
				for (auto ri:temp)
				{
					std::cout<<"收费用户: "<<ri.name<<"用户ID: "<<ri.ID<<std::endl;
				}
			}
			char chQuery[128];
			sprintf(chQuery,"UPDATE chargeinfo SET now_charge_times = %d,last_charge_date = '%s' WHERE chargeID = %d;",ch.now_charge_times,now.c_str(),ch.chargeID);
			if(mysql_query(data->dat,chQuery)!=0)
			{
				std::string error = mysql_error(data->dat);
				std::cout<<"同步收费出错: "+error<<std::endl;
				system("pause");
				manager_charge_menu();
				return false;
			}
		}
	}
	std::cout<<"同步成功"<<std::endl;
	system("pause");
	manager_charge_menu();
	return true;
}
void Manager::manager_charge_menu()
{
	system("cls");
	std::cout<<"0.返回上级菜单\n1.查看收费信息\n2.新增收费信息\n3.修改收费信息\n4.删除收费信息\n5.同步收费信息\n请输入操作: ";
	int choice;
    std::string input;
    if (std::cin.peek() == '\n') {
        std::cin.ignore(); 
    }
    getline(std::cin,input);
    if(Manager::is_exit(input))
    {
        manager_main_menu();
        return;
    }
    if(Manager::is_number(input))
    {
        choice = std::stoi(input);
    }else{
        std::cout<<"不明确的选项"<<std::endl;
        system("pause");
        manager_charge_menu();
        return;
    }
	switch (choice)
	{
		case 1:
			system("cls");
			display_all_charge();
			break;
		case 2:
			add_charge_info();
			break;
		case 3:
			update_charge_info();
			break;
		case 4:
			delete_charge_info();
			break;
		case 5:
			synchronize_charge_info();
			break;
		default:
			manager_main_menu();
			break;
	}
}
bool Manager::add_house_info()
{
	system("cls");
	std::cout<<"已为您分配房型ID,是否引用?y/n: ";
	std::string use;
	std::cin>>use;
	if(is_exit(use))
	{
		manager_house_menu();
		return false;
	}
	int ID = -1;
	std::string type;
	std::string area;
	if(use!="y\0")
	{
		std::cout<<"请输入自定ID: ";
		std::string inputID;
		std::cin>>inputID;
		if(is_exit(inputID))
		{
			manager_house_menu();
			return false;
		}
		if(!is_number(inputID))
		{
			std::cout<<"不合法的房型id"<<std::endl;
			system("pause");
			manager_house_menu();
			return false;
		}
		ID = std::stoi(inputID);
		if(house_is_exist(ID))
		{
			std::cout<<"房型ID已存在,请检查后重试"<<std::endl;
			system("pause");
			manager_house_menu();
			return false;
		}
	}
	std::cout<<"请输入房型类名: ";
	std::string inputHouse;
	std::cin>>inputHouse;
	if(is_exit(inputHouse))
	{
		manager_house_menu();
		return false;
	}
	type = inputHouse;
	std::cout<<"请输入房型面积: ";
	std::string inputArea;
	std::cin>>inputArea;
	if(is_exit(inputArea))
	{
		manager_house_menu();
		return false;
	}
	if(not_double_str(inputArea))
	{
		std::cout<<"请输入正确的面积"<<std::endl;
		system("pause");
		manager_house_menu();
		return false;
	}
	area = inputArea;
	area+="m^2";
	char toQuery[128];
	if(ID==-1)
	{
		sprintf(toQuery,"INSERT INTO houseinfo (houseType,area) VALUES('%s','%s');",type.c_str(),area.c_str());
	}else{
		sprintf(toQuery,"INSERT INTO houseinfo (houseTypeID,houseType,area) VALUES(%d,'%s','%s');",ID,type.c_str(),area.c_str());
	}
	if (mysql_query(data->dat,toQuery)!=0)
		{
			std::string error = mysql_error(data->dat);
			Database::log_error("insert house info error: "+ error);
			std::cout<<"insert house info error: "+ error;
			system("pause");
			manager_house_menu();
			return false;
			
		}else{
			std::cout<<"新建房型完成"<<std::endl;
			system("pause");
			return true;
		}
	return false;
}
bool Manager::charge_is_exist(int id)
{
	char toQuery[128];
	sprintf(toQuery,"SELECT * FROM chargeinfo WHERE chargeID = %d;",id);
	if(mysql_query(data->dat,toQuery)!=0)
	{
		std::string error = mysql_error(data->dat);
		std::cout<<"检测收费信息时出错: "+error;
		Database::log_error("check charge exist error: "+error);
		return false;
		system("pause");
	}else{
		MYSQL_RES* res = mysql_store_result(data->dat);
		MYSQL_ROW row = mysql_fetch_row(res);
		mysql_free_result(res);
		return row!=NULL;
	}
	return false;
}
bool Manager::delete_charge_info()
{
	system("cls");
	display_all_charge();
	std::cout<<"请输入要删除的收费ID: ";
	std::string input;
	std::cin>>input;
	if(is_exit(input))
	{
		manager_charge_menu();
		return false;
	}
	if(!is_number(input))
	{
		std::cout<<"不合法的收费ID"<<std::endl;
		system("pause");
		manager_charge_menu();
		return false;
	}
	int choice = std::stoi(input);
	if(!charge_is_exist(choice))
	{
		std::cout<<"不存在的收费ID,请检查后重试"<<std::endl;
		system("pause");
		manager_charge_menu();
		return false;
	}
	std::cout<<"确认要删除吗? y/n: ";
	std::string makeSure;
	std::cin>>makeSure;
	if(makeSure!="y\0")
	{
		manager_charge_menu();
		return false;
	}
	char toQuery[128];
	sprintf(toQuery,"DELETE FROM chargeinfo WHERE chargeID = %d;",choice);
	if(mysql_query(data->dat,toQuery)!=0)
	{
		std::string error = mysql_error(data->dat);
		std::cout<<"删除收费信息时出错: "+error<<std::endl;
		Database::log_error("delet chargeyinfo error: "+error);
		manager_charge_menu();
		return false;
	}
	std::cout<<"删除成功"<<std::endl;
	system("pause");
	manager_charge_menu();
	return true;
}
void Manager::manager_repair_menu()
{
	system("cls");
	std::cout<<"0.返回上级菜单\n1.查看未受理的报修信息\n2.查看已解决的报修信息\n3.删除已解决的报修信息\n4.标记已解决报修信息"<<std::endl;
	std::cout<<"请输入你的操作: ";
	int choice;
    std::string input;
    if (std::cin.peek() == '\n') {
        std::cin.ignore(); 
    }
    getline(std::cin,input);
    if(Manager::is_exit(input))
    {
       manager_user_menu();
        return;
    }
    if(Manager::is_number(input))
    {
        choice = std::stoi(input);
    }else{
        std::cout<<"不明确的选项"<<std::endl;
        system("pause");
        manager_repair_menu();
        return;
    }
	switch(choice)
	{
		case 1:
			system("cls");
			display_unaccepted_repairmsg();
			break;
		case 2:
			system("cls");
			display_solved_repairmsg();
			break;
		case 3:
			system("cls");
			delete_solved_repairmsg();
			break;
		case 4:
			system("cls");
			target_unaccepted_repairmsg();
		default:
			manager_main_menu();
			break;
	}
	return;
}
bool Manager::parking_is_exist(std::string parkingID)
{
	char checkIsExist[128];
	sprintf(checkIsExist,"SELECT * FROM residents WHERE parkingID = '%s';",parkingID.c_str());
	if (mysql_query(data->dat,checkIsExist)!=0)
	{
		std::string error = mysql_error(data->dat);
		Database::log_error("check parkingID exist error: "+error);
		system("pause");
		return false;
	}
	else{
		MYSQL_RES* res = mysql_store_result(data->dat);
		if (res==nullptr)
		{
			std::string error = mysql_error(data->dat);
			Database::log_error("check parkingIDexist error: "+error);
			return false;
		}else{
			MYSQL_ROW row = mysql_fetch_row(res);
			mysql_free_result(res);
			return row!=NULL;
		}
		}
		return false;
}