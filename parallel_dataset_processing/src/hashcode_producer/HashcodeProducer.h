/*
 * HascodeProducer.h
 *
 *  Created on: Dec 17, 2017
 *      Author: batuhan
 */

#ifndef HASCODEPRODUCER_H_
#define HASCODEPRODUCER_H_

#include "string"
#include "sstream"
#include "vector"

using namespace std;

class HashcodeProducer {

public:
	HashcodeProducer();
	virtual ~HashcodeProducer();
	unsigned int createHashCode(vector<int> &positions);

private:
	string preparePositionsAsText(vector<int> &positions);
};

#endif /* HASCODEPRODUCER_H_ */
