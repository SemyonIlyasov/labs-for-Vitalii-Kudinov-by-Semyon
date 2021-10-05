#include <iostream>
#include <list>
#include <iterator>
#include <assert.h>

using namespace std;

typedef struct Student {
	unsigned age;
	unsigned weight;
	friend bool operator==(Student& a, Student& b)
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
		HashTable b;
		b.buf_size *= 2;
		for (int i = 0; i < b.buf_size; ++i)
			b.train = new wagon[b.buf_size];

		for (int i = 0; i < buf_size; ++i)
		{
			train[i].conductor = train[i].passengers.begin();
			for (; train[i].conductor != train[i].passengers.end(); ++(train[i].conductor))
			{
				unsigned int vhash = _hf(train[i].conductor->first, b.buf_size);
				b.train[vhash].passengers.push_back({train[i].conductor->first , train[i].conductor->second });
			}
		}
		buf_size = b.buf_size;
		train = new wagon[buf_size];
		capacity = double(alive) / buf_size;
		for (int i = 0; i < b.buf_size; ++i)
		{
			b.train[i].conductor = b.train[i].passengers.begin();
			for (; b.train[i].conductor != b.train[i].passengers.end(); ++(b.train[i].conductor))
			{
				unsigned int vhash = _hf(b.train[i].conductor->first, b.buf_size);
				train[vhash].passengers.push_back({ b.train[i].conductor->first , b.train[i].conductor->second });
			}
		}
	}

public:
	void output()																// special function for examples 
	{
		cout << "Table characteristics: \n";
		cout << "size: " << size() << "\n";
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
	~HashTable()																// вроде норм, но надо уточнить
	{
		for (unsigned int i = 0; i < buf_size; ++i)
			train[i].passengers.clear();
		//delete train;
	};

	HashTable(const HashTable& b) noexcept											// вроде норм
	{
		buf_size = b.buf_size;
		alive = b.alive;
		capacity = b.capacity;
		train = new wagon[b.buf_size];
		for (int i = 0; i < b.buf_size; i++)
			train[i] = b.train[i];
	};

	HashTable(HashTable&& b) noexcept											// просто скопировал из HashTable& b вроде работает
	{
		buf_size = b.buf_size;
		alive = b.alive;
		capacity = b.capacity;
		train = new wagon[b.buf_size];
		for (int i = 0; i < b.buf_size; i++)
			train[i] = b.train[i];
	};



	HashTable& operator=(const HashTable& b)											// вроде нормs
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

	HashTable& operator=(HashTable&& b)											// хз просто скопировал из предыдущей функции вроде работает
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
		return *this;
	};
	// Обменивает значения двух хэш-таблиц.
	void swap(HashTable& b)														// вроде норм
	{
		HashTable f(b);
		b = *this;
		*this = f;
	};
	// Очищает контейнер.
	void clear()																// вроде норм
	{
		capacity = 0;
		buf_size = default_size;
		alive = 0;
		delete(train);
	};
	// Удаляет элемент по заданному ключу.
	bool erase(const Key& k)													// вроде норм
	{
		if (contains(k))
		{
			unsigned int vhash = _hf(k, size());
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
	// Вставка в контейнер. Возвращаемое значение - успешность вставки.
	bool insert(const Key& k, const Value& v)									// вроде норм
	{
		if (!contains(k))
		{
			unsigned int vhash = _hf(k, size());
			pair<Key, Value> para = { k, v};
			train[vhash].passengers.push_back(para);
			alive += 1;
			capacity = double(alive) / buf_size;
			if (capacity >= rehash_size)
				_rehash();
			return 1;
		}
		return 0;
	};
	// Проверка наличия значения по заданному ключу.
	bool contains(const Key& k) const											// вроде норм		
	{
		unsigned int vhash = _hf(k, size());
		train[vhash].conductor = train[vhash].passengers.begin();
		while (train[vhash].conductor != train[vhash].passengers.end())
		{
			if (train[vhash].conductor->first == k)
				return 1;
			train[vhash].conductor++;
		}
		return 0;
	};
	// Возвращает значение по ключу. Небезопасный метод.
	// В случае отсутствия ключа в контейнере, следует вставить в контейнер
	// значение, созданное конструктором по умолчанию и вернуть ссылку на него.
	Value& operator[](const Key& k)												// вроде норм
	{
		if(!contains(k))
			insert(k, { 0,0 });
		return	at(k);
	};
	// Возвращает значение по ключу. Бросает исключение при неудаче.
	Value& at(const Key& k)														// всем норм
	{
		unsigned int vhash = _hf(k, size());
		train[vhash].conductor = train[vhash].passengers.begin();
		while (train[vhash].conductor != train[vhash].passengers.end())
		{
			if (train[vhash].conductor->first == k)
				return train[vhash].conductor->second;
			train[vhash].conductor++;
		}
		assert(vhash < 0);
	};
	const Value& at(const Key& k) const											// вроде (нужно ли как-то изменять функцию для констант?)
	{
		unsigned int vhash = _hf(k, size());
		train[vhash].conductor = train[vhash].passengers.begin();
		while (train[vhash].conductor != train[vhash].passengers.end())
		{
			if (train[vhash].conductor->first == k)
				return train[vhash].conductor->second;
			train[vhash].conductor++;
		}
		assert(vhash < 0);
	};
	double cap()																// доп. ф-я 
	{
		assert(capacity != 0);
		return capacity;
	}

	size_t size() const															// вроде норм
	{
		return buf_size;
	};
	bool empty() const															// вроде норм
	{
		if (alive == 0) return 1;
		return 0;
	};
	friend bool operator==(const HashTable& a, const HashTable& b);			// хз

	friend bool operator!=(const HashTable& a, const HashTable& b);			// хз
};

bool operator==(const HashTable& a, const HashTable& b)
{
	if (a.alive == b.alive && a.buf_size == b.buf_size)
	{
		for (int i = 0; i < a.buf_size; i++)
			if (a.train[i].passengers != b.train[i].passengers)
				return 0;
		return 1;
	}
	return 0;
}
bool operator!=(const HashTable& a, const HashTable& b)
{
	if (a == b)
		return 0;
	return 1;
}

int main()
{

	HashTable a; 
	a.output();
	a.insert("a", {1,1});
	a.insert("b", { 2,2 });
	a.insert("c", { 3,3 });
	a.insert("d", { 4,4 });
	a.insert("e", { 5,5 });
	cout << "----------------------------------------------------------\n";
	a.output();
	a.insert("f", { 6,6 });
	cout << "----------------------------------------------------------\n";
	a.output();
	return 0;
}

