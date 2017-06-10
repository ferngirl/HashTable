#include<stdio.h>
#include<vector>
#include<iostream>
#include<string>
using namespace std;

enum State//设置每个存储节点的状态
{
	EMPTY,//空
	EXIST,//存在值
	DELETE//值被删除（伪删除）
};

template<class k,class v>
struct HashNode
{
	HashNode()
	:_s(EMPTY)
	{}

	pair<k, v> _kv;
	State  _s;
};

//解决哈希表现在只能存储整型数值的问题

static size_t BKDRHash(const char * str)
{
	unsigned int seed = 131; // 31 131 1313 13131 131313
	unsigned int hash = 0;
	while (*str)
	{
		hash = hash * seed + (*str++);
	}
	return (hash & 0x7FFFFFFF);
}

template<class k>
struct _HashFun
{
	size_t operator()(const k& key)
	{
		return key;//默认是整型
	}
};

//模板特例化
template<>
struct _HashFun<string>
{
	size_t operator()(const string key)
	{
		return BKDRHash(key.c_str());
	}
};

template<class k,class v,class HashF = _HashFun<k> >
class HashTable
{
public:
	HashTable(size_t size = 11)
		:_size(0)
	{
		_hash.resize(GetNextPrime(size));//存在问题，怎么改成动态的
	}

	bool Insert(const k key, const v value)
	{
		CheckTable();
		size_t hashAddr = HashFun(key);//哈希地址
		size_t index = hashAddr;

		while(_hash[index]._s == EXIST)//若插入节点位置存在值
		{
			if (_hash[index]._kv.first == key)//并且节点位置的值与key相等，这时直接退出
				return false;
			index++;//查找后面的合适位置
			if (index == _hash.size() - 1)//若找到最后一个位值时，则回到下标为0的位置
				index = 0;
			if (index == hashAddr)//当找完一圈，退出，避免陷入死循环
				return false;
		}
		//找到了合适的插入位置，插入节点
		_hash[index]._kv.first = key;
		_hash[index]._kv.second = value;
		_hash[index]._s = EXIST;
		_size++;
		return true;
	}

	pair<HashNode<k, v>*, bool> Find(const k& key)
	{
		size_t HashAddr = HashFun(key);
		size_t index = HashAddr;

		while (_hash[index]._s != EMPTY)//当值存在时，可以查找，直到遇见EMPTY节点
		{
			if (_hash[index]._s==EXIST && _hash[index]._kv.first == key)
				return pair<&_hash[index], true>;//成功找到
			index++;
			if (index == _hash.size())
				index = 0;
			if (index == HasgAddr)
				return pair<&_hash[index], false>;
		}
		return pair<NULL, false>;//没有找到
	}

	bool Remove(const k& key)
	{
		pair<HashNode<k, v>*, bool> pos = Find(key);
		if (pos.second == false)
			return false;
		pos.first->_s == DELETE;
		_size--;
	}

private:
	size_t HashFun(const k& key)
	{
		HashF f;
		return f(key)%_hash.size();
	}

	void CheckTable()
	{
		if ((_size * 10) / _hash.size() > 7)
		{
			//搬移元素，重新计算地址
			HashTable<k,v > hs(GetNextPrime(_hash.size()));// 一个新的哈希表
			for (size_t idx = 0; idx < _hash.size(); ++idx)
			{
				if (_hash[idx]._s == EXIST)
				{
					hs.Insert(_hash[idx]._kv.first, _hash[idx]._kv.second);
				}
			}
			swap(*this, hs);

		}
	}

	//解决求取哈希地址时，除数是质数的问题
	size_t GetNextPrime(size_t num)
	{
		const int _PrimeSize = 28;
		static const unsigned long _PrimeList[_PrimeSize] =
		{
			53ul, 97ul, 193ul, 389ul, 769ul,
			1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
			49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
			1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
			50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
			1610612741ul, 3221225473ul, 4294967291ul
		};
		for (size_t idx = 0; idx < _PrimeSize; ++idx)
		{
			if (num < _PrimeList[idx])
			{
				return _PrimeList[idx];
			}
		}
		return _PrimeList[_PrimeSize];//若不满足上述条件，返回最大一个质数
	}
private:
	vector<HashNode<k,v>> _hash;
	size_t _size;//vector中实际存在的有效元素的个数
};