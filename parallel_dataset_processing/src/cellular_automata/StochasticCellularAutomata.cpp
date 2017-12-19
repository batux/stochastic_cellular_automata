/*
 * StochasticCellularAutomata.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: batuhan
 */

#include "StochasticCellularAutomata.h"

StochasticCellularAutomata::StochasticCellularAutomata(TrainingDataset *trainingDataset, HashcodeProducer *hashcodeProducer) {

	this->trainingDataset = trainingDataset;
	this->hashcodeProducer = hashcodeProducer;
}

StochasticCellularAutomata::~StochasticCellularAutomata() {
	// TODO Auto-generated destructor stub
}

void StochasticCellularAutomata::run() {

	int iterationCount = 0;
	bool runFlag = true;
	this->timeoutOccured = false;

	milliseconds startTimeInMillis = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	long startTimeAsLong = startTimeInMillis.count();

	while(runFlag) {

		Cell *cellForHeatPropagation = this->trainingDataset->selectRandomCell();
		heatPropagation(cellForHeatPropagation);

		Cell *cellForStateTransfer = this->trainingDataset->selectRandomCell();
		stateTransfer(cellForStateTransfer);

		if ((iterationCount % 10) == 0) {

			vector<ControlPoint*> validationControlPoints = this->trainingDataset->getValidationControlPoints();
			runFlag = isReachedToStopState(validationControlPoints);

			milliseconds runnigTimeInMillis = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
			long runnigTimeAsLong = runnigTimeInMillis.count();

			long timeDiffInMillis = runnigTimeAsLong - startTimeAsLong;
			int timeDiffInSeconds = (int) (timeDiffInMillis / 1000) % 60 ;

			if(timeDiffInSeconds > 20) {
				runFlag = false;
				this->timeoutOccured = true;
			}
		}

		iterationCount++;
	}

}

bool StochasticCellularAutomata::isReachedToStopState(vector<ControlPoint*> &validationControlPoints) {

	if(validationControlPoints.size() == 0) {
		return false;
	}

	int visitedValidationCellCount = 0;

	for(int i=0; i < validationControlPoints.size(); i++) {

		ControlPoint *controlPoint = validationControlPoints[i];
		Cell *cell = this->trainingDataset->findCellByHashCode(controlPoint->getHashCodeOfCell());

		if((cell != NULL) && (cell->getLabelValue() > -1)) {
			visitedValidationCellCount++;
		}
	}

	int cellCountForStopState = validationControlPoints.size() * this->validationRatio;

	if(visitedValidationCellCount < cellCountForStopState) {
		return false;
	}

	return true;
}

void StochasticCellularAutomata::heatPropagation(Cell *cell) {

	vector<Cell*> neighbours = findNeighbours(cell);
	float heatAverage = calculateHeatAverage(cell, neighbours);

	if(!cell->isInitialState()) {
		cell->setPower(heatAverage);
	}

	// Propagate heat to neighbours!
	for(int i=0; i < neighbours.size(); i++) {
		Cell* neighbour = neighbours[i];
		if (neighbour != NULL && !neighbour->isInitialState()) {
			neighbour->setPower(heatAverage);
		}
	}
}

void StochasticCellularAutomata::stateTransfer(Cell *cell) {

	if (cell != NULL && (cell->getLabelValue() >= 0) && (cell->getPower() > this->heatThreshold)) {

		vector<Cell*> neighbours = findNeighbours(cell);

		for(int i=0; i < neighbours.size(); i++) {

			Cell* neighbour = neighbours[i];
			if(neighbour != NULL && (neighbour->getLabelValue() < -1) && (neighbour->getPower() > this->heatThreshold)) {
				neighbour->setLabelValue(cell->getLabelValue());
				stateTransfer(neighbour);
			}
		}
	}
}

// Helper functions for SCA!
Cell* StochasticCellularAutomata::findNeighbour(vector<int> positionsOfCenterCell, int neighbourSideIndex) {

	int indexDelta = 1;

	int dataDimension = this->trainingDataset->getDataDimension();

	if(neighbourSideIndex >= dataDimension) {
		indexDelta = -1;
	}

	int positionIndexOfNeighbour = neighbourSideIndex % dataDimension;

	positionsOfCenterCell[positionIndexOfNeighbour] += indexDelta;

	// Create hashcode from new position and check it in cell list!

	unsigned int hashCode = this->hashcodeProducer->createHashCode(positionsOfCenterCell);

	Cell *cell = this->trainingDataset->findCellByHashCode(hashCode);

	if(cell == NULL) {
		cell = new Cell(positionsOfCenterCell, false, this->hashcodeProducer);
	}

	return cell;
}

vector<Cell*> StochasticCellularAutomata::findNeighbours(Cell *cell) {

	vector<Cell*> neighbours;

	int neighbourLimit = 2 * this->trainingDataset->getDataDimension();

	for (int neighbourSideIndex = 0; neighbourSideIndex < neighbourLimit; neighbourSideIndex++) {

		Cell *cell = findNeighbour(cell->getPositions(), neighbourSideIndex);

		if(cell != NULL) {
			this->trainingDataset->addCellInExperiment(cell);
			neighbours.push_back(cell);
		}
	}

	return neighbours;
}

float StochasticCellularAutomata::calculateHeatAverage(Cell *cell, vector<Cell*> &cells) {

	float heatAverage = 0.0f;
	float totalHeatValue = 0.0f;
	int neighbourLimit = (2 * this->trainingDataset->getDataDimension()) + 1;

	for(int i=0; i < cells.size(); i++) {
		Cell *cell = cells[i];
		totalHeatValue += cell->getPower();
	}

	totalHeatValue += cell->getPower();

	heatAverage = (totalHeatValue) / ((float)neighbourLimit);
	return heatAverage;
}

void abc() {

	typedef std::chrono::high_resolution_clock Time;
	typedef std::chrono::duration<float> fsec;

	auto t0 = Time::now();
	auto t1 = Time::now();

	fsec fs = t1 - t0;

	milliseconds ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());

	long val = ms.count();

	cout << val << endl;

}

