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

int Comunicator::getRank(){
	return rank;
}

int Comunicator::getNumProcesses(){
	return numProcesses;
}

void Comunicator::finalize(){
	MPI_Finalize();
}

void Comunicator::sendStack(){
	//todo
}

void Comunicator::receiveStack(){
	//todo
>>>>>>> 22988f890d6e49c75ad6a973bf9b256a69f024a2
}

Communicator::~Communicator() {

}
