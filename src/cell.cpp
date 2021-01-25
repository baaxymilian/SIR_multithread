/*
 * cell.cpp
 *
 *  Created on: 22 sty 2021
 *      Author: Admin
 */

#include "cell.h"
#include <math.h>
#include <cstdlib>

double risk = 0.1;

sir::Cell::Cell()
	: id{0}, state{State::susceptible}, neighbors{}
{

}

sir::Cell::Cell(const int id)
	: id{id}, state{State::susceptible}, neighbors{}
{

}

auto sir::Cell::setId(int id) -> void{
	this->id = id;
}


auto sir::Cell::getId() -> int{
	return this->id;
}

auto sir::Cell::setState(State state) -> void{
	this->state = state;
}

auto sir::Cell::getState() -> State{
	return this->state;
}

auto sir::Cell::setNeighbors(int size) -> void{
	// Upper row
	if(this->id > size){
		// Same column
		this->neighbors.push_back(this->id - size);
		// Left column
		if(this->id > size - 1 && this->id % size != 0){
			this->neighbors.push_back(this->id - size - 1);
		}
		// Right column
		if(this->id % size != size - 1){
			this->neighbors.push_back(this->id - size + 1);
		}
	}
	// Same row
	// Left column
	if(this->id > 0 && this->id % size != 0)
		this->neighbors.push_back(this->id -1);
	// Right column
	if(this->id < size*size && this->id % size != size - 1)
		this->neighbors.push_back(this->id + 1);

	// Lower row
	if(this->id < size*size - size){
		// Same row
		this->neighbors.push_back(this->id + size);
		// Left column
		if(this->id % size != 0){
			this->neighbors.push_back(this->id + size - 1);
		}
		// Right column
		if(this->id < size*size - 1 && this->id % size != size - 1){
			this->neighbors.push_back(this->id + size + 1);
		}
	}
}

auto sir::Cell::update(double risk_multiplier) -> Cell{
	switch(this->state){
		case susceptible:
			for(auto it = this->neighbors.begin(); it != this->neighbors.end(); ++it){
				auto prob = rand() % 100;
				if(1 - (pow(1 - risk, risk_multiplier)) > prob)
					this->setState(infectous);
			}
			break;
		case infectous:
			--this->recovery_time;
			if(this->recovery_time < 1){
				this->setState(recovered);
			}
			break;
		case recovered:
			break;
		default:
			break;
	}
	return *this;
}
