/*
 * Communicator.cpp
 *
 *  Created on: 12.4.2010
 *      Author: dawe
 */

#include "Communicator.h"

Communicator::Communicator(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
}

int Communicator::getRank(){
	return rank;
}

int Communicator::getNumProcesses(){
	return numProcesses;
}

void Communicator::finalize(){
	MPI_Finalize();
}

void Communicator::sendStack(){
	//todo
}

void Communicator::receiveStack(){
	//todo
}

Communicator::~Communicator() {

}
