#include<stdio.h>
#include<vector>
#include<iostream>
using namespace std;

enum State//����ÿ���洢�ڵ��״̬
{
	EMPTY,//��
	EXIST,//����ֵ
	DELETE//ֵ��ɾ����αɾ����
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
		_hash.resize(size);//�������⣬��ô�ĳɶ�̬��
	}

	bool Insert(const k& key, const v& value)
	{
		CheckTable();
		size_t hashAddr = HashFun(key);//��ϣ��ַ
		size_t index = hashAddr;

		while(_hash[index]._s == EXIST)//������ڵ�λ�ô���ֵ
		{
			if (_hash[index]._kv.first == key)//���ҽڵ�λ�õ�ֵ��key��ȣ���ʱֱ���˳�
				return false;
			index++;//���Һ���ĺ���λ��
			if (index == _hash.size() - 1)//���ҵ����һ��λֵʱ����ص��±�Ϊ0��λ��
				index = 0;
			if (index == hashAddr)//������һȦ���˳�������������ѭ��
				return false;
		}
		//�ҵ��˺��ʵĲ���λ�ã�����ڵ�
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

		while (_hash[index]._s != EMPTY)//��ֵ����ʱ�����Բ��ң�ֱ������EMPTY�ڵ�
		{
			if (_hash[index]._s==EXIST && _hash[index]._kv.first == key)
				return pair<&_hash[index], true>;//�ɹ��ҵ�
			index++;
			if (index == _hash.size())
				index = 0;
			if (index == HasgAddr)
				return pair<&_hash[index], false>;
		}
		return pair<NULL, false>;//û���ҵ�
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
			_hash.resize(11);//11�Ǹ�ȷ������ֵ��������
		else if ((_size * 10) / _hash.size() > 7)
		{
			size_t newsize = _hash.size() * 2;
			//����Ԫ�أ����¼����ַ
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
	size_t _size;//vector��ʵ�ʴ��ڵ���ЧԪ�صĸ���
};