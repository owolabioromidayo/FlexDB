#ifndef GRAPH_HPP_INCLUDED_
#define GRAPH_HPP_INCLUDED_

#include <string>
#include <set>
#include <unordered_map>

#include "types.hpp"
#include "node.hpp"

class Graph{
    public:
        Graph(std::string iname);

        void add_node(Node nnode);

        void add_edge(std::string id1, std::string id2, std::string label, TableData table);
        void add_edge(std::string id1,std::string id2, std::string label, TableData table, std::string id);

        void delete_node(std::string id);

        Node& get_node(std::string id);
        std::vector<Node>  get_nodes_by_type(std::string type);
        // void get_nodes_by_name(std::string name);

        // void get_edge(std::string id);
        // void get_edge(std::string source, std::string dest, std::string label);
        // void get_edges_by_label(std::string label);
        // void get_edges_by_source(std::string source);
        // void get_edges_by_dest(std::string dest);



        void serialize();
        void deserialize(std::string filename);

        void set_name(std::string newName);
        std::string get_name();

        void __repr__();

    private:
        std::string name;
        std::unordered_map<std::string, Node> nodeMap{};
        std::set<Edge> edges;

};
#endif // GRAPH_HPP_INCLUDED_