/*
 * ExperimentResult.h
 *
 *  Created on: Dec 23, 2017
 *      Author: batuhan
 */

#ifndef EXPERIMENTRESULT_H_
#define EXPERIMENTRESULT_H_

using namespace std;

class ExperimentResult {
public:

	ExperimentResult();
	virtual ~ExperimentResult();

	void setStartTime(long startTime);
	void setEndTime(long endTime);
	void setSuccessRate(float successRate);
	float getSuccessRate();

private:
	long startTime;
	long endTime;
	float succesRate;

	float calculateAlgorithmRuntime();
};

#endif /* EXPERIMENTRESULT_H_ */
