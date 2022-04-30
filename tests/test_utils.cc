#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include "../src/utils.cc"
#include "../src/types.hpp"


int main(){
    std::string a = "g().V().return_here(m, b).info().hereme()";

    ExprDetails expr = utils::get_expr_map(a);
    std::vector<std::string> functions = expr.functions;
    std::unordered_map<std::string, std::vector<std::string>> umap = expr.umap;

    for(auto it = umap.begin(); it != umap.end(); ++it){
        std::cout << "Function name: "<< it->first << "\t";
        std::vector<std::string> m =  it->second;

        for (auto it2 = m.begin(); it2 != m.end(); ++it2){
            std::cout << *it2 << "\t";
        }

        std::cout << std::endl;
    }

    for (auto it= functions.begin(); it != functions.end(); ++it){
        std::cout << *it << "\t";
    }

     std::cout << std::endl;
    return 0;
}