#include "GlobalHeader.h"

vector<string> tokenize(string, string);
vector<TrainSched> readSched(string);
double** fillCostMatrix(vector<TrainSched>, int, map<int, int>);
map<int, int> buildAssociateArray(vector<TrainSched>);
void makebase(double**, int, int, int);
void algorythmLittle(double**, int);
int getKeyFromValue(map<int, int>, int);
bool equals(TrainSched, TrainSched);
void findOptimalPath(double**, double**, vector<TrainSched>, map<int, int>, int);
void printSchedule(vector<TrainSched>);
void printMatrix(double**, int);
void printAssociateArray(map<int, int>);