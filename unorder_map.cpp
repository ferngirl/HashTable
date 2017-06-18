#include"HashBacket.cpp"

template<class k, class v>
class Unorder_Map
{
public:
	typename typedef HashTable<k, v>::Iterator iterator;
public:
	Unorder_Map(size_t size)
		:_ht(size)
	{}

	~Unorder_Map()
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
	pair<iterator, bool> Insert(k& key, v& value)
	{
		return _ht.InsertUnique(key, value);
	}

	size_t Erase(const k& key)
	{
		return _ht.RemoveUnique(key);
	}

	void Clear()
	{
		_ht.Clear();
	}

	////////////////Element access////////////////
	v& operator[](const k&  key)//¥Ê‘⁄Œ Ã‚
	{
		pair<iterator, bool> ret = _ht.InsertUnique(key, v());
		//return (*(ret.first).first);
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

	size_t bucket(const k& key)
	{
		return _ht.Bucket();
	}

private:
	HashTable<k, v> _ht;
};