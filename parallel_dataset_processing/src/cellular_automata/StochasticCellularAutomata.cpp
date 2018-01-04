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

	srand((int)time(0));

	while(runFlag) {

		Cell *cellForHeatPropagation = this->trainingDataset->selectRandomCell(rand());
		heatPropagation(cellForHeatPropagation);

		Cell *cellForStateTransfer = this->trainingDataset->selectRandomCell(rand());
		stateTransfer(cellForStateTransfer);

		if ((iterationCount % 10) == 0) {

			vector<ControlPoint*> validationControlPoints = this->trainingDataset->getValidationControlPoints();
			runFlag = !isReachedToStopState(validationControlPoints);

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

bool StochasticCellularAutomata::isTimeoutOccured() {
	return this->timeoutOccured;
}

bool StochasticCellularAutomata::isReachedToStopState(vector<ControlPoint*> &validationControlPoints) {

	if(validationControlPoints.size() == 0) {
		return false;
	}

	int visitedValidationCellCount = 0;

	for(unsigned int i=0; i < validationControlPoints.size(); i++) {

		ControlPoint *controlPoint = validationControlPoints.at(i);
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

	if (cell != NULL && cell->getPower() >= 30) {

		vector<Cell*> neighbours = findNeighbours(cell);
		float heatAverage = calculateHeatAverage(cell, neighbours);

		if(!cell->isInitialState()) {
			cell->setPower(heatAverage);
		}

		// Propagate heat to neighbours!
		for(unsigned int i=0; i < neighbours.size(); i++) {
			Cell* neighbour = neighbours.at(i);
			if (neighbour != NULL && !neighbour->isInitialState()) {
				neighbour->setPower(heatAverage);
			}
		}
	}
}

void StochasticCellularAutomata::stateTransfer(Cell *cell) {

	queue<Cell*> cellQueue;
	cellQueue.push(cell);

	while(cellQueue.size() > 0) {

		Cell *currentNeighbourCell = cellQueue.front();
		cellQueue.pop();

		if (currentNeighbourCell != NULL && (currentNeighbourCell->getLabelValue() >= 0) && (currentNeighbourCell->getPower() > this->heatThreshold)) {

			vector<Cell*> neighbours = findNeighbours(currentNeighbourCell);

			for(unsigned int i=0; i < neighbours.size(); i++) {

				Cell* neighbour = neighbours.at(i);
				if(neighbour != NULL && (neighbour->getLabelValue() < 0) && (neighbour->getPower() > this->heatThreshold)) {
					neighbour->setLabelValue(currentNeighbourCell->getLabelValue());
					cellQueue.push(neighbour);

					/* TODO: It was added for testing!
						vector<ControlPoint*> validationControlPoints = this->trainingDataset->getValidationControlPoints();

						for(int i=0; i < validationControlPoints.size(); i++) {

							ControlPoint *controlPoint = validationControlPoints.at(i);

							if(controlPoint->getHashCodeOfCell() == neighbour->getHashCode()) {
								cout << "CHANGED VALIDATION POINT (EXPECTED): " << controlPoint->getLabelValue() << endl;
								cout << "CHANGED VALIDATION POINT (ACTUAL): " << neighbour->getLabelValue() << endl;
							}
						}
					*/

				}
			}
		}
	}
}

// Helper functions for SCA!
Cell* StochasticCellularAutomata::findNeighbour(vector<int> positionsOfCenterCell) {

	// Create hashcode from new position and check it in cell list!

	int hashCode = this->hashcodeProducer->createHashCode(positionsOfCenterCell);

	Cell *cell = this->trainingDataset->findNeighbourCellByHashCode(hashCode);

	return cell;
}

vector<Cell*> StochasticCellularAutomata::findNeighbours(Cell *cell) {

	vector<Cell*> neighbours;

	int neighbourLimit = 2 * this->trainingDataset->getDataDimension();

	for (int neighbourSideIndex = 0; neighbourSideIndex < neighbourLimit; neighbourSideIndex++) {

		vector<int> positions = cell->getPositions();

		vector<int> positionsOfNeighbour = preparePositionsOfNeighbour(positions, neighbourSideIndex);

		Cell *neighbourCell = findNeighbour(positionsOfNeighbour);

		if(neighbourCell == NULL) {
			neighbourCell = new Cell(positionsOfNeighbour, false, this->hashcodeProducer);
			this->trainingDataset->addCellToInitialCells(neighbourCell);
			this->trainingDataset->addCellToSelectableCells(neighbourCell);
		}

		neighbours.push_back(neighbourCell);
	}

	return neighbours;
}

vector<int> StochasticCellularAutomata::preparePositionsOfNeighbour(vector<int> positionsOfCenterCell, int neighbourSideIndex) {

	int indexDelta = 1;

	int dataDimension = this->trainingDataset->getDataDimension();

	if(neighbourSideIndex >= dataDimension) {
		indexDelta = -1;
	}

	int positionIndexOfNeighbour = neighbourSideIndex % dataDimension;

	positionsOfCenterCell[positionIndexOfNeighbour] += indexDelta;

	return positionsOfCenterCell;
}

float StochasticCellularAutomata::calculateHeatAverage(Cell *cell, vector<Cell*> &cells) {

	float heatAverage = 0.0f;
	float totalHeatValue = 0.0f;
	int neighbourLimit = (2 * this->trainingDataset->getDataDimension()) + 1;

	for(unsigned int i=0; i < cells.size(); i++) {
		Cell *cell = cells.at(i);
		totalHeatValue += cell->getPower();
	}

	totalHeatValue += cell->getPower();

	heatAverage = (totalHeatValue) / ((float)neighbourLimit);
	return heatAverage;
}

Cell* StochasticCellularAutomata::findNeighbourCell(Cell *emptyCell) {

	bool foundState = false;
	Cell* foundNeighbourCell = NULL;

	queue<Cell*> cellQueue;
	cellQueue.push(emptyCell);

	milliseconds startTimeInMillis = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	long startTimeAsLong = startTimeInMillis.count();

	while(!foundState && (cellQueue.size() > 0)) {

		Cell *currentNeighbourCell = cellQueue.front();
		cellQueue.pop();

		vector<Cell*> neighbours = this->findNeighbours(currentNeighbourCell);

		for(unsigned int i=0; i < neighbours.size(); i++) {
			Cell* neighbourCell = neighbours.at(i);

			if(neighbourCell != NULL) {
				cellQueue.push(neighbourCell);
			}

			if(neighbourCell != NULL && (neighbourCell->getLabelValue() >= 0)) {
				foundNeighbourCell = neighbourCell;
				currentNeighbourCell->setLabelValue(neighbourCell->getLabelValue());
				emptyCell->setLabelValue(neighbourCell->getLabelValue());
				foundState = true;
				break;
			}
		}

		milliseconds runnigTimeInMillis = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
		long runnigTimeAsLong = runnigTimeInMillis.count();

		long timeDiffInMillis = runnigTimeAsLong - startTimeAsLong;
		int timeDiffInSeconds = (int) (timeDiffInMillis / 1000) % 60 ;

		if(timeDiffInSeconds > 20) {
			foundState = true;
		}
	}

	return foundNeighbourCell;
}

// alt taraf ile code dublication var!
float StochasticCellularAutomata::calculateSuccessRatioByValidationPoints() {

	determineStateOfEmptyValidationCells();

	vector<ControlPoint*> validationControlPoints = this->trainingDataset->getValidationControlPoints();
	return calculateSuccessRatio(validationControlPoints);
}

float StochasticCellularAutomata::calculateSuccessRatioByTestPoints() {

	determineStateOfEmptyTestCells();

	vector<ControlPoint*> testControlPoints = this->trainingDataset->getTestControlPoints();
	return calculateSuccessRatio(testControlPoints);
}

float StochasticCellularAutomata::calculateSuccessRatio(vector<ControlPoint*> &controlPoints) {

	int correctlyClassifiedCellCount = 0;
	int totalVisitedTestCellCount = 0;

	for(unsigned int i=0; i < controlPoints.size(); i++) {

		ControlPoint *controlPoint = controlPoints.at(i);

		if(controlPoint != NULL) {
			Cell* cell = this->trainingDataset->findCellByHashCode(controlPoint->getHashCodeOfCell());

			if(cell != NULL) {
				totalVisitedTestCellCount++;
			}
			if(cell != NULL && (cell->getLabelValue() >= 0)) {
				if(cell->getLabelValue() == controlPoint->getLabelValue()) {
					correctlyClassifiedCellCount++;
				}
			}
		}
	}

	if(totalVisitedTestCellCount == 0) {
		return 0.0f;
	}

	float succesRatio = ((float) correctlyClassifiedCellCount) / ((float) totalVisitedTestCellCount);
	return succesRatio;
}

// code dublication var !
void StochasticCellularAutomata::determineStateOfEmptyValidationCells() {

	vector<ControlPoint*> selectedControlPoints = this->trainingDataset->getValidationControlPoints();
	determineStateOfEmptyCells(selectedControlPoints);
}

void StochasticCellularAutomata::determineStateOfEmptyTestCells() {

	vector<ControlPoint*> selectedControlPoints = this->trainingDataset->getTestControlPoints();
	determineStateOfEmptyCells(selectedControlPoints);
}

void StochasticCellularAutomata::determineStateOfEmptyCells(vector<ControlPoint*> &selectedControlPoints) {

	for(unsigned int i=0; i < selectedControlPoints.size(); i++) {

		ControlPoint* controlPoint = selectedControlPoints.at(i);

		if(controlPoint != NULL) {

			Cell* cell = this->trainingDataset->findCellByHashCode(controlPoint->getHashCodeOfCell());

			if(cell != NULL && (cell->getLabelValue() < 0)) {

				// Determine the state of cell from neighbours
				Cell *neighbourCell = findNeighbourCell(cell);
				//cell->setLabelValue(neighbourCell->getLabelValue());
			}
		}
	}
}

void StochasticCellularAutomata::setValidationRatio(float validationRatio) {
	this->validationRatio = validationRatio;
}

float StochasticCellularAutomata::getValidationRatio() {
	return this->validationRatio;
}
