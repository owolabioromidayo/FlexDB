#ifndef NODE_HPP_INCLUDED_
#define NODE_HPP_INCLUDED_


#include <string>
#include <set>

#include "types.hpp"
#include "../lib/sole/sole.hpp"

class Node{

    public:
        //constructors
        Node(std::string iname, std::string itype);
        Node(std::string iname, std::string itype, std::string id);
        Node(std::string iname, std::string itype, Table itable);
        Node(std::string iname, std::string itype,  Table itable, std::set<std::string> connections);
        


        //connection operations
        std::set<std::string> get_connections();
        void add_connection(Node* n);
        void add_connection(std::string n);

        // table operations
        Table get_table();
        std::string get_row(std::string key);
        void insert_row(std::string key, std::string value);
        void edit_row(std::string key, std::string value);
        void delete_row(std::string key, std::string value);
        void clear_row(std::string key);
        void clear_table();

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
        Table table;
        
};

struct Edge{
    std::string label;
    std::string source;
    std::string dest;
    std::string id;
    // std::string _id = sole::uuid4().str()
    int weight;

    bool operator<(const Edge& rhs) const{
        if (weight < rhs.weight){
            return true;
        }
        return false;
    }

};

#endif //NODE_HPP_INCLUDED_