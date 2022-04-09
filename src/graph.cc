#include <fstream>
#include <iostream>
#include <map>

#include "../lib/json.hpp"
#include "types.hpp"
#include "node.hpp"
#include "graph.hpp"


using json = nlohmann::json;

Graph::Graph(std::string iname){
    name = iname;
};

void Graph::set_name(std::string newName){
    name = newName;
};
        
std::string Graph::get_name(){
    return name;
};

void Graph::add_node(Node nnode){ 
    std::string nodeId = nnode.get_id();
    nodeMap.insert(std::make_pair(nodeId,  nnode));
};

void Graph::add_edge(Node* n1, Node* n2, std::string label, TableData table){
    Edge n;
    n.source = n1->get_name();
    n.dest = n2->get_name();
    n.label = label;
    n.table = Table(table);
    n.id = n1->get_id() + "->" + n2->get_id();

    edges.insert(n);
    n1->add_connection(n2);

}

void Graph::serialize(){
    json j;
    std::unordered_map<std::string, Node>::iterator iter; 

    for(iter = nodeMap.begin(); iter != nodeMap.end(); iter++){
        Node currNode = iter->second;
        std::string nodeId = currNode.get_id();


        j["nodes"][nodeId] = {}; //save each node
        j["nodes"][nodeId]["name"] = currNode.get_name();
        j["nodes"][nodeId]["type"] = currNode.get_type();
        j["nodes"][nodeId]["table"] = {};  //store table 

        std::map<std::string, std::string>::iterator tableIter;
        TableData table = currNode.table.get_table();
        for(tableIter = table.begin(); tableIter != table.end(); tableIter++){
            std::string key = tableIter->first;
            std::string value = tableIter->second;
            j["nodes"][nodeId]["table"][key] = value;
        }

        // store connections - list has to be of form std::vector for json serialization
        std::set<std::string> connections = currNode.get_connections();
        std::vector<std::string> vectorisedConnections(connections.size());
        std::copy(connections.begin(), connections.end(), vectorisedConnections.begin());
        j["nodes"][nodeId]["connections"] = vectorisedConnections;


    }

    //store connections directly from edges set in graph
    std::set<Edge>::iterator edgesIter;
    for(edgesIter = edges.begin(); edgesIter != edges.end(); edgesIter++){
        std::string id = edgesIter->id;

        j["edges"][id]["label"] = edgesIter->label;
        j["edges"][id]["source"] = edgesIter->source;
        j["edges"][id]["dest"] = edgesIter->dest;
        j["edges"][id]["table"] = {};  

        std::map<std::string, std::string>::iterator tableIter;
        TableData table = edgesIter->table.get_table();
        for(tableIter = table.begin(); tableIter != table.end(); tableIter++){
            std::string key = tableIter->first;
            std::string value = tableIter->second;
            j["edges"][id]["table"][key] = value;
        }

    }

    //save as graph name
    std::ofstream out;
    out.open(name + ".json");
    out << j.dump(4) << std::endl;
    out.close();

}
    

void Graph::deserialize(std::string filename){
   
    std::ifstream i(filename);
    json j;
    i >> j;

    json nodes = j["nodes"];
    //create new nodes
    for (json::iterator it = nodes.begin(); it != nodes.end(); ++it){
    
        std::string nodeId= it.key();
        json nodeData = it.value();

        Node n(nodeData["name"] , nodeData["type"], nodeId);

        //insert table data back into node
        json table = nodeData["table"];
        for (json::iterator it2 = table.begin(); it2 != table.end(); ++it2){
            n.table.insert_row(it2.key(), it2.value());
        }

        //insert connections back
        json connections = nodeData["connections"];
        for (json::iterator it2 = connections.begin(); it2 != connections.end(); ++it2){
            n.add_connection(*it2);
        }

        this->add_node(n);
    }

    //const auto iterators are bad for maps apparently
    // for(std::map<std::string, Node*>::iterator it= nodeMap.begin(); it != nodeMap.end(); ++it){
    //     std::cout << it->first << std::endl;
    // }

    //rebuild edges
     json edges = j["edges"];

     for (json::iterator it = edges.begin(); it != edges.end(); ++it){
         
        json edge_data = it.value();
     
        std::string _n1 = it.key().substr(0, it.key().find("->"));
        std::string _n2 = it.key().substr(it.key().find("->")+2, -1);

        Node n1 = nodeMap.at(_n1);
        Node n2 = nodeMap.at(_n2);

        json table = edge_data["table"];
        // for (json::iterator it2 = table.begin(); it2 != table.end(); ++it2){
        //     table[it2.key()] = it2.value();
        // }

        this->add_edge(&n1, &n2, edge_data["label"], table);

    }
    
}


void Graph::__repr__(){
    std::cout << name << std::endl;

    std::cout << "\nNodeMap" << std::endl;
    for(std::unordered_map<std::string, Node>::iterator it = nodeMap.begin(); it != nodeMap.end(); ++it){
        std::cout << it->first << " \n ";
        it->second.__repr__();
        std::cout<< "\n\n";
    }

    std::cout << "\nEdges" << std::endl;
    for( std::set<Edge>::iterator it = edges.begin(); it != edges.end(); ++it){
        it->__repr__();
    }
}
