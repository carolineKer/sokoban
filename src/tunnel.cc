#include "tunnel.h"
#include <iostream>

std::list<Tunnel> Tunnel::tunnels;

bool Tunnel::operator==(const Tunnel& t) const
{
	std::cout << "Compare " << t.a.i << " " << 
		t.a.j << " " << std::endl;
	std::cout << "With " << this->a.i << " " << 
		this->a.j << " " << std::endl;

	return(this->a == t.a || 
			this->a == t.b ||
			this->b == t.a ||
			this->b == t.b);
}

bool Tunnel::operator!=(const Tunnel& a) const
{
	return  !(*this == a);
}

bool Tunnel::operator<(const Tunnel& t1) const
{
	return (this->a < t1.a && this->b < t1.b);
}
