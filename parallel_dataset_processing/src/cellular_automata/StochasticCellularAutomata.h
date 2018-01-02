/*
 * StochasticCellularAutomata.h
 *
 *  Created on: Dec 17, 2017
 *      Author: batuhan
 */

#ifndef STOCHASTICCELLULARAUTOMATA_H_
#define STOCHASTICCELLULARAUTOMATA_H_

#include "chrono"
#include "queue"
#include "vector"
#include "../dataset/Cell.h"
#include "../training_dataset/TrainingDataset.h"
#include "../hashcode_producer/HashcodeProducer.h"

using namespace std;
using namespace std::chrono;

class StochasticCellularAutomata {

public:
	StochasticCellularAutomata(TrainingDataset *trainingDataset, HashcodeProducer *hashcodeProducer);
	virtual ~StochasticCellularAutomata();
	void heatPropagation(Cell *cell);
	void stateTransfer(Cell *cell);
	void run();
	bool isTimeoutOccured();
	void setValidationRatio(float validationRatio);
	float getValidationRatio();
	float calculateSuccessRatioByValidationPoints();
	float calculateSuccessRatioByTestPoints();

private:
	const float heatThreshold = 30.0f;
	float validationRatio = 0.95f;

	TrainingDataset *trainingDataset;
	HashcodeProducer *hashcodeProducer;
	bool timeoutOccured = false;

	Cell* findNeighbour(vector<int> positionsOfCenterCell);
	vector<Cell*> findNeighbours(Cell *cell);
	float calculateHeatAverage(Cell *cell, vector<Cell*> &cells);
	bool isReachedToStopState(vector<ControlPoint*> &validationControlPoints);
	void determineStateOfEmptyTestCells();
	void determineStateOfEmptyValidationCells();
	void determineStateOfEmptyCells(vector<ControlPoint*> &selectedControlPoints);
	Cell* findNeighbourCell(Cell *emptyCell);
	float calculateSuccessRatio(vector<ControlPoint*> &controlPoints);
	vector<int> preparePositionsOfNeighbour(vector<int> positionsOfCenterCell, int neighbourSideIndex);
};

#endif /* STOCHASTICCELLULARAUTOMATA_H_ */
