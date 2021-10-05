#include <iostream>
#include <list>
#include <iterator>
#include <assert.h>

using namespace std;

typedef struct Student {
	unsigned age;
	unsigned weight;
}Student;

typedef string Key;
typedef Student Value;

typedef struct wagon wagon;

struct Node
{
	Value data;
	Key key;
	int state;

};
typedef struct Node node;

struct wagon
	/*
		consists of indentifier, list of passengers and iterator conductor
	*/
{
	//unsigned int id;
	list <pair<Key, node>> passengers;
	list <pair<Key, node>> ::iterator conductor;
};


class HashTable
{
protected:

	static const int default_size = 8;					// начальный размер нашей таблицы
	constexpr static const double rehash_size = 0.75;	// коэффициент, при котором произойдет увеличение таблицы
	wagon* train;										// array of wagons
	int buf_size;										 // all space
	int alive;											 // not deleted
	int dead;											 // deleted
	double capacity;									 // (dead + alive) div amount < 0.75

	unsigned int _hf(Key key)	const					 // Hash Function
	{
		hash <Key> hf;
		return hf(key) % size();
	}

	void _rehash()
	{

	}

public:
	int tstnum;
	HashTable()	noexcept															// вроде норм
	{
		capacity = 0;
		buf_size = default_size;
		alive = 0;
		dead = 0;
		train = new wagon[default_size];


	};
	~HashTable()																// вроде норм, но надо уточнить
	{
		delete train;
	};

	HashTable(const HashTable& b) noexcept											// вроде норм
	{
		buf_size = b.buf_size;
		alive = b.alive;
		dead = b.dead;
		capacity = b.capacity;
		train = new wagon[b.buf_size];
		for (int i = 0; i < b.buf_size; i++)
			train[i] = b.train[i];
	};

	HashTable(HashTable&& b) noexcept											// просто скопировал из HashTable& b
	{
		buf_size = b.buf_size;
		alive = b.alive;
		dead = b.dead;
		capacity = b.capacity;
		train = new wagon[b.buf_size];
		for (int i = 0; i < b.buf_size; i++)
			train[i] = b.train[i];
	};



	HashTable& operator=(const HashTable& b)											// ?
	{

	};
	HashTable& operator=(HashTable&& b)											// ?
	{

	};
	// Обменивает значения двух хэш-таблиц.
	void swap(HashTable& b)														// ?
	{

	};
	// Очищает контейнер.
	void clear()																// ?
	{

	};
	// Удаляет элемент по заданному ключу.
	bool erase(const Key& k)													// вроде норм
	{
		if (contains(k))
		{
			unsigned int vhash = _hf(k);
			train[vhash].conductor = train[vhash].passengers.begin();
			while (train[vhash].conductor != train[vhash].passengers.end())
			{
				if (train[vhash].conductor->first == k)
				{
					train[vhash].passengers.remove(*train[vhash].conductor);
					dead += 1;
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
	bool insert(const Key& k, const Value& v)									// вроде норм TODO _rehash
	{
		if (!contains(k))
		{
			unsigned int vhash = _hf(k);
			node* n = new node;
			n->data = v;
			n->state = 0;
			n->key = k;
			pair<Key, node> para = { k, *n };
			train[vhash].passengers.push_back(para);
			alive += 1;
			capacity = (dead + alive) / buf_size;
			if (capacity >= rehash_size)
				_rehash();
			return 1;
		}
		return 0;
	};
	// Проверка наличия значения по заданному ключу.
	bool contains(const Key& k) const											// вроде норм		
	{
		unsigned int vhash = _hf(k);
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
	Value& operator[](const Key& k)												// ?
	{

	};
	// Возвращает значение по ключу. Бросает исключение при неудаче.
	Value& at(const Key& k)														// ?
	{

	};
	const Value& at(const Key& k) const											// ?
	{

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
		if (alive == 0 && dead == 0) return 1;
		return 0;
	};
	//friend bool operator==(const HashTable& a, const HashTable& b);			// хз

	//friend bool operator!=(const HashTable& a, const HashTable& b);			// хз
};

//bool operator==(const HashTable& a, const HashTable& b);
//bool operator!=(const HashTable& a, const HashTable& b);

int main()
{
	HashTable a;
	Value* v = new Value;
	v->age = 10;
	v->weight = 100;
	Key k = "ABOBA";
	a.insert(k, *v);
	delete v;
	cout << a.contains(k);
}

