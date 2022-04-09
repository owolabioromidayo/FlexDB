#include <fstream>
#include <iostream>
#include <cstdio>

#include "../lib/json.hpp"
#include "../src/node.cc"
#include "../src/graph.cc"


using json = nlohmann::json;

int main(){

    Graph test("mean");

    Node n1("sdfsdf", "Person", {{"hello", "there"}});
    Node n2("mello", "Person", {{"may", "me"}});

    test.add_node(n2);
    test.add_node(n1);
    test.add_edge(n1.get_id(), n2.get_id(), "sdvds", {{"strength_rel", "20"}, {"compose", "20"}}); 

    auto people = test.get_node_ids_by_type("Person");
    std::cout << "Nodes with Type Person \n";
    for(auto it = people.begin(); it != people.end(); ++it){
       test.get_node(*it).__repr__();
    }

    auto l = test.get_node_ids_by_name("mello");
    std::cout << "Nodes with Name mello \n";
    for(auto it = l.begin(); it != l.end(); ++it){
       test.get_node(*it).__repr__();
    }

    auto m = test.get_edge_ids_by_label("sdvds");
    std::cout << "Edges with Label sdvds \n";
    for(auto it = m.begin(); it != m.end(); ++it){
       test.get_edge(*it).__repr__();
    }

    auto n = test.get_edge_ids_by_source(n1.get_id());
    std::cout << "Edges with Source n1.id  \n";
    for(auto it = n.begin(); it != n.end(); ++it){
       test.get_edge(*it).__repr__();
    }

    auto o = test.get_edge_ids_by_dest(n2.get_id());
    std::cout << "Edges with Dest n2.id \n";
    for(auto it = o.begin(); it != o.end(); ++it){
       test.get_edge(*it).__repr__();
    }


    auto p = test.get_edge_ids_by_dest(n2.get_id());
    std::cout << "Edges with Dest n2.id \n";
    for(auto it = p.begin(); it != p.end(); ++it){
       test.get_edge(*it).__repr__();
    }

    auto q = test.get_edge_ids_by_dest(n1.get_id());
    std::cout << "Blank Response : Edges with Dest n1.id \n";
    for(auto it = q.begin(); it != q.end(); ++it){
       test.get_edge(*it).__repr__();
    }



    std::cout << "Query Tests Passed! \n"; 

    return 0;


}
