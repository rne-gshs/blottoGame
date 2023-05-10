#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <cstdint>
#include <omp.h>
#include <fstream>
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
void writeDB(db::chainDataBase& originalDB, std::string _fileName, uint32_t constituency_size)
{
	std::ofstream write;
	write.open(_fileName);

	if(!write.is_open())
	{
		std::cerr << "File No Exist " << _fileName << std::endl;
		exit(3);
	}

	std::vector<std::string> _DB = originalDB.write();
	for(auto &i:_DB)
		write << i << std::endl;
}

int main()
{
	uint32_t constituency_size, citizen_size, citizen_sum = 0;
	std::vector<uint32_t> partyRuling;

	std::cin >> constituency_size >> citizen_size;
	for(unsigned int i = 0; i < constituency_size; i++)
	{
		uint32_t _a;
		std::cin >> _a;

		citizen_sum += _a;
		partyRuling.emplace_back(_a);
	}

	if(citizen_size != citizen_sum)
	{
		std::cerr << "Wrong Input : k = " << citizen_size << ", but your citizen size = " << citizen_sum << std::endl;
		exit(1);
	}

	db::chainDataBase originalDB;
	readDB(originalDB, "original.txt", constituency_size);

	uint32_t repeat_num, testPerRepeat;
	std::cin >> repeat_num >> testPerRepeat;
	std::cerr << "Read Complete" << std::endl;

	while(repeat_num-->0)
	{
		db::chainDataBase newDB(originalDB);

		for(unsigned int i = 0; i < testPerRepeat; i++)
		{
			std::vector<uint32_t> partyOppo = originalDB.getTarget();
			int32_t _result = blt::game(partyRuling, partyOppo);
			if(_result == 0) newDB.mod(partyOppo, 1, "draw");
			else if(_result > 0) newDB.mod(partyOppo, 1, "win");
			else newDB.mod(partyOppo, 1, "lose");
		}

		originalDB = newDB;
	}

	writeDB(originalDB, "result.txt", constituency_size);

	return 0;
}