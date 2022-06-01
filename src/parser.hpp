#ifndef PARSER_HPP_INCLUDED_
#define PARSER_HPP_INCLUDED_

#include <string>
#include <unordered_map>

#include "graph.hpp"
#include "node.hpp"
#include "types.hpp"


class Parser{
    //based on Gremlin Query Language for Graph Databases

    private:
      
        Graph g {"temp"}; //becasue Graph g("temp") doesnt work , C++ is confused.
        EdgeList g_edges = {};
        NodeList g_nodes = {};


        //for storing internal state
        EdgeList curr_edges = {};
        NodeList curr_nodes = {};
        bool isV = true; //does current instruction concern g.E() or g.V() 
        std::unordered_map<std::string, std::vector<std::string>> valueMap; 
        
        Edge currEdge;
        Node currNode;
    
        std::string out_header_string;
        int _count = 0;
        int _limit = 0;



        std::set<std::string> starting_ops = {"g"};
        std::set<std::string> end_ops = {"out","values","count", "limit" };
        std::set<std::string> collection_ops = {"V","E"};
        std::set<std::string> mutation_ops = {"addNode", "addEdge", "delNode", "delEdge", "getEdge", "getNode"};
        std::set<std::string> inference_ops = {"groupCount","groupCountBy", "has", "hasNot", "rankBy" };
        std::set<std::string> traversal_ops = {"out"};

        std::set<std::string> all_ops;
        void init_all_ops();

        std::unordered_map<std::string, std::vector<std::string>> f_args = {
            {"V", {}},
            {"E", {}},
            {"out", {}}, //print final output
            {"values", {"list<string>"}},
            {"count", {}}, //get count of collection
            {"limit", {"int"}}, // convertible to int
            {"groupCount", {}},
            {"groupCountBy", {"string"}}, // 'arg'
            {"has", {"list<string>"}}, // [a1,a2,a3]
            {"hasNot", {"list<string>"}}, // [a1,a2,a3]
            {"rankBy", {"string"}} //string -> label where value is int/float/string
            //how do we typecheck nodes and edges, havent finished proto yet

        };

        std::unordered_map<std::string, std::vector<std::string>> prev_map = { //mapping of acceptable pre functions
            {"g", {}}, 
            {"V", {"g"}},
            {"E", {"g"}},
            {"out", {"limit", "has", "hasNot", "V", "E", "groupCount", "groupCountBy", "values"}},
            {"values", {"has", "V", "E"}},
            {"count", {"V", "E", "hasNot", "has" }},
            {"limit", {"V", "E", "has", "hasNot" }}, 
            {"groupCount", {"has", "V", "E"}},
            {"groupCountBy",  {"has", "V", "E"}},
            {"has", {"V", "E"}}, 
            {"hasNot", {"V", "E"}}, 
            {"rankBy", {"has", "hasNot", "V", "E"}},
        };


        void V(std::string prev_func); // set currEdges
        void E(std::string prev_func); // set currNodes

        void has(std::string prev_func, std::vector<std::string> labels);
        void hasNot(std::string prev_func, std::vector<std::string> labels);
        void rankBy(std::string prev_func, std::string property, bool ascending = true);


        //mutation ops
        void getNode(std::string prev_func, std::string id);
        void getEdge(std::string prev_func, std::string id);
        void addNode(std::string prev_func, Node nnode); // copy args of Graph.add
        void addEdge(std::string prev_func, Edge n_edge);
        void delNode(std::string prev_func,Node nnode);
        void delEdge(std::string prev_func, Edge n_edge);


        void groupCountBy(std::string prev_func, std::string label);
        void groupCount(std::string prev_func);
        

        //end ops
        void limit(std::string prev_func, int limit); // limits output of current list based on instruction
        void values(std::string prev_func, std::vector<std::string> labels); //extract these values from curr;
        void count(std::string prev_func); //return count of curr items : g.V().count()


        //traversal operations
        void out(std::string prev_func); // print results 

        
        

    public:
        Parser(); // functions should be restricted without a graph ERR no graph, init with empty graph
        Parser(Graph g);


        void repl();
        bool is_valid_expr(std::string expr); // check if query expression is valid
        void resolve_query(std::string query);
        bool readGraph(std::string filename);




    /***
        out * Outgoing adjacent vertices.

        in * Incoming adjacent vertices.

        both * Both incoming and outgoing adjacent vertices.

        outE * Outgoing incident edges.

        inE * Incoming incident edges.

        bothE * Both outgoing and incoming incident edges.

        outV Outgoing vertex.

        inV Incoming vertex.

        otherV The vertex that was not the vertex we came from.
    ***/

        
};


#endif //PARSER_HPP_INCLUDED_