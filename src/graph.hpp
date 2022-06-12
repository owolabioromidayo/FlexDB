#ifndef GRAPH_HPP_INCLUDED_
#define GRAPH_HPP_INCLUDED_

#include <string>
#include <set>
#include <unordered_map>

#include "types.hpp"
#include "node.hpp"

typedef std::vector<Node> NodeList;
typedef std::vector<Edge> EdgeList;

class Graph
{
    public:
        Graph(std::string iname);

        void add_node(Node nnode);
        void add_edge(std::string id1, std::string id2, std::string label, TableData table);
        void add_edge(std::string id1,std::string id2, std::string label, TableData table, std::string id);

        void delete_node(std::string id);

        Node& get_node(std::string id);
        NodeList get_nodes(std::string type);
        std::vector<std::string> get_node_ids_by_type(std::string type);
        std::vector<std::string> get_node_ids_by_name(std::string name);

        Edge& get_edge(std::string id);
        EdgeList get_edges();
        std::string get_edge_id(std::string source, std::string dest, std::string label);
        std::vector<std::string> get_edge_ids_by_label(std::string label);
        std::vector<std::string> get_edge_ids_by_source(std::string source);
        std::vector<std::string> get_edge_ids_by_dest(std::string dest);

        NodeList get_nodes_by_labels(std::vector<std::string> labels);
        EdgeList get_edges_by_label(std::string label);

        void serialize();
        void deserialize(std::string filename);

        void set_name(std::string newName);
        std::string get_name();

        void __repr__();

    private:
        std::string name;
        std::unordered_map<std::string, Node> nodeMap{};
        std::unordered_map<std::string, Edge> edgeMap{};
};
#endif // GRAPH_HPP_INCLUDED_