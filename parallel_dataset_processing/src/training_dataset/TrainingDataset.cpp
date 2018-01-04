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

void TrainingDataset::addCellToInitialCells(Cell* cell) {
	this->initialCells.push_back(cell);
}

void TrainingDataset::addCellToSelectableCells(Cell* cell) {
	this->selectableCells.push_back(cell);
}

void TrainingDataset::clearCellsOfExperiment() {

	this->initialCells.clear();
	vector<Cell*>(this->initialCells).swap(this->initialCells);

	this->initialPoints.clear();
	vector<Cell*>(this->initialPoints).swap(this->initialPoints);

	this->selectableCells.clear();
	vector<Cell*>(this->selectableCells).swap(this->selectableCells);
}

void TrainingDataset::clearControlPoints() {

	this->testControlPoints.clear();
	vector<ControlPoint*>(this->testControlPoints).swap(this->testControlPoints);

	this->validationControlPoints.clear();
	vector<ControlPoint*>(this->validationControlPoints).swap(this->validationControlPoints);

	this->selectedControlPointList.clear();
	vector<ControlPoint*>(this->selectedControlPointList).swap(this->selectedControlPointList);
}

Cell* TrainingDataset::selectRandomCell(int randValue) {

	int randomCellSelectionLimit = this->selectableCells.size();

	int randomlySelectedCellIndex = randValue % randomCellSelectionLimit;

//	cout << "RANDOMLY SELECTED INDEX : " << randomlySelectedCellIndex << endl;

	return this->initialCells[randomlySelectedCellIndex];
}

Cell* TrainingDataset::findCellByHashCode(int hashCode) {

	Cell *foundCell = NULL;

	for(vector<Cell*>::iterator cell = this->initialCells.begin(); cell != this->initialCells.end(); cell++) {

		if(hashCode == (*cell)->getHashCode()) {
			foundCell = (*cell);
			return foundCell;
		}
	}

	return foundCell;
}

Cell* TrainingDataset::findNeighbourCellByHashCode(int hashCode) {

	Cell *foundCell = NULL;

	for(vector<Cell*>::iterator cell = this->initialCells.begin(); cell != this->initialCells.end(); cell++) {

		if(hashCode == (*cell)->getHashCode()) {
			foundCell = (*cell);
			return foundCell;
		}
	}

	return foundCell;
}

void TrainingDataset::createTrainingset() {

	prepareTestControlPoints();
	prepareInitialPoints();
	prepareValidationControlPoints();
	prepareInitialCells();

	this->selectedControlPointIndexList.clear();
	vector<int>(this->selectedControlPointIndexList).swap(this->selectedControlPointIndexList);
}

void TrainingDataset::prepareInitialPoints() {

	int counter = 0;
	for(list<Cell*>::iterator cell = this->allInitialCells->begin(); cell != this->allInitialCells->end(); cell++) {

		if(find(this->selectedControlPointIndexList.begin(),
			    this->selectedControlPointIndexList.end(), counter) == this->selectedControlPointIndexList.end()) {
			this->initialPoints.push_back((*cell));
		}
		counter++;
	}
}

void TrainingDataset::prepareInitialCells() {
	// TODO: selectedControlPointList Bu listeyi ekrana yazdır.
	int counter = 0;
	vector<int> uniqueHashCodeList;

	for(list<Cell*>::iterator cell = this->allInitialCells->begin(); cell != this->allInitialCells->end(); cell++) {

		int currentHashCode = (*cell)->getHashCode();

		bool hasCurrentHashcode = false;

		for(vector<int>::iterator hashCodeIterator = uniqueHashCodeList.begin(); hashCodeIterator != uniqueHashCodeList.end(); hashCodeIterator++) {
			if(currentHashCode == (*hashCodeIterator)) {
				hasCurrentHashcode = true;
				break;
			}
		}

		bool hasInitialPoint = false;

		for(vector<Cell*>::iterator pointIterator = this->initialPoints.begin(); pointIterator != this->initialPoints.end(); pointIterator++) {

			if((*pointIterator) == (*cell)) {
				hasInitialPoint = true;
				break;
			}
		}

		if(hasInitialPoint) {
			if(!hasCurrentHashcode) {
				this->initialCells.push_back((*cell));
				this->selectableCells.push_back((*cell));
				uniqueHashCodeList.push_back(currentHashCode);
			}
			else {
				(*cell)->setPower((*cell)->getPower() + 100.0f);
			}
		}
		else {
			if(!hasCurrentHashcode) {
				this->initialCells.push_back((*cell));
				uniqueHashCodeList.push_back(currentHashCode);
			}
		}
	}

//	cout << "ALL INITIAL WAS SCANNED : " << counter << endl;
}

void TrainingDataset::prepareTestControlPoints() {
	prepareControlPoints(this->testControlPoints, 1);
}

void TrainingDataset::prepareValidationControlPoints() {
	prepareControlPoints(this->validationControlPoints, 2);
}

void TrainingDataset::prepareControlPoints(vector<ControlPoint*> &controlPoints, int controlPointType) {

	int allCellListSize = this->allInitialCells->size();
	int testControlPointListSize = allCellListSize * (0.2);

	srand((int)time(0));

	for(int i=0; i < testControlPointListSize; i++) {

		int randomlySelectedCellIndex = rand() % allCellListSize;

		while(find(this->selectedControlPointIndexList.begin(),
				   this->selectedControlPointIndexList.end(), randomlySelectedCellIndex) != this->selectedControlPointIndexList.end()) {

			randomlySelectedCellIndex = rand() % allCellListSize;
		}

		//cout << randomlySelectedCellIndex << "_" << controlPointType << endl;

		list<Cell*>::iterator cellIterator = this->allInitialCells->begin();
		advance(cellIterator, randomlySelectedCellIndex);

		// Set power value to zero for validation and test control cells!
		(*cellIterator)->setLabelValue(-1);
		(*cellIterator)->setPower(0.0f);
		(*cellIterator)->setInitialState(false);

		ControlPoint *controlPoint = new ControlPoint((*cellIterator)->getHashCode(), (*cellIterator)->getDataRow()->getLabelValue(), controlPointType);
		controlPoints.push_back(controlPoint);
		this->selectedControlPointList.push_back(controlPoint);
		this->selectedControlPointIndexList.push_back(randomlySelectedCellIndex);
	}
}


/*
void TrainingDataset::putCellToMap(Cell* cell) {

	int hashCode = cell->getHashCode();

	bool existSubCellList = hasSubCellListByHashcode(hashCode);

	if(existSubCellList) {
		vector<Cell*> subCellList = this->cellMap.at(hashCode);
		subCellList.push_back(cell);
	}
	else {
		vector<Cell*> newSubCellList;
		newSubCellList.push_back(cell);
		this->cellMap.insert({ hashCode, newSubCellList });
	}
}

Cell* TrainingDataset::getCellFromMap(int hashCode) {

}

bool TrainingDataset::hasSubCellListByHashcode(int &hashCode) {

	auto iterator = this->cellMap.find(hashCode);
    if (iterator == this->cellMap.end()) {
    	return false;
    }

    return true;
}

*/
