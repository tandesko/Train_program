#include "GlobalHeader.h"

TrainSched::TrainSched(int n, int s, int d, double c, Time st, Time et) {
	this->number = n;
	this->startPoint = s;
	this->destPoint = d;
	this->cost = c;
	this->starttime = st;
	this->endtime = et;
}

string TrainSched::toString() {
	ostringstream strout;
	strout << this->number << " " << this->startPoint << " " << this->destPoint << " " << this->cost << " " << this->starttime.toString() << " " << this->endtime.toString();
	return strout.str();
}

int TrainSched::duration() {
	return abs(endtime.inSeconds() - starttime.inSeconds());
}

int TrainSched::getStartPoint() {
	return this->startPoint;
}

int TrainSched::getDestPoint() {
	return this->destPoint;
}

int TrainSched::getNumber() {
	return this->number;
}

double TrainSched::getCost() {
	return this->cost;
}