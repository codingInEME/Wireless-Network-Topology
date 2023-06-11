#ifndef GRAPHVIZ_H
#define GRAPHVIZ_H

#include "graph.h"
#include "router.h"
#include <fstream>


#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <cstdlib>
#elif __APPLE__
#include <cstdlib>
#endif

class graphviz{
public:
    void static generateFile(graph<router> &g, std::string fileName)
    {
        std::ofstream file;
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


    void static generateImage(std::string fileName)
    {
        std::string a = "dot -Kfdp -n -Tpdf -o" + fileName + ".pdf " + fileName + ".dot";
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


    void static openImage(std::string fileName){

    #ifdef _WIN32
        // Windows-specific code
        std::string a = "start " + fileName;
        const char *command = a.c_str();
        system(command);
    #elif __linux__
        std::string a = "gio open " + fileName;
        const char *command = a.c_str();
        system(command);
    #elif __APPLE__
        // macOS-specific code
        system(command);
    #else
    #error Unsupported operating system
    #endif
    }

    void static generateFile_highlight(graph<router> &g, std::string fileName, std::vector<node<router>> &v, bool only)
    {
        std::ofstream file;

        if(!only)
        {
            file.open(fileName + ".dot");
            file << "strict graph G {\n";
            for (auto vertex = g.begin(); vertex != g.end(); ++vertex)
            {
                file << "\t" << vertex->value.name << "\t[";

                if (find(v.begin(), v.end(), *vertex) != v.end())
                {
                    file << "color = red";
                }

                file << " pos = \"" << vertex->value.location.getX() << "," << vertex->value.location.getY() << "!\"]\n";

                file << "\t" << vertex->value.name << " -- {";
                for (auto neighbor = vertex->adj_vertices.begin(); neighbor != vertex->adj_vertices.end(); ++neighbor)
                    file << g[*neighbor]->value.name << " ";

                file << "}\n";
            }
            file << "}\n";

            file.close();
        }else{
            file.open(fileName + "_only.dot");
            file << "strict graph G {\n";
            for (int i = 0 ; i < v.size()-1;i++)
            {
                file << "\t" << v[i].value.name << "\t[";
                file << "color = red";
                file << " pos = \"" << v[i].value.location.getX() << "," << v[i].value.location.getY() << "!\"]\n";

                file << "\t" << v[i].value.name << " -- ";
                file << v[i+1].value.name << '\n';
            }
            file << "\t" << v[v.size()-1].value.name << "\t[";
            file << "color = red";
            file << " pos = \"" << v[v.size()-1].value.location.getX() << "," << v[v.size()-1].value.location.getY() << "!\"]\n";
            file << "}\n";

            file.close();
        }
    }



};


#endif // GRAPHVIZ_H
