#include <iostream>
#include <string>
#include <iomanip>
#include "point.cpp"
#include "graph.cpp"

using namespace std;

struct router
{
	string name;
	point location;
	double range = 1;

	friend ostream &operator<<(ostream &os, router &r)
	{
		cout << r.name;
		//<< "\t: " << '(' << r.location.getX() << " \t,      " << r.location.getY() << ") \t";
		return os;
	}

	bool operator==(const router& r) const
	{
		return name == r.name;
	}

	bool operator!=(const router& r) const
	{
		return name != r.name;
	}
};

template <class T>
bool distance_sort(node<T> n1, node<T> n2)
{
	router r1 = n1.value, r2 = n2.value;
	point const origin(0, 0);
	return r1.location.distance(origin) < r2.location.distance(origin);
}

template <class T>
bool x_sort(node<T> n1, node<T> n2)
{
	router r1 = n1.value, r2 = n2.value;
	return r1.location.getX() < r2.location.getX();
}

template <class T>
bool y_sort(node<T> n1, node<T> n2)
{
	router r1 = n1.value, r2 = n2.value;
	return r1.location.getY() < r2.location.getY();
}

double get_attr_x(const node<router> &n)
{
	return n.value.location.getX();
}

double get_attr_y(const node<router> &n)
{
	return n.value.location.getY();
}

bool inside_range(router center, router p)
{
	/*double a = pow(p.location.getX() - center.location.getX(), 2) + pow(p.location.getY() - center.location.getY(), 2);
	return a <= 1;*/
	return true;
}

class udg_generation
{
private:
	double fRand(double fMin, double fMax)
	{
		double f = static_cast<double>(rand()) / RAND_MAX;
		return fMin + f * (fMax - fMin);
	}

public:
	graph<router> generate(int router_count, int grid_size)
	{
		graph<router> network(false);

		srand(0);
		for (int i = 0; i < router_count; i++)
		{
			router r;

			r.name = to_string(i + 1);
			r.location.setX(fRand(0, grid_size));
			r.location.setY(fRand(0, grid_size));

			network.insert_vertex(r);
		}
		network.sort_(x_sort);

		cout << "X-Sorted:\n";
		cout << fixed << setprecision(2);
		network.display();

		cout << "network.begin(): " << network.begin()->value.name << "\n";
		cout << "network.end() " << network.end()->value.name << "\n";

		for (auto node = network.begin(); node < network.end(); node++) {
			auto iter_l = network.get_lower_bound(node->value.location.getX() - node->value.range, get_attr_x);
			cout << "lower_bound: " << iter_l->value.name << '\n';

			auto iter_u = network.get_upper_bound(node->value.location.getX() + node->value.range, get_attr_x);
			cout << "upper_bound: " << iter_u->value.name << '\n';

			for (auto i = iter_l; i < iter_u; ++i) {
				if (node != i && inside_range(node->value, i->value)) // node != i avoids current router's connection to itself
					network.insert_edge(node->value, iter_l->value);
			}
		}
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