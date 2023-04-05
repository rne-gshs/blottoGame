//
// Created by cygnus330 on 2023-04-05.
//

#include "error.h"

#include <iostream>
#include <string>
#include <vector>

namespace err
{
	void queryErr(std::string name, std::vector<std::string> log, int32_t errorCode)
	{
		std::cerr << "Query Error: " << name << std::endl;
		for(std::string &i:log)
			std::cerr << i << std::endl;
		exit((int)errorCode);
	}
}