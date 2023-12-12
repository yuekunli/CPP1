#pragma once
namespace VectorSeparateDeclareDefine1 {


template <typename Object>
class Vector
{
public:
	explicit Vector(int initSize = 0);

	Vector(const Vector& rhs);

	Vector& operator=(const Vector& rhs);

	~Vector();

	Vector(Vector&& rhs);

	Vector& operator=(Vector&& rhs);

	void resize(int newSize);

	void reserve(int newCapacity);

	Object& operator[](int index);

	const Object& operator[] (int index) const;

	bool empty() const;

	int size() const;

	int capcity() const;

	void push_back(const Object& x);

	void push_back(Object&& x);

	void pop_back();

	const Object& back() const;

	typedef Object* iterator;
	typedef const Object* const_iterator;

	iterator begin();

	const_iterator begin() const;

	iterator end();

	const_iterator end() const;



	static const int SPARE_CAPACITY = 16;


private:
	int theSize;
	int theCapacity;
	Object* objects;
};


}