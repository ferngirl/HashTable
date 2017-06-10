#include<stdio.h>
#include<vector>
#include<iostream>
#include<string>
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

//�����ϣ������ֻ�ܴ洢������ֵ������

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
		return key;//Ĭ��������
	}
};

//ģ��������
template<>
struct _HashFun<string>
{
	size_t operator()(const string key)
	{
		return BKDRHash(key.c_str());
	}
};

template<class k,class v,class HashF = _HashFun<k>,bool IsLine = true>
class HashTable
{
public:
	HashTable(size_t size = 11)
		:_size(0)
	{
		_hash.resize(GetNextPrime(size));
	}

	bool Insert(const k key, const v value)
	{
		CheckTable();
		size_t hashAddr = HashFun(key);//��ϣ��ַ
		size_t index = hashAddr;
		size_t i = 1;//����̽���е�̽�����

		while(_hash[index]._s == EXIST)//������ڵ�λ�ô���ֵ
		{
			if (_hash[index]._kv.first == key)//���ҽڵ�λ�õ�ֵ��key��ȣ���ʱֱ���˳�
				return false;
			//����ģ�����IsLine���ж�ѡ���ɢ�з�������̽�⻹�Ƕ���̽�⣬Ĭ��������̽��
			if (IsLine)
				DetectedLine(index);
			else
				Detected2(index, i);
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
		size_t i = 1;//����̽���е�̽�����

		while (_hash[index]._s != EMPTY)//��ֵ����ʱ�����Բ��ң�ֱ������EMPTY�ڵ�
		{
			if (_hash[index]._s==EXIST && _hash[index]._kv.first == key)
				return pair<&_hash[index], true>;//�ɹ��ҵ�

			if (IsLine())
				DetectedLine(index);
			else
				Detected2(index, i);
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
	void DetectedLine(size_t& index)//����̽��
	{
		index++;//���Һ���ĺ���λ��
		if (index == _hash.size() - 1)//���ҵ����һ��λֵʱ����ص��±�Ϊ0��λ��
			index = 0;
	}

	void Detected2(size_t& index,size_t& i)//����̽��
	{
		index = index + 2 * i + 1;
		i++;
		if (index >= _hash.size())
			index = 0;
	}

	size_t HashFun(const k& key)
	{
		HashF f;
		return f(key)%_hash.size();
	}

	void CheckTable()
	{
		if ((_size * 10) / _hash.size() > 7)
		{
			//����Ԫ�أ����¼����ַ
			HashTable<k,v,_HashFun<k>,IsLine> hs(GetNextPrime(_hash.size()));// һ���µĹ�ϣ��
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

	//�����ȡ��ϣ��ַʱ������������������
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
		return _PrimeList[_PrimeSize];//�������������������������һ������
	}
private:
	vector<HashNode<k,v>> _hash;
	size_t _size;//vector��ʵ�ʴ��ڵ���ЧԪ�صĸ���
};