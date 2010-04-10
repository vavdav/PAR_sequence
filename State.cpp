/*
 * State.cpp
 *
 *  Created on: Mar 4, 2010
 *      Author: smejkalp
 */

#include "State.h"
#include <iostream>

using namespace std;


State::State(matrix *adjacency, int depth) {
	this->depth = depth;
	this->adjacency = adjacency;
	this->numberOfVertices = this->adjacency->size();
}

State* State::getCopy(){
	State *newState;
	matrix* newAdjacency = new matrix(this->numberOfVertices);
	for(int i = 0; i < this->numberOfVertices;i++ ){
		vector<int> *v = new vector<int>(this->numberOfVertices);
		newAdjacency->at(i) = v;
	}

	for(int i = 0; i < this->numberOfVertices;i++ ){
		for(int j = 0; j < this->numberOfVertices; j++){
				newAdjacency->at(i)->at(j) = this->adjacency->at(i)->at(j);
				newAdjacency->at(j)->at(i) = this->adjacency->at(i)->at(j);
		}
	}
	newState = new State(newAdjacency, this->depth+1);
	return newState;
}

State* State::getStateWithoutEdge(int edgeNumber){
	State *newState = this->getCopy();
	int x=0;
	for(int i = 0; i < this->numberOfVertices; i++){
		for(int j = i+1; j < this->numberOfVertices; j++){
			//cout << "edge no. " << x << "[" << i <<  ", " << j << "]" << endl;
			if(x == edgeNumber){
				newState->adjacency->at(i)->at(j) = 0;
				newState->adjacency->at(j)->at(i) = 0;
				return newState;
			}
			x++;
		}
	}
	return newState;
}


State** State::getSuccessors(){
	int numberOfSuccessors = this->getNumberOfEdges();
	matrix** newAdjacencies = new matrix*[numberOfSuccessors];
	matrix *newAdjacency;
	int iToChange = -1;
	int jToChange = -1;
	bool successorFound;
	vector<int> *v;
	State** states = new State*[numberOfSuccessors];
	for(int adjacency_index = 0; adjacency_index < numberOfSuccessors; adjacency_index++ ){
		matrix* new_adjacency = new matrix(this->numberOfVertices);
		for(int i = 0; i < this->numberOfVertices;i++ ){
			v = new vector<int>(this->numberOfVertices);
			new_adjacency->at(i) = v;
		}
		newAdjacencies[adjacency_index] = new_adjacency;
		newAdjacency = newAdjacencies[adjacency_index];
		successorFound = false;
		for(int i = 0; i < this->numberOfVertices; i++){
			for(int j = i; j < this->numberOfVertices; j++){
				newAdjacency->at(i)->at(j) = this->adjacency->at(i)->at(j);
				newAdjacency->at(j)->at(i) = this->adjacency->at(i)->at(j);
				if(!successorFound && this->numberOfVertices*i+j > iToChange*this->numberOfVertices+jToChange){
					if(newAdjacency->at(i)->at(j) == 1){
						iToChange = i;
						jToChange = j;
						successorFound = true;
					}
				}
			}
		}
		if (successorFound){
			newAdjacency->at(iToChange)->at(jToChange) = 0;
			newAdjacency->at(jToChange)->at(iToChange) = 0;
		}

	}

	for (int i=0; i<numberOfSuccessors; i++){
		states[i] = new State(newAdjacencies[i], this->depth+1);
	}
	return states;
}

void State::print(){
	cout << "State : number of verticies = " << this->numberOfVertices << endl << "*******************" << endl;
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


State::~State() {
	for(int i = 0; i < this->numberOfVertices; i ++){
		delete this->adjacency->at(i);
	}
	delete this->adjacency;
}
