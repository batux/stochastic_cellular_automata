/*
 * ControlPoint.cpp
 *
 *  Created on: Dec 10, 2017
 *      Author: batuhan
 */

#include "ControlPoint.h"

ControlPoint::ControlPoint(unsigned int hashCodeOfCell, int labelValue, int controlPointType) {
	this->hashCodeOfCell = hashCodeOfCell;
	this->labelValue = labelValue;
	this->controlPointType = controlPointType;
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

int ControlPoint::getControlPointType() {
	return this->controlPointType;
}
