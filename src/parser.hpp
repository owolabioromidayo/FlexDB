#ifndef PARSER_HPP_INCLUDED_
#define PARSER_HPP_INCLUDED_

#include <string>
#include <unordered_map>

#include "graph.cc"
#include "node.cc"


typedef std::vector<Node> NodeList;
typedef std::vector<Edge> EdgeList;


class Parser{
    //based on Gremlin Query Language for Graph Databases

    
    /** 
        example queries
        g.V().has("name","Jane Doe").out("knows").values("name")

        g.V().match(as("song").out("sungBy").as("artist"),
        as("song").out("writtenBy").as("writer"),as("artist").
        has("name","Garcia"),where(as("song").values("performances").
        is(gt(300)))).select("song","writer").by("name")

        Graph graph = GraphFactory.open(...);
        GraphTraversalSource g = traversal().withEmbedded(graph);

         double avg = g.V().has("name",name).
         out("knows").out("created").
         values(property).mean().next();

        System.out.printIn("Average rating :" +avg);


        gremlin> g.V(marko).out('knows').values('name').path()
        ==>[v[1],v[2],vadas]
        ==>[v[1],v[4],josh]


        g.V().hasLabel('airport').values('code')

        g.V().hasLabel('airport').groupCount().by('country')    // groupCountdby(label)


        g.V().hasLabel('airport').has('code','DFW')

        g.V().has('airport','code','DFW').values()
        g.V().has('airport','code','DFW').values('city')


        g.E().has('dist')
        g.V().hasNot('region')


        FOLLOWING 2 ARE THE SAME
        g.E().hasLabel('route').count()
        g.V().outE('route').count()


        im not including the complex functions. maybe later


        g.V().has('airport','code','AUS').out().values('code').fold()

    **/

    private:

        Graph g;
        EdgeList g_edges = {};
        NodeList g_nodes = {};

        EdgeList curr_edges = {};
        NodeList curr_nodes = {};
        bool isV = true; //does current instruction concern g.E() or g.V() 

        NodeList V(); // set currEdges
        EdgeList E(); // set currNodes

        void has(std::string[] labels);
        void has(std::string property, std::string value); 
        void hasLabel(std::string label);
        void rankBy(std::string property, bool ascending = true);


        //mutation ops
        Node getNode(std::string id);
        Edge getEdge(std::string id);
        void addNode(Node nnode); // copy args of Graph.add
        void addEdge(Edge n_edge);
        void delNode(Node nnode);
        void delEdge(Edge n_edge);

    
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
        bool is_valid_expr(std::string); // check if query expression is valid
        void resolve_query(std::string query);
        void readGraph(std::string filename);

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

        
}


#endif //PARSER_HPP_INCLUDED_