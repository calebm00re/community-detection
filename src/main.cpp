#include <iostream>
#include <chrono>
#include "graf.h"


//function to

int main(int argc, char** argv) {
    auto start = chrono::high_resolution_clock::now();
    graf g;
    string s = argv[1];
    g.make_graf(s);
    g.make_communities();
    g.make_output();
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
    cout << "The program took " << duration.count() << " seconds" << endl;
    return 0;
}
