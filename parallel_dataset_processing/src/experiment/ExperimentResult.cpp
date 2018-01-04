/*
 * ExperimentResult.cpp
 *
 *  Created on: Dec 23, 2017
 *      Author: batuhan
 */

#include "ExperimentResult.h"

ExperimentResult::ExperimentResult() {
	// TODO Auto-generated constructor stub

}

ExperimentResult::~ExperimentResult() {
	//delete this;
}

void ExperimentResult::setStartTime(long startTime) {
	this->startTime = startTime;
}

void ExperimentResult::setEndTime(long endTime) {
	this->endTime = endTime;
}

void ExperimentResult::setSuccessRate(float successRate) {
	this->succesRate = succesRate;
}

float ExperimentResult::getSuccessRate() {
	return this->succesRate;
}

