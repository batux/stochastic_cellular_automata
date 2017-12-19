/*
 * ControlPoint.h
 *
 *  Created on: Dec 10, 2017
 *      Author: batuhan
 */

#ifndef CONTROLPOINT_H_
#define CONTROLPOINT_H_

using namespace std;

#include "string"

using namespace std;

class ControlPoint {

public:
	ControlPoint(unsigned int hashCodeOfCell, int labelValue);
	virtual ~ControlPoint();
	unsigned int getHashCodeOfCell();
	int getLabelValue();

private:
	unsigned int hashCodeOfCell;
	int labelValue;

};

#endif /* CONTROLPOINT_H_ */
