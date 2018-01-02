/*
 * CellPositionCalculator.cpp
 *
 *  Created on: Dec 12, 2017
 *      Author: batuhan
 */

#include "CellPositionCalculator.h"

CellPositionCalculator::CellPositionCalculator() {

}

CellPositionCalculator::~CellPositionCalculator() {
	delete this;
}

list<Cell*>* CellPositionCalculator::calculatePositionsForEachCell(int &rangeLength, list<Cell*> *allCells, DatasetMeasure *datasetMeasure) {

	for(list<Cell*>::iterator cell = allCells->begin(); cell != allCells->end(); cell++) {

		calculatePositionsForCell(rangeLength, *cell, datasetMeasure);
	}

	return allCells;
}

Cell* CellPositionCalculator::calculatePositionsForCell(int &rangeLength, Cell* cell, DatasetMeasure *datasetMeasure) {

	vector<float> minVal = datasetMeasure->getMinimumValues();
	vector<float> maxVal = datasetMeasure->getMaximumValues();
	vector<float> rangeVal = datasetMeasure->getRangeValues();

	cell->prepareCellForExperiment();

	list<float> dataItems = cell->getDataRow()->getDataItemList();

	vector<int> tmpPositions;

	int counter = 0;
	for(list<float>::iterator dataItem = dataItems.begin(); dataItem != dataItems.end(); dataItem++) {

		int position = (int) (((*dataItem) - minVal[counter]) * (rangeLength / rangeVal[counter]));

		if (position == maxVal[counter]) {
			position = position - 1;
		}

		tmpPositions.push_back(position);

		counter++;
	}

	cell->setPositions(tmpPositions);

	return cell;
}

