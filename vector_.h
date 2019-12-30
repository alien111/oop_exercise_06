#ifndef VECTOR_H_
#define VECTOR_H_ 

#include "vertex.h"
#include <cmath>
#include <numeric>
#include <limits>

template<class T>
struct Vector {
	explicit Vector(T a, T b);
	double length() const;
	double x;
	double y;
	double operator* (Vector b) ;
	bool operator== (Vector b);
};

template<class T>
Vector<T>::Vector(T a, T b) {
	x = b.x - a.x;
	y = b.y - a.y;
}

template<class T>
double Vector<T>::length() const{
	return sqrt(x * x + y * y);
}

template<class T>
double Vector<T>::operator* (Vector<T> b) {
	return x * b.x + y * b.y;
}

template<class T>
bool Vector<T>::operator== (Vector<T> b) {
	return std::abs(x - b.x) < std::numeric_limits<double>::epsilon() * 100 
	&& std::abs(y - b.y) < std::numeric_limits<double>::epsilon() * 100;
}

template<class T>
bool isParallel(const Vector<T> a, const Vector<T> b) {
	return (a.x * b.y - a.y * b.y) == 0;
}

template<class T>
bool isPerpendicular(const Vector<T> a, const Vector<T> b) {
	return (a.x * b.x + a.y * b.y) == 0;
}


#endif