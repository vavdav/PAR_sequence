/*
 * Communicator.h
 *
 *  Created on: 12.4.2010
 *      Author: dawe
 */

#ifndef COMMUNICATOR_H_ERTH56V5YHJY45J35YJ357
#define COMMUNICATOR_H_ERTH56V5YHJY45J35YJ357

#include "mpi.h"

class Communicator {
public:
	Communicator(int argc, char* argv[]);
	virtual ~Communicator();

	int rank;
	int numProcesses;

	int getRank();
	int getNumProcesses();

	void finalize();

	void sendStack();
	void receiveStack();
};

#endif /* COMMUNICATOR_H_ERTH56V5YHJY45J35YJ357 */
