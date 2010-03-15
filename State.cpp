/*
 * State.cpp
 *
 *  Created on: Mar 4, 2010
 *      Author: smejkalp
 */

#include "State.h"
#include <iostream>

using namespace std;


State::State(matrix *incidence, int depth) {
	this->depth = depth;
	this->incidence = incidence;
	this->numberOfVertices = this->incidence->size();
	//cout << "verti = " << this->numberOfVertices << endl;
}

State** State::getSuccessors(){
	int numberOfSuccessors = this->getNumberOfEdges();
	matrix** newIncidences = new matrix*[numberOfSuccessors];
	matrix *newIncidence;
	int iToChange = -1;
	int jToChange = -1;
	bool successorFound;
	vector<int> *v;

	State** states = new State*[numberOfSuccessors];
	for(int incidence_index = 0; incidence_index < numberOfSuccessors; incidence_index++ ){
		matrix* adjacency = new matrix(numberOfSuccessors);
		for(int i = 0; i < numberOfSuccessors;i++ ){
			v = new vector<int>(numberOfSuccessors);
			adjacency->at(i) = v;
		}
		newIncidences[incidence_index] = adjacency;
		newIncidence = newIncidences[incidence_index];
		successorFound = false;
		for(int i = 0; i < this->numberOfVertices; i++){
			//cout << "dbp #4" << endl;
			for(int j = i; j < this->numberOfVertices; j++){
				//cout << "dbp #5" << endl;
				//cout << "newIncidence->at(" << i << ")size() = " << newIncidence->at(i)<< endl;
				newIncidence->at(i)->at(j) = this->incidence->at(i)->at(j);
				//cout << "dbp #6" << endl;
				newIncidence->at(j)->at(i) = this->incidence->at(i)->at(j);
				//cout << this->incidence->at(i).at(j) << " ";
				if(!successorFound && this->numberOfVertices*i+j > iToChange*this->numberOfVertices+jToChange){
					if(newIncidence->at(i)->at(j) == 1){
						iToChange = i;
						jToChange = j;
						successorFound = true;
					}
				}
			}
			//cout << endl;
		}
		if (successorFound){
			newIncidence->at(iToChange)->at(jToChange) = 0;
			newIncidence->at(jToChange)->at(iToChange) = 0;
		}

	}

	for (int i=0; i<numberOfSuccessors; i++){
		states[i] = new State(newIncidences[i], this->depth+1);
	}
	//cout << "this->numberOfVerticesss = " << this->numberOfVertices << endl;
	return states;

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
	cout << "State : verticies = " << this->numberOfVertices << " rly" << endl << "*******************" << endl;
	for(int i = 0; i < 5; i++){
		for(int j = 0; j < 5; j++){
			cout << this->incidence->at(i)->at(j) << " ";
		}
		cout << endl;
	}
	cout << "*******************" << endl;
}

bool State::isBipartite(){
	int* color = new int[this->numberOfVertices];
	int* d = new int[this->numberOfVertices];
	int* p = new int[this->numberOfVertices];
	for (int i=0; i<this->numberOfVertices; i++){
		color[i] = 0;
		d[i] = 99999;
		p[i] = 0;
	}
	int pom;
	int start = 0;
	queue <int> bfsFront;
	color[start] = 1;
	d[start] = 0;
	bfsFront.push(start);
	int path1Length;
	int path2Length;

	while (!bfsFront.empty()){
		pom = bfsFront.front();
		//cout << "in vertex " << pom << endl;
		bfsFront.pop();
		for (int i=0; i<this->numberOfVertices; i++){
			if(this->incidence->at(pom)->at(i) == 1){
				if (color[i] == 0) {
					color[i] = 1;
					d[i] = d[pom] + 1;
					p[i] = pom;
					bfsFront.push(i);
				} else {
					//cout << "I was here " << i << " old d=" << d[i]%2 << " ?= " << (d[pom] + 1)%2 << endl;
					path1Length = (d[i])%2;
					path2Length = (d[pom] + 1)%2;
					if(path1Length != path2Length){
						return false;
					}
				}


			}
		color[pom] = 2;
		}
	}
	return true;
}

int State::getNumberOfSuccessors(int index){
	int numOnes = 0;
	for(int j = 0; j < this->numberOfVertices; j++){
		if (this->incidence->at(index)->at(j) == 1){
			numOnes++;
		}
	}
	return numOnes;
}

int State::getNumberOfEdges(){
	int numOnes = 0;
	for(int i = 0; i < this->numberOfVertices; i++){
		for(int j = 0; j < this->numberOfVertices; j++){
			if (this->incidence->at(i)->at(j) == 1){
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
