#include"HashBacket.cpp"


template<class k, class v>
class Unorder_MultiMap
{
public:
	typename typedef HashTable<k, v>::Iterator iterator;
public:
	Unorder_MultiMap(size_t size)
		:_ht(size)
	{}

	~Unorder_MultiMap()
	{
		_ht.Clear();
	}

	////////////Capacity//////////////
	bool Empty()
	{
		return _ht.Empty();
	}

	size_t Size()
	{
		return _ht.Size();
	}

	size_t Max_size()
	{
		return _ht.Max_size();
	}

	//////////////////Iterator//////////

	iterator& Begin()
	{
		return _ht.Begin();
	}

	iterator End()
	{
		return _ht.End();
	}

	/////?????
	iterator Find(const k& key)
	{
		return _ht.Find();
	}

	/////////////Modify//////////////////
	pair<iterator, bool> Insert(k& key, v& value)//库里实现传的是键值对
	{
		return _ht.Insert_Equal(key, value);
	}

	size_t Erase(const k& key)
	{
		return _ht.RemoveEqual(key);
	}

	void Clear()
	{
		_ht.Clear();
	}

	/////////////////buckets/////////////////

	size_t bucket_count()
	{
		return _ht.BucketCount();
	}

	size_t bucket_size(const size_t& bucketnum)
	{
		return _ht.BucketInCount(bucketnum);
	}

	size_t bucket(k& key)
	{
		return _ht.Bucket(key);
	}

private:
	HashTable<k, v> _ht;
};