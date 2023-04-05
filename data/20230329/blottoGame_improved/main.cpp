#include <iostream>
#include <utility>
#include <vector>
#include <array>
#include <algorithm>
#include <cstdint>
#include <omp.h>
#include "blotto.h"

struct st
{
	std::vector<uint32_t> v;
	uint32_t w;
	uint32_t d;
	uint32_t l;

	st(std::vector<uint32_t> v, uint32_t w, uint32_t d, uint32_t l) : v(std::move(v)), w(w), d(d), l(l) {}
	bool operator < (st &a) const {return w > a.w;}
};

std::vector<st> buff;

void f(int32_t s, std::vector<uint32_t> &t)
{
	if(t.size() == 4)
	{
		t.push_back(s);
		std::array<uint32_t, 3> gameSum{};
		//A[2]: win, A[1]: draw, A[0]:lose

		omp_set_num_threads(19);
		#pragma omp parallel for schedule(dynamic)
		for(uint32_t i = 0; i < 50000; i++)
		{
			std::vector<uint32_t> v;
			blt::gen(5, 20, v);
			int32_t cnt = blt::game(t, v);

			uint8_t result;
			if(cnt > 0) result = 2;
			else if(cnt == 0) result = 1;
			else result = 0;

			#pragma omp atomic
			gameSum[result]++;
		}

		buff.emplace_back(t, gameSum[2], gameSum[1], gameSum[0]);
		t.pop_back();
		return;
	}

	for(int32_t i = s; i >= 0; i--)
	{
		t.push_back(i);
		f(s - i, t);
		t.pop_back();
	}
}

int main()
{
	std::vector<uint32_t> t;
	f(20, t);

	std::cerr << "sorting" << std::endl;

	std::sort(buff.begin(), buff.end());
	for(auto &i:buff)
	{
		for(uint32_t j = 0; j < 5; j++)
			std::cout << i.v[j] << " ";
		std::cout << " win: " << i.w << " draw: " << i.d << " lose: " << i.l << std::endl;
	}

	return 0;
}