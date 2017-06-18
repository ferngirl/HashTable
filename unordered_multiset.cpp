#include"HashBacket.cpp"

template<class k>
class Unordered_MultiSet
{
public:
	typedef HashTable<int, int>::Iterator iterator;

	Unordered_MultiSet(size_t size)
		:_ht(size)
	{}


	/////////////////Capacity//////////////
	bool empty()
	{
		return _ht.Empty();
	}

	bool size()
	{
		return _ht.Size();
	}

	bool max_size()
	{
		return size_t(-1);
	}

	//////////////////Iterators////////////////
	iterator& begin()
	{
		return _ht.Begin();
	}

	iterator& end()
	{
		return _ht.End();
	}

	////////////////////element loolup///////////////
	iterator find(const k& key)
	{
		return _ht.Find();
	}

	size_t count(k& key)
	{
		return _ht.Count(key);
	}


	/////////////////Modify//////////////////////
	pair<iterator, bool> insert(const k& key)
	{
		return _ht.Insert_Equal(key, key);
	}

	size_t erase(const k& key)
	{
		return _ht.Insert_Equal(key);
	}

	void clear()
	{
		return _ht.Clear();
	}

	/////////////Buckets////////////////////////
	size_t bucket_count()
	{
		return _ht.BucketCount();
	}

	size_t bucket_size(size_t blacketNo)
	{
		return _ht.BucketInCount(blacketNo);
	}

	size_t bucket(const k& key)
	{
		return _ht.Bucket(key);
	}
private:
	HashTable<k, k> _ht;
};