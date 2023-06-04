#include <iostream>
#include <string>
#include <iomanip>
#include "graph.h"
#include "router.h"
#include "path.h"
#include <chrono>
#include <fstream>
#include "udg_generation.h"
#include "topology_control.h"
#include "graphviz.h"




void totalDegree_and_max(graph<router> &g, int &totalDegree, int &maxDegree)
{
    totalDegree = 0;
    maxDegree = 0;
    for_each(g.begin(), g.end(), [&](node<router> &vertex)
             {
                 int currDegree = vertex.adj_vertices.size();
                 totalDegree += currDegree;
                 if(currDegree > maxDegree)
                     maxDegree = currDegree; });
}


std::string run_experiments(bool printGraphs)
{
    udg_generation gen;
    topologyControl tc;
    const int GRID_SIZE = 10;

    std::string output = "";

    output += "\t\tEXPERIMENT - I\n";
    std::cout << "\t\tEXPERIMENT - I\n";

     output += "\tWithout Topology Control\t\t\n\n";
    std::vector<graph<router>> networks1;
    std::vector<graph<router>> networks1_xtc;

    for (int val = 500; val < 1000; val = val + 50)
    {
        int totalDegree, maxDegree;
        float avgDegree;

        networks1.push_back(gen.generate(val, GRID_SIZE,1));

        totalDegree_and_max(networks1[networks1.size() - 1], totalDegree, maxDegree);
        avgDegree = totalDegree / static_cast<float>(val);

        output += "network " + std::to_string(val) + ":\n";
        output += "average degree = " + std::to_string(avgDegree) + '\n';
        output += "maximum degree = " + std::to_string(maxDegree) + "\n\n";

        std::cout << "network " << val << ":\n";
        std::cout << "average degree = " << avgDegree << '\n';
        std::cout << "maximum degree = " << maxDegree << "\n\n";
    }

    output += "\tWith Topology Control\t\t\n\n";

    for (int i = 0; i < 10; i++)
    {
        int totalDegree, maxDegree;
        float avgDegree;

        networks1_xtc.push_back(tc.XTC_protocol(networks1[i]));

        totalDegree_and_max(networks1_xtc[networks1_xtc.size() - 1], totalDegree, maxDegree);
        avgDegree = totalDegree / static_cast<float>(networks1_xtc[i].vertex_count());

        output += "network " + std::to_string(networks1_xtc[i].vertex_count()) + ":\n";
        output += "average degree = " + std::to_string(avgDegree) + '\n';
        output += "maximum degree = " + std::to_string(maxDegree) + "\n\n";

        std::cout << "network " << networks1_xtc[i].vertex_count() << '\n';
        std::cout << "average degree = " << avgDegree << '\n';
        std::cout << "maximum degree = " << maxDegree << "\n\n";
    }

     output += "\t\t--------------------\n\n";
    std::cout << "\t\t--------------------\n\n";

     output += "\t\tEXPERIMENT - II\n";
    std::cout << "\t\tEXPERIMENT - II\n";

    int router_count = 1000;
    graph<router> network2 = gen.generate(router_count, 10,1);
    graph<router> network2_xtc = tc.XTC_protocol(network2);
    struct temp
    {
        int source, dest;
        temp(int source, int dest) : source(source), dest(dest) {}
    };
    std::vector<temp> randVertices;

    path pObj;
    for (int i = 0; i < 10; i++)
    {
        randVertices.push_back(temp(rand() % router_count, rand() % router_count));

        output += "source: " + network2[randVertices[i].source]->value.name + " destination: " + network2[randVertices[i].dest]->value.name + '\n';
        output += "path: ";

        std::cout << "source: " << network2[randVertices[i].source]->value.name << " destination: " << network2[randVertices[i].dest]->value.name << '\n';
        std::cout << "path: ";
        auto p = pObj.find_path(network2, randVertices[i].source, randVertices[i].dest);


        output += path::display_path(p);
        if (p.size() == 0)
        {
            output += "path does not exist\n";
            std::cout << "path does not exist\n";
        }else {
            output += "Distance of path: " + std::to_string(path::path_distance(p)) + "\n\n";
            std::cout << "Distance of path: " << (path::path_distance(p)) << "\n\n";
        }

        std::cout << '\n';
    }

     output += "\t\t--------------------\n\n";
    std::cout << "\t\t--------------------\n\n";

    output += "\t\tEXPERIMENT - III\n";
    std::cout << "\t\tEXPERIMENT - III\n";

    for (int i = 0; i < 10; i++)
    {
        output += "source: " + network2_xtc[randVertices[i].source]->value.name + " destination: " + network2_xtc[randVertices[i].dest]->value.name + '\n';
        output += "path: ";

        std::cout << "source: " << network2_xtc[randVertices[i].source]->value.name << " destination: " << network2_xtc[randVertices[i].dest]->value.name << '\n';
        std::cout << "path: ";
        auto p = pObj.find_path(network2_xtc, randVertices[i].source, randVertices[i].dest);

        output += path::display_path(p);
        if (p.size() == 0){
            output += "path does not exist\n";
            std::cout << "path does not exist\n";
        }else {
            output += "Distance of path: " + std::to_string(path::path_distance(p)) + "\n\n";
            std::cout << "Distance of path: " << (path::path_distance(p)) << "\n\n";
        }

        output += '\n';
        std::cout << '\n';
    }

    if (printGraphs)
    {
        std::cout << "\t\tGenerating Graph DOT Files...\n\n";
        for (int i = 0; i < 10; i++)
        {
            graphviz::generateFile(networks1[i], "graph_" + std::to_string(networks1[i].vertex_count()));
            graphviz::generateFile(networks1_xtc[i], "graph_" + std::to_string(networks1[i].vertex_count()) + "_xtc");
        }

        std::cout << "\t\tGenerating Graph PDF...\n\n";
        for (int i = 0; i < 10; i++)
        {
            graphviz::generateImage("graph_" + std::to_string(networks1[i].vertex_count()));
            graphviz::generateImage("graph_" + std::to_string(networks1[i].vertex_count()) + "_xtc");
        }

        graphviz::generateImage("graph_1000");
        graphviz::generateImage("graph_1000_xtc");
    }

    return output;
}



//int main()
//{
//    udg_generation grid;
//    topologyControl tc;

//    std::cout << "Welcome to Wireless Network Topology" << std::endl;
//    std::cout << "Enter number of routers :";
//    int no_routers,grid_size;
//    std::cin >> no_routers;
//    std::cout << "Enter grid size SxS:";
//    std::cin >> grid_size;
//    bool loop = true;
//    graph<router> net(false);
//    graph<router> xtc_net(false);
//    int input;

//    while(loop){
//        std::cout << " 1. Generate Graph. \n 2. Apply Topology Control. \n 3. Display Graph. \n 4. Display Graph with Topology Control.\n";
//        std::cout << "Enter choice: ";
//        std::cin >> input;
//        switch (input)
//        {
//        case 1:
//        {
//            net = grid.generate(no_routers, grid_size);
//            //net.display();
//            std::cout << "Graph generated\n";
//        }
//        break;
//        case 2:
//        {
//            auto start1 = std::chrono::high_resolution_clock::now();
//            xtc_net = tc.XTC_protocol(net);
//            auto stop1 = std::chrono::high_resolution_clock::now();
//            auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1);
//            std::cout << "\n" << duration1.count() << std::endl;
//        }
//        break;
//        case 3:
//        {
//            net.display();
//        }
//        break;
//        case 4:
//        {
//            xtc_net.display();
//        }
//        break;
//        default:
//            loop = false;
//            break;
//        }
//    }

//    //net.display();
//    auto start3 = std::chrono::high_resolution_clock::now();
//    generateFile(net, "graph");
//    generateImage("graph");
//    std::cout << "\n\n";
//    auto stop3 = std::chrono::high_resolution_clock::now();
//    auto duration3 = std::chrono::duration_cast<std::chrono::microseconds>(stop3 - start3);
//    std::cout << "\n"
//              << duration3.count() << std::endl;


//    //net.display();
//    auto start2 = std::chrono::high_resolution_clock::now();
//    generateFile(xtc_net, "graph_xtc");
//    generateImage("graph_xtc");
//    auto stop2 = std::chrono::high_resolution_clock::now();
//    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start2);
//    std::cout << "\n"
//              << duration2
//                     .count()
//              << std::endl;
//}
