#ifndef GRAPH_H
#define GRAPH_H


#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <set>
#include <stack>
#include <algorithm>
#include <cmath>

//using namespace std;
template <class T>
struct node
{
    T value;
    std::list<int> adj_vertices;

    bool operator<(const node<T> &n) const
    {
        return value < n.value;
    }

    bool operator==(const node<T> &n) const
    {
        return value == n.value;
    }

    bool operator!=(const node<T> &n) const
    {
        return !(value == n.value);
    }
};

template <class T>
class graph
{
private:
    bool directed;
    std::vector<node<T>> vertices;

    bool isOutOfVectorRange(int ID)
    {
        return ID < 0 || ID > vertex_count();
    }

public:
    graph(bool directed) : directed(directed) {}
    void destroy()
    {
        vertices.clear();
    }

    int vertex_count()
    {
        return vertices.size();
    }

    int edges_count()
    {
        int total = 0;

        for_each(vertices.begin(), vertices.end(), [&](node<T> &vertex)
                 { total += vertex.adj_vertices.size(); });

        return directed ? total : total / 2;
    }

    typename std::vector<node<T>>::iterator begin()
    {
        return find_vertex_by_index(0);
    }

    typename std::vector<node<T>>::iterator end()
    {
        return find_vertex_by_index(vertex_count() - 1);
    }

    typename std::vector<node<T>>::iterator find_vertex_by_index(int index)
    {
        auto iter = vertices.begin() + index;

        return iter;
    }

    typename std::vector<node<T>>::iterator find_vertex_by_name(std::string name)
    {
        auto iter = vertices.begin();
        while (iter < vertices.end())
        {
            if (iter->value.name == name)
                break;
            ++iter;
        }
        return iter;
    }

    bool edge_exist(int ID1, int ID2)
    {
        if (ID1 > vertex_count())
            return false;

        auto location = find(vertices[ID1].adj_vertices.begin(), vertices[ID1].adj_vertices.end(), ID2);
        return location == vertices[ID1].adj_vertices.end() ? false : true;
    }

    void insert_vertex(T name)
    {
        node<T> v;
        v.value = name;
        vertices.push_back(v);
    }

    /*void insert_edge(typename vector<node<T>>::iterator vertex1, int ID2)
    {
        if (isOutOfVectorRange(ID2))
            return;

        vertex1->adj_vertices.push_back(ID2);

        if (!directed && vertex1 - vertices.begin() != ID2)
            vertices[ID2].adj_vertices.push_back(vertex1 - vertices.begin());
    }*/

    void insert_edge(int ID1, int ID2)
    {
        if (isOutOfVectorRange(ID1) || isOutOfVectorRange(ID2))
            return;

        vertices[ID1].adj_vertices.push_back(ID2);

        if (!directed && ID1 != ID2)
            vertices[ID2].adj_vertices.push_back(ID1);
    }

    void delete_vertex(int ID)
    {
        vertices.erase(remove_if(vertices.begin(), vertices.end(), [&](node<T> &vertex)
                                 {
                                     vertex.adj_vertices.remove(ID); // removing the vertex value from adjacency list of every vertex
                                     return vertex == ID;            // if vertex is found then it is erased from vector
                                 }),
                       vertices.end());
    }

    void delete_edge(int ID1, int ID2)
    {
        if (!edge_exist(ID1, ID2))
            return;

        vertices[ID1].adj_vertices.remove(ID2);

        if (!directed)
            vertices[ID2].adj_vertices.remove(ID1);
    }

    bool isEmpty()
    {
        return vertices.empty();
    }

    std::string display()
    {
        std::string output ="";
        for (auto vertex : vertices)
        {
            output += (vertex.value.name) + " : (" + std::to_string(vertex.value.location.getX()) + ", " + std::to_string(vertex.value.location.getY()) + ")";
            output += " : { ";
            for (auto adjacent_vertex : vertex.adj_vertices)
            {
                output += (vertices[adjacent_vertex].value.name) + ' ';
            }
            output += "}\n";
        }

        std::cout << output;
        return output;
    }

    void sort_()
    {
        std::sort(vertices.begin, vertices.end, [&](node<T> node_1, node<T> node_2)
                  { return node_1.value < node_2.value; });
    }

    void sort_(bool (*func)(node<T>, node<T>))
    {
        std::sort(vertices.begin(), vertices.end(), func);
    }

    template <class key_type>
    typename std::vector<node<T>>::iterator get_closest(key_type key, key_type (*attribute)(const node<T> &))
    {
        typename std::vector<node<T>>::iterator begin = vertices.begin();
        typename std::vector<node<T>>::iterator end = vertices.end();
        typename std::vector<node<T>>::iterator it = vertices.begin();
        // if(list.)
        if (key > attribute(*--vertices.end()))
            return --vertices.end();

        while (attribute(*it) != key)
        {
            int size = (end - begin) / 2;
            it = begin + size;
            if (attribute(*it) != key)
            {
                if (key > attribute(*it))
                {
                    begin = ++it;
                }
                else
                {
                    if (it != begin)
                        end = --it;
                    else
                        return it;
                }
            }
        }
        return it;
    }

    typename std::vector<node<T>>::iterator operator[](int index)
    {
        return find_vertex_by_index(index);
    }
};

#endif // GRAPH_H
