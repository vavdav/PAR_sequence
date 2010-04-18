/*
 * Communicator.h
 *
 *  Created on: 12.4.2010
 *      Author: dawe
 */

#ifndef COMMUNICATOR_H_ERTH56V5YHJY45J35YJ357
#define COMMUNICATOR_H_ERTH56V5YHJY45J35YJ357

#include "mpi.h"
#include "State.h"

class Communicator {

	MPI_Status status;

public:

	static const int REQUEST_WORK = 1;
	static const int SENDING_WORK = 2;
	static const int NO_WORK = 3;
	static const int TOKEN_WHITE = 4;
	static const int TOKEN_BLACK = 5;
	static const int TERMINATE = 6;
	static const int SOLUTION = 7;


	Communicator(int argc, char* argv[]);
	virtual ~Communicator();

	int rank;
	int numProcesses;

	int getRank();
	int getNumProcesses();

	void finalize();

	void sendStack();
	void receiveStack();

	void sendState(State* stateToSend, int proccessorID);
	State* receiveState();

	int hasReceivedMessages();
	int getMessageType();
};

#endif /* COMMUNICATOR_H_ERTH56V5YHJY45J35YJ357 */
