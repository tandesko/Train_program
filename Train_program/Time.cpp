#include "GlobalHeader.h"

Time::Time() {
	this->secs = 0;
	this->mins = 0; 
	this->hours = 0;
}
Time::Time(int h, int m, int s) {
	this->secs = s;
	this->mins = m;
	this->hours = h;
}

string Time::toString() {
	ostringstream strout;
	strout << std::setfill('0') << std::setw(2) << this->hours << ":" 
		<< std::setfill('0') << std::setw(2) << this->mins << ":" 
		<< std::setfill('0') << std::setw(2) << this->secs;
	return strout.str();
}

int Time::inSeconds() {
	return (this->hours * 3600 + this->mins * 60 + this->secs);
}