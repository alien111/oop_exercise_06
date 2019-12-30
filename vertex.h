#ifndef D_VERTEX_H
#define D_VERTEX_H 1

#include <iostream>

template<class T>
struct vertex {
	T x;
	T y;

};

template<class T>
std::istream& operator>> (std::istream& is, vertex<T>& p) {
	is >> p.x >> p.y;
	return is;
}

template<class T>
std::ostream& operator<< (std::ostream& os, const vertex<T>& p) {
	os << p.x << ' ' << p.y << '\n';
	return os;
}

template<class T>
vertex<T> operator+(vertex<T> lhs,vertex<T> rhs){
    vertex<T> res;
    res.x = lhs.x + rhs.x;
    res.y = lhs.y + rhs.y;
    return res;
}

template<class T>
bool operator == (vertex<T> a, vertex<T> b) {
	return (a.x == b.x && a.y == b.y);
}

template<class T>
bool operator != (vertex<T> a, vertex<T> b) {
	return (a.x != b.x || a.y != b.y);
}


template<class T>
vertex<T>& operator/= (vertex<T>& vertex, int number) {
    vertex.x = vertex.x / number;
    vertex.y = vertex.y / number;
    return vertex;
}

#endif // D_VERTEX_H