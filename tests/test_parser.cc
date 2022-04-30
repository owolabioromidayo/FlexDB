#include "../src/parser.cc"

int main(){
    Parser p;
    std::cout << p.is_valid_expr("g.V()") << std::endl;
    std::cout << p.is_valid_expr("g.V().groupCount().limit(5)") << std::endl;
    std::cout << p.is_valid_expr("g.E().hasLabel(\"brain\").values()") << std::endl;

    return 0;
}

