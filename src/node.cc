#include <set>
#include <map>
#include <string>

#include "types.hpp"
#include "node.hpp"


//constructors
Node::Node(std::string iname, std::string itype){
    if (iname == ""){
        throw "Field 'Name' of Class Node cannot be empty.";
    }
    if (itype == ""){
        throw "Field 'Type' of Class Node cannot be empty.";
    }
    name = iname;
    type = itype;
};

Node::Node(std::string iname, std::string itype, std::string iid){
    if (iname == ""){
        throw "Field 'Name' of Class Node cannot be empty.";
    }
    if (itype == ""){
        throw "Field 'Type' of Class Node cannot be empty.";
    }
    if (iid == ""){
        throw "Field 'ID' of Class Node cannot be empty.";
    }

    name = iname;
    type = itype;
    id = iid;
};


Node::Node(std::string iname, std::string itype, TableData itable){
    if (iname == ""){
        throw "Field 'Name' of Class Node cannot be empty.";
    }
    if (itype == ""){
        throw "Field 'Type' of Class Node cannot be empty.";
    }

    name = iname;
    table = Table(itable);
    type = itype;
};


Node::Node(std::string iname, std::string itype, TableData itable, std::set<std::string> connections){
    if (iname == ""){
        throw "Field 'Name' of Class Node cannot be empty.";
    }
    if (itype == ""){
        throw "Field 'Type' of Class Node cannot be empty.";
    }

    name = iname;
    table = Table(itable);
    connections = connections;
    type = itype;
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

//setters and getters

void Node::__repr__(){
    std::cout << "Representation for Node "<< name << " of Type "<< type << std::endl;

    std::cout << "\nConnections" << std::endl;
    for(std::set<std::string>::iterator it = connections.begin(); it != connections.end(); ++it){
        std::cout << *it << std::endl;
    }

    std::cout << "\nTable" << std::endl;
    TableData _table = table.get_table();
    for( TableData::iterator it = _table.begin(); it != _table.end(); ++it){
        std::cout << it->first << " - " << it->second<< std::endl;  
    }
    std::cout << "\n"; 


}

std::string Node::get_name(){ return name;}
std::string Node::get_type(){ return type;}
std::string Node::get_id(){ return id;}

