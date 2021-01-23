/*
 * population.cpp
 *
 *  Created on: 22 sty 2021
 *      Author: Admin
 */

#include "population.h"
#include <iostream>

sir::Population::Population(const int size)
{
	auto id = 0;
	this->size = size;
	this->cells.resize(size*size);
	this->cells.assign(size*size, sir::Cell(id));
	for(auto it = this->cells.begin(); it !=  this->cells.end(); ++it){
		it->setId(id);
		it->setNeighbors(size);
		id++;
		/*std::cout << it->getId() << ": ";
		for(auto itt = it->neighbors.begin(); itt != it->neighbors.end(); ++itt){
			std::cout << *itt << " ";
		}
		std::cout << std::endl;*/
	}
}

sir::Population::Population(const Population &pop)
{
	this->size = pop.size;
	this->cells.resize(pop.size*pop.size);
	this->cells = pop.cells;
}

auto sir::Population::print() -> void{
	std:: cout << std::endl;
	auto state = susceptible;
	for(auto it = this->cells.begin(); it !=  this->cells.end(); ++it){

		state = it->getState();
		switch(state){
			case susceptible:
				std::cout << "o";
				break;
			case infectous:
				std::cout << "x";
				break;
			case recovered:
				std::cout << "+";
				break;
			default:
				std::cout << "o";
		}
		if((it->getId() + 1) % (this->size) == 0)
			std::cout << std::endl;

	}
	std:: cout << std::endl;
}

auto sir::Population::changeCells(std::vector<int> l) -> void{
	auto cit = this->cells.begin();
	for(auto it = l.begin(); it != l.end(); ++it){
		if(*it < this->size*size && *it > 0){
			cit = this->cells.begin() + *it;
			cit->setState(infectous);
		}
	}
}

auto sir::Population::updateSingle(int id) -> Cell{
	auto risk_multiplier = 0;
	auto neighbors = this->cells[id].neighbors;
	auto n_size = neighbors.size();
	for(auto i = 0; i < n_size; i++){
		if(this->cells[neighbors[i]].getState() == infectous)
			risk_multiplier++;
	}
		return this->cells[id].update(risk_multiplier);
}

auto sir::Population::updateRange(int begin, int end) -> Population{
	Population temp_pop(*this);
	for(auto i = begin; i < end; i++){
		temp_pop.cells[i] = this->updateSingle(this->cells[i].getId());
	}
	return temp_pop;
}


auto sir::Population::getCell(int id) -> Cell{
	return this->cells[id];
}
