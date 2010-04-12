/*
 * Communicator.h
 *
 *  Created on: 12.4.2010
 *      Author: dawe
 */

#ifndef COMMUNICATOR_H_
#define COMMUNICATOR_H_

#include "mpi.h"

class Communicator {
public:
	Communicator();
	virtual ~Communicator();

	int rank;
	int numProcesses;

	int getRank();
	int getNumProcesses();

	void finalize();

	void sendStack();
	void receiveStack();
};

#endif /* COMMUNICATOR_H_ */
