/*
 * DatasetMeasure.cpp
 *
 *  Created on: Dec 12, 2017
 *      Author: batuhan
 */

#include "DatasetMeasure.h"

DatasetMeasure::DatasetMeasure() {

}

DatasetMeasure::~DatasetMeasure() {
	// TODO Auto-generated destructor stub
}

void DatasetMeasure::createDatasetMeasures(list<Cell*> *allCells) {

	list<Cell*>::iterator firstCell = allCells->begin();
	advance(firstCell, 1);

	int dimension = (*firstCell)->getDataRow()->getDataItemList().size();
	initializeDatasetMeasureVectors(dimension);

	// TODO: Parallel Computing!
	for(list<Cell*>::iterator cell = allCells->begin(); cell != allCells->end(); cell++) {

		list<float> dataItems = (*cell)->getDataRow()->getDataItemList();

		int dataItemCounter = 0;

		for(list<float>::iterator dataItem = dataItems.begin(); dataItem != dataItems.end(); dataItem++) {

			float currentMinValue = this->minimumValues.at(dataItemCounter);
			if(currentMinValue > (*dataItem)) {
				this->minimumValues[dataItemCounter] = (*dataItem);
			}

			float currentMaxValue = this->maximumValues.at(dataItemCounter);
			if(currentMaxValue < (*dataItem)) {
				this->maximumValues[dataItemCounter] = (*dataItem);
			}

			dataItemCounter++;
		}
	}

	for(int i=0; i < this->rangeValues.size(); i++) {
		this->rangeValues[i] = this->maximumValues[i] - this->minimumValues[i];
	}
}

void DatasetMeasure::initializeDatasetMeasureVectors(int dimension) {

	for(int i=0; i < dimension; i++) {
		this->minimumValues.push_back(this->MIN_INIT_VALUE);
	}

	for(int i=0; i < dimension; i++) {
		this->maximumValues.push_back(this->MAX_INIT_VALUE);
	}

	for(int i=0; i < dimension; i++) {
		this->rangeValues.push_back(0);
	}
}

vector<float> DatasetMeasure::getRangeValues() {
	return this->rangeValues;
}

vector<float> DatasetMeasure::getMinimumValues() {
	return this->minimumValues;
}

vector<float> DatasetMeasure::getMaximumValues() {
	return this->maximumValues;
}
