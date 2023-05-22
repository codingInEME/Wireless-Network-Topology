#include <iostream>
#include <list>
#include <vector>
#include <queue>
#include <unordered_map>
#include <stack>
#include <algorithm>

using namespace std;

template <class T>
struct node {
    T name;
    list<T> adj_vertices;

    bool operator<(const node<T>& n) const
    {
        return name < n.name;
    }
};

template <class T>
class graph {
private:
    bool directed;
    vector<node<T>> vertices;

public:
    graph(bool directed) : directed(directed) {}
    void destroy() {
        vertices.clear();   
    }

    int vertex_count() {
        return vertices.size();
    }

    int edges_count() {
        int total = 0;

        for_each(vertices.begin(), vertices.end(), [&](node<T>& vertex) {
            total += vertex.adj_vertices.size();
            }
        );

        return directed ? total : total / 2;
    }

    node<T>* find_vertex(T name) {
        node<T>* found = nullptr;

        for_each(vertices.begin(), vertices.end(), [&](node<T>& vertex) {
            if (vertex.name == name)
                found = &vertex;
            }
        );
        return found;
    }

    bool edge_exist(T vertex1, T vertex2) {
        if (node<T>* obj = find_vertex(vertex1))
        {
            typename list<T>::template iterator location = find(obj->adj_vertices.begin(), obj->adj_vertices.end(), vertex2);

            return location == obj->adj_vertices.end() ? false : true;
        }
        return false;
    }

    void insert_vertex(T name) {
        node<T> v;
        v.name = name;
        vertices.push_back(v);
    }

    void edge_insert(T vertex1, T vertex2) {
        node<T>* obj1 = find_vertex(vertex1), * obj2 = find_vertex(vertex2);

        if (obj1 != nullptr && obj2 != nullptr && !edge_exist(vertex1, vertex2)) {
            obj1->adj_vertices.push_back(vertex2);

            if (!directed)
                obj2->adj_vertices.push_back(vertex1);
        }
    }

    void delete_vertex(T name)
    {
        vertices.erase(remove_if(vertices.begin(), vertices.end(), [&](node<T>& vertex) {
            vertex.adj_vertices.remove(name); // removing the vertex name from adjacency list of every vertex
            return vertex.name == name; // if vertex is found then it is erased from vector
            }
        ), vertices.end());
    }

    void delete_edge(T vertex1, T vertex2)
    {
        if (!edge_exist(vertex1, vertex2))
            return;

        node<T>* obj1 = find_vertex(vertex1), * obj2 = find_vertex(vertex2);

        obj1->adj_vertices.remove(vertex2);

        if (!directed)
        {
            if (!edge_exist(vertex2, vertex1))
                return;

            obj2->adj_vertices.remove(vertex1);
        }

    }

    void bfs(T start)
    {
        if (!find_vertex(start))
            return;

        queue<T> q;
        unordered_map<T, bool> visited;
        q.push(start);
        visited[start] = true;
        while (!q.empty()) {
            T curr = q.front();
            cout << curr << ' ';
            q.pop();
            node<T>* vertex = find_vertex(curr);

            for (auto adj_vert : vertex->adj_vertices)
            {
                if (visited[adj_vert])
                    continue;

                visited[adj_vert] = true;
                q.push(adj_vert);
            }
        }
        cout << '\n';
    }

    void dfs(T start)
    {
        if (!find_vertex(start))
            return;

        stack<T> s;
        unordered_map<T, bool> visited;
        s.push(start);
        visited[start] = true;
        while (!s.empty()) {
            T curr = s.top();
            cout << curr << ' ';
            s.pop();
            node<T>* vertex = find_vertex(curr);

            for (auto adj_vert : vertex->adj_vertices)
            {
                if (visited[adj_vert])
                    continue;

                visited[adj_vert] = true;
                s.push(adj_vert);
            }
        }
        cout << '\n';
    }

    bool isEmpty() {
        return vertices.empty();
    }

    void display()
    {
        for (auto vertex : vertices)
        {
            cout << vertex.name;
            cout << " : { ";
            for (auto edge : vertex.adj_vertices)
            {
                cout << edge << ' ';
            }
            cout << "}\n";
        }
    }

    void sort_()
    {
        std::sort(vertices.begin, vertices.end, [&](node<T> node_1, node<T> node_2) {return node_1.name < node_2.name;});
    }

    void sort_(bool (*func)(node<T>, node<T>))
    {
        std::sort(vertices.begin(), vertices.end(), func);
    }

    template <class key_type>
    typename vector<node<T>>::iterator get_lower_bound(key_type key, key_type (*attribute)(const node<T>&)) {
        return std::lower_bound(vertices.begin(), vertices.end(), key, 
            [&](node<T> obj, key_type k){
                return attribute(obj) < k;
        });
    }

    template <class key_type>
    typename vector<node<T>>::iterator get_upper_bound(key_type key, key_type(*attribute)(const node<T>&)) {
        return std::upper_bound(vertices.begin(), vertices.end(), key,
            [&](node<T> obj, key_type k) {
                return attribute(obj) < k;
            });
    }
};