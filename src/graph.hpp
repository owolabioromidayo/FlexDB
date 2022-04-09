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
        void add_edge(Node* n1, Node* n2, std::string label, TableData table);
        void add_edge(Node* n1, Node* n2, std::string label, TableData table, std::string id);

        void delete_node(std::string id);

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