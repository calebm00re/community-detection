//
// Created by Caleb Moore on 10/31/21.
//

#ifndef PA03_3353_GRAF_H
#define PA03_3353_GRAF_H
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <math.h>
#include <boost/graph/graph_utility.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/property_map/dynamic_property_map.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/connected_components.hpp>

using namespace std;
using namespace boost;

struct GraphData {
    string label;
    long value;
    long node_id;
};

class graf {
private:
    typedef adjacency_list<vecS, vecS, undirectedS, GraphData> Graph;
    typedef graph_traits<Graph>::vertex_descriptor VertexDesc;
    typedef graph_traits<Graph>::edge_descriptor edgeDesc;
    typedef graph_traits<Graph>::vertex_iterator vertex_iter;
    Graph g;
    std::map<pair<int, int>, int> edgeCount;
    double edgeTot;
    double modNum = -1;
    Graph best;
    int toCut = 0;
    int finalCut;

public:
    void make_graf(string &);
    void print_v();
    void print_e();
    void make_communities();
    void paths();
    void modularity();
    void make_output();
};
#endif //PA03_3353_GRAF_H
