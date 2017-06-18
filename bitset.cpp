#include<iostream>
#include<vector>
#include<assert.h>
using namespace std;

//λͼ
//�����⣺��40�ڸ����ظ����޷���������û�Ź��򡣸���һ���޷�����������ο����ж�һ�����Ƿ�����40�ڸ�����

template<size_t N>//N�������λ��
class BitSet
{
public:
	BitSet()
	{
		_s.resize( (N >> 5) + 1);
	}

	BitSet(unsigned long value)//��value�ı���λ������_s��
	{
		_s.push_back(value);
	}

	BitSet(const string& s)
	{
		//�ж��ַ����Ƿ����Ҫ�󣬼�ֻ��0����1
		for (size_t idx = 0; idx < s.size(); ++idx)
		{
			if ('0' != s[idx] && '1' != s[idx])
			{
				cout << "������ַ������ͷ�" << endl; 
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

	void Set(size_t whichBit)//whichBitλ����1
	{
		assert(whichBit < N);
		size_t index = whichBit / 32;//index��ʾ_s��Ӧ���±�
		size_t pos = whichBit % 32;//pos��ʾ�����ֽ��ж�Ӧ�ı���λ����������

		_s[index] = _s[index] | (1 << pos);
	}

	void reSize(size_t whichBit)//whichBit��λ������0
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

	size_t Count()//λͼ�б���λΪ1�ĸ���
	{
		char* strBitOfHex = "\0\1\1\2\1\2\2\3\1\2\2\3\2\3\3\4";
		size_t count = 0;
		for (size_t idx = 0; idx <= N / 32; ++idx)
		{
			
			size_t value = _s[idx];//ʮ�����ƣ�����������1�ĸ��������硰0x12345678��
			size_t i = sizeof(char);
			while (i--)
			{
				char charValue = value;//4�ֽ���ȡ��͵�1���ֽڣ�8λ��78��
				count += strBitOfHex[charValue & 0x0F];//�����4λ1�ĸ�������8����1������Ϊ1
				charValue = charValue >> 4;//����4������λ��ȡ����7����1�ĸ���Ϊ3
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