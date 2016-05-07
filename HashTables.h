#ifndef __HASHTABLES_H__
#define __HASHTABLES_H__

enum Status
{
	EXIST,
	EMPTY,
	DELETE,
};

template<class K>
struct HashFunce
{
	size_t operator()(const K& key)
	{
		return key;
	}
};

template<>
struct HashFunce<string>
{
	size_t value = 0;
	size_t operator()(const string& str)
	{
		for (int i = 0; i < str.size(); i++)
		{
			value += str[i];
		}
		return value;
	}

};

template<class K,class V>
struct KeyValue
{
	K _key;
	V _value;

	KeyValue(const K& key = K(), const V& value = V())
		:_key(key)
		, _value(value)
	{}
};

template<class T,template <class> class HashFuncer = HashFunce>
class HashTables
{
public:
	HashTables()
		:_tables(NULL)
		, _status(NULL)
		, _size(0)
		, _capacity(0)
	{}

	HashTables(const int& size)
		:_tables(new T[size])
		, _status(new Status[size])
		, _size(0)
		, _capacity(size)
	{
		for (int i = 0; i < _capacity; i++)
		{
			_status[i] = EMPTY;
		}
	}

	bool Insert(const T& key)
	{
		HashFuncer<T> hf;
		size_t i = hf(key);
		//静态
		/*if (_size == _capacity)
		{
			cout << "hashTable is full!" << endl;
			return false;
		}*/

		//动态
		_CheckCapacity();
		

		size_t index = HashKey(i);
		int j = 1;
		while (1)
		{
			if (_tables[index] == key)
				return false;
			if (_status[index] != EXIST)
			{
				_tables[index] = key;
				_status[index] = EXIST;
				_size++;
				return true;
			}
			
			index=(index+j*j)%_capacity;
			++j;
		}
		return false;
	}

	int Find(const T& key)
	{
		int i = 1;
		HashFuncer<T> hf;
		int in = hf(key);
		int index = HashKey(in);
		while (_status[index] != EMPTY)
		{
			if (_tables[index] == key && _status[index] == EXIST)
			{
				return index;
			}

			index = (index+i*i)%_capacity;
			++i;
		}

		return -1;
	}

	bool remove(const T& key)
	{
		int index = Find(key);

		if (index != -1)
		{
			_status[index] = DELETE;
			--_size;
			return true;
		}

		return false;
	}

	size_t HashKey(const int& key)
	{
		return key%_capacity;
	}


	void PrintHashTable()
	{
		for (int i = 0; i < _capacity; i++)
		{
			if (_status[i] == EXIST)
			{
				cout << "["<< i << "]" << "E:" << _tables[i] << endl;
			}
			else if (_status[i] == DELETE)
			{
				cout << "["<< i << "]" << "D:" << _tables[i] << endl;
			}
			else
			{
				cout << "["<< i << "]" << "N:" << "NULL" << endl;
			}
		}
	}


	void _CheckCapacity()
	{
		if (_size * 10 >= _capacity * 7)//哈希表的负荷为0.7
		{
			HashTables<T> tmp(2 * _capacity);
			for (int i = 0; i < _capacity; i++)
			{
				tmp.Insert(_tables[i]);
			}

			Swap(tmp);
		}
	}

	void Swap(HashTables<T>& tmp)
	{
		swap(_tables, tmp._tables);
		swap(_status, tmp._status);
		swap(_size, tmp._size);
		swap(_capacity, tmp._capacity);
	}



private:
	T* _tables;
	Status* _status;
	size_t _size;
	size_t _capacity;

};


void HashTest()
{
	HashTables<int> ht1(10);
	ht1.Insert(89);
	ht1.Insert(18);
	ht1.Insert(49);
	ht1.Insert(58);
	ht1.Insert(9);
	ht1.PrintHashTable();
	cout<<ht1.Find(9)<<endl;

	ht1.remove(58);
	ht1.Insert(48);
	ht1.Insert(1);
	ht1.Insert(2);
	ht1.PrintHashTable();

	HashTables<string,HashFunce> ht2(2);
	ht2.Insert("abcd");
	ht2.Insert("abcde");
	ht2.Insert("abcdef");
	ht2.Insert("影皇");
	ht2.Insert("影帝");
	ht2.Insert("影后");
	cout << ht2.Find("影后") << endl;
	ht2.remove("影后");

	ht2.PrintHashTable();

}








#endif //__HASHTABLES_H__