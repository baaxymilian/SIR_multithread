/*
 * population.h
 *
 *  Created on: 22 sty 2021
 *      Author: Admin
 */

#include <vector>
#include "cell.h"

#ifndef SRC_POPULATION_H_
#define SRC_POPULATION_H_

namespace sir
{
	class Population{
		private:
			int size;
			std::vector<sir::Cell> cells;
		public:
			auto updateRange(int, int) -> Population;
			auto updateSingle(int) -> Cell;
			auto changeCells(std::vector<int>) -> void;
			auto print() -> void;
			auto getCell(int) -> Cell;
			Population(const int);
			Population(const Population&);

	};
}


#endif /* SRC_POPULATION_H_ */
