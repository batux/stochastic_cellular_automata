/*
 * TrainingDataset.cpp
 *
 *  Created on: Dec 10, 2017
 *      Author: batuhan
 */

#include "TrainingDataset.h"

TrainingDataset::TrainingDataset(int dataDimension, list<Cell*> *allInitialCells) {

	this->dataDimension = dataDimension;
	this->allInitialCells = allInitialCells;
}

TrainingDataset::~TrainingDataset() {
	// TODO Auto-generated destructor stub
}

int TrainingDataset::getDataDimension() {
	return this->dataDimension;
}

void TrainingDataset::addCellInExperiment(Cell* cell) {
	this->createdCellsInExperiment.push_back(cell);
}

vector<ControlPoint*> TrainingDataset::getValidationControlPoints() {
	return this->validationControlPoints;
}

vector<ControlPoint*> TrainingDataset::getTestControlPoints() {
	return this->testControlPoints;
}

Cell* TrainingDataset::selectRandomCell() {

	int randomCellSelectionLimit = this->initialCells.size() + this->createdCellsInExperiment.size();

	srand((int)time(0));
	int randomlySelectedCellIndex = rand() % randomCellSelectionLimit;

	if(randomlySelectedCellIndex < this->initialCells.size()) {
		return this->initialCells[randomlySelectedCellIndex];
	}
	else {
		randomlySelectedCellIndex = randomlySelectedCellIndex - this->initialCells.size();
		return this->createdCellsInExperiment[randomlySelectedCellIndex];
	}

}

Cell* TrainingDataset::findCellByHashCode(unsigned int hashCode) {

	Cell *foundCell = NULL;

	for(list<Cell*>::iterator cell = this->allInitialCells->begin(); cell != this->allInitialCells->end(); cell++) {

		if(hashCode == (*cell)->getHashCode()) {
			foundCell = (*cell);
			break;
		}
	}

	for(vector<Cell*>::iterator cell = this->createdCellsInExperiment.begin(); cell != this->createdCellsInExperiment.end(); cell++) {

		if(hashCode == (*cell)->getHashCode()) {
			foundCell = (*cell);
			break;
		}
	}

	return foundCell;
}

void TrainingDataset::createTrainingset() {
	prepareTestControlPoints();
	prepareValidationControlPoints();
	prepareInitialCells();
}

void TrainingDataset::prepareInitialCells() {

	for(list<Cell*>::iterator cell = this->allInitialCells->begin(); cell != this->allInitialCells->end(); cell++) {

		int currentHashCode = (*cell)->getHashCode();

		bool hasCurrentHashcode = false;
		for(int i=0; i < this->selectedControlPointIndexList.size(); i++) {
			if(currentHashCode == this->testControlPoints[i]->getHashCodeOfCell()) {
				hasCurrentHashcode = true;
				break;
			}
		}

		if(!hasCurrentHashcode) {
			this->initialCells.push_back((*cell));
		}
	}
}

void TrainingDataset::prepareTestControlPoints() {
	prepareControlPoints(this->testControlPoints);
}

void TrainingDataset::prepareValidationControlPoints() {
	prepareControlPoints(this->validationControlPoints);
}

void TrainingDataset::prepareControlPoints(vector<ControlPoint*> &controlPoints) {

	int allCellListSize = this->allInitialCells->size();
	int testControlPointListSize = this->dataDimension * (0.2);

	for(int i=0; i < testControlPointListSize; i++) {

		srand((int)time(0));
		int randomlySelectedCellIndex = rand() % allCellListSize;

		while(find(this->selectedControlPointIndexList.begin(),
				   this->selectedControlPointIndexList.end(), randomlySelectedCellIndex) != this->selectedControlPointIndexList.end()) {

			srand((int)time(0));
			randomlySelectedCellIndex = rand() % allCellListSize;
		}

		list<Cell*>::iterator cellIterator = this->allInitialCells->begin();
		advance(cellIterator, randomlySelectedCellIndex);

		// Set power value to zero for validation and test control cells!
		(*cellIterator)->setLabelValue(-1);
		(*cellIterator)->setPower(0.0f);

		ControlPoint *controlPoint = new ControlPoint((*cellIterator)->getHashCode(), (*cellIterator)->getDataRow()->getLabelValue());
		controlPoints.push_back(controlPoint);
		this->selectedControlPointIndexList.push_back(randomlySelectedCellIndex);
	}
}
