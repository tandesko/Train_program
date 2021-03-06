#include "GlobalHeader.h"

/*
* Function for splitting a string by delimiters. Simply split
* a string by any delimeter (";" - by default).
*/
vector<string> tokenize(string s, string del = ";") {
	vector<string> attributes;
	int start = 0;
	int end = s.find(del);
	while (end != -1) {
		attributes.push_back(s.substr(start, end - start));
		start = end + del.size();
		end = s.find(del, start);
	}
	attributes.push_back(s.substr(start, end - start));
	return attributes;
}


/*
* Function for reading a schedule from CSV-file. We write a schedule in
* a vector, that consists of objects of TrainSched class, which 
* contains all fields needed to operate with.
*/
vector<TrainSched> readSched(string path) {
	vector<TrainSched> sched;
	ifstream file(path);
	if (!file.is_open())
		throw runtime_error("Could not open file!");
	else {
		string line;
		while (getline(file, line)) {
			vector<string> attributes = tokenize(line);
			if (attributes.size() == 6) {
				vector<string> timest = tokenize(attributes[attributes.size() - 2], ":"); //dividing into time measurement units 
				vector<string> timeen = tokenize(attributes[attributes.size() - 1], ":");
				TrainSched tr = TrainSched(
					stoi(attributes[0]), //number of a train
					stoi(attributes[1]), //starting point
					stoi(attributes[2]), //ending point
					stod(attributes[3]), //cost of a trip
					Time(stoi(timest[0]), stoi(timest[1]), stoi(timest[2])), //departure time
					Time(stoi(timeen[0]), stoi(timeen[1]), stoi(timeen[2])) //arrival time
				);
				sched.push_back(tr);
			}
		}
		return sched;
	}
}

/*
* Function for filling in matrix with costs. We fill in 2D-array with minimal costs, 
* taken from the schedule.
*/
double** fillCostMatrix(vector<TrainSched> trains, int size, map<int, int> assarr) {
	double** a = new double* [size];
	for (int i = 0; i < size; i++)
		a[i] = new double[size];
	double min_cost;
	bool flag;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			min_cost = trains[0].getCost();
			flag = 0;
			for (unsigned int k = 0; k < trains.size(); k++) {
				if (assarr[trains[k].getStartPoint()] == i && assarr[trains[k].getDestPoint()] == j) {
					if (trains[k].getCost() < min_cost)
						min_cost = trains[k].getCost();
					flag = 1;
				}
			}
			if (!flag)
				a[i][j] = -1;
			else
				a[i][j] = min_cost;
		}
	}
	return a;
}

/*
* Function for building associate array (map). We interpretate starting and
* destination points as 0,1,2,...,n to operate easier with cycles.
*/
map<int, int> buildAssociateArray(vector<TrainSched> trains) {
	map<int, int> tmp;
	map<int, int>::iterator it;
	int cnt = 0;
	for (unsigned int i = 0; i < trains.size(); i++) {
		if (tmp.find(trains[i].getStartPoint()) == tmp.end()) {
			tmp[trains[i].getStartPoint()] = cnt;
			cnt++;
		}
	}
	return tmp;
}

/*
* Function used by Little's algorithm.
*/
void makebase(double** Matrix, int amountOfGraphs, int ik, int jk)
{
	int i, j;
	for (i = 0; i < amountOfGraphs; i++) if (Matrix[i][jk] >= 0) Matrix[i][jk] = -1;
	for (j = 0; j < amountOfGraphs; j++) if (Matrix[ik][j] >= 0) Matrix[ik][j] = -1;
	Matrix[ik][jk] = -2;
	if (Matrix[jk][ik] >= 0) Matrix[jk][ik] = -1;
}

/*
* Function for finding a way through all cities and returning back to the starting one
* with optimal cost by using Little's algorithm. We enter 2D-matrix with costs 
* and receive the optimal path. Detailed description of the algorithm can be found there:
* https://www.researchgate.net/publication/307554084_On_the_Optimization_and_Parallelizing_Little_Algorithm_for_Solving_the_Traveling_Salesman_Problem
*/
void algorithmLittle(double** Matrix, int amountOfGraphs) {
	double minv, miniv, minjv, maxv;
	bool flag;
	int cnt;
	int i2 = 0, j2 = 0, i3 = 0, j3 = 0;

	for (int i = 0; i < amountOfGraphs; i++) Matrix[i][i] = -1;
	for (int c = 0; c < amountOfGraphs; c++)
	{
		// looking for one-in-row or one-in-col element
		flag = 0;
		for (int i = 0; (i < amountOfGraphs) && (flag == 0); i++)
		{
			cnt = 0;
			for (int j = 0; j < amountOfGraphs; j++) if (Matrix[i][j] >= 0)
			{
				i2 = i;
				j2 = j;
				cnt++;
			}
			if (cnt == 1) flag = 1;
		}

		for (int j = 0; (j < amountOfGraphs) && (flag == 0); j++)
		{
			cnt = 0;
			for (int i = 0; i < amountOfGraphs; i++) if (Matrix[i][j] >= 0)
			{
				i2 = i;
				j2 = j;
				cnt++;
			}
			if (cnt == 1) flag = 1;
		}

		if (flag == 1)
		{
			makebase(Matrix, amountOfGraphs, i2, j2);
			continue;
		}

		// minus mins
		for (int i = 0; i < amountOfGraphs; i++)
		{
			minv = 30000;
			for (int j = 0; j < amountOfGraphs; j++)
				if ((Matrix[i][j] >= 0) && (Matrix[i][j] < minv)) minv = Matrix[i][j];
			for (int j = 0; j < amountOfGraphs; j++) if (Matrix[i][j] >= 0) Matrix[i][j] -= minv;
		}

		for (int j = 0; j < amountOfGraphs; j++)
		{
			minv = 30000;
			for (int i = 0; i < amountOfGraphs; i++)
				if ((Matrix[i][j] >= 0) && (Matrix[i][j] < minv)) minv = Matrix[i][j];
			for (int i = 0; i < amountOfGraphs; i++) if (Matrix[i][j] >= 0) Matrix[i][j] -= minv;
		}

		// checking nulls and looking for base values
		maxv = -1;
		for (int i = 0; i < amountOfGraphs; i++) for (int j = 0; j < amountOfGraphs; j++) if (Matrix[i][j] == 0)
		{
			miniv = 30000;
			minjv = 30000;
			for (i2 = 0; i2 < amountOfGraphs; i2++) if ((Matrix[i2][j] >= 0) && (i2 != i) && (Matrix[i2][j] < miniv)) miniv = Matrix[i2][j];
			for (j2 = 0; j2 < amountOfGraphs; j2++) if ((Matrix[i][j2] >= 0) && (j2 != j) && (Matrix[i][j2] < minjv)) minjv = Matrix[i][j2];
			if (miniv + minjv > maxv)
			{
				maxv = miniv + minjv;
				i3 = i;
				j3 = j;
			}
		}
		makebase(Matrix, amountOfGraphs, i3, j3);
	}
}

/*
* Function that returns a key from a value in a map.
*/
int getKeyFromValue(map<int, int> assArr, int value)
{
	for (auto& it : assArr) {
		if (it.second == value) {
			return it.first;
		}
	}
	return -1;
}

/*
* Function that prints optimal path from matrix, that was received from Little's algorithm.
* Matrix was copied, because it will be transformed by the Little's algorithm.
* But we need original matrix in order to restore an element from train schedule.
*/
void printOptimalPath(double** Matrix, double** origMatrix, vector<TrainSched> trains, map<int, int> assArr, int amountOfGraphs) {
	vector<vector<TrainSched>> results;
	int k = 0, l = 0;
	int i2 = 0;
	for (int c = 0; c < amountOfGraphs; c++)
	{
		for (int j = 0; j < amountOfGraphs; j++) if (Matrix[i2][j] == -2)
		{
			for (TrainSched i : trains) {
				if (i.getStartPoint() == getKeyFromValue(assArr, i2) &&
					i.getDestPoint() == getKeyFromValue(assArr, j) &&
					i.getCost() == origMatrix[i2][j]){
					printf("\t\t%c\n", 25);
					cout << i.toString() << endl;
					break;
				}
			}
			i2 = j;
			break;
		}
	}
}

/*
* Function for printing a schedule of trains.
*/
void printSchedule(vector<TrainSched> trains) {
	for (TrainSched i : trains) {
		cout << i.toString() << endl;
	}
}

/* 
* Function for printing a matrix of costs.
*/
void printMatrix(double** Matrix, int amountOfGraphs) {
	cout << "\t1\t2\t3\t4\t5\t6\n" << "--------------------------------------------------\n";
	for (signed int i = 0; i < amountOfGraphs; i++) {
		cout << i + 1 << "|\t";
		for (signed int j = 0; j < amountOfGraphs; j++)
			cout << Matrix[i][j] << "\t";
		cout << endl;
	}
}

/*
* Function for printing an associate array (map).
*/
void printAssociateArray(map<int, int> assArr) {
	map<int, int>::iterator it;
	for (it = assArr.begin(); it != assArr.end(); ++it) {
		cout << it->first << " => " << it->second << '\n';
	}
}