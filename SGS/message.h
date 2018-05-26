#pragma once
#include <string>

enum SGSMSGLEVEL {
	MT_INFO,
	MT_WARNING,
	MT_ERROR
};

class sgsMsg {
private:
	SGSMSGLEVEL level;
	std::string msg;

public:
	sgsMsg(std::string s, SGSMSGLEVEL l) {
		msg = s;
		level = l;
	}

	std::string getMsg() {
		switch (level) {
		case MT_INFO:
			return "Info: " + msg;
		case MT_WARNING:
			return "Warning: " + msg;
		case MT_ERROR:
			return "Error: " + msg;
		default:
			return msg;
		}
	}
	SGSMSGLEVEL getLevel() { return level; }
};