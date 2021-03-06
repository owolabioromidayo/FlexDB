#include <fstream>
#include <iostream>
#include <map>

#include "../lib/json.hpp"
#include "types.hpp"
#include "node.hpp"
#include "graph.hpp"



using json = nlohmann::json;

Graph::Graph(std::string iname)
{
    this->name = iname;
};

void Graph::set_name(std::string newName)
{
    this->name = newName;
};
        
std::string Graph::get_name()
{
    return name;
};

void Graph::add_node(Node nnode)
{ 
    nodeMap.insert(std::make_pair(nnode.get_id(),  nnode));
};

void Graph::add_edge(std::string id1, std::string id2, std::string label, TableData table)
{
    Edge n;
    n.source = id1;
    n.dest = id2;
    n.label = label;
    n.table = Table(table);
    
    auto it = nodeMap.find(id1);
    if (it == nodeMap.end())
        throw "ID does not exist!";
    
    edgeMap.insert(std::make_pair(n.id, n));
    it->second.add_connection(id2);

}

void Graph::add_edge(std::string id1, std::string id2, std::string label, TableData table, std::string id)
{
    Edge n;
    n.source = id1;
    n.dest = id2;
    n.label = label;
    n.table = Table(table);
    n.id = id;

   
    auto it = this->nodeMap.find(id1);
    if (it == this->nodeMap.end())
        throw "ID does not exist!";
    
    edgeMap.insert(std::make_pair(n.id, n));
    it->second.add_connection(id2);
}

void Graph::delete_node(std::string id)
{
    this->nodeMap.erase(id);
    auto it =  edgeMap.begin();
    while(it != edgeMap.end())
    {
        Edge currEdge = it->second;
        if (currEdge.source == id || currEdge.dest == id)
            it = edgeMap.erase(it);
        ++it;
    }
}

void Graph::delete_connection(std::string id1, std::string id2)
{
    auto it =  edgeMap.begin();
    while(it != edgeMap.end())
    {
        Edge curr = it->second;
        if ((curr.source == id1 && curr.dest == id2 ) || (curr.dest == id1 && curr.source == id2))
            it = edgeMap.erase(it);
        ++it;
    }
    Node n1 = this->nodeMap[id1];
    n1.delete_connection(id2);

    Node n2 = this->nodeMap[id2];
    n2.delete_connection(id1);
}

void Graph::delete_edge(std::string id)
{
    this->edgeMap.erase(id);
    auto it =  nodeMap.begin();
    while(it != nodeMap.end())
    {
        Node currNode = it->second;
        currNode.delete_connection(id);
        ++it;
    }
}



void Graph::serialize(){
    json j;
    std::unordered_map<std::string, Node>::iterator iter; 

    for(iter = nodeMap.begin(); iter != nodeMap.end(); iter++)
    {
        Node currNode = iter->second;
        std::string nodeId = currNode.get_id();


        j["nodes"][nodeId] = {}; //save each node
        j["nodes"][nodeId]["name"] = currNode.get_name();
        j["nodes"][nodeId]["type"] = currNode.get_type();
        j["nodes"][nodeId]["table"] = {};  //store table 

        std::map<std::string, std::string>::iterator tableIter;
        TableData table = currNode.table.get_table();
        for(tableIter = table.begin(); tableIter != table.end(); tableIter++)
        {
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
    for(auto it = edgeMap.begin(); it != edgeMap.end(); ++it)
    {
        Edge currEdge = it->second;
        std::string id = currEdge.id;

        j["edges"][id]["label"] = currEdge.label;
        j["edges"][id]["source"] = currEdge.source;
        j["edges"][id]["dest"] = currEdge.dest;
        j["edges"][id]["table"] = {};  

        std::map<std::string, std::string>::iterator tableIter;
        TableData table = currEdge.table.get_table();
        for(tableIter = table.begin(); tableIter != table.end(); tableIter++)
        {
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
    

void Graph::deserialize(std::string filename)
{
    try{
        std::ifstream i(filename);
        json j;
        i >> j;

        json nodes = j["nodes"];
        //create new nodes
        for (json::iterator it = nodes.begin(); it != nodes.end(); ++it)
        {
            std::string nodeId= it.key();
            json nodeData = it.value();

            Node n(nodeData["name"] , nodeData["type"], nodeId);
            //insert table data back into node
            json table = nodeData["table"];
            for (json::iterator it2 = table.begin(); it2 != table.end(); ++it2)
                n.table.insert_row(it2.key(), it2.value());

            //insert connections back
            json connections = nodeData["connections"];
            for (json::iterator it2 = connections.begin(); it2 != connections.end(); ++it2)
                n.add_connection(*it2);

            this->add_node(n);
        }

        //rebuild edges
        json edges = j["edges"];
        for (json::iterator it = edges.begin(); it != edges.end(); ++it)
        {
            json edgeData = it.value();
            TableData table = (TableData) edgeData["table"];
            this->add_edge(edgeData["source"], edgeData["source"], edgeData["label"], table, it.key());
        }
    }catch (int e){
        std::cout << "invalid file \n";
    }
    
}


void Graph::__repr__()
{
    std::cout << "Representation for graph "<< name << "." << std::endl;
    std::cout << "\nNodes" << std::endl;
    for(auto it = nodeMap.begin(); it != nodeMap.end(); ++it)
    {
        it->second.__repr__();
        std::cout<< "\n";
    }
    std::cout << "Edges" << std::endl;
    for( auto it = edgeMap.begin(); it != edgeMap.end(); ++it)
        it->second.__repr__();
}

//GETTERS
Node& Graph::get_node(std::string id)
{
    auto it = nodeMap.find(id);
    if (it == nodeMap.end())
        throw "Node of id " + id + " does not exist."; 
    else
        return  it->second;
}

std::vector<std::string> Graph::get_node_ids_by_type (std::string type)
{
    std::vector<std::string> matchingNodes;
    for(auto it= this->nodeMap.begin(); it != this->nodeMap.end(); ++it)
    {
       Node currNode = it->second;
       if (currNode.get_type() == type)
           matchingNodes.push_back(currNode.get_id());
    }
    return matchingNodes;
} 


std::vector<std::string> Graph::get_node_ids_by_name(std::string name)
{
    std::vector<std::string> matchingNodes;
    for(auto it= this->nodeMap.begin(); it != this->nodeMap.end(); ++it)
    {
       Node currNode = it->second;
       if (currNode.get_name() == name)
           matchingNodes.push_back(currNode.get_id());
    }
    return matchingNodes;
} 

Edge& Graph::get_edge(std::string id)
{
    auto it = edgeMap.find(id);
    if (it == edgeMap.end())
        throw "Edge of id " + id + " does not exist."; 
    else
        return  it->second;
}

 std::string Graph::get_edge_id(std::string source, std::string dest, std::string label)
 {
     for(auto it = edgeMap.begin(); it != edgeMap.end(); ++it)
     {
         Edge currEdge = it->second;
         if(currEdge.source == source  && currEdge.dest == dest && currEdge.label == label)
             return currEdge.id;
     }
     throw "Edge of Source: " + source + ", Dest: " + dest + " and Label: " + label + " does not exist.";
 }


std::vector<std::string> Graph::get_edge_ids_by_label(std::string label)
{
    std::vector<std::string> matchingEdges;
    for(auto it = edgeMap.begin(); it != edgeMap.end(); ++it)
    {  
        Edge currEdge = it->second;
        if (currEdge.label == label)
            matchingEdges.push_back(currEdge.id);
    }
    return matchingEdges;
}

EdgeList Graph::get_edges_by_label(std::string label)
{
    std::vector<std::string> matchingEdges;
    EdgeList E;

    for(auto it = edgeMap.begin(); it != edgeMap.end(); ++it)
    {  
        Edge currEdge = it->second;
        if (currEdge.label == label)
            E.push_back(currEdge);
    }
    return E;
}

std::vector<std::string> Graph::get_edge_ids_by_source(std::string source)
{
    std::vector<std::string> matchingEdges;
    for(auto it = edgeMap.begin(); it != edgeMap.end(); ++it)
    {  
        Edge currEdge = it->second;
        if (currEdge.source == source)
            matchingEdges.push_back(currEdge.id);
    }
    return matchingEdges;
}

std::vector<std::string> Graph::get_edge_ids_by_dest(std::string dest)
{
    std::vector<std::string> matchingEdges;
    for(auto it = edgeMap.begin(); it != edgeMap.end(); ++it)
    {  
        Edge currEdge = it->second;
        if (currEdge.dest == dest)
            matchingEdges.push_back(currEdge.id);
    }
    return matchingEdges;
}

NodeList Graph::get_nodes(std::string type = "")
{
    NodeList V;
    if(type == "")
    {
        for(auto it= this->nodeMap.begin(); it != this->nodeMap.end(); ++it)
            V.push_back(it->second);
    }
    else
    {
        for(auto it= this->nodeMap.begin(); it != this->nodeMap.end(); ++it)
        {
            if(it->second.get_type() == type)
                V.push_back(it->second);
        }
    }
    return V;
}


EdgeList Graph::get_edges()
{
    EdgeList E;
    for(auto it= this->edgeMap.begin(); it != this->edgeMap.end(); ++it)
        E.push_back(it->second);
    return E;
}

NodeList Graph::get_nodes_by_labels(std::vector<std::string> labels)
{
    NodeList V;
    bool hasLabels = false;
    for(auto it= this->nodeMap.begin(); it != this->nodeMap.end(); ++it)
    {
        for(auto it2 = labels.begin(); it2 != labels.end(); ++it2)
        {
            if (it->second.table.get_row(*it2) == "")
            {
                hasLabels = false;
                break;
            }
        }
        if (hasLabels)
        {
            V.push_back(it->second);
            hasLabels = false;
        }
    }
    return V;
}

NodeList Graph::get_out_nodes(Node node)
{
    NodeList retList = {};
    std::set<std::string> conns = node.get_connections();
    for( auto it = conns.begin(); it != conns.end(); ++it)
            retList.push_back(this->get_node(*it));

    return retList; 
}