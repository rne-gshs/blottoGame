//
// Created by cygnus330 on 2023-04-05.
//

#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <vector>
#include <map>
#include <string>
#include <cstdint>
#include <tuple>

namespace db
{
	// 게임 결과저장
	class dataBase
	{
	 private:
		struct wdl
		{
		 public:
			uint32_t win;
			uint32_t draw;
			uint32_t lose;

			wdl(uint32_t w, uint32_t d, uint32_t l);
		};
		struct result : public db::dataBase::wdl
		{
			std::vector<uint32_t> batch;

			bool operator < (result &a) const {return this->win > a.win;}
			bool operator == (result &a) const {return this->batch == a.batch;}
			result(uint32_t w, uint32_t d, uint32_t l, const std::vector<uint32_t> &v);
		};
		//legacy
		//std::vector<db::dataBase::result> data;
		std::map<std::vector<uint32_t>, db::dataBase::wdl> dataMap;

	 public:
		void push(uint32_t w, uint32_t d, uint32_t l, const std::vector<uint32_t> &v);
		void add(std::string T, std::vector<uint32_t> &b);
		void addN(std::string T, std::vector<uint32_t> &b, int32_t n);
		std::tuple<int, int, int> get(std::vector<uint32_t> b);
	};
}

#endif //_DATABASE_H_
