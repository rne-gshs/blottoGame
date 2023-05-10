//
// Created by cygnus330 on 2023-04-05.
//

#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <string>
#include <cstdint>
#include <tuple>
#include <iostream>
#include <random>

namespace db
{
	//결과저장
	class chainDataBase
	{
	 protected:
		struct wdl
		{
			wdl()
			{
				win = 0;
				draw = 0;
				lose = 0;
			}

			uint32_t win;
			uint32_t draw;
			uint32_t lose;

			wdl(uint32_t w, uint32_t d, uint32_t l) : win(w), draw(d), lose(l) {};

			bool operator < (const wdl &a) const
			{
				if(this->win == a.win)
				{
					if(this->draw == a.draw)
						return (this->lose < a.lose);

					return (this->draw < a.draw);
				}

				return (this->win < a.win);
			}

			bool operator == (const wdl &a) const
			{
				return (this->win == a.win && this->draw == a.draw && this->lose == a.lose);
			}
		};

		double WRsum = 0;
		uint32_t size = 0;
		std::vector<double> v, vs;
		std::vector<std::vector<std::uint32_t>> vc;

		std::map<std::vector<uint32_t>, wdl> DB_vecToWdl;
		bool isChanged;

	 public:
		chainDataBase()
		{
			this->isChanged = true;
			this->DB_vecToWdl.clear();
		}
		chainDataBase(const chainDataBase& ori)
		{
			this->WRsum = ori.WRsum;
			this->size = ori.size;
			this->v = ori.v;
			this->vs = ori.vs;
			this->vc = ori.vc;

			this->isChanged = ori.isChanged;
			this->DB_vecToWdl = ori.DB_vecToWdl;
		}

		void push(const uint32_t w, const uint32_t d, const uint32_t l, const std::vector<uint32_t>& vpush)
		{
			isChanged = true;

			wdl T = wdl(w, d, l);

			auto vtw = std::make_pair(vpush, T);
			DB_vecToWdl.insert(vtw);
		}
		void mod(const std::vector<uint32_t>& vmod, const uint32_t n, std::string type)
		{
			isChanged = true;

			wdl* pm = &DB_vecToWdl[vmod];
			if(type == "win")
				pm->win += n;
			else if(type == "draw")
				pm->draw += n;
			else if(type == "lose")
				pm->lose += n;
			else
			{
				std::cerr << "Wrong Query " << type << std::endl;
				exit(10011);
			}

		}
		std::vector<uint32_t> getTarget()
		{

			if(isChanged)
			{
				isChanged = false;
				v.resize(0);
				vc.resize(0);
				vs.assign(1, 0.0);

				WRsum = 0;
				size = 0;

				auto beginPtr = DB_vecToWdl.begin();
				auto endPtr = DB_vecToWdl.end();
				for(auto idx = beginPtr; idx != endPtr; idx++)
				{
					uint32_t winCnt = idx->second.win;
					uint32_t gameCnt = winCnt + idx->second.lose + idx->second.draw;
					double WR = winCnt / (double)gameCnt;

					size++;
					WRsum += WR;

					v.emplace_back(WR);
					vs.push_back(WR + vs.back());
					vc.emplace_back(idx->first);
				}
			}

			std::random_device _rd;
			std::uniform_real_distribution<double> _uniform(0, WRsum);
			std::mt19937 _engine(_rd());
			double x = _uniform(_engine);
			unsigned int x_ptr = std::lower_bound(vs.begin(), vs.end(), x) - vs.begin();

			return vc[x_ptr - 1];
		}

		std::vector<std::string> write()
		{
			auto startIdx = DB_vecToWdl.begin();
			auto endIdx = DB_vecToWdl.end();

			std::vector<std::pair<std::vector<uint32_t>, wdl>> originalBuffer;
			for(auto i = startIdx; i != endIdx; i++)
				originalBuffer.emplace_back(*i);

			auto comp = [](auto &a, auto &b)->bool{return !(a.second < b.second);};
			std::sort(originalBuffer.begin(), originalBuffer.end(), comp);

			std::vector<std::string> buffer;
			for(auto &i:originalBuffer)
			{
				wdl& ptr = i.second;

				std::string st = "";
				for(auto &j:i.first)
				{
					std::string _s = std::to_string(j);
					st += _s;
					st += " ";
				}

				char* p = (char *)malloc(sizeof(char) * 1000);
				sprintf(p, "win: %u draw: %u lose: %u", ptr.win, ptr.draw, ptr.lose);
				std::string ptos(p);

				st += ptos;
				buffer.emplace_back(st);
			}

			return buffer;
		}
	};
}

#endif //_DATABASE_H_
