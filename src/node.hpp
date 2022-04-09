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
    Table table;

    bool operator<(const Edge& rhs) const{
        if (label < rhs.label){
            return true;
        }
        return false;
    }


    //table operations
    Table get_table(){return table; }

    void insert_row(std::string key, std::string value){
        Table::iterator it;
        it = table.find(key);
        if(it == table.end()) 
            table[key] = value;
        else{
            throw "Key already exists in Edge with source " + source + "and dest " + dest + "with Label " + label  + " and  ID" + id + ".";
        }
    }

    void edit_row(std::string key, std::string value){
        table[key] = value;
    }

    void delete_row(std::string key, std::string value){
        table.erase(key);
    }

    std::string get_row(std::string key){
        Table::iterator it;
        it = table.find(key);
        if(it == table.end()) 
            return "";
        else
            return it->second;
    }

    void clear_row(std::string key){
        Table::iterator it;
        it = table.find(key);
        if(it != table.end()) 
            table[key] = "";
    }

    void clear_table(){
        table.clear();
    }

};

#endif //NODE_HPP_INCLUDED_