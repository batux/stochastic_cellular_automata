/*
 * ExperimentApplier.h
 *
 *  Created on: Dec 23, 2017
 *      Author: batuhan
 */

#ifndef EXPERIMENTAPPLIER_H_
#define EXPERIMENTAPPLIER_H_

#include "chrono"
#include "string"
#include "ExperimentResult.h"
#include "../training_dataset/DatasetMeasure.h"
#include "../training_dataset/CellPositionCalculator.h"
#include "../cellular_automata/StochasticCellularAutomata.h"

using namespace std;
using namespace std::chrono;

class ExperimentApplier {

public:
	ExperimentApplier(int dataDimension, list<Cell*> *allCells);
	virtual ~ExperimentApplier();
	ExperimentResult run();

private:
	int rangeLength = 10;
	int maxRangeLength = 20;
	StochasticCellularAutomata *stochasticCellularAutomata = NULL;
	TrainingDataset *trainingDataset = NULL;
	HashcodeProducer *hashcodeProducer = NULL;
	DatasetMeasure *datasetMeasure = NULL;
	CellPositionCalculator *cellPositionCalculator = NULL;

	void prepareCellsForExperiment();
	void adjustInitialRangeLengths();
	float adjustValidationRatio();
	float adjustCellularAutomataSize(float succesRatio);
};

#endif /* EXPERIMENTAPPLIER_H_ */
