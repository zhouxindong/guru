#pragma once
#ifndef _GURU_DIGEST_H_
#define _GURU_DIGEST_H_

#include "../gvals.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>

_GURU_BEGIN

/*
** digest used md5
*/
class digest_md5 
{
public:
	digest_md5() noexcept
	{
		_reset();
	}

	~digest_md5() noexcept
	{
		delete[] _block;
	}

	digest_md5& 
	str_md5(const char* s) noexcept
	{
		_str = s;
		_reset();
		_calc_str();
		return *this;
	}

	digest_md5& 
	file_md5(const char* path) 
	{
		_path = path;
		_reset();
		_calc_file();
		return *this;
	}

	std::string get_md5() 
	{
		std::stringstream ss;

		uint64_t buf[4] = { A, B, C, D };
		for (int i = 0; i < 4; ++i) 
		{
			unsigned char *tmp = (unsigned char *)&buf[i];
			for (int j = 0; j < 4; ++j) 
			{
				ss << std::setfill('0') << std::setw(2) << std::hex << (int)tmp[j];
			}
		}

		return ss.str();
	}

private:
	void 
	_reset() noexcept
	{
		A = 0x67452301;
		B = 0xefcdab89;
		C = 0x98badcfe;
		D = 0x10325476;
	}

	void
	_calc_str() noexcept
	{
		if (_block != nullptr) 
		{
			delete[](_block);
			_block = nullptr;
		}
		unsigned long length = 0;
		while (1)
		{
			int len = _read_str(_str);
			if (len == 64) 
			{
				length += len;
				_str += len;
				_calc_chunk((uint32_t *)_chunk);
			}
			else 
			{
				length += len;
				_file_size = length * 8;
				unsigned char *rep = _fill(len);
				_calc_chunk((uint32_t *)_chunk);
				if (rep) 
				{
					_calc_chunk((uint32_t *)_chunk);
				}
				break;
			}
		}
	}

	void 
	_calc_file() 
	{
		if (_block != nullptr)
		{
			delete[](_block);
			_block = nullptr;
		}
		std::streamsize length = 0;
		std::fstream fs;
		fs.open(_path, std::fstream::binary | std::fstream::in);
		while (1) 
		{
			fs.read((char *)_chunk, 64);
			if (fs.gcount() == 64) 
			{
				length += fs.gcount();
				_calc_chunk((uint32_t *)_chunk);
			}
			else 
			{
				length += fs.gcount();
				_file_size = length * 8;
				unsigned char *rep = _fill(fs.gcount());
				_calc_chunk((uint32_t *)_chunk);
				if (rep) 
				{
					_calc_chunk((uint32_t *)rep);
				}
				break;
			}
		}
	}

	unsigned char *
	_fill(std::streamsize n) noexcept
	{
		if (n >= 56) 
		{
			auto len = 64 - n;
			_chunk[n] = 0x80;
			for (auto i = n + 1; i < 64; ++i) 
			{
				_chunk[i] = 0x0;
			}
			_block = new unsigned char[64]();
			unsigned char *slide = _block + 56;
			(*((uint64_t *)slide)) = _file_size;
		}
		else 
		{
			_chunk[n] = 0x80;
			for (auto i = n + 1; i < 57; ++i) 
			{
				_chunk[i] = 0x0;
			}
			_block = nullptr;
			unsigned char *slide = _chunk + 56;
			(*((uint64_t *)slide)) = _file_size;
		}
		return _block;
	}

	void 
	_calc_chunk(uint32_t *chk) noexcept
	{
		uint32_t a = A, b = B, c = C, d = D;
		uint32_t logic, g, tmp;

		for (uint32_t i = 0; i < 64; ++i) 
		{
			if (i >= 0 && i < 16)
			{
				logic = _F(b, c, d);
				g = i;
			}
			else if (i >= 16 && i < 32)
			{
				logic = _G(b, c, d);
				g = (5 * i + 1) % 16;
			}
			else if (i >= 32 && i < 48) 
			{
				logic = _H(b, c, d);
				g = (3 * i + 5) % 16;
			}
			else
			{
				logic = _I(b, c, d);
				g = (7 * i) % 16;
			}

			tmp = d;
			d = c;
			c = b;
			b = b + _shifting(a + logic + chk[g] + k[i], s[i]);
			a = tmp;
		}

		A += a;
		B += b;
		C += c;
		D += d;
	}

	uint32_t 
	_F(uint32_t x, uint32_t y, uint32_t z) noexcept
	{
		return (x & y) | ((~x) & z);
	}

	uint32_t 
	_G(uint32_t x, uint32_t y, uint32_t z) noexcept 
	{
		return (x & z) | (y & (~z));
	}

	uint32_t 
	_H(uint32_t x, uint32_t y, uint32_t z) noexcept
	{
		return x ^ y ^ z;
	}

	uint32_t 
	_I(uint32_t x, uint32_t y, uint32_t z) noexcept
	{
		return y ^ (x | (~z));
	}

	uint32_t 
	_shifting(uint32_t i, int n) noexcept
	{
		return (i << n) ^ (i >> (32 - n));
	}

	int 
	_read_str(const char *str) noexcept
	{
		int len = 0;
		for (int i = 0; i < 64; ++i) 
		{
			if (str[i] == 0)
			{
				break;
			}
			_chunk[i] = (unsigned char)str[i];
			++len;
		}
		return len;
	}

private:
	static const int N = 64;
	uint32_t k[N] = 
	{
		7, 12, 17, 22, 7, 12, 17, 22,
		7, 12, 17, 22, 7, 12, 17, 22,
		5, 9, 14, 20, 5, 9, 14, 20,
		5, 9, 14, 20, 5, 9, 14, 20,
		4, 11, 16, 23, 4, 11, 16, 23,
		4, 11, 16, 23, 4, 11, 16, 23,
		6, 10, 15, 21, 6, 10, 15, 21,
		6, 10, 15, 21, 6, 10, 15, 21 
	};

	uint32_t s[N] = 
	{
		0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
		0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
		0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
		0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
		0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
		0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
		0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
		0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
		0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
		0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
		0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
		0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
		0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
		0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
		0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
		0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 
	};

	const char* _path = nullptr;
	const char* _str = nullptr;

	uint32_t A;
	uint32_t B;
	uint32_t C;
	uint32_t D;

	uint8_t _chunk[64] = { 0 };
	uint8_t* _block = nullptr;
	uint64_t _file_size = 0;
};


_GURU_END

#endif /* _GURU_DIGEST_H_ */