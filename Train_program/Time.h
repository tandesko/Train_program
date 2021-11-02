#ifndef Time_H
#define Time_H

using namespace std;

class Time {
public:
	Time();
	Time(int, int, int);
	string toString();
	int inSeconds();
private:
	int secs;
	int hours;
	int mins;
};

#endif


