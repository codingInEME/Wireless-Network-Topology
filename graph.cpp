#include <iostream>
#include <list>
#include <vector>
#include <queue>
#include <unordered_map>
#include <set>
#include <stack>
#include <algorithm>
#include <cmath>

using namespace std;

template <class T>
struct node
{
    T value;
    list<typename vector<node<T>>::iterator> adj_vertices;

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
    vector<node<T>> vertices;

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

    typename vector<node<T>>::iterator begin()
    {
        return find_vertex_by_index(0);
    }

    typename vector<node<T>>::iterator end()
    {
        return find_vertex_by_index(vertex_count() - 1);
    }

    /*typename vector<node<T>>::iterator find_vertex(T name)
    {
        auto iter = vertices.begin();
        while (iter < vertices.end())
        {
            if (iter->value == name)
                break;
            ++iter;
        }

        return iter;
    }*/

    typename vector<node<T>>::iterator find_vertex_by_index(int index)
    {
        auto iter = vertices.begin() + index;

        return iter;
    }

    bool edge_exist(typename vector<node<T>>::iterator vertex1, typename vector<node<T>>::iterator vertex2)
    {
        auto obj = vertex1;

        if (obj != vertices.end())
        {
            auto location = find(obj->adj_vertices.begin(), obj->adj_vertices.end(), vertex2);

            return location == obj->adj_vertices.end() ? false : true;
        }
        return false;
    }

    void insert_vertex(T name)
    {
        node<T> v;
        v.value = name;
        vertices.push_back(v);
    }

    void insert_edge(typename vector<node<T>>::iterator vertex1, typename vector<node<T>>::iterator vertex2)
    {

        auto obj1 = vertex1, obj2 = vertex2;

        if (obj1 != vertices.end() && obj2 != vertices.end()) //&& !edge_exist(vertex1, vertex2->value))
        {
            vertex1->adj_vertices.push_back(vertex2);

            if (!directed && vertex1 != vertex2)
                vertex2->adj_vertices.push_back(vertex1);
        }
    }

    void delete_vertex(typename vector<node<T>>::iterator vertex_name)
    {
        vertices.erase(remove_if(vertices.begin(), vertices.end(), [&](node<T> &vertex)
                                 {
                                     vertex.adj_vertices.remove(vertex_name); // removing the vertex value from adjacency list of every vertex
                                     return vertex == *vertex_name;           // if vertex is found then it is erased from vector
                                 }),
                       vertices.end());
    }

    void delete_edge(typename vector<node<T>>::iterator vertex1, typename vector<node<T>>::iterator vertex2)
    {
        auto obj1 = vertex1, obj2 = vertex2;

        obj1->adj_vertices.remove(vertex2);

        if (!directed)
            obj2->adj_vertices.remove(vertex1);
    }

    void bfs(typename vector<node<T>>::iterator start)
    {
        queue<typename vector<node<T>>::iterator> q;
        set<typename vector<node<T>>::iterator> visited;
        q.push(start);
        visited.insert(start);
        while (!q.empty())
        {
            auto curr = q.front();
            cout << curr->value << ' ';
            q.pop();
            auto vertex = curr;

            for (auto adj_vert : vertex->adj_vertices)
            {
                if (visited.find(adj_vert) != visited.end())
                    continue;

                visited.insert(adj_vert);
                q.push(adj_vert);
            }
        }
        cout << '\n';
    }

    void dfs(typename vector<node<T>>::iterator start)
    {
        if (find_vertex(start) == vertices.end())
            return;

        stack<typename vector<node<T>>::iterator> s;
        set<typename vector<node<T>>::iterator> visited;
        s.push(start);
        visited.insert(start);
        while (!s.empty())
        {
            auto curr = s.top();
            cout << curr->value << ' ';
            s.pop();
            auto vertex = find_vertex(curr);

            for (auto adj_vert : vertex->adj_vertices)
            {
                if (visited.find(adj_vert) != visited.end())
                    continue;

                visited.insert(adj_vert);
                s.push(adj_vert);
            }
        }
        cout << '\n';
    }

    bool isEmpty()
    {
        return vertices.empty();
    }

    void display()
    {
        for (auto vertex : vertices)
        {
            cout << vertex.value << " : (" << vertex.value.location.getX() << ", " << vertex.value.location.getY() << ")";
            cout << " : { ";
            for (auto adjacent_vertex : vertex.adj_vertices)
            {
                cout << adjacent_vertex->value << ' ';
            }
            cout << "}\n";
        }
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
    typename vector<node<T>>::iterator get_lower_bound(key_type key, key_type (*attribute)(const node<T> &))
    {
        typename vector<node<T>>::iterator begin = vertices.begin();
        typename vector<node<T>>::iterator end = vertices.end();
        typename vector<node<T>>::iterator it = vertices.begin();
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
                /*if (begin == end || begin > end) {
                    if (*it > key) return it;
                    return ++it;
                }*/
            }
        }
        return it;
    }

    template <class key_type>
    typename vector<node<T>>::iterator get_upper_bound(key_type key, key_type (*attribute)(const node<T> &))
    {
        typename vector<node<T>>::iterator begin = vertices.begin();
        typename vector<node<T>>::iterator end = vertices.end();
        typename vector<node<T>>::iterator it = vertices.begin();
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
                /*if (begin == end || begin > end) {
                    if (*it > key) return it;
                    return ++it;
                }*/
            }
        }
        return it;
    }
};