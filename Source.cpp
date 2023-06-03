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
    double range = 1;

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

bool distance_sort(node<router> n1, node<router> n2)
{
    router r1 = n1.value, r2 = n2.value;
    const point origin(0, 0);
    return r1.location.distance(origin) < r2.location.distance(origin);
}

bool x_sort(node<router> n1, node<router> n2)
{
    router r1 = n1.value, r2 = n2.value;
    return r1.location.getX() < r2.location.getX();
}

bool y_sort(node<router> n1, node<router> n2)
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
            auto iter_l = network.get_closest(node->value.location.getX() - node->value.range, get_attr_x);

            auto iter_u = network.get_closest(node->value.location.getX(), get_attr_x); // extreme right of circle

            for (auto i = iter_l; i < iter_u; ++i)
            {
                if (inside_range(node->value, i->value) && node != i)
                {
                    network.insert_edge(node - network.begin(), i - network.begin());
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
    int neighborID;

    connection(vector<node<router>>::iterator vertex, int neighborID)
    {
        this->vertex = vertex;
        this->neighborID = neighborID;
    }
};

class topologyControl
{
public:
    graph<router> XTC_protocol(graph<router> &g)
    {
        list<connection> marked_for_delete;

        auto start1 = std::chrono::high_resolution_clock::now();

        for (auto vertex = g.begin(); vertex != g.end(); ++vertex)
        {
            for (auto neighbor = vertex->adj_vertices.begin(); neighbor != vertex->adj_vertices.end(); ++neighbor)
            {
                for (auto nextNeighbor = g[*neighbor]->adj_vertices.begin(); nextNeighbor != g[*neighbor]->adj_vertices.end(); ++nextNeighbor)
                {
                    if (vertex->value.location.distance((g[*nextNeighbor]->value.location)) < vertex->value.location.distance((g[*neighbor]->value.location)) &&
                        (g[*nextNeighbor]->value.location.distance(g[*neighbor]->value.location) < vertex->value.location.distance(g[*neighbor]->value.location)))
                    {
                        connection c(g[vertex - g.begin()], *neighbor);
                        marked_for_delete.push_back(c);
                    }
                }
            }
        }

        for (auto iter = marked_for_delete.begin(); iter != marked_for_delete.end(); ++iter)
        {
            g.delete_edge(iter->vertex - g.begin(), iter->neighborID);
        }

        auto stop1 = std::chrono::high_resolution_clock::now();
        auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1);

        cout << "\n"
             << duration1.count() << endl;

        return g;
    }
};

class path
{
    graph<router> g;

    double find_angle(point current, point next, point destination)
    {
        double a = destination.distance(next);
        double b = destination.distance(current);
        double c = current.distance(next);
        return (b * b + c * c - a * a) / (2 * b * c);
    }

    bool isVisited(node<router> vertex, vector<node<router>> &vertices)
    {
        auto iter = vertices.begin();
        while (iter < vertices.end())
        {
            if (iter->value.name == vertex.value.name)
                return true;
            ++iter;
        }
        return false;
    }

public:
    path(graph<router> &g) : g(g) {}

    // template <class T>
    vector<node<router>> find_path(string start, string destination)
    {
        vector<node<router>> path_vec;
        vector<node<router>> is_visited;

        vector<node<router>>::iterator current_node = g.find_vertex_by_name(start);
        vector<node<router>>::iterator destination_node = g.find_vertex_by_name(destination);
        path_vec.push_back(*current_node);
        int next_node;
        while (current_node->value != destination_node->value && !isVisited(*current_node, is_visited))
        {
            is_visited.push_back(*current_node);
            double next_angle = -2;
            for (auto adj_node = current_node->adj_vertices.begin(); adj_node != current_node->adj_vertices.end(); ++adj_node)
            {
                double temp_angle = (find_angle(current_node->value.location, g[*adj_node]->value.location, destination_node->value.location));
                if (temp_angle > next_angle)
                {
                    next_angle = temp_angle;
                    next_node = (*adj_node);
                }
            }
            current_node = g[next_node];
            path_vec.push_back(*current_node);
        }

        return path_vec;
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
            file << g[*neighbor]->value.name << " ";

        file << "}\n";
    }
    file << "}\n";

    file.close();
}

void generateImage(string fileName)
{

    string a = "dot -Kfdp -n -Tsvg -o" + fileName + ".svg " + fileName + ".dot";
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

    cout << "Welcome to Wireless Network Topology" << endl;
    cout << "Enter number of routers :";
    int no_routers,grid_size;
    cin >> no_routers;
    cout << "Enter grid size SxS:";
    cin >> grid_size;
    bool loop = true;
    graph<router> net(false);
    int input;

    while(loop){
        cout << " 1. Generate Graph. \n 2. Apply Topology Control. \n 3. Display Graph. \n 4. Display Graph with Topology Control.\n";
        cout << "Enter choice: ";
        cin >> input;
        switch (input)
        {
        case 1:
        {
            net = grid.generate(no_routers, grid_size);
            //net.display();
            cout << "Graph generated\n";
        }
        break;
        default:
         loop = false;
        break;
        }
    }

    //net.display();
    auto start3 = std::chrono::high_resolution_clock::now();
    generateFile(net, "graph");
    generateImage("graph");
    cout << "\n\n";
    auto stop3 = std::chrono::high_resolution_clock::now();
    auto duration3 = std::chrono::duration_cast<std::chrono::microseconds>(stop3 - start3);
    cout << "\n"
         << duration3.count() << endl;

    auto start1 = std::chrono::high_resolution_clock::now();
    tc.XTC_protocol(net);
    auto stop1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1);
    cout << "\n"
         << duration1.count() << endl;

    net.display();
    auto start2 = std::chrono::high_resolution_clock::now();
    generateFile(net, "graph_xtc");
    generateImage("graph_xtc");
    auto stop2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start2);
    cout << "\n"
         << duration2
                .count()
         << endl;
}
