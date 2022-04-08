#include <iostream>
#include <string>
#include "../lib/sole/sole.hpp"


int main() {
    sole::uuid u4 = sole::uuid4();
    std::string ustring = u4.str(); //sole::uuid4().str();

    std::cout << "u4 string " << " -> " << ustring << std::endl;
}
