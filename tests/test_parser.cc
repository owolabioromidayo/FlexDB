#include "../src/node.cc"
#include "../src/graph.cc"
#include "../src/parser.cc"



int main(){
    Graph test("mean");
    test.deserialize("mean.json"); 

    Parser p(test);
    std::cout << p.is_valid_expr("g().V()") << std::endl;
    std::cout << p.is_valid_expr("g().V().groupCount().limit(5)") << std::endl;
    std::cout << p.is_valid_expr("g().E().hasLabel(\"brain\").values()") << std::endl;

    return 0;
}

