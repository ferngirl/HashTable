#include<iostream>
#include<vector>
#include<assert.h>
using namespace std;

//位图
//面试题：给40亿个不重复的无符号整数，没排过序。给定一个无符号整数，如何快速判断一个数是否在这40亿个数中

template<size_t N>//N代表比特位数
class BitSet
{
public:
	BitSet()
	{
		_s.resize( (N >> 5) + 1);
	}

	BitSet(unsigned long value)//将value的比特位拷贝进_s中
	{
		_s.push_back(value);
	}

	BitSet(const string& s)
	{
		//判断字符串是否符合要求，即只有0或则1
		for (size_t idx = 0; idx < s.size(); ++idx)
		{
			if ('0' != s[idx] && '1' != s[idx])
			{
				cout << "输入的字符串不和法" << endl; 
				return;
			}
		}
		_s.resize((N >> 5) + 1);
		for (size_t idx = 0; idx < N; ++idx)
		{
			if (s[idx] == '1')
			{
				Set(idx);
			}
		}
	}

	void Set(size_t whichBit)//whichBit位置置1
	{
		assert(whichBit < N);
		size_t index = whichBit / 32;//index表示_s对应的下标
		size_t pos = whichBit % 32;//pos表示对用字节中对应的比特位（从右向左）

		_s[index] = _s[index] | (1 << pos);
	}

	void reSize(size_t whichBit)//whichBit的位置上置0
	{
		assert(whichBit < N);
		size_t index = whichBit/32;
		size_t pos = whichBit % 32;

		_s[index] &= ~(1 << pos);
	}

	int& operator[](size_t whichBit)
	{
		assert(whichBit < N);
		size_t index = whichBit / 32;
		size_t pos = whichBit % 32;

		return _s[index] & (~(1 << pos));
	}

	size_t Size()
	{
		return N;
	}

	size_t Count()//位图中比特位为1的个数
	{
		char* strBitOfHex = "\0\1\1\2\1\2\2\3\1\2\2\3\2\3\3\4";
		size_t count = 0;
		for (size_t idx = 0; idx <= N / 32; ++idx)
		{
			
			size_t value = _s[idx];//十六进制，计算数字中1的个数，例如“0x12345678”
			size_t i = sizeof(char);
			while (i--)
			{
				char charValue = value;//4字节中取最低的1个字节，8位“78”
				count += strBitOfHex[charValue & 0x0F];//计算低4位1的个数，‘8’的1个个数为1
				charValue = charValue >> 4;//右移4个比特位，取到‘7’，1的个数为3
				count += strBitOfHex[charValue & 0x0F];
			}
		}
		return count;
	}

	bool Test(size_t whichBit)
	{
		assert(whichBit < N);
		size_t index = whichBit / 32;
		size_t pos = whichBit % 32;

		return (1==_s[index] & (1 << pos));
	}
	
	bool any()
	{
		for (size_t idx = 0; idx < N / 32; ++idx)
		{
			int pos = sizeof(int);
			while(pos--)
			{
				if ((_s[idx] & (1 >> pos) )== '0')
					return false;
			}
		}
		return true;
	}
private:
	vector<int> _s;
};