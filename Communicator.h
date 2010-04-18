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
#include <stack>

using namespace std;

class Communicator {

public:

	static const int REQUEST_WORK = 1;
	static const int SENDING_WORK = 2;
	static const int SENDING_WORK_SIZE = 8;
	static const int NO_WORK = 3;
	static const int TOKEN_WHITE = 4;
	static const int TOKEN_BLACK = 5;
	static const int TERMINATE = 6;
	static const int SOLUTION = 7;


	Communicator(int argc, char* argv[]);
	virtual ~Communicator();

	MPI_Status status;

	int sizeSetForProccessorID;
	int stackSize;
	int stateSize;
	int numOfVertices;

	bool isWaiting;
	bool hasSentToken;

	int processorToAskForWork;


	void sendWhiteToken(int toProccessor);
	void sendBlackToken(int toProccessor);

	int rank;
	int numProcesses;

	int getRank();
	int getNumProcesses();

	void finalize();

	void sendStack(stack<State*> *stack, int proccessorID);
	void receiveStack(stack<State*> *stack, int proccessorID);
	void receiveStackSize();
	void requestWork();

	void sendTerminateToAll();
	void sendNoWork(int processorID);
	void sendState(State* stateToSend, int proccessorID);
	State* receiveState();
	void receiveBestSolution();
	int hasReceivedMessages();
	int getMessageType();
};

#endif /* COMMUNICATOR_H_ERTH56V5YHJY45J35YJ357 */
