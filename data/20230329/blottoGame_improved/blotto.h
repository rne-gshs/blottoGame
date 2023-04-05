#ifndef _BLOTTO_H_
#define _BLOTTO_H_

#include <vector>
#include <cstdint>

namespace blt
{
	void gen(uint32_t, uint32_t, std::vector<uint32_t>&);
	int32_t game(std::vector<uint32_t>, std::vector<uint32_t>);
} // blt

#endif //_BLOTTO_H_
