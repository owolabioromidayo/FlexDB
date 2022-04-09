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

    test.__repr__();
    auto people = test.get_nodes_by_type("Person");
    for(auto it = people.begin(); it != people.end(); ++it){
        it->__repr__();
    }


    std::cout << "Query Tests Passed! \n"; 

    return 0;


}
