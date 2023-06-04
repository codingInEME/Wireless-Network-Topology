#ifndef TOPOLOGY_CONTROL_H
#define TOPOLOGY_CONTROL_H

#include <iostream>
#include "router.h"
#include "graph.h"
#include <chrono>


class topologyControl
{
private:
    struct connection
    {
        std::vector<node<router>>::iterator vertex;
        int neighborID;

        connection(std::vector<node<router>>::iterator vertex, int neighborID)
        {
            this->vertex = vertex;
            this->neighborID = neighborID;
        }
    };

public:
    graph<router> XTC_protocol(graph<router> g)
    {
        std::list<connection> marked_for_delete;

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

        std::cout << "\n"
                  << duration1.count() << std::endl;

        return g;
    }
};


#endif // TOPOLOGY_CONTROL_H
