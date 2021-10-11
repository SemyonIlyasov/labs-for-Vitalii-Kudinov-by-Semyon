#pragma once
#include <iostream>
#include <list>
#include <iterator>
#include "L_2_t.cpp"

typedef struct Student Student;
typedef string Key;
typedef Student Value;
typedef struct wagon wagon;


bool operator==(pair<Key, Value> a, pair<Key, Value> b);

struct wagon;

class HashTable
{
protected:

	static const int default_size = 8;					// начальный размер таблицы
	constexpr static const double rehash_size = 0.75;	// коэффициент, при котором произойдет увеличение таблицы
	wagon* train;										// array of wagons
	int buf_size;										 // all space
	int alive;											 // not deleted
	double capacity;									 // (alive) / buf_size < 0.75

	unsigned int _hf(Key key, unsigned int sz)	const;
	void _rehash();
public:
	void output();
	HashTable()	noexcept;
	~HashTable() noexcept;
	HashTable(const HashTable& b) noexcept;
	HashTable(HashTable&& b) noexcept;


	HashTable& operator=(const HashTable& b);
	HashTable& operator=(HashTable&& b);


	void swap(HashTable& b);

	void clear();
	bool erase(const Key& k);
	bool insert(const Key& k, const Value& v);
	bool contains(const Key& k) const;

	Value& operator[](const Key& k);


	Value& at(const Key& k);
	const Value& at(const Key& k) const;
	double cap();
	size_t size() const;
	bool empty() const;
	friend bool operator==(const HashTable& a, const HashTable& b);			

	friend bool operator!=(const HashTable& a, const HashTable& b);		
};

bool operator==(const HashTable& a, const HashTable& b);
bool operator!=(const HashTable& a, const HashTable& b);