/*
 * DatasetMeasure.h
 *
 *  Created on: Dec 12, 2017
 *      Author: batuhan
 */

#ifndef DATASETMEASURE_H_
#define DATASETMEASURE_H_

#include "vector"
#include "limits"
#include "../dataset/Cell.h"

using namespace std;

class DatasetMeasure {

public:
	DatasetMeasure(list<Cell*> *allCells);
	virtual ~DatasetMeasure();
	vector<float> getRangeValues();
	vector<float> getMinimumValues();
	vector<float> getMaximumValues();

private:
	vector<float> rangeValues;
	vector<float> minimumValues;
	vector<float> maximumValues;
	const int MIN_INIT_VALUE = numeric_limits<int>::max();
	const int MAX_INIT_VALUE = numeric_limits<int>::min();

	void createDatasetMeasures(list<Cell*> *allCells);
	void initializeDatasetMeasureVectors(int dimension);
};

#endif /* DATASETMEASURE_H_ */
