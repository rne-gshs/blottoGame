//
// Created by cygnus330 on 2023-04-05.
//

#include "blotto.h"

#include <iostream>
#include <vector>
#include <array>
#include <random>
#include <cmath>
#include <cstdint>

std::random_device rd;
std::mt19937 mt_engine(rd());

namespace blt
{
	// 제너레이터
	void gen(uint32_t n, uint32_t k, std::vector<uint32_t> &b) // 1000
	{
		b.resize(n);
		std::vector<bool> v(n+k-1, false);
		std::fill(v.begin(), v.begin() + k, true);
		//std::fill(v.begin() + k, v.end(), false);
		std::shuffle(v.begin(), v.end(), mt_engine);

		uint32_t cnt = 0, idx = 0, sz = v.size();
		for(uint32_t i = 0; i < sz; i++)
		{
			if(v[i])
				cnt++;
			else
			{
				b[idx++] = cnt;
				cnt = 0;
			}
		}
		b[n-1] = cnt;
	}

	// 게임 진행
	int32_t game(std::vector<uint32_t>& a, std::vector<uint32_t>& b) // 2000
	{
		uint32_t sz = a.size();
		if(sz != b.size())
		{
			std::cerr << "blt::game" << std::endl;
			std::cerr << "wrong array size " << a.size() << " " << b.size() << std::endl;
			exit(2000);
		}

		std::array<uint32_t, 3> w{};
		//w[2]: win, w[1]: draw, w[0]:lose
		for(uint32_t i = 0; i < sz; i++)
		{
			int64_t t = (int64_t)a[i] - (int64_t)b[i];

			if(t > 0) w[2]++;
			else if(t == 0) w[1]++;
			else w[0]++;
		}

		return (int32_t)(w[2] - w[0]);
	}

} // blt