#include <iostream>
#include <utility>
#include <vector>
#include <array>
#include <algorithm>
#include <cstdint>
#include <tuple>
#include <omp.h>
#include "blotto.h"
#include "database.h"

int main()
{
	// test code
	/*std::vector<uint32_t> A = {1, 1, 1, 1, 1}, B = {0, 0, 1, 2, 2};
	db::dataBase a;
	a.push(0, 0, 0, A);
	a.push(0, 0, 0, B);

	a.add("draw", A);
	a.add("win", B);
	a.add("draw", A);
	a.addN("draw", A, 100);

	auto T = a.get(std::vector<uint32_t>{1, 1, 1, 1, 1});
	std::cout << std::get<1>(T);*/
}