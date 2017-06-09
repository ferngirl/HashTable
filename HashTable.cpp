#include<stdio.h>
#include<vector>
#include<iostream>
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

template<class k,class v>
class HashTable
{
public:
	HashTable(size_t size = 11)
		:_size(0)
	{
		_hash.resize(size);//存在问题，怎么改成动态的
	}

	bool Insert(const k& key, const v& value)
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
		return key%_hash.size();
	}

	void CheckTable()
	{
		if (_hash.size() == 0)
			_hash.resize(11);//11是个确定的数值，不合适
		else if ((_size * 10) / _hash.size() > 7)
		{
			size_t newsize = _hash.size() * 2;
			//搬移元素，重新计算地址
			HashTable<k,v > hs(newsize);
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
private:
	vector<HashNode<k,v>> _hash;
	size_t _size;//vector中实际存在的有效元素的个数
};