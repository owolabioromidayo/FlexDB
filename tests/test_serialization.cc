#include <fstream>
#include <iostream>
#include <cstdio>

#include "../lib/json.hpp"
#include "../src/node.cc"
#include "../src/graph.cc"


using json = nlohmann::json;

int main(){

    Graph test("mean");

    Node n1("sdfsdf", {{"hello", "there"}});
    Node n2("mello", {{"may", "me"}});

    test.add_node(n2);
    test.add_node(n1);
    test.add_edge(&n1, &n2, "sdvds", 4); 

    test.serialize();

    Graph m("sdf");
    m.deserialize("mean.json"); 
    m.serialize();

    std::cout << "Serialization Tests Passed! \n"; 


    return 0;


}
