#pragma GCC optimize("O3")
#pragma GCC target("avx2,avx,fma")

#include <iostream>
#include <vector>
#include <array>
#include <functional>
#include <cstdint>
#include <fstream>
#include <chrono>
#include "blotto.h"
#include "database.h"

void readDB(db::chainDataBase& originalDB, std::string _fileName, uint32_t constituency_size)
{
	std::ifstream read;
	read.open(_fileName);

	if(!read.is_open())
	{
		std::cerr << "File No Exist " << _fileName << std::endl;
		exit(2);
	}

	while(!read.eof())
	{
		std::vector<uint32_t> _v;
		for(unsigned int i = 0; i < constituency_size; i++)
		{
			unsigned int _a;
			read >> _a;

			_v.emplace_back(_a);
		}

		unsigned int w, d, l;
		std::string _w, _d, _l;
		read >> _w >> w >> _d >> d >> _l >> l;

		originalDB.push(w, d, l, _v);
	}
}
void writeDB(db::chainDataBase& originalDB, uint32_t constituency_size, uint32_t citizen_size, double lambda, uint32_t rp, uint32_t gcnt)
{
	std::ofstream write;
	char p[1000];
	sprintf(p, "result/result_%d_%d_%.04lf_%d_%d.txt", constituency_size, citizen_size, lambda, rp, gcnt);
	write.open(p);

	if(!write.is_open())
	{
		std::cerr << "File No Exist " << p << std::endl;
		exit(3);
	}

	auto _DB = originalDB.write();
	for(auto &i:_DB)
	{
		int sz = i.first.size() - 3;
		for(int j = 0; j < sz; j++)
			write << i.first[j] << " ";
		auto p = i.first.end();
		write << "win: " << *(p - 3) << " draw: " << *(p - 2) << " lose: " << *(p - 1) << " val: " << i.second << std::endl;
	}
}

void makeset(std::vector<std::vector<uint32_t>>& v, uint32_t con_size, uint32_t citizen_size)
{
	std::vector<uint32_t> a;

	std::function<void(uint32_t)> f = [&](uint32_t c) -> void
	{
		if(a.size() == con_size - 1)
		{
			a.emplace_back(citizen_size - c);
			v.emplace_back(a);
			a.pop_back();
			return;
		}

		for(uint32_t i = 0; i <= (citizen_size - c); i++)
		{
			a.emplace_back(i);
			f(c + i);
			a.pop_back();
		}
	};

	f(0);
}

int main(int argc, char* argv[])
{
	uint32_t constituency_size, citizen_size, citizen_sum = 0, argCount = 0;
	uint32_t testPerRepeat, testCount;
	double lambda;
	std::vector<uint32_t> partyRuling;

	if(argc == 4)
	{
		constituency_size = std::stoul(argv[argCount++]);
		citizen_size = std::stoul(argv[argCount++]);

		testCount = std::stoul(argv[argCount++]);
		testPerRepeat = std::stoul(argv[argCount++]);
	}
	else
	{
		std::cin >> constituency_size >> citizen_size >> lambda;
		std::cin >> testCount >> testPerRepeat;
	}

	db::chainDataBase originalDB;
	readDB(originalDB, "original.txt", constituency_size);

	std::cerr << "Read Complete" << std::endl;

	std::vector<std::vector<uint32_t>> s;
	makeset(s, constituency_size, citizen_size);

	for(int tcnt = 1; tcnt <= testCount; tcnt++)
	{
		std::cerr << "Remain Epoch " << tcnt << std::endl;
		auto startTime = std::chrono::high_resolution_clock::now();
		db::chainDataBase newDB(lambda);

		const int sz = (int)s.size();
		for(int i = 0; i < sz; i++)
		{
			partyRuling = s[i];
			std::array<uint32_t, 3> r{};
			r.fill(0);

			for(int j = 0; j < testPerRepeat; j++)
			{
				std::vector<uint32_t> partyOppo = originalDB.getTarget();
				int32_t _result = blt::game(partyRuling, partyOppo);
				if(_result == 0) r[1]++;
				else if(_result > 0) r[0]++;
				else r[2]++;
			}

			newDB.push(r[0], r[1], r[2], partyRuling);
		}

		originalDB = newDB;
		writeDB(originalDB, constituency_size, citizen_size, lambda, tcnt, testPerRepeat);

		auto endTime = std::chrono::high_resolution_clock::now();
		auto deltaTime = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();
		std::cerr << deltaTime << "s" << std::endl;
	}

	return 0;
}