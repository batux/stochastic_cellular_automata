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
	DatasetMeasure();
	virtual ~DatasetMeasure();
	void createDatasetMeasures(list<Cell*> *allCells);
	vector<float> getRangeValues();
	vector<float> getMinimumValues();
	vector<float> getMaximumValues();

private:
	vector<float> rangeValues;
	vector<float> minimumValues;
	vector<float> maximumValues;
	const int MIN_INIT_VALUE = numeric_limits<int>::max();
	const int MAX_INIT_VALUE = numeric_limits<int>::min();

	void initializeDatasetMeasureVectors(int dimension);
};

#endif /* DATASETMEASURE_H_ */
