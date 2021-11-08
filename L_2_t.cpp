#include <iostream>
#include <list>
#include <iterator>

using namespace std;

typedef struct Student {
	unsigned age;
	unsigned weight;
	friend bool operator==(Student& a, Student& b)
	{
		return (a.age == b.age && a.weight == b.weight);
	}
	friend bool operator==(const Student& a, const Student& b)
	{
		return (a.age == b.age && a.weight == b.weight);
	}
}Student;

typedef string Key;
typedef Student Value;


typedef struct wagon wagon;


bool operator==(pair<Key, Value> a, pair<Key, Value> b)
{
	return (a.second == b.second && a.first == b.first);
}

struct wagon
	/*
		consists of indentifier, list of passengers and iterator conductor
	*/
{
	//unsigned int id;
	list <pair<Key, Value>> passengers;
	list <pair<Key, Value>> ::iterator conductor;
};


class HashTable
{
protected:

	static const int default_size = 8;					// начальный размер таблицы
	constexpr static const double rehash_size = 0.75;	// коэффициент, при котором произойдет увеличение таблицы
	wagon* train;										// array of wagons
	int buf_size;										 // all space
	int alive;											 // not deleted
	double capacity;									 // (alive) / buf_size < 0.75

	unsigned int _hf(Key key, unsigned int sz)	const					 // Hash Function
	{
		hash <Key> hf;
		return hf(key) % sz;
	}

	void _rehash()
	{
		int old = buf_size;
		buf_size *= 2;
		auto buf = new wagon[buf_size];

		for (int i = 0; i < old; ++i)
		{
			train[i].conductor = train[i].passengers.begin();
			for (; train[i].conductor != train[i].passengers.end(); ++(train[i].conductor))
			{
				unsigned int vhash = _hf(train[i].conductor->first, buf_size);
				buf[vhash].passengers.push_back({ train[i].conductor->first , train[i].conductor->second });
			}
		}
		// TODO delete train;
		train = buf;
		capacity = double(alive) / buf_size;
	}

public:
	void output()																// special function for examples 
	{
		cout << "Table characteristics: \n";
		cout << "size: " << buf_size << "\n";
		cout << "alive: " << alive << "\n";
		cout << "capacity: " << capacity << "\n";
		cout << "Table: " << "\n";
		for (int i = 0; i < buf_size; i++)
		{
			cout << "Key " << i << ": ";
			train[i].conductor = train[i].passengers.begin();
			for (; train[i].conductor != train[i].passengers.end(); ++(train[i].conductor))
			{
				cout << train[i].conductor->first << "; " << train[i].conductor->second.age << " " << train[i].conductor->second.weight;
			}
			cout << "\n";
		}
	}
	HashTable()	noexcept															// вроде норм
	{
		capacity = 0;
		buf_size = default_size;
		alive = 0;
		train = new wagon[buf_size];


	};
	~HashTable()	noexcept														// вроде норм, но надо уточнить
	{
		for (unsigned int i = 0; i < buf_size; ++i)
			train[i].passengers.clear();
	};

	HashTable(const HashTable& b) noexcept											// вроде норм
	{
		buf_size = b.buf_size;
		alive = b.alive;
		capacity = b.capacity;
		train = new wagon[b.buf_size];
		train = b.train;
	};

	HashTable(HashTable&& b) noexcept											
	{
		buf_size = b.buf_size;
		alive = b.alive;
		capacity = b.capacity;
		train = new wagon[b.buf_size];
		train = b.train;
		b.train = nullptr;
		b.buf_size = 0;
		b.capacity = 0;
	};



	HashTable& operator=(const HashTable& b)											
	{
		if (this == &b) return *this;

		for (int i = 0; i < buf_size; ++i)
		{
			train[i].passengers.clear();
			train[i].passengers = *(new list <pair<Key, Value>>);
			b.train[i].conductor = b.train[i].passengers.begin();
			for (; b.train[i].conductor != b.train[i].passengers.end(); ++(b.train[i].conductor))
				train[i].passengers.push_back(*(b.train[i].conductor));
		}
		buf_size = b.buf_size;
		alive = b.alive;
		capacity = b.capacity;
		return *this;
	};

	HashTable& operator=(HashTable&& b)										
	{
		for (int i = 0; i < buf_size; ++i)
		{
			train[i].passengers.clear();
			train[i].passengers = *(new list <pair<Key, Value>>);
			b.train[i].conductor = b.train[i].passengers.begin();
			for (; b.train[i].conductor != b.train[i].passengers.end(); ++(b.train[i].conductor))
				train[i].passengers.push_back(*(b.train[i].conductor));
		}
		buf_size = b.buf_size;
		alive = b.alive;
		capacity = b.capacity;
		// delete train
		b.buf_size = 0;
		b.alive = 0;
		b.capacity = 0;

		return *this;

	};



	void swap(HashTable& b)													
	{
		std::swap(buf_size, b.buf_size);
		std::swap(alive, b.alive);
		wagon *g = b.train;
		b.train = train;
		train = g;
		auto d = capacity;
		capacity = b.capacity;
		b.capacity = d;
	};



	void clear()															
	{
		capacity = 0;
		buf_size = default_size;
		alive = 0;
		//delete train;
	};

	bool erase(const Key& k)													
	{
		if (contains(k))
		{
			unsigned int vhash = _hf(k, buf_size);
			train[vhash].conductor = train[vhash].passengers.begin();
			while (train[vhash].conductor != train[vhash].passengers.end())
			{
				if (train[vhash].conductor->first == k)
				{
					train[vhash].passengers.remove(*train[vhash].conductor);
					alive -= 1;
					return 1;
				}
				train[vhash].conductor++;
			}
			return 1;
		}
		return 0;
	};

	bool insert(const Key& k, const Value& v)								
	{
		if (!contains(k))
		{
			unsigned int vhash = _hf(k, buf_size);
			pair<Key, Value> para = { k, v };
			train[vhash].passengers.push_back(para);
			alive += 1;
			capacity = double(alive) / buf_size;
			if (capacity >= rehash_size)
				_rehash();
			return 1;
		}
		return 0;
	};

	bool contains(const Key& k) const										
	{
		unsigned int vhash = _hf(k, buf_size);
		train[vhash].conductor = train[vhash].passengers.begin();
		while (train[vhash].conductor != train[vhash].passengers.end())
		{
			if (train[vhash].conductor->first == k)
				return 1;
			train[vhash].conductor++;
		}
		return 0;
	};



	Value& operator[](const Key& k)											
	{
		if (!contains(k))
			insert(k, { 0,0 });
		return	at(k);
	};



	Value& at(const Key& k)														
	{
		unsigned int vhash = _hf(k, buf_size);
		train[vhash].conductor = train[vhash].passengers.begin();
		while (train[vhash].conductor != train[vhash].passengers.end())
		{
			if (train[vhash].conductor->first == k)
				return train[vhash].conductor->second;
			train[vhash].conductor++;
		}
		throw std::out_of_range("This element doesn't exist");
	};
	const Value& at(const Key& k) const											
	{
		unsigned int vhash = _hf(k, buf_size);
		train[vhash].conductor = train[vhash].passengers.begin();
		while (train[vhash].conductor != train[vhash].passengers.end())
		{
			if (train[vhash].conductor->first == k)
				return train[vhash].conductor->second;
			train[vhash].conductor++;
		}
		throw std::out_of_range("This element doesn't exist");
	};
	double cap()																// доп. ф-я 
	{
		if(capacity < 0) 
			throw std::out_of_range("Capacity < 0");
		return capacity;
	}

	size_t size() const															// вроде норм
	{
		return alive;
	};
	bool empty() const															// вроде норм
	{
		if (alive == 0) return 1;
		return 0;
	};
	friend bool operator==(const HashTable& a, const HashTable& b);		

	friend bool operator!=(const HashTable& a, const HashTable& b);			
};

bool operator==(const HashTable& a, const HashTable& b)
{
	if (a.alive == b.alive && a.buf_size == b.buf_size)
	{
		for (int i = 0; i < a.buf_size; i++)
			if (a.train[i].passengers != b.train[i].passengers)
				return false;
		return true;
	}
	return false;
}
bool operator!=(const HashTable& a, const HashTable& b)
{
	if (a == b)
		return false;
	return true;
}

