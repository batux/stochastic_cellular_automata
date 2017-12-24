/*
 * ExperimentApplier.cpp
 *
 *  Created on: Dec 23, 2017
 *      Author: batuhan
 */

#include "ExperimentApplier.h"

ExperimentApplier::ExperimentApplier(int dataDimension, list<Cell*> *allCells) {

	this->datasetMeasure = new DatasetMeasure();
	this->cellPositionCalculator = new CellPositionCalculator();
	this->hashcodeProducer = new HashcodeProducer();

	this->trainingDataset = new TrainingDataset(dataDimension, allCells);
	this->stochasticCellularAutomata = new StochasticCellularAutomata(this->trainingDataset, this->hashcodeProducer);

	this->datasetMeasure->createDatasetMeasures(allCells);
}

ExperimentApplier::~ExperimentApplier() {
	// TODO Auto-generated destructor stub
}

void ExperimentApplier::prepareCellsForExperiment() {

	this->trainingDataset->clearCellsOfExperiment();

	this->trainingDataset->clearControlPoints();

	this->cellPositionCalculator->calculatePositionsForEachCell(this->rangeLength, this->trainingDataset->getAllInitialCells(), this->datasetMeasure);

	this->trainingDataset->createTrainingset();

	this->rangeLength++;
}

void ExperimentApplier::adjustInitialRangeLengths() {

	this->rangeLength = 10;
	this->maxRangeLength = 20;

	if(this->trainingDataset->getDataDimension()) {
		this->rangeLength = 20;
		this->maxRangeLength = 30;
	}
}

float ExperimentApplier::adjustValidationRatio() {

	float succesRatio = 0.0f;

	int iterationCount = 0;

	// If we break the cellular automata algorithm because of time out, we have to adjust validation ratio!
	while(this->stochasticCellularAutomata->isTimeoutOccured()) {

		this->maxRangeLength = this->rangeLength;
		this->rangeLength = this->rangeLength / 2;

		if(this->rangeLength < 5){
			this->stochasticCellularAutomata->setValidationRatio(this->stochasticCellularAutomata->getValidationRatio() - 0.10f);
			this->rangeLength = 5;
			this->maxRangeLength = 10;
		}

		prepareCellsForExperiment();
		this->stochasticCellularAutomata->run();

		if(!this->stochasticCellularAutomata->isTimeoutOccured()) {
			succesRatio = this->stochasticCellularAutomata->calculateSuccessRatioByValidationPoints();
			cout << "Iteration: " + iterationCount << "Success Ratio: " + to_string(succesRatio) << endl;
		}
		else {
			cout << "Time out is occured. " << "Success Ratio: 0" << endl;
		}
	}

	return succesRatio;
}

float ExperimentApplier::adjustCellularAutomataSize(float succesRatio) {

	float currentSuccessRatio = succesRatio;
	float calculatedSuccessRatio = 0.0f;

	int iterationCount = 0;

	while(this->maxRangeLength > this->rangeLength){
		prepareCellsForExperiment();
		this->stochasticCellularAutomata->run();
		// get results here ...

		if(!this->stochasticCellularAutomata->isTimeoutOccured()) {
			calculatedSuccessRatio = this->stochasticCellularAutomata->calculateSuccessRatioByValidationPoints();
			cout << "Iteration: " + iterationCount << "Success Ratio: " + to_string(succesRatio) << endl;
		}
		else {
			cout << "Time out is occured. " << "Success Ratio: 0" << endl;
		}

		if(calculatedSuccessRatio > currentSuccessRatio) {
			cout << "Previous suc. rat. : " << currentSuccessRatio << endl;
			currentSuccessRatio = calculatedSuccessRatio;
			cout << "New suc. rat. : " << currentSuccessRatio << endl;
		}

		this->rangeLength++;
	}

	return currentSuccessRatio;
}

ExperimentResult ExperimentApplier::run() {

	milliseconds startTimeInMillis = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	long startTimeAsLong = startTimeInMillis.count();

	ExperimentResult *experimentResult = new ExperimentResult();

	adjustInitialRangeLengths();

	float succesRatioByValidationPoints = adjustValidationRatio();

	this->rangeLength++;

	succesRatioByValidationPoints = adjustCellularAutomataSize(succesRatioByValidationPoints);

	cout << "FINAL SUCCESS RATIO by VALIDATION POINTS : " + to_string(succesRatioByValidationPoints) << endl;

	prepareCellsForExperiment();

	this->stochasticCellularAutomata->run();

	float succesRatioByTestPoints = this->stochasticCellularAutomata->calculateSuccessRatioByTestPoints();


	milliseconds endTimeInMillis = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	long endTimeAsLong = endTimeInMillis.count();

	experimentResult->setStartTime(startTimeAsLong);
	experimentResult->setEndTime(endTimeAsLong);
	experimentResult->setSuccessRate(succesRatioByTestPoints);

	return (*experimentResult);
}
