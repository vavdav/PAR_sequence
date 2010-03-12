/*
 * State.cpp
 *
 *  Created on: Mar 4, 2010
 *      Author: smejkalp
 */

#include "State.h"
#include <iostream>


State::State(matrix *incidence) {
	vector<int> *v;
	this->incidence = incidence;

	for(int i = 0; i < 5; i++){
				for(int j = 0; j < 5; j++){
					int x = this->incidence->at(i).at(j);

					/*if ( == 1){
						numOnes++;
					}*/
				}
				cout << endl;
			}
}

/*State** State::getSuccessor(){
	int numOnes = 0;
	for(int i = 0; i < 5; i++){
		for(int j = 0; j < 5; j++){
			//if (incidence[i][j] == 1){
				numOnes++;
			}
		}
	}
	/*numOnes = numOnes/2;
	State** successorArray = new State*[numOnes];
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
}*/

int State::getNumberOfEdges(){
	int numOnes = 0;
		for(int i = 0; i < 5; i++){
			for(int j = 0; j < 5; j++){
				//incidence[i][j] = 1;

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
