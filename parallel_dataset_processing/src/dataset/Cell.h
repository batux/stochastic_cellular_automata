/*
 * Cell.h
 *
 *  Created on: Dec 5, 2017
 *      Author: batuhan
 */

#ifndef CELL_H_
#define CELL_H_

#include "list"
#include "string"
#include "vector"
#include "DataRow.h"
#include "../hashcode_producer/HashcodeProducer.h"

using namespace std;

class Cell {

public:
	Cell(DataRow* dataRow, HashcodeProducer *hashcodeProducer);
	Cell(vector<int> positions, bool initialState, HashcodeProducer *hashcodeProducer);
	virtual ~Cell();
	DataRow* getDataRow();
	void setDataRow(DataRow* dataRow);
	vector<int> getPositions();
	void setPositions(vector<int> positions);
	string toString();
	unsigned int getHashCode();
	void clearPositionList();
	void setPower(float power);
	float getPower();
	void setInitialState();
	bool isInitialState();
	void setLabelValue(int labelValue);
	int getLabelValue();
	void prepareCellForExperiment();


private:

	unsigned int hashCodeValue;
	float power;
	bool initialState;
	int labelValueForExperiment;
	vector<int> positions;
	DataRow* dataRow;
	HashcodeProducer *hashcodeProducer;

};

#endif /* CELL_H_ */
