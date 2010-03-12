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
	this->numberOfVertices = this->incidence->size();
}

State* State::getSuccessors(){

	int numberOfSuccessors = this->getNumberOfEdges();
	matrix** newIncidences = new matrix*[numberOfSuccessors];
	matrix *newIncidence;
	int iToChange = -1;
	int jToChange = -1;
	bool successorFound;



	for(int incidence_index = 0; incidence_index < numberOfSuccessors; incidence_index++ ){
		newIncidences[incidence_index] = new matrix(this->numberOfVertices, vector<int>(this->numberOfVertices));
		newIncidence = newIncidences[incidence_index];
		successorFound = false;
		for(int i = 0; i < this->numberOfVertices; i++){
			for(int j = i; j < 5; j++){
				newIncidence->at(i).at(j) = this->incidence->at(i).at(j);
				newIncidence->at(j).at(i) = this->incidence->at(i).at(j);
				cout << this->incidence->at(i).at(j) << " ";
				if(!successorFound){
					if(newIncidence->at(i).at(j) == 1){
						iToChange = i;
						jToChange = j;
						successorFound = true;
					}
				}
			}
			cout << endl;
		}
		if (iToChange >= 0){
			newIncidence->at(iToChange).at(jToChange) = 0;
			newIncidence->at(jToChange).at(iToChange) = 0;
		}
	}





	State *newState = new State(newIncidence);

	return newState;

	/*
	int numOnes = getNumberOfEdges();
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
	return successorArray;*/
}


void State::print(){
	cout << "State : V = " << this->numberOfVertices << endl << "*******************" << endl;
	for(int i = 0; i < 5; i++){
		for(int j = 0; j < 5; j++){
			cout << this->incidence->at(i).at(j) << " ";
		}
		cout << endl;
	}
	cout << "*******************" << endl;
}


int State::getNumberOfEdges(){
	int numOnes = 0;
	for(int i = 0; i < this->numberOfVertices; i++){
		for(int j = 0; j < this->numberOfVertices; j++){
			if (this->incidence->at(i).at(j) == 1){
				numOnes++;
			}
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
