#ifndef TOPOLOGY_CONTROL_H
#define TOPOLOGY_CONTROL_H

#include <iostream>
#include "router.h"
#include "graph.h"
#include <chrono>


class topologyControl
{
public:
    graph<router> XTC_protocol(graph<router> &g)
    {
        graph<router> h = g;

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
                        h.delete_edge(vertex - g.begin(), *neighbor);
                    }
                }
            }
        }

        auto stop1 = std::chrono::high_resolution_clock::now();
        auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1);

        std::cout << "\n"
                  << duration1.count() << std::endl;

        return h;
    }
};


#endif // TOPOLOGY_CONTROL_H
