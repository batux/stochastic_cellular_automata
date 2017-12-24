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

list<Cell*>* TrainingDataset::getAllInitialCells() {
	return this->allInitialCells;
}

vector<ControlPoint*> TrainingDataset::getSelectedControlPointList() {
	return this->selectedControlPointList;
}

void TrainingDataset::clearCellsOfExperiment() {

	this->initialCells.clear();
	this->createdCellsInExperiment.clear();
}

void TrainingDataset::clearControlPoints() {

	this->testControlPoints.clear();
	vector<ControlPoint*>(this->testControlPoints).swap(this->testControlPoints);

	this->validationControlPoints.clear();
	vector<ControlPoint*>(this->validationControlPoints).swap(this->validationControlPoints);

	this->selectedControlPointList.clear();
	vector<ControlPoint*>(this->selectedControlPointList).swap(this->selectedControlPointList);
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

	this->selectedControlPointIndexList.clear();
	vector<int>(this->selectedControlPointIndexList).swap(this->selectedControlPointIndexList);
}

void TrainingDataset::prepareInitialCells() {

	for(list<Cell*>::iterator cell = this->allInitialCells->begin(); cell != this->allInitialCells->end(); cell++) {

		int currentHashCode = (*cell)->getHashCode();

		bool hasCurrentHashcode = false;
		for(int i=0; i < this->selectedControlPointList.size(); i++) {

			ControlPoint *controlPoint = this->selectedControlPointList[i];

			if(currentHashCode == controlPoint->getHashCodeOfCell() &&
			   controlPoint->getControlPointType() == 1) {

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
	prepareControlPoints(this->testControlPoints, 1);
}

void TrainingDataset::prepareValidationControlPoints() {
	prepareControlPoints(this->validationControlPoints, 2);
}

void TrainingDataset::prepareControlPoints(vector<ControlPoint*> &controlPoints, int controlPointType) {

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

		ControlPoint *controlPoint = new ControlPoint((*cellIterator)->getHashCode(), (*cellIterator)->getDataRow()->getLabelValue(), controlPointType);
		controlPoints.push_back(controlPoint);
		this->selectedControlPointList.push_back(controlPoint);
		this->selectedControlPointIndexList.push_back(randomlySelectedCellIndex);
	}
}
