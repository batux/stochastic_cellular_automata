/*
 * TrainingDataset.h
 *
 *  Created on: Dec 10, 2017
 *      Author: batuhan
 */

#ifndef TRAININGDATASET_H_
#define TRAININGDATASET_H_

#include "vector"
#include "unordered_map"
#include "cstdlib"
#include "ctime"
#include "algorithm"
#include "ControlPoint.h"
#include "../dataset/Cell.h"
#include "../dataset/Dataset.h"
#include "../dataset/CellListProducer.h"

using namespace std;

class TrainingDataset {

public:
	TrainingDataset(int dataDimension, list<Cell*> *allInitialCells);
	virtual ~TrainingDataset();
	void addCellInExperiment(Cell* cell);
	int getDataDimension();
	Cell* findCellByHashCode(unsigned int hashCode);
	void createTrainingset();
	Cell* selectRandomCell();
	vector<ControlPoint*> getValidationControlPoints();
	vector<ControlPoint*> getTestControlPoints();

private:

	int dataDimension;
	list<Cell*> *allInitialCells;
	vector<Cell*> initialCells;
	vector<Cell*> createdCellsInExperiment;
	vector<ControlPoint*> testControlPoints;
	vector<ControlPoint*> validationControlPoints;
	vector<int> selectedControlPointIndexList;
	void prepareInitialCells();
	void prepareTestControlPoints();
	void prepareValidationControlPoints();
	void prepareControlPoints(vector<ControlPoint*> &controlPoints);

};

#endif /* TRAININGDATASET_H_ */
