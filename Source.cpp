#include <iostream>
#include <string>
#include <iomanip>
#include "point.cpp"
#include "graph.cpp"
#include <chrono>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <cstdlib>
#elif __APPLE__
#include <cstdlib>
#endif

using namespace std;

struct router
{
    string name;
    point location;
    double range = 12;

    friend ostream &operator<<(ostream &os, router &r)
    {
        cout << r.name;
        return os;
    }

    bool operator==(const router &r) const
    {
        return name == r.name;
    }

    bool operator!=(const router &r) const
    {
        return !(name == r.name);
    }
};

template <class T>
bool distance_sort(node<T> n1, node<T> n2)
{
    router r1 = n1.value, r2 = n2.value;
    const point origin(0, 0);
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

bool inside_range(router center, router p)
{
    double a = pow(p.location.getX() - center.location.getX(), 2) + pow(p.location.getY() - center.location.getY(), 2);
    return a <= pow(center.range, 2);
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

        auto start1 = std::chrono::high_resolution_clock::now();
        for (auto node = network.begin(); node < network.end() + 1; ++node)
        {
            auto iter_l = network.get_lower_bound(node->value.location.getX() - node->value.range, get_attr_x);

            auto iter_u = network.get_upper_bound(node->value.location.getX(), get_attr_x); // extreme right of circle

            for (auto i = iter_l; i < iter_u; ++i)
            {
                if (inside_range(node->value, i->value) && node != i)
                {
                    network.insert_edge(node, i);
                }
            }
        }
        auto stop1 = std::chrono::high_resolution_clock::now();
        auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1);

        cout << "\n"
             << duration1.count() << endl;

        return network;
    }
};

struct connection
{
    vector<node<router>>::iterator vertex;
    vector<node<router>>::iterator neighbor;

    connection(vector<node<router>>::iterator vertex, vector<node<router>>::iterator neighbor)
    {
        this->vertex = vertex;
        this->neighbor = neighbor;
    }
};

class topologyControl
{
public:
    graph<router> XTC_protocol(graph<router> &g)
    {
        list<connection> marked_for_delete;

        auto start1 = std::chrono::high_resolution_clock::now();
        bool allow = false;
        for (auto vertex = g.begin(); vertex != g.end(); ++vertex)
        {
            for (auto neighbor = vertex->adj_vertices.begin(); neighbor != vertex->adj_vertices.end(); ++neighbor)
            {
                for (auto nextNeighbor = (*neighbor)->adj_vertices.begin(); nextNeighbor != (*neighbor)->adj_vertices.end(); ++nextNeighbor)
                {
                    if (vertex->value.location.distance((*nextNeighbor)->value.location) < vertex->value.location.distance((*neighbor)->value.location) &&
                        ((*nextNeighbor)->value.location.distance((*neighbor)->value.location) < vertex->value.location.distance((*neighbor)->value.location)))
                    {
                        connection c(g.find_vertex_by_index(g.end() - vertex - 1), *neighbor);
                        marked_for_delete.push_back(c);
                    }
                }
            }
        }

        for (auto iter = marked_for_delete.begin(); iter != marked_for_delete.end(); ++iter)
        {
            g.delete_edge(iter->vertex, iter->neighbor);
        }

        auto stop1 = std::chrono::high_resolution_clock::now();
        auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1);

        cout << "\n"
             << duration1.count() << endl;

        return g;
    }
};

void generateFile(graph<router> &g, string fileName)
{
    ofstream file;
    file.open(fileName + ".dot");

    file << "strict graph G {\n";
    for (auto vertex = g.begin(); vertex != g.end(); ++vertex)
    {
        file << "\t" << vertex->value.name << "\t[pos = \""
             << vertex->value.location.getX() << "," << vertex->value.location.getY() << "!\"]\n";

        file << "\t" << vertex->value.name << " -- {";
        for (auto neighbor = vertex->adj_vertices.begin(); neighbor != vertex->adj_vertices.end(); ++neighbor)
            file << (*neighbor)->value.name << " ";
        file << "}\n";
    }
    file << "}\n";

    file.close();
}

void generateImage(string fileName)
{

    string a = "dot -Kfdp -n -Tpng -o" + fileName + ".png " + fileName + ".dot";
    const char *command = a.c_str();

#ifdef _WIN32
    // Windows-specific code
    system(command);
#elif __linux__
    // Linux-specific code
    system(command);
#elif __APPLE__
    // macOS-specific code
    system(command);
#else
#error Unsupported operating system
#endif
}

int main()
{
    udg_generation grid;
    topologyControl tc;

    graph<router> net = grid.generate(1000, 100);
    cout << "udg generated\n";
    net.display();
    generateFile(net, "graph");
    generateImage("graph");
    cout << "\n\n";

    auto start1 = std::chrono::high_resolution_clock::now();
    tc.XTC_protocol(net);
    auto stop1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1);
    cout << "\n"
         << duration1.count() << endl;

    net.display();
    generateFile(net, "graph_xtc");
    generateImage("graph_xtc");
}
