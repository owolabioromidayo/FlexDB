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
    std::string name = nnode.get_name();
    nodeMap.insert(std::make_pair(name,  nnode));
};

void Graph::add_edge(Node* n1, Node* n2, std::string label, int weight){
    Edge n;
    n.source = n1->get_name();
    n.dest = n2->get_name();
    n.label = label;
    n.weight = weight;
    n.id = n.source + "-" + n.dest;

    edges.insert(n);
    n1->add_connection(n2);

}

void Graph::serialize(){
    json j;
    std::unordered_map<std::string, Node>::iterator iter; 

    for(iter = nodeMap.begin(); iter != nodeMap.end(); iter++){
        std::string nodeName = iter->first;
        Node curr_node = iter->second;

        j["nodes"][nodeName] = {}; //save each node
        j["nodes"][nodeName]["table"] = {};  //store table 

        std::map<std::string, std::string>::iterator tableIter;
        Table table = curr_node.get_table();
        for(tableIter = table.begin(); tableIter != table.end(); tableIter++){

            std::string key = tableIter->first;
            std::string value = tableIter->second;

            j["nodes"][nodeName]["table"][key] = value;
        }

        // store connections - list has to be of form std::vector for json serialization
        std::set<std::string> connections = curr_node.get_connections();
        std::vector<std::string> vectorisedConnections(connections.size());
        std::copy(connections.begin(), connections.end(), vectorisedConnections.begin());
        j["nodes"][nodeName]["connections"] = vectorisedConnections;


    }

    //store connections directly from edges set in graph
    std::set<Edge>::iterator edgesIter;
    for(edgesIter = edges.begin(); edgesIter != edges.end(); edgesIter++){
        std::string id = edgesIter->id;

        j["edges"][id]["label"] = edgesIter->label;
        j["edges"][id]["weight"] = edgesIter->weight;
        j["edges"][id]["source"] = edgesIter->source;
        j["edges"][id]["dest"] = edgesIter->dest;
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
    
        Node n(it.key());
        json node_data = it.value();

        //insert table data back into node
        json table = node_data["table"];
        for (json::iterator it2 = table.begin(); it2 != table.end(); ++it2){
            n.insert_row(it2.key(), it2.value());
        }

        //insert connections back
        json connections = node_data["connections"];
        for (json::iterator it2 = connections.begin(); it2 != connections.end(); ++it2){
            n.add_connection(*it2);
        }

        add_node(n);
    }

    //const auto iterators are bad for maps apparently
    // for(std::map<std::string, Node*>::iterator it= nodeMap.begin(); it != nodeMap.end(); ++it){
    //     std::cout << it->first << std::endl;
    // }

    //rebuild edges
     json edges = j["edges"];

     for (json::iterator it = edges.begin(); it != edges.end(); ++it){
         
        json edge_data = it.value();
     
        std::string _n1 = it.key().substr(0, it.key().find("-"));
        std::string _n2 = it.key().substr(it.key().find("-")+1, -1);

        Node n1 = nodeMap.at(_n1);
        Node n2 = nodeMap.at(_n2);

        this->add_edge(&n1, &n2, edge_data["label"], edge_data["weight"]);

    }
    
}


void Graph::__repr__(){
    std::cout << name << std::endl;

    std::cout << "\nnodemap" << std::endl;
    for(std::unordered_map<std::string, Node>::iterator it = nodeMap.begin(); it != nodeMap.end(); ++it){
        std::cout << it->first << " - " << (it->second).get_name()  << std::endl;
    }

    std::cout << "\nedges" << std::endl;
    for( std::set<Edge>::iterator it = edges.begin(); it != edges.end(); ++it){
  
        std::cout << "id " << it->id<< std::endl;
        std::cout << "label " << it->label<< std::endl;
        std::cout << "weight " << it->weight<< std::endl;
        std::cout << "source " << it->source<< std::endl;
        std::cout << "dest " << it->dest<< std::endl;

        
    }


}


// if the error doesnt occur for pure objects then the error rmust stem from my deserialization