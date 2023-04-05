//
// Created by cygnus330 on 2023-04-05.
//

#include "database.h"

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include "error.h"

namespace db
{
	dataBase::wdl::wdl(const uint32_t w, const uint32_t d, const uint32_t l) : win(w), draw(d), lose(l) {}
	dataBase::result::result(const uint32_t w, const uint32_t d, const uint32_t l, const std::vector<uint32_t> &v) : db::dataBase::wdl(w, d, l), batch(v) {}
	void dataBase::push(uint32_t w, uint32_t d, uint32_t l, const std::vector<uint32_t> &v) {data.emplace_back(w, d, l, v);}
	void dataBase::add(const std::string T, std::vector<uint32_t> &b)
	{
		result A = result(0, 0, 0, b);
		auto idx = std::find(data.begin(), data.end(), A);
		if(idx == data.end())
			err::queryErr("db::dataBase::add()", std::vector<std::string>{"Wrong Vector"}, 10000);

		if(T == "win") idx->win++;
		else if(T == "draw") idx->draw++;
		else if(T == "lose") idx->lose++;
		else err::queryErr("db::dataBase::add()", std::vector<std::string>{"Wrong Query", T}, 10001);
	}
	void dataBase::addN(const std::string T, std::vector<uint32_t> &b, int32_t n)
	{
		result A = result(0, 0, 0, b);
		auto idx = std::find(data.begin(), data.end(), A);
		if(idx == data.end())
			err::queryErr("db::dataBase::addN()", std::vector<std::string>{"Wrong Vector"}, 10000);

		if(T == "win") idx->win += n;
		else if(T == "draw") idx->draw += n;
		else if(T == "lose") idx->lose += n;
		else err::queryErr("db::dataBase::addN()", std::vector<std::string>{"Wrong Query", T}, 10001);
	}
	std::tuple<int, int, int> dataBase::get(const std::vector<uint32_t> b)
	{
		result A = result(0, 0, 0, b);
		auto idx = std::find(data.begin(), data.end(), A);
		if(idx == data.end())
		{
			std::cerr << "Not In DB" << std::endl;
			std::cerr << "Query db::dataBase::get()" << std::endl;
			for(auto &i:b)
				std::cerr << i << " ";
			std::cerr << std::endl;
			exit(10000);
		}

		return std::tuple<int, int, int>{idx->win, idx->draw, idx->lose};
	}
} // db