#pragma once
#include <string>
#include <deque>
#include <thread>
#include <mutex>
#include <fstream>
#include <condition_variable>
class Logs
{
public:
	~Logs();
	Logs(const Logs& logs) = delete;
	Logs& operator=(const Logs& logs) = delete;
	static std::string get_now_time();
	static Logs* create_Logs();
	void write_logs();
	std::deque<std::string> logMSG;
	std::mutex mtx;
	std::condition_variable getMSG;
private:
	Logs();
	std::ofstream log;
	std::thread logPut;
	static bool isCreated;
};

