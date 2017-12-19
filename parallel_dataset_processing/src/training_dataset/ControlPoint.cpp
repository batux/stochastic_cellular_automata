/*
 * ControlPoint.cpp
 *
 *  Created on: Dec 10, 2017
 *      Author: batuhan
 */

#include "ControlPoint.h"

ControlPoint::ControlPoint(unsigned int hashCodeOfCell, int labelValue) {
	this->hashCodeOfCell = hashCodeOfCell;
	this->labelValue = labelValue;
}

ControlPoint::~ControlPoint() {
	delete this;
}

unsigned int ControlPoint::getHashCodeOfCell() {
	return this->hashCodeOfCell;
}

int ControlPoint::getLabelValue() {
	return this->labelValue;
}

