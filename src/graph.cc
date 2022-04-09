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

void Graph::add_edge(std::string id1, std::string id2, std::string label, TableData table){
    Edge n;
    n.source = id1;
    n.dest = id2;
    n.label = label;
    n.table = Table(table);
    
    edges.insert(n);
    auto it = this->nodeMap.find(id1);
    if (it == this->nodeMap.end())
        throw "ID does not exist!";
    
    it->second.add_connection(id2);

}

void Graph::add_edge(std::string id1, std::string id2, std::string label, TableData table, std::string id){
    Edge n;
    n.source = id1;
    n.dest = id2;
    n.label = label;
    n.table = Table(table);
    n.id = id;

    edges.insert(n);
    auto it = this->nodeMap.find(id1);
    if (it == this->nodeMap.end())
        throw "ID does not exist!";
    
    it->second.add_connection(id2);}

void Graph::delete_node(std::string id){

    this->nodeMap.erase(id);
    std::set<Edge>::iterator edgesIter =  this->edges.begin();
    while(edgesIter != edges.end()){
        if (edgesIter->source == id || edgesIter->dest == id){
            edgesIter = edges.erase(edgesIter);
        }else{
            ++edgesIter;
        }
    }
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



    //rebuild edges
     json edges = j["edges"];

     for (json::iterator it = edges.begin(); it != edges.end(); ++it){
        json edge_data = it.value();
        TableData table = (TableData) edge_data["table"];
        this->add_edge(edge_data["source"], edge_data["source"], edge_data["label"], table, it.key());
    }
    
}


void Graph::__repr__(){
    std::cout << "Representation for graph "<< name << "." << std::endl;

    std::cout << "\nNodes" << std::endl;
    for(std::unordered_map<std::string, Node>::iterator it = nodeMap.begin(); it != nodeMap.end(); ++it){
        it->second.__repr__();
        std::cout<< "\n";
    }

    std::cout << "Edges" << std::endl;
    for( std::set<Edge>::iterator it = edges.begin(); it != edges.end(); ++it){
        it->__repr__();
    }
}



//GETTERS
Node& Graph::get_node(std::string id){
    auto it = nodeMap.find(id);
    if (it == nodeMap.end()){
        throw "Node of id " + id + " does not exist."; 
    }
    else{
        return  it->second;
    }
}

std::vector<Node> Graph::get_nodes_by_type(std::string type){
    std::vector<Node> matchingNodes;
    for(auto it= this->nodeMap.begin(); it != this->nodeMap.end(); ++it){
       Node currNode = it->second;
       if (currNode.get_type() == type){
           matchingNodes.push_back(currNode);
       }
    }
    return matchingNodes;
} 
