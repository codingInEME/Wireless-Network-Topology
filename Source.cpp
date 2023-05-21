#include <iostream>
#include <string>
#include "point.cpp"
#include "graph.cpp"

using namespace std;

struct router
{
	string name;
	point location;

	friend ostream& operator<<(ostream& os, router& r)
	{
		cout << r.name << "\t: " << '(' << r.location.getX() << "\t,      " << r.location.getY() << "\t";
		return os;
	}
};

template<class T>
bool distance_sort(node<T> n1, node<T> n2)
{
	router r1 = n1.name, r2 = n2.name;
	point const origin(0, 0);
	return r1.location.distance(origin) < r2.location.distance(origin);
}

template<class T>
bool x_sort(node<T> n1, node<T> n2)
{
	router r1 = n1.name, r2 = n2.name;
	return r1.location.getX() < r2.location.getX();
}

template<class T>
bool y_sort(node<T> n1, node<T> n2)
{
	router r1 = n1.name, r2 = n2.name;
	return r1.location.getY() < r2.location.getY();
}

class udg_generation
{
private:
	int randrange(int low, int high)
	{
		return rand() % (high - low + 1) + low;
	}
public:
	graph<router> generate(int router_count, int grid_size)
	{
		graph<router> network(false);

		srand(time_t(0));
		for (int i = 0; i < router_count; i++) {
			router r;

			r.name = to_string(i + 1);
			r.location.setX(randrange(0, grid_size));
			r.location.setY(randrange(0, grid_size));

			network.insert_vertex(r);
		}
		network.sort_(x_sort);

		cout << "X-Sorted:\n";
		network.display();

		return network;
	}
};

int main()
{
	udg_generation grid;

	graph<router> net = grid.generate(10, 50);
	
	return 0;
}