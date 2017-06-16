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
class HashTable;

//为哈希桶添加一个迭代器

template<class k, class v, class Ref, class Ptr, class HashF>
class HashBacketIterator
{
public:
	typedef HashBacketIterator<k, v, Ref, Ptr, HashF> Self;
	typedef HashNode<k, v> Node;

public:
	HashBacketIterator()
	{}

	HashBacketIterator(Node* pNode, HashTable<k, v, HashF>* ht)
		:_pNode(pNode)
		, _ht(ht)
	{}

	HashBacketIterator(Self& it)
		:_pNode(it._pNode)
		, _ht(it._ht)
	{}

	Ref operator*()
	{
		return _pNode->_kv;
	}

	Ptr operator->()
	{
		return &(operator*());
	}

	Self& operator++()
	{
		Next();
		return *this;
	}

	Self operator++(int)
	{
		Self temp;
		Next();
		return temp;
	}

	//哈希桶的迭代器没有前置++

	bool operator==(const Self it)
	{
		return (it->_ht == _ht) && (it->_pNode == _pNode);
	}
	/*
	bool operator!=(const Self it)
	{
		return !(operator==(it));
	}
	*/
	
	bool operator!=(const Self& it)
	{
		return (it._ht!= _ht) || (it._pNode != _pNode);
	}
	
private:
	void Next()
	{
		if (_pNode->_pNext)
		{
			_pNode = _pNode->_pNext;
		}
		else
		{
			size_t backetNo = _ht->HashFun(_pNode->_kv.first);
			for (size_t idx = backetNo + 1; idx < _ht->Size(); ++idx)
			{
				if (_ht->_hash[idx])
				{
					_pNode = _ht->_hash[idx];
					return;
				}
			}
			_pNode = NULL;
		}
	}

private:
	HashTable<k, v, HashF>* _ht;
	Node* _pNode;
};


template<class k, class v, class HashF = _HashFun<k> >
class HashTable
{
public:
	friend class HashBacketIterator<k, v,pair<k, v>&, pair<k, v>*, HashF>;
	typedef HashNode<k, v> Node;
	typedef HashBacketIterator<k, v, pair<k, v>&, pair<k, v>*, _HashFun<k> > Iterator;
public:
	HashTable(size_t size)
		:_size(0)
	{
		_hash.resize(GetNextPrime(size));
		for (size_t idx = 0; idx < _hash.size(); ++idx)
		{
			_hash[idx] = NULL;
		}
	}

	~HashTable()
	{
		Clear();
	}


	void Clear()
	{
		if (_size != 0)
		{
			for (size_t idx = 0; idx < _hash.size(); ++idx)
			{
				Node* pCur = _hash[idx];
				while (pCur)
				{
					_hash[idx] = pCur->_pNext;
					delete pCur;
					pCur = _hash[idx];
				}
			}
			_size = 0;
			_hash.clear();
		}
	}

	bool Insert_Equal(const k& key, const v& value)
	{
		CheckCapacity();
		size_t hashAddr = HashFun(key);
		Node* pNewNode = new Node(key, value);

		pNewNode->_pNext = _hash[hashAddr];
		_hash[hashAddr] = pNewNode;
		++_size;
		return true;
	}

	bool InsertUnique(const k& key,const k& value)
	{
		CheckCapacity();
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

	size_t BucketInCount(const size_t& bucketnum)
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

	////////////Iterator///////////
	Iterator& Begin()
	{
		for (size_t idx = 0; idx < _hash.size(); ++idx)
		{
			if (_hash[idx])
			{
				return Iterator(_hash[idx], this);
			}
		}
	}

	Iterator End()
	{
		return Iterator(NULL, this);
	}


private:

	void CheckCapacity()//哈希表的扩容，当哈希表中有效元素的个数大于等于哈希桶的个数时，则扩容
	{
		if (_size >= _hash.size())
		{
			HashTable<k, v/*, HashF*/> ht(2*_size);
			for (size_t idx = 0; idx < _hash.size(); ++idx)
			{
				Node* pCur = _hash[idx];
				while (pCur)
				{
					ht.Insert_Equal(pCur->_kv.first, pCur->_kv.second);
					pCur = pCur->_pNext;
				}
			}
			Swap(ht);
			//swap(*this, ht);
		}
	}

	void Swap(HashTable<k, v,HashF>& ht)
	{
		//swap(ht._hash,_hash);//该交换方式会在底层创建了临时空间用来交换，利用了辅助内存和多次拷贝，效率低
		_hash.swap(ht._hash);//该方式在底层只是改变vector的三个迭代器指针
		swap(_size, ht._size);
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
public:
	size_t HashFun(const k& key)
	{
		return HashF()(key) % _hash.size();
		//return key%_hash.size();
	}
private:
	vector<Node*> _hash;
	size_t _size;
};