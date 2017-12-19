/*
 * CellListProducer.h
 *
 *  Created on: Dec 5, 2017
 *      Author: batuhan
 */

#ifndef CELLLISTPRODUCER_H_
#define CELLLISTPRODUCER_H_

#include "omp.h"
#include "Cell.h"
#include "Dataset.h"
#include "../hashcode_producer/HashcodeProducer.h"

using namespace std;

class CellListProducer {

public:
	CellListProducer(HashcodeProducer *hashcodeProducer);
	virtual ~CellListProducer();
	list<Cell*> createCellList(Dataset* dataset);

private:
	HashcodeProducer *hashcodeProducer;

};

#endif /* CELLLISTPRODUCER_H_ */
