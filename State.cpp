/*
 * State.cpp
 *
 *  Created on: Mar 4, 2010
 *      Author: smejkalp
 */

#include "State.h"
#include <iostream>

using namespace std;

State::State(matrix *incidence) {
	this->incidence = incidence;
}
/*
State** State::getSuccessor(){


	int numberOfEdges = getNumberOfEdges();

	/*State** successorArray = new State*[numberOfEdges];
	for(int i = 0; i < 5; i++){
		for(int j = 0; j < 5; j++){
			if (incidence[i][j] == 1){
				incidence[i][j] = 0;
				incidence[j][i] = 0;

				State *successorState = new State(incidence);

				incidence[i][j] = 1;
				incidence[j][i] = 1;
			}

		}
	}
}
*/
int State::getNumberOfEdges(){
	int numOnes = 0;
		for(int i = 0; i < 5; i++){
			for(int j = 0; j < 5; j++){
				incidence[i][j] = 1;

				/*if ( == 1){
					numOnes++;
				}*/
			}
		}
	return numOnes/2;
}

State::~State() {
	// TODO Auto-generated destructor stub
}

std::ostream& State::operator<<(std::ostream& os){
	return os << "state wooooohooooo";
}
