/*
 * HascodeProducer.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: batuhan
 */

#include "HashcodeProducer.h"

HashcodeProducer::HashcodeProducer() {
	// TODO Auto-generated constructor stub

}

HashcodeProducer::~HashcodeProducer() {
	// TODO Auto-generated destructor stub
}

int HashcodeProducer::createHashCode(vector<int> &positions) {

	int value = 0 ;

	string positionsAsText = preparePositionsAsText(positions);

	for(char &letter : positionsAsText) {
		value = (37 * value) + letter;
	}

	return value;
//
//	 int h = hash;
//	        if (h == 0 && value.length > 0) {
//	            char val[] = value;
//
//	            for (int i = 0; i < value.length; i++) {
//	                h = 31 * h + val[i];
//	            }
//	            hash = h;
//	        }
//	        return h;

}

string HashcodeProducer::preparePositionsAsText(vector<int> &positions) {

	stringstream stringStream;

	int positionSize = positions.size();

	if(positionSize > 0) {
		for(int pValue : positions) {
			stringStream << pValue;
		}
	}
	else {
		stringStream << "";
	}

	string positionsAsText = stringStream.str();
	return positionsAsText;
}
