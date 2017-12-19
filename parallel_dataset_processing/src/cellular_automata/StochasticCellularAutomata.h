/*
 * StochasticCellularAutomata.h
 *
 *  Created on: Dec 17, 2017
 *      Author: batuhan
 */

#ifndef STOCHASTICCELLULARAUTOMATA_H_
#define STOCHASTICCELLULARAUTOMATA_H_

#include "chrono"
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

private:
	const float heatThreshold = 30.0f;
	const float validationRatio = 0.95f;

	TrainingDataset *trainingDataset;
	HashcodeProducer *hashcodeProducer;
	bool timeoutOccured = false;

	Cell* findNeighbour(vector<int> positionsOfCenterCell, int neighbourSideIndex);
	vector<Cell*> findNeighbours(Cell *cell);
	float calculateHeatAverage(Cell *cell, vector<Cell*> &cells);
	bool isReachedToStopState(vector<ControlPoint*> &validationControlPoints);
};

#endif /* STOCHASTICCELLULARAUTOMATA_H_ */
