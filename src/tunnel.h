#ifndef __TUNNEL_H__
#define __TUNNEL_H__
#include "point.h"
#include <list>

class Tunnel
{
	public:
		Tunnel(const Point& a, const Point& b, int length = 0): a(a), b(b),
		length(length)
		{
		};

		~Tunnel() { 
		};

		const Point a;
		const Point b;

		bool operator<(const Tunnel& t1) const;
		bool operator==(const Tunnel& t) const;
		bool operator!=(const Tunnel& a) const;

		static std::list<Tunnel> tunnels;
		const int length;


};

/*
Tunnel Tunnel::operator+(const Tunnel& a, const Tunnel& b)
{
	return Tunnel(a.a + b.a, a.b+b.b);
};

Tunnel operator-(const Tunnel& a, const Tunnel& b)
{
	return Tunnel(a.a - b.a, a.b-b.b);
};
*/



#endif /*__TUNNEL_H__*/
