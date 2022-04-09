#ifndef NODE_HPP_INCLUDED_
#define NODE_HPP_INCLUDED_


#include <string>
#include <set>

#include "types.hpp"
#include "../lib/sole/sole.hpp"

class Node{

    public:
        Table table;

        //constructors
        Node(std::string iname, std::string itype);
        Node(std::string iname, std::string itype, std::string id);
        Node(std::string iname, std::string itype, TableData itable);
        Node(std::string iname, std::string itype,  TableData itable, std::set<std::string> connections);

        //connection operations
        std::set<std::string> get_connections();
        void add_connection(Node* n);
        void add_connection(std::string n);

        void __repr__();

        //setters and getters
        std::string get_name();
        std::string get_type();
        std::string get_id();

    private:
        std::string name;
        std::string type;
        std::string id = sole::uuid4().str();
        std::set<std::string> connections {};
       
        
};

struct Edge{
    std::string label;
    std::string source;
    std::string dest;
    std::string id = sole::uuid4().str();
    Table table;

    bool operator<(const Edge& rhs) const{
        if (label < rhs.label){
            return true;
        }
        return false;
    }


    void __repr__() const{

        std::cout << "Representation for Edge "<< id << " with Label "<< label << std::endl;

        std::cout << "\tid " << id<< std::endl;
        std::cout << "\tlabel " << label<< std::endl;
        std::cout << "\tsource " << source<< std::endl;
        std::cout << "\tdest " << dest<< std::endl;

        std::cout << "\tTABLE" << "\n";
        TableData _table = this->table.get_table();
        for(TableData::iterator it = _table.begin(); it != _table.end(); ++it){
            std::cout << "\t\t"<<it->first << " : "<<  it->second << std::endl;
        }
    }

};

#endif //NODE_HPP_INCLUDED_