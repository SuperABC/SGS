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
	const char *message() {
		switch (level) {
		case MT_INFO:
			return ("Info: " + msg).data();
		case MT_WARNING:
			return ("Warning: " + msg).data();
		case MT_ERROR:
			return ("Error: " + msg).data();
		default:
			return msg.data();
		}
	}
};