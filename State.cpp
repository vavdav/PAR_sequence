/*
 * State.cpp
 *
 *  Created on: Mar 4, 2010
 *      Author: smejkalp
 */

#include "State.h"
#include <iostream>

using namespace std;


State::State(matrix *adjacency, int depth, int edgeIndex) {
	this->depth = depth;
	this->adjacency = adjacency;
	this->edgeIndex = edgeIndex;
	this->numberOfVertices = this->adjacency->size();
}


State** State::getSuccessors(){
	matrix** newAdjacencies = new matrix*[2];
	matrix *newAdjacency;
	int iToChange = -1;
	int jToChange = -1;
	bool successorFound;
	vector<int> *v;
	State** states = new State*[2];
	//cout << "get successors" << endl;
	successorFound = false;
	for(int adjacency_index = 0; adjacency_index < 2; adjacency_index++ ){
		matrix* new_adjacency = new matrix(this->numberOfVertices);
		for(int i = 0; i < this->numberOfVertices; i++){
			v = new vector<int>(this->numberOfVertices);
			new_adjacency->at(i) = v;
		}
		newAdjacencies[adjacency_index] = new_adjacency;
		newAdjacency = newAdjacencies[adjacency_index];
		for(int i = 0; i < this->numberOfVertices; i++){
			for(int j = i; j < this->numberOfVertices; j++){
				newAdjacency->at(i)->at(j) = this->adjacency->at(i)->at(j);
				newAdjacency->at(j)->at(i) = this->adjacency->at(i)->at(j);
				//POROVNAM ZDA JSEM ZA INDEXEM AKTUALNI HO SOUSEDA
				if(!successorFound && this->numberOfVertices*i+j > edgeIndex){
					if(newAdjacency->at(i)->at(j) == 1){
						iToChange = i;
						jToChange = j;
						successorFound = true;
					}
				}
			}
		}
	}
	if (successorFound){
		newAdjacencies[0]->at(iToChange)->at(jToChange) = 0;
		newAdjacencies[0]->at(jToChange)->at(iToChange) = 0;
	}

	for (int i=0; i<2; i++){
		states[i] = new State(newAdjacencies[i], this->depth+1, this->numberOfVertices*iToChange+jToChange);
	}
	return states;
}

void State::print(){
	//cout << "State : number of verticies = " << this->numberOfVertices << endl << "*******************" << endl;
	for(int i = 0; i < this->numberOfVertices; i++){
		for(int j = 0; j < this->numberOfVertices; j++){
			cout << this->adjacency->at(i)->at(j) << " ";
		}
		cout << endl;
	}
	cout << "*******************" << endl;
}

int State::isBipartite(){
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

	int bipartite = 1;

	while (!bfsFront.empty()){
		pom = bfsFront.front();
		//cout << "in vertex " << pom << endl;
		bfsFront.pop();
		for (int i=0; i<this->numberOfVertices; i++){
			if(this->adjacency->at(pom)->at(i) == 1){
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
						//return false;
						bipartite = 0;
						//return 0;
					}
				}
			}
		color[pom] = 2;
		}
	}
	for(int i = 0; i<this->numberOfVertices; i++){
		if(color[i] == 0) {
			bipartite = -1;
		}
	}

	return bipartite;
}

int State::getNumberOfSuccessors(int index){
	int numOnes = 0;
	for(int j = 0; j < this->numberOfVertices; j++){
		if (this->adjacency->at(index)->at(j) == 1){
			numOnes++;
		}
	}
	return numOnes;
}

int State::getNumberOfEdges(){
	int numOnes = 0;
	for(int i = 0; i < this->numberOfVertices; i++){
		for(int j = 0; j < this->numberOfVertices; j++){
			if (this->adjacency->at(i)->at(j) == 1){
				numOnes++;
			}
		}
	}
	return numOnes/2;
}

char * State::serialize(char * buffer, int position){
	int length = sizeof(int)*numberOfVertices*numberOfVertices+sizeof(int)*2;
	char ZERO = 0;
	char ONE = 1;

	MPI_Pack(&depth, 1, MPI_INT, buffer, length, &position, MPI_COMM_WORLD);
	MPI_Pack(&edgeIndex, 1, MPI_INT, buffer, length, &position, MPI_COMM_WORLD);

	for(int i = 0; i < numberOfVertices; i++){
		for(int j = 0; j < numberOfVertices; j++){
			if(this->adjacency->at(i)->at(j)){
				MPI_Pack(&ONE, 1, MPI_CHAR, buffer, length, &position, MPI_COMM_WORLD);
			} else {
				MPI_Pack(&ZERO, 1, MPI_CHAR, buffer, length, &position, MPI_COMM_WORLD);
			}
		}
	}
	return buffer;
}

State* State::deserialize(char* buffer, int position, int numberOfVertices){
	char ZERO = 0;
	char ONE = 1;

	int length = sizeof(int)*numberOfVertices*numberOfVertices+sizeof(int)*2;

	int depth;
	MPI_Unpack(buffer, length, &position, &depth, 1, MPI_INT, MPI_COMM_WORLD);
	int edges;
	MPI_Unpack(buffer, length, &position, &edges, 1, MPI_INT, MPI_COMM_WORLD);

	char isEdge;

	matrix* adjacency = new matrix(numberOfVertices);
	for(int i = 0; i < numberOfVertices; i++){
		adjacency->at(i) = new vector<int>(numberOfVertices);
		for(int j = 0; j < numberOfVertices; j++){
			MPI_Unpack(buffer, length, &position, &isEdge, 1, MPI_CHAR, MPI_COMM_WORLD);
			if(isEdge == ONE){
				adjacency->at(i)->at(j) = 1;
			} else {
				adjacency->at(i)->at(j) = 0;
			}
		}
	}
	State *newState = new State(adjacency, depth, edges);

	return newState;
}

State::~State() {
	for(int i = 0; i < this->numberOfVertices; i ++){
		delete this->adjacency->at(i);
	}
	delete this->adjacency;
}
