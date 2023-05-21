#include <iostream>
#include <vector>
#include "point.cpp"
#include "graph.cpp"

using namespace std;

struct router
{
	string name;
	point location;
};

template<class T>
bool distance_sort(node<T> n1, node<T> n2)
{
	router r1 = n1.name, r2 = n2.name;
	point const origin(0, 0);
	return r1.location.distance(origin) < r2.location.distance(origin);
}

class udg_generation
{
private:
	int randrange(int low, int high)
	{
		srand(time_t(0));
		return rand() % (high - low + 1) + low;
	}
public:
	vector<router> generate(int router_count, int grid_size)
	{
		int const half_size = grid_size / 2;
		graph<router> network(false);
		for (int i = 0; i < router_count; i++) {
			router r;

			r.location.setX(randrange(0 - half_size, half_size));
			r.location.setY(randrange(0 - half_size, half_size));

			network.insert_vertex(r);
		}
		network.sort_(distance_sort);
	}
};

int main()
{

	return 0;
}