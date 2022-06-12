#include "../src/node.cc"
#include "../src/graph.cc"
#include "iostream"
#include "../src/parser.cc"



int main(){
    Graph test("mean");
    test.deserialize("mean.json"); 

    Parser p(test);
    std::cout << p.is_valid_expr("g().V()") << std::endl;
    std::cout << p.is_valid_expr("g().V().groupCount().limit(5).out()") << std::endl;
    std::cout << p.is_valid_expr("g().E().hasLabel(brain).values().out()") << std::endl;
    std::cout << p.is_valid_expr("g().E().has([brain | stream ]).limit(5).out()") << std::endl;
    p.resolve_query("g().V(Person).count().out()");

    return 0;
}

