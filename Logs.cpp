#include <thread>
#include <sstream>
#include <iomanip>
#include <mutex>
#include <condition_variable>
#include "Logs.h"

bool Logs::isCreated = false;
Logs* Logs::create_Logs()
{
	if (Logs::isCreated==false)
	{
		Logs* log = new Logs();
		Logs::isCreated = true;
		return log;
	}
	return nullptr;
}

Logs::Logs()
{
	this->log.open("F:\\vscode\\cpp\\cxsj\\logs.txt", std::ios::app);
	this->logPut = std::thread(&Logs::write_logs, this);
}
Logs::~Logs()
{
	this->log.close();
}
std::string Logs::get_now_time()
{
	auto now = std::chrono::system_clock::now();
	auto timeT = std::chrono::system_clock::to_time_t(now);
	std::stringstream ss;
	ss << std::put_time(std::localtime(&timeT), "%Y-%m-%d %X");
	return ss.str();
}
void Logs::write_logs()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(mtx);
		getMSG.wait(lock, [this] { return !logMSG.empty(); });
		if (!logMSG.empty()) {
			this->log << "[" << this->get_now_time() << "]:  " << logMSG.front() << std::endl;
			logMSG.pop_front();
		}
	}
}
