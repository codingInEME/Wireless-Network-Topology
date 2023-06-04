#ifndef UDG_GENERATION_H
#define UDG_GENERATION_H
#include <iostream>
#include "router.h"
#include "graph.h"
#include <chrono>


class udg_generation
{
private:
    double fRand(double fMin, double fMax)
    {
        double f = static_cast<double>(rand()) / RAND_MAX;
        return fMin + f * (fMax - fMin);
    }

    bool static distance_sort(node<router> n1, node<router> n2)
    {
        router r1 = n1.value, r2 = n2.value;
        const point origin(0, 0);
        return r1.location.distance(origin) < r2.location.distance(origin);
    }

    bool static x_sort(node<router> n1, node<router> n2)
    {
        router r1 = n1.value, r2 = n2.value;
        return r1.location.getX() < r2.location.getX();
    }

    bool static y_sort(node<router> n1, node<router> n2)
    {
        router r1 = n1.value, r2 = n2.value;
        return r1.location.getY() < r2.location.getY();
    }

    double static get_attr_x(const node<router> &n)
    {
        return n.value.location.getX();
    }

    bool inside_range(router center, router p)
    {
        double a = pow(p.location.getX() - center.location.getX(), 2) + pow(p.location.getY() - center.location.getY(), 2);
        return a <= pow(center.range, 2);
    }

public:
    graph<router> generate(int router_count, int grid_size, int range){
        graph<router> network(false);


        for (int i = 0; i < router_count; i++)
        {
            router r;

            r.name = std::to_string(i + 1);
            r.range = range;
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

        std::cout << "\n"
                  << duration1.count() << std::endl;

        return network;
    }
};




#endif // UDG_GENERATION_H
