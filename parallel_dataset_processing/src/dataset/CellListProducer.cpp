/*
 * CellListProducer.cpp
 *
 *  Created on: Dec 5, 2017
 *      Author: batuhan
 */

#include "CellListProducer.h"

CellListProducer::CellListProducer(HashcodeProducer *hashcodeProducer) {
	this->hashcodeProducer = hashcodeProducer;
}

CellListProducer::~CellListProducer() {
	// TODO Auto-generated destructor stub
}

list<Cell*> CellListProducer::createCellList(Dataset* dataset) {

	list<Cell*> allCells;

	list<DataRow*> dataRows = dataset->getDataRows();

//	#pragma omp parallel
//	{
//		#pragma omp single
//		{
			for(list<DataRow*>::iterator dataRow = dataRows.begin(); dataRow != dataRows.end(); dataRow++) {

//				#pragma omp task firstprivate(dataRow) shared(allCells)
//				{
					//DataRow* clonedDatarow = (*dataRow)->clone();

					Cell* cell = new Cell((*dataRow), this->hashcodeProducer);
					allCells.push_back(cell);
//				}
			}
//		}
//	}

	return allCells;
}

