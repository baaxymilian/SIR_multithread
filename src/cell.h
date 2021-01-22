/*
 * cell.h
 *
 *  Created on: 22 sty 2021
 *      Author: Bartek
 */

#ifndef CELL_H_
#define CELL_H_

#include <vector>

namespace sir
{
	enum State{
		susceptible,
		infectous,
		recovered
	};

	class Cell
	{
	private:
		int id;
		int recovery_time = 14;
		State state;

	public:
		std::vector<int> neighbors;
		auto setId(int) -> void;
		auto getId() -> int;
		auto setState(State) -> void;
		auto getState() -> State;
		auto setNeighbors(int) -> void;
		auto update(double) -> Cell;
		Cell();
		Cell(int id);
	};
}



#endif /* CELL_H_ */
