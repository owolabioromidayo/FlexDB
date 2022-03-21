#ifndef NODE_HPP_INCLUDED_
#define NODE_HPP_INCLUDED_


#include <string>
#include <set>

#include "types.hpp"

class Node{

    public:
        Node(std::string iname);
        Node(std::string iname, Table itable);
        Node(std::string iname, Table itable, std::set<std::string> connections);

        std::string get_name();

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

    private:
        std::string name;
        std::set<std::string> connections {};
        Table table;
        
};

struct Edge{
    std::string label;
    std::string  source;
    std::string  dest;
    std::string id;
    int weight;

    bool operator<(const Edge& rhs) const{
        if (weight < rhs.weight){
            return true;
        }
        return false;
    }

} ;


#endif //NODE_HPP_INCLUDED_