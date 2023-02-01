//
// Created by Caleb Moore on 10/31/21.
//

#include "graf.h"

void graf::make_graf(string & s) {
    ifstream infile;
    infile.open(s, ifstream::in);
    boost::dynamic_properties dp;
    dp.property("label", boost::get(&GraphData::label, g));
    dp.property("value", boost::get(&GraphData::value, g));
    dp.property("node_id", boost::get(&GraphData::node_id, g));
    boost::read_graphml(infile, g, dp);
    best = g;
//    cout << "size of 0: " << endl;
//    cout << g.m_vertices[0].m_out_edges.size() << endl;
//    int c = 1;
//    typename graph_traits<Graph>::out_edge_iterator out_i, out_end;
//    for (boost::tie(out_i, out_end) = out_edges(vertex(0, g), g);
//         out_i != out_end; ++out_i)
//    {
//        auto e = *out_i;
//        cout << "this is mSource: " << c << endl;
//        cout << e.m_source << endl;
//        c++;
//    }
}

void graf::print_v() {
    typedef property_map<Graph, vertex_index_t>::type IndexMap;
    IndexMap index = get(vertex_index, g);

    std::cout << "vertices(g) = ";
    std::pair<vertex_iter, vertex_iter> vp;
    for (vp = vertices(g); vp.first != vp.second; ++vp.first)
        std::cout << index[*vp.first] <<  " ";
    std::cout << std::endl;
}

void graf::print_e() {
    typedef property_map<Graph, vertex_index_t>::type IndexMap;
    IndexMap index = get(vertex_index, g);

    std::cout << "edges(g) = ";
    graph_traits<Graph>::edge_iterator ei, ei_end;
    for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
        std::cout << "(" << index[source(*ei, g)]
                  << "," << index[target(*ei, g)] << ") ";
    std::cout << std::endl;
}

void graf::paths() {
    Graph::vertex_iterator v, vend, vv, vvend;
    vector<VertexDesc> path;
    VertexDesc vGoal, source;
    int count;
    for(tie(v, vend) = vertices(g); v != vend; ++v) {
        vGoal = *v;
        std::vector<VertexDesc> pr(num_vertices(g));
        std::vector<int> d(num_vertices(g));
        auto idmap = get(vertex_index, g);

        //run dijkstras with all of them as goals
        //referenceing the stack overflow dijstra
        dijkstra_shortest_paths(g, vGoal, predecessor_map(make_iterator_property_map(pr.begin(), idmap)).distance_map(
                make_iterator_property_map(d.begin(), idmap)).weight_map(make_constant_property<edgeDesc>(1.0)));
        //make all of them the source
        count = pr.size();
        for(tie(vv, vvend) = vertices(g); vv != vvend; ++vv) {
            source = *vv;
            if(pr[source] != source && source != vGoal) {
                while (pr[source] != vGoal) {
                    if (source > pr[source]) {
                        edgeCount[make_pair(pr[source], source)]++;
                    } else {
                        edgeCount[make_pair(source, pr[source])]++;
                    }
                    //path.push_back(source);
                    source = pr[source];
                }
                if(source > vGoal){
                    edgeCount[make_pair(vGoal, source)]++;
                }
                else{
                    edgeCount[make_pair(source, vGoal)]++;
                }
            }
        }
    }
    edgeTot = edgeCount.size();
}

void graf::make_communities() {
    while(1 == 1) {
        edgeCount.clear();
        paths();
        modularity();
        if (edgeTot == 0) {
            return;
        }
        int max = -1;
        auto toGo = edgeCount.begin()->first;
        for (auto a: edgeCount) {
            if (a.second > max) {
                max = a.second;
                toGo = a.first;
            }
        }
        //delete edge from graph
        typedef property_map<Graph, vertex_index_t>::type IndexMap;
        IndexMap index = get(vertex_index, g);
        graph_traits<Graph>::edge_iterator ei, ei_end;
        //find the proper edge
        for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
            if (index[source(*ei, g)] == toGo.first && index[target(*ei, g)] == toGo.second) {
                break;
            }
        }
        edgeTot--;
        g.remove_edge(*ei);
        toCut++;
    }
}

void graf::modularity() {
    vector<int> component(num_vertices(g));
    size_t numComponents = connected_components(g, &component[0]);
    int high = -1;
    double tmpMod = 0;
    if (numComponents > 1){
        for(auto& a : component){
            if (a > high){
                high = a;
            }
        }
        vector<set<int>> pieces(high+1);
        for(int j = 0; j < component.size(); j++){
            pieces[component[j]].emplace(j);
        }
        for(auto& q : pieces){
            double edgeNum;
            double outNum = 0;
            double outTotal = edgeTot * 2;
            for(auto& r : q){
                outNum += g.m_vertices[r].m_out_edges.size();
            }
            edgeNum = outNum / 2;
            tmpMod += (((edgeNum / edgeTot) - (pow((outNum / outTotal), 2))) * toCut/numComponents/edgeNum);
        }
        if(tmpMod > modNum){
            modNum = tmpMod;
            best = g;
            finalCut = toCut;
        }
    }
}

void graf::make_output() {
    ofstream output;
    output.open("graphed.txt");
    write_graphviz(output, best, make_label_writer(get(&GraphData::label, best)));
    vector<int> component(num_vertices(best));
    cout << "there are: " << connected_components(best, &component[0]) << " components when cutting " << finalCut << " edges" << endl;
}
