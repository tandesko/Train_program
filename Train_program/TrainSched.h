#ifndef TrainSched_H
#define TrainSched_H
#include "GlobalHeader.h"

class Time;

class TrainSched {
public:
	TrainSched(int, int, int, double, Time, Time);
	string toString();
	int duration();
	int getStartPoint();
	int getDestPoint();
	int getNumber();
	double getCost();
private:
	int number;
	int startPoint;
	int destPoint;
	double cost;
	Time starttime;
	Time endtime;
};

#endif