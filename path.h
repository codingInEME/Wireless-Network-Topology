#ifndef PATH_H
#define PATH_H

#include"router.h"
#include"point.h"
#include<vector>
#include "graph.h"


class path
{
    double find_angle(point current, point next, point destination)
    {
        double a = destination.distance(next);
        double b = destination.distance(current);
        double c = current.distance(next);
        return acos((b * b + c * c - a * a) / (2 * b * c));
    }

    bool isVisited(int ID, std::vector<int> &vertices)
    {
        auto iter = vertices.begin();
        while (iter != vertices.end())
        {
            if (*iter == ID)
                return true;
            ++iter;
        }
        return false;
    }

    std::vector<node<router>> gen_path_vector(graph<router> &g, std::stack<std::vector<node<router>>::iterator> path, std::vector<int> visited)
    {
        std::vector<node<router>> v;
        while (!path.empty())
        {
            if (isVisited(path.top() - g.begin(), visited)) // common nodes are the actual path
                v.push_back(*path.top());
            path.pop();
        }
        std::reverse(v.begin(), v.end());
        return v;
    }

public:
    std::string static display_path(const std::vector<node<router>> &path)
    {
        std::string output="";
        for (auto iter = path.begin(); iter != path.end(); ++iter)
        {
            output += (iter->value.name) + "->";
        }
        output = output.substr(0, output.size()-2);
        output += '\n';

        std::cout << output;
        return output;
    }


    std::vector<node<router>> find_path(graph<router> &g, std::string start, std::string destination)
    {
        return find_path(g, g.find_vertex_by_name(start) - g.begin(), g.find_vertex_by_name(destination) - g.begin());
    }

    std::vector<node<router>> find_path(graph<router> &g, int start, int destination)
    {
        std::stack<std::vector<node<router>>::iterator> path;

        std::vector<node<router>>::iterator current_node = g[start], destination_node = g[destination];

        path.push(current_node);
        std::vector<int> visited;
        while (true)
        {
            while (!path.empty() && isVisited(path.top() - g.begin(), visited)) // removing all visited nodes from stack
                path.pop();

            if (path.empty())
                break;

            auto curr = path.top();
            visited.push_back(curr - g.begin());

            if (visited[visited.size() - 1] == destination)
                break;

            curr->adj_vertices.sort([&](int &n1, int &n2)
                                    {
                                        double n1_angle = find_angle(curr->value.location, g[n1]->value.location, destination_node->value.location);
                                        double n2_angle = find_angle(curr->value.location, g[n2]->value.location, destination_node->value.location);

                                        return n2_angle < n1_angle; });

            for (auto adj_vert : curr->adj_vertices)
            {
                if (isVisited(adj_vert, visited))
                    continue;

                path.push(g[adj_vert]);
            }
        }
        return gen_path_vector(g, path, visited);
    }

    double static path_distance(std::vector<node<router>> path_vec){
        double total_distance = 0;
        for(int i = 0 ; i < path_vec.size()-1; i++){
            total_distance += path_vec[i].value.location.distance(path_vec[i+1].value.location);
        }
        return total_distance;
    }
};



#endif // PATH_H
