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
	ControlPoint(int hashCodeOfCell, int labelValue, int controlPointType);
	virtual ~ControlPoint();
	int getHashCodeOfCell();
	int getLabelValue();
	int getControlPointType();

private:
	int hashCodeOfCell;
	int labelValue;
	// 1: Test point, 2: Validation point
	int controlPointType = -1;

};

#endif /* CONTROLPOINT_H_ */
