#include<stdio.h>
#include<assert.h>
#include<vector>
#include<iostream>
using namespace std;


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

template<class k, class v>
struct HashNode
{
	HashNode(const k& key, const v& value)
	:_pNext(NULL)
	{
		_kv.first = key;
		_kv.second = value;
	}

	pair<k, v> _kv;
	HashNode<k, v>* _pNext;
};

template<class k, class v, class HashF = _HashFun<k> >
class HashTable
{
	typedef HashNode<k, v> Node;
public:
	HashTable(size_t size)
		:_size(0)
	{
		_hash.resize(/*GetNextPrime*/size);
	}

	bool Insert_Equal(const k& key, const v& value)
	{
		size_t hashAddr = HashFun(key);
		Node* pNewNode = new Node(key, value);

		pNewNode->_pNext = _hash[hashAddr];
		_hash[hashAddr] = pNewNode;
		++_size;
		return true;
	}

	bool InsertUnique(const k& key,const k& value)
	{
		size_t hashAddr = HashFun(key);
		Node* pCur = _hash[hashAddr];

		Node* pNewNode = new Node(key, value);

		while (pCur)
		{
			if (pCur->_kv.first == key)
				return false;
			pCur = pCur->_pNext;
		}

		pNewNode->_pNext = _hash[hashAddr];
		_hash[hashAddr] = pNewNode;
		++_size;
		return true;
	}

	bool RemoveEqual(const k& key)
	{
		size_t hashAddr = HashFun(key);
		Node* pCur = _hash[hashAddr];
		Node* pPrev = NULL;
		size_t oldsize = _size;

		while (pCur)
		{
			if (pCur->_kv.first == key)
			{
				if (pPrev == NULL)
				{
					_hash[hashAddr] = pCur->_pNext;
					delete pCur;
					--_size;
					pCur = _hash[hashAddr];
				}
				else
				{
					pPrev->_pNext = pCur->_pNext;
					delete pCur;
					--_size;
					pCur = pPrev->_pNext;
				}
			}
			else
			{
				pPrev = pCur;
				pCur = pCur->_pNext;
			}

		}
		if (oldsize != _size)
			return true;
		return false;
	}

	bool RemoveUnique(const k& key)
	{
		size_t hashAddr = HashFun(key);
		Node* pCur = _hash[hashAddr];
		Node* pPrev = NULL;

		while (pCur)
		{
			if (pCur->_kv.first == key)
			{
				if (pPrev == NULL)//说明删除的节点是链表中第一个节点
				{
					_hash[hashAddr] = pCur->_pNext;
					delete pCur;
					--_size;
					return true;
				}
				else
				{
					pPrev->_pNext = pCur->_pNext;
					delete pCur;
					--_size;
					return true;
				}
			}
			pPrev = pCur;
			pCur = pCur->_pNext;
		}
		return false;
	}

	Node* Find(const k& key)const
	{
		size_t hashAddr = HashFun(key);
		Node* pCur = _hash[hasshAddr];

		while (pCur)
		{
			if (pCur->_kv.first == key)
				return pCur;
			pCur = pCur->_pNext;
		}
		return NULL;
	}

	bool Empty()const
	{
		return _size == 0;
	}

	size_t Size()const
	{
		return _size;
	}

	size_t Count(const k& key)const//得到 可以值为key的元素个数
	{
		size_t hashAddr = HashFun(key);
		Node* pCur = _hash[hashAddr];
		size_t count = 0;

		while (pCur)
		{
			if (pCur->_kv.first == key)
				count++;
			pCur = pCur->_pNext;
		}
		return count;
	}

	size_t BucketCount()const
	{
		return _hash.size();
	}

	size_t BucketInCount(const k bucketnum)
	{
		assert(bucketnum <= _hash.size());
		Node* pCur = _hash[bucketnum];
		size_t count = 0;

		while (pCur)
		{
			count++;
			pCur = pCur->_pNext;
		}
		return count;
	}
private:
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

	size_t HashFun(const k& key)
	{
		return HashF()(key) % _hash.size();
	}
private:
	vector<Node*> _hash;
	size_t _size;
};