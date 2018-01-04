/*
 * Cell.cpp
 *
 *  Created on: Dec 5, 2017
 *      Author: batuhan
 */

#include "Cell.h"

Cell::Cell(DataRow* clonedDatarow, HashcodeProducer *hascodeProducer) {
	this->hashCodeValue = 0;
	// Initial power value for each initial cell,
	// Validation and Test cells will have "0" power value!
	this->power = 100.0f;
	this->initialState = true;
	this->dataRow = clonedDatarow;
	this->hashcodeProducer = hascodeProducer;
	this->labelValueForExperiment = this->dataRow->getLabelValue();
}

Cell::Cell(vector<int> positions, bool initialState, HashcodeProducer *hashcodeProducer) {
	this->hashCodeValue = 0;
	this->power = 0.0f;
	this->initialState = false;
	this->dataRow = NULL;
	this->hashcodeProducer = hashcodeProducer;
	this->labelValueForExperiment = -1;
	this->positions = positions;
}

Cell::~Cell() {
	// TODO Auto-generated destructor stub
}

vector<int> Cell::getPositions() {
	return this->positions;
}

void Cell::setPositions(vector<int> positions) {
	this->positions = positions;
}

void Cell::setPower(float power) {
	this->power = power;
}

float Cell::getPower() {
	return this->power;
}

void Cell::setLabelValue(int labelValue) {
	this->labelValueForExperiment = labelValue;
}

int Cell::getLabelValue() {
	return this->labelValueForExperiment;
}

void Cell::setInitialState(bool initialState) {
	this->initialState = initialState;
}

bool Cell::isInitialState() {
	return this->initialState;
}

DataRow* Cell::getDataRow() {
	return this->dataRow;
}

void Cell::prepareCellForExperiment() {

	this->power = 100.0f;

	this->initialState = true;

	// Set origin label value from data set which is in RAM!
	this->labelValueForExperiment = this->dataRow->getLabelValue();

	// Clear positions for new experiment!
	clearPositionList();

	this->hashCodeValue = 0;
}

int Cell::getHashCode() {

	if(this->hashCodeValue == 0) {
		this->hashCodeValue = this->hashcodeProducer->createHashCode(this->positions);
	}

	return this->hashCodeValue;
}

void Cell::clearPositionList() {

	if(this->positions.size() > 0) {
		this->positions.clear();
		vector<int>(this->positions).swap(this->positions);
	}
}

string Cell::toString() {

	stringstream stringStream;

	stringStream << "Label: ";
	stringStream << (this->dataRow->getLabelValue());
	stringStream << " ";

	list<float> dataItems = (*this->dataRow).getDataItemList();

	for(list<float>::iterator dataItem = dataItems.begin(); dataItem != dataItems.end(); dataItem++) {
		stringStream << (*dataItem);
		stringStream << " ";
	}

	string dataItemAsText = stringStream.str();

	return dataItemAsText;
}
