//============================================================================
// Name        : parallel_dataset_processing.cpp
// Author      : Batuhan Duzgun
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "iostream"
#include "queue"
#include "file/FileReader.h"
#include "dataset/Dataset.h"
#include "dataset/DatasetTemplate.h"
#include "dataset/CellListProducer.h"
#include "experiment/ExperimentApplier.h"

using namespace std;

int main() {

	FileReader *fileReader = new FileReader("dataset/Aust.txt");

	list<string> lines = fileReader->getAllLines();

	// (*fileReader).printAllLines(lines);

	list<int> columnIndexes;
	columnIndexes.push_back(4);
	columnIndexes.push_back(7);
	columnIndexes.push_back(9);
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

	int dataDimension = datasetTemplate->getColumnIndexes().size();

	HashcodeProducer *hashcodeProducer = new HashcodeProducer();

	CellListProducer* cellListProducer = new CellListProducer(hashcodeProducer);
	list<Cell*> cellListForTrainingPhase = cellListProducer->createCellList(dataSet);

	cout << "Cell size: " << cellListForTrainingPhase.size() << endl;

	queue<ExperimentResult> experimentResultQueue;
	ExperimentApplier *experimentApplier = new ExperimentApplier(dataDimension, &cellListForTrainingPhase);

	for(int i=0; i < 10; i++) {
		ExperimentResult experimentResult = experimentApplier->run();
		experimentResultQueue.push(experimentResult);
	}

	return 0;
}
