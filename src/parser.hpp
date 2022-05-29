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

        EdgeList curr_edges = {};
        NodeList curr_nodes = {};
        bool isV = true; //does current instruction concern g.E() or g.V() 

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
            {"values", {"string"}},
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
            {"values", {"out", "has", "V", "E"}},
            {"count", {"V", "E", "hasNot", "has" }},
            {"limit", {"V", "E", "has", "hasNot" }}, 
            {"groupCount", {"has", "V", "E"}},
            {"groupCountBy",  {"has", "V", "E"}},
            {"has", {"V", "E"}}, 
            {"hasNot", {"V", "E"}}, 
            {"rankBy", {"has", "hasNot", "V", "E"}},
        };


        NodeList V(); // set currEdges
        EdgeList E(); // set currNodes

        void has(std::string labels[]);
        void hasNot(std::string labels[]);
        void rankBy(std::string property, bool ascending = true);


        //mutation ops
        Node getNode(std::string id);
        Edge getEdge(std::string id);
        void addNode(Node nnode); // copy args of Graph.add
        void addEdge(Edge n_edge);
        void delNode(Node nnode);
        void delEdge(Edge n_edge);

    
        //inference ops
        void groupCount();
        void groupCountBy(std::string label);
        

        //end ops
        void limit(int limit); // limits output of current list based on instruction
        void unfold(); // display all info  
        void fold(); // put it all in a list
        void values(); //extract these values from curr;
        void count(); //return count of curr items : g.V().count()


        //traversal operations
        void out(); // get vertices of outgoing edges with any label
        void out(std::string label); // get vertices of outgoing edges with label
        void path(); // get complete path of objects in relation to last item
        

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