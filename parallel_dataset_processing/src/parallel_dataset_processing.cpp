//============================================================================
// Name        : parallel_dataset_processing.cpp
// Author      : Batuhan Duzgun
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "iostream"
#include "file/FileReader.h"
#include "dataset/Dataset.h"
#include "dataset/DatasetTemplate.h"
#include "dataset/CellListProducer.h"
#include "training_dataset/DatasetMeasure.h"
#include "training_dataset/CellPositionCalculator.h"
#include "hashcode_producer/HashcodeProducer.h"

using namespace std;

int main() {

	FileReader *fileReader = new FileReader("dataset/Aust.txt");

	list<string> lines = fileReader->getAllLines();

	// (*fileReader).printAllLines(lines);

	list<int> columnIndexes;
	columnIndexes.push_back(2);
	columnIndexes.push_back(3);
	columnIndexes.push_back(4);
	columnIndexes.push_back(12);
	columnIndexes.push_back(13);

	DatasetTemplate *datasetTemplate = new DatasetTemplate();
	datasetTemplate->setName("aust");
	datasetTemplate->setDimension(14);
	datasetTemplate->setColumnIndexes(columnIndexes);

	Dataset *dataSet = new Dataset(datasetTemplate, lines, false);

	cout << "Dataset preparing process..." << endl;
	dataSet->createDataset();

	cout << "Dataset printing process..." << endl;
	dataSet->printAllDataRows();

	int dataDimension = dataSet->getDatasetTemplate()->getDimension();

	HashcodeProducer *hashcodeProducer = new HashcodeProducer();

	CellListProducer* cellListProducer = new CellListProducer(hashcodeProducer);
	list<Cell*> cellListForTrainingPhase = cellListProducer->createCellList(dataSet);

	cout << "Cell size: " << cellListForTrainingPhase.size() << endl;

	list<Cell*> *cellListForTrainingPhasePtr = &cellListForTrainingPhase;
	DatasetMeasure *datasetMeasure = new DatasetMeasure(cellListForTrainingPhasePtr);

	vector<float> minVal = datasetMeasure->getMinimumValues();
	vector<float> maxVal = datasetMeasure->getMaximumValues();
	vector<float> rangeVal = datasetMeasure->getRangeValues();

	int rangeLength = 10;

	CellPositionCalculator *cellPositionCalculator = new CellPositionCalculator();
	cellListForTrainingPhasePtr = cellPositionCalculator->calculatePositionsForEachCell(rangeLength, cellListForTrainingPhasePtr, datasetMeasure);

	int counter = 1;
	for(list<Cell*>::iterator cell = cellListForTrainingPhase.begin(); cell != cellListForTrainingPhase.end(); cell++) {
		cout << "Index: " << counter << (*cell)->toString() << endl;
		counter++;
	}

	return 0;
}
