/*
 * CellPositionCalculator.h
 *
 *  Created on: Dec 12, 2017
 *      Author: batuhan
 */

#ifndef CELLPOSITIONCALCULATOR_H_
#define CELLPOSITIONCALCULATOR_H_

#include "list"
#include "../dataset/Cell.h"
#include "DatasetMeasure.h"

using namespace std;

class CellPositionCalculator {

public:
	CellPositionCalculator();
	virtual ~CellPositionCalculator();
	list<Cell*>* calculatePositionsForEachCell(int &rangeLength, list<Cell*> *allCells, DatasetMeasure *datasetMeasure);
	Cell* calculatePositionsForCell(int &rangeLength, Cell* cell, DatasetMeasure *datasetMeasure);
};

#endif /* CELLPOSITIONCALCULATOR_H_ */
