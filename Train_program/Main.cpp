#include "GlobalHeader.h"

int main() {
	vector<TrainSched> trainSchedule = readSched("test_task_data.csv");
	//printSchedule(trainSchedule);
	map<int, int> assArr = buildAssociateArray(trainSchedule);
	//printAssociateArray(assArr);
	int amountOfGraphs = assArr.size();
	double** Matrix = fillCostMatrix(trainSchedule, amountOfGraphs, assArr);
	//printMatrix(Matrix, amountOfGraphs);
	double** origMatrix = new double* [amountOfGraphs];
	for (int i = 0; i < amountOfGraphs; i++) {
		origMatrix[i] = new double[amountOfGraphs];
		for (int j = 0; j < amountOfGraphs; j++)
			origMatrix[i][j] = Matrix[i][j];
	}
	algorythmLittle(Matrix, amountOfGraphs);
	findOptimalPath(Matrix, origMatrix, trainSchedule, assArr, amountOfGraphs);
	return 0;
}