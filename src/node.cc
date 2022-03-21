#include <set>
#include <map>
#include <string>

#include "types.hpp"
#include "node.hpp"


//constructors
Node::Node(std::string iname){
    name = iname;
};

Node::Node(std::string iname, Table itable){
    name = iname;
    table = itable;
};

Node::Node(std::string iname, Table itable, std::set<std::string> connections){
    name = iname;
    table = itable;
    connections = connections;
};


std::string Node::get_name(){
    return name;
};


//connection operations
std::set<std::string> Node::get_connections(){
    return connections;
}

void Node::add_connection(Node *n){
    connections.insert(n->get_name());
}

void Node::add_connection(std::string n){
    connections.insert(n);
}


// table operations
Table Node::get_table(){
    return table;
}

void Node::insert_row(std::string key, std::string value){
    table[key] = value;
}

void Node::edit_row(std::string key, std::string value){
    table[key] = value;
}

void Node::delete_row(std::string key, std::string value){
    table.erase(key);
}

std::string Node::get_row(std::string key){
    Table::iterator it;
    it = table.find(key);
    if(it == table.end()) 
        return "";
    else
        return it->second;
}

void Node::clear_row(std::string key){
    Table::iterator it;
    it = table.find(key);
    if(it != table.end()) 
        table[key] = "";
}

void Node::clear_table(){
    table.clear();
}



void Node::__repr__(){
    std::cout << "Representation for Node "<< name << std::endl;

    std::cout << "\nConnections" << std::endl;
    for(std::set<std::string>::iterator it = connections.begin(); it != connections.end(); ++it){
        std::cout << *it << std::endl;
    }

    std::cout << "\nTable" << std::endl;
    for( Table::iterator it = table.begin(); it != table.end(); ++it){
        std::cout << it->first << " - " << it->second<< std::endl;
        
    }

    std::cout << "\n"; 


}
