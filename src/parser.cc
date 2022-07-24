#include <string>
#include <unordered_map>
#include <iostream>
#include <stdexcept>
#include <algorithm>

#include "parser.hpp"
#include "utils.cc"


Parser::Parser(Graph g)
{
    this->g = g;
    this->init_all_ops();
}

Parser::Parser()
{
    bool done = false;
    while(!done){
        std::string filename = "";
        std::cout << "Enter path name to graph file: ";
        std::cin >> filename;

        try{
            done = this->readGraph(filename);
        }
        catch(std::invalid_argument& e){
            std::cout << "File could not be loaded.";
        }
    }

    this->init_all_ops();
}

void Parser::init_all_ops()
{
    all_ops.insert(starting_ops.begin(), starting_ops.end()); 
    all_ops.insert(end_ops.begin(), end_ops.end()); 
    all_ops.insert(collection_ops.begin(), collection_ops.end()); 
    all_ops.insert(inference_ops.begin(), inference_ops.end()); 
    all_ops.insert(traversal_ops.begin(), traversal_ops.end()); 
}

bool Parser::readGraph(std::string filename)
{
       try
       {
            this->g.deserialize(filename);
            return true;
        } 
        catch(int e)
        {
            throw std::invalid_argument("File does not exist.\n");
            return false;
        }
}

bool Parser::is_valid_expr(std::string expr)
{ 
    //decompose expr
    ExprDetails n_expr ;
    std::vector<std::string> functions ;
    std::unordered_map<std::string, std::vector<std::string>> umap ;
    try{

        n_expr = utils::get_expr_map(expr);
        functions = n_expr.functions;
        umap = n_expr.umap;

        std::cout << "Validating expr " << expr << std::endl;
        //return empty expr
        if(functions.size() < 2)
        {
            std::cout << "Query is not long enough" << std::endl;
            return false;
        }

    }catch (int e){
        std::cout << "Expr does not make proper use of braces\n";
        return false;
    }
    //do a simple pass to check if all ops are there before digging further 
    for(int i=1; i < functions.size(); ++i)
    {
        std::string curr = functions.at(i);
    
        if(*std::find(all_ops.begin(), all_ops.end(), curr) != curr)
        {
            std::cout << "Function " << curr << " is not in argument list" << std::endl;
            return false;
        }
    }

    //check starting ops
    if(*std::find(starting_ops.begin(), starting_ops.end(), functions.at(0)) != functions.at(0))
    {
        std::cout << "Cannot start query with function " << functions.at(0) << std::endl ;
        return false;
    }

    //check middle ops not starting or ending
    //we really need to think about the networking of these functions

    //check ending ops
    if(*std::find(end_ops.begin(), end_ops.end(), functions.back()) != functions.back())
    {
        std::cout << "Cannot end query with function " << functions.back() << std::endl ;
        return false;
    }

    for(int i=1; i < functions.size() - 1; ++i) //these are the middle ops
    {
        std::string curr_func = functions.at(i);
        std::string prev_func = functions.at(i-1);

        if( *std::find(this->prev_map[curr_func].begin(), this->prev_map[curr_func].end(), prev_func) != prev_func) //this means chain is invalid
        {
            std::cout << "Function " << prev_func << " cannot precede " << curr_func << std::endl;
            return false;
        }
    }

    //typecheck args using f_args
    for(int i=0; i< functions.size(); i++)
    {
        std::string curr = functions.at(i);

        std::vector<std::string> args = umap[curr];
        std::vector<std::string> correct = this->f_args[curr];

        //return if arg lengths dont match  
        if(correct.size() == 0 )
        {
            std::cout << "Neglecting args for function " << curr << std::endl;
            continue;
        }else if(args.size() != correct.size())
            std::cout << "Argument lists at function " << curr << " dont match you:func " << args.size() <<" : " << correct.size() << std::endl;
            // return false;

        //arg comparison
        for(int i=0; i< args.size(); ++i )
        { 
            //here we try to see if we can convert the types going down from least likely
            std::string curr_arg = args[i];
            std::string curr_type = correct[i];

            if (curr_type.compare("int") == 0)
            {
                    //try convert string -> int
                    try
                    {
                        int x = std::stoi(curr_arg);
                    }
                    catch(int e)
                    { 
                        std::cout << "Cannot convert " << curr_arg << " to type int" << std::endl;
                        return false;
                    }
                    
            } 
            else if (curr_type.compare("float") == 0)
            {
                    //try convert string -> int
                    try
                    {
                        float x = std::stof(curr_arg);
                    }
                    catch(int e)
                    { 
                        std::cout << "Cannot convert " << curr_arg << " to type float" << std::endl;
                        return false;
                    }
                    
            } 
            else if (curr_type.compare("list<int>") == 0)
            {
                //try parse [1,2,34] exp -> member type must also be specified
                int l = curr_arg.length();
                if(!(curr_arg[0] == '[' && curr_arg[l-1] == ']')){
                    std::cout << curr_arg << " is not of type list<int>" << std::endl;
                    return false;
                }
                try{
                    std::string without_brackets = "";
                    std::vector<std::string> args = {};
                    for (int i = 1; i < curr_arg.length() -1 ; i++)
                        without_brackets += curr_arg[i];

                    std::vector<std::string> ls = utils::split(without_brackets, "|");
                    for (int i = 0; i < ls.size(); i++)
                    {
                        ls[i] = utils::trim_spaces(ls[i]);
                        //check if all are of type int
                        try{
                            int x = std::stoi(ls[i]);
                        }catch(int e){
                            std::cout << "Cannot convert list items of " << curr_arg << " to type int \n";
                            return false;
                        }
                    } 
                } 
                catch(int e) 
                { 
                    return false;
                }

            } 
            else if (curr_type.compare("list<float>") == 0)
            {
                //try parse [1,2,34] exp -> member type must also be specified
                int l = curr_arg.length();
                if(!(curr_arg[0] == '[' && curr_arg[l-1] == ']'))
                {
                    std::cout << curr_arg << " is not of type list<float>" << std::endl;
                    return false;
                }
                try{
                    std::string without_brackets = "";
                    std::vector<std::string> args = {};
                    for (int i = 1; i < curr_arg.length() -1 ; i++)
                        without_brackets += curr_arg[i];

                    std::vector<std::string> ls = utils::split(without_brackets, "|");
                    for (int i = 0; i < ls.size(); i++)
                    { 
                        ls[i] = utils::trim_spaces(ls[i]);
                        //check if all are of type int
                        try{
                            int x = std::stof(ls[i]);
                        }catch(int e){
                            std::cout << "Cannot convert list items of " << curr_arg << " to type float\n";
                            return false;
                        }
                    } 
                } 
                catch(int e) 
                { 
                    return false;
                }

            } 
            else if (curr_type.compare("list<string>") == 0)
            {
                //try parse [1,2,34] exp -> member type must also be specified
                int l = curr_arg.length();
                if(!(curr_arg[0] == '[' && curr_arg[l-1] == ']')){
                    std::cout << curr_arg << " is not of type list<string>" << std::endl;
                    return false;
                }

                try{
                    std::string without_brackets = "";
                    std::vector<std::string> args = {};
                    for (int i = 1; i < curr_arg.length() -1 ; i++)
                        without_brackets += curr_arg[i];
                    std::cout << "Without brackets" <<  without_brackets << std::endl;

                    std::vector<std::string> ls = utils::split(without_brackets, "|");
                    for (int i = 0; i < ls.size(); i++)
                    { 
                        ls[i] = utils::trim_spaces(ls[i]);
                        std::cout << ls[i] << "|" << std::endl;
                    } 
                } 
                catch(int e) 
                { 
                    std::cout << curr_arg << " is not in list form \n";
                    return false;
                }
            } 
            
            else if(curr_type.compare("string") == 0)
                continue;
            else
            {
                //unaccepted type
                std::cout << "Unaccepted type " << curr_type << std::endl; 
                return false;
            }
        }
    }

    return true;

}

void Parser::resolve_query(std::string query)
{
    if (this->is_valid_expr(query))
    {
        //get all functions and values again
        std::cout << "Extracting query details \n";
        ExprDetails n_expr = utils::get_expr_map(query);
        std::vector<std::string> functions = n_expr.functions;
        std::unordered_map<std::string, std::vector<std::string>> umap = n_expr.umap;

        std::cout << "Resetting internal state \n";
        //reset internal state
        this->_count = 0;
        this->_limit = 0;
        this->curr_edges = {};
        this->curr_nodes = {};
        this->isV = true;
        
        std::string s_arg = "";
        int int_arg = 0;
        float float_arg = 0.0;
        std::vector<std::string> string_list_arg = {};
        std::vector<int> int_list_arg = {};
        std::vector<float> float_list_arg = {};

        std::string curr_arg;
        std::string curr_arg_type;


        std::cout << "Executing Queries \n";
        for( int i=1; i< functions.size(); i++)
        {
            std::string curr_func = functions[i];
            std::string prev_func = functions[i-1];
            
            if (f_args[curr_func].size() > 0)
            {
                curr_arg = umap[curr_func][0];
                curr_arg_type = f_args[curr_func][0];

                if(curr_arg_type == "string")
                    s_arg = curr_arg;
                else if(curr_arg_type == "int")
                    int_arg = std::stoi(curr_arg);
                else if(curr_arg_type == "float")
                    float_arg = std::stof(curr_arg);
                else if(curr_arg_type == "list<string>")
                {
                    std::string without_brackets = "";
                    std::vector<std::string> args = {};
                    for (int i = 1; i < curr_arg.length() -1 ; i++)
                        without_brackets += curr_arg[i];
                    
                    std::vector<std::string> ls = utils::split(without_brackets, "|");
                    for (int i = 0; i < ls.size(); i++)
                    { 
                        ls[i] = utils::trim_spaces(ls[i]);
                        std::cout << ls[i] << "|" << std::endl;
                    } 

                    string_list_arg = ls;
                }
                else if(curr_arg_type == "list<float>")
                {
                    float_list_arg = {};
                    std::string without_brackets = "";
                    std::vector<std::string> args = {};
                    for (int i = 1; i < curr_arg.length() -1 ; i++)
                        without_brackets += curr_arg[i];
                    std::vector<std::string> ls = utils::split(without_brackets, "|");
                    for (int i = 0; i < ls.size(); i++)
                    { 
                        ls[i] = utils::trim_spaces(ls[i]);
                        float_list_arg.push_back(std::stof(ls[i]));
                    }
                }
                
                else if(curr_arg_type == "list<int>")
                {
                    int_list_arg = {};
                    std::string without_brackets = "";
                    std::vector<std::string> args = {};
                    for (int i = 1; i < curr_arg.length() -1 ; i++)
                        without_brackets += curr_arg[i];

                    std::vector<std::string> ls = utils::split(without_brackets, "|");
                    for (int i = 0; i < ls.size(); i++)
                    {
                        ls[i] = utils::trim_spaces(ls[i]);
                        int_list_arg.push_back(std::stoi(ls[i]));
                    } 
                }
            }
            else
               std::cout << "Neglecting args for function " << curr_func << std::endl;

            //based on the function, get and pass the types
            std::cout << "Executing function " << curr_func << std::endl;
            if(curr_func.compare("V") == 0) { this->V(prev_func, s_arg);}
            else if(curr_func.compare("E") == 0) { this->E(prev_func);}
            else if(curr_func.compare("out") == 0) { this->out(prev_func);}
            else if(curr_func.compare("values") == 0) { this->values(prev_func, string_list_arg);}
            else if(curr_func.compare("count") == 0) { this->count(prev_func);}
            else if(curr_func.compare("limit") == 0) { this->limit(prev_func, int_arg);}
            else if(curr_func.compare("groupCount") == 0) { this->groupCount(prev_func);}
            else if(curr_func.compare("groupCountBy") == 0) { this->groupCountBy(prev_func, s_arg);}
            else if(curr_func.compare("has") == 0) { this->has(prev_func, string_list_arg);}
            else if(curr_func.compare("hasNot") == 0) { this->hasNot(prev_func, string_list_arg);}
            else if(curr_func.compare("rankBy") == 0) { this->rankBy(prev_func, s_arg);}
            else  //ideally we would raise an error
                std::cout << "Function " << curr_func << "has no implementation.\n";
        }

    }else{
        std::cout << "Query not valid \n";
    }
}


void Parser::V(std::string prev_func, std::string type)
{
    this->isV = true;
    this->curr_nodes =  this->g.get_nodes(type);
}

void Parser::E(std::string prev_func)
{
    this->isV = false;
    this->curr_edges = this->g.get_edges();
}


void Parser::values(std::string prev_func, std::vector<std::string> labels)
{
    std::string label;
    std::string table_value;
    this->out_header_string = "";

    if (this->isV)
    {
        this->out_header_string = "ID\tName";
        for(auto it = this->curr_nodes.begin(); it != this->curr_nodes.end(); ++it)
        {
            Node node = *it;
            std::string nodeId = node.get_id();
            this->valueMap[nodeId] = {nodeId, node.get_name()};
            for (auto it2 = labels.begin(); it2 != labels.end(); ++it2)
            {
                label = *it2;
                this->out_header_string += "\t" + label;
                table_value = node.table.get_row(label);
                this->valueMap[nodeId].push_back(table_value);
            }
        }
    }
    else 
    {
        this->out_header_string = "ID\tLabel\tSource\tDest";
        for(auto it = this->curr_edges.begin(); it != this->curr_edges.end(); ++it)
        {
            Edge edge = *it;
            std::string edgeId = edge.id;
            this->valueMap[edgeId] = {edge.id, edge.label, edge.source, edge.dest};
            for (auto it2 = labels.begin(); it2 != labels.end(); ++it2)
            {
                label = *it2;
                this->out_header_string += "\t" + label;
                table_value = edge.table.get_row(label);
                this->valueMap[edgeId].push_back(table_value);
            }
        }
    }
}


void Parser::has(std::string prev_func, std::vector<std::string> labels)
{
    if (this->isV )
        this->curr_nodes = this->g.get_nodes_by_labels(labels);
    else
    {
        if(labels.size() > 1)
        {
            std::cout << "Edges cannot have more than one label";
            return;
        }
        this->curr_edges = this->g.get_edges_by_label(labels[0]);
    }

}


void Parser::out(std::string prev_func)
{
    std::cout << this->out_header_string << std::endl;
    if(prev_func == "values")
    {
       for(auto it = this->valueMap.begin(); it != valueMap.end(); ++it)
       {
          for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
              std::cout << *it2 << "\t";
          std::cout << "\n";
       }
    }
    else if(prev_func == "count")
        std::cout << this->_count << std::endl;
}

void Parser::count(std::string prev_func)
{
    this->out_header_string = "";
    if(this->isV)
        this->_count = this->curr_nodes.size();
    else
        this->_count = this->curr_edges.size();
}

void Parser::limit(std::string prev_func, int limit)
{
    this->_limit = limit;
    if(this->isV)
    {
        if (limit < this->curr_nodes.size())
            this->curr_nodes = std::vector<Node>(this->curr_nodes.begin(), this->curr_nodes.begin() + limit);    
        else
            std::cout << "Limit is greater than vector size \n";
    }
    else
    {
        //truncate
        if (limit < this->curr_edges.size())
            this->curr_edges= std::vector<Edge>(this->curr_edges.begin(), this->curr_edges.begin() + limit);    
        else
            std::cout << "Limit is greater than vector size \n";
    }
}

void Parser::groupCount(std::string prev_func)
{
    //do nothing for now
}

void Parser::groupCountBy(std::string prev_func, std::string label)
{
    //do nothing for now
}

void Parser::hasNot(std::string prev_func, std::vector<std::string> labels)
{
    //do nothing for now
}

void Parser::rankBy(std::string prev_func, std::string property, bool ascending)
{
    //do nothing for now
}

void Parser::selectEdges(std::string prev_func, std::vector<std::string> ids)
{
    this->curr_edges = {}; 
    for(int i=0; i < ids.size(); ++i)
    {
        try
        {
            Edge& edge= this->g.get_edge(ids[i]);
            this->curr_edges.push_back(edge);

        }catch(int e){
            continue;
        }
    }

}
        
void Parser::selectNodes(std::string prev_func, std::vector<std::string> ids)
{
    this->curr_nodes= {}; 
    for(int i=0; i < ids.size(); ++i)
    {
        try
        {
            Node& node = this->g.get_node(ids[i]);
            this->curr_nodes.push_back(node);

        }catch(int e){
            continue;
        }
    }

}

void Parser::addNode(std::string prev_func, std::string name, std::string type, std::vector<std::string> mapL)
{
    Node n(name, type);
    int max = mapL.size() ;
    for(int i=0; i < max; i+=2)
    {
        if((i + 1) == max)
                break;
        n.table.insert_row(mapL[i], mapL[i+1]);
    }
    this->g.add_node(n);
}


void Parser::addEdge(std::string prev_func, std::string source_id, std::string dest_id,  std::string label, std::vector<std::string> mapL) 
{
    TableData table;
    int max = mapL.size() ;
    for(int i=0; i < max; i+=2)
    {
        if((i + 1) == max)
                break;
        table[mapL[i]]  =  mapL[i+1];
    }

    this->g.add_edge(source_id, dest_id, label, table);
}


void Parser::delNodesByIds(std::string prev_func, std::vector<std::string> ids)
{
    for(int i=0; i < ids.size(); i++)
        this->g.delete_node(ids[i]);
}

void Parser::delEdgesByIds(std::string prev_func, std::vector<std::string> ids)
{
    for(int i=0; i < ids.size(); i++)
        this->g.delete_edge(ids[i]);
}

void Parser::updateNodeTable(std::string prev_func, std::string id, std::vector<std::string> mapL)
{
    Node& node = this->g.get_node(id);
    int max = mapL.size() ;
    
    for(int i=0; i < max; i+=2)
    {
        if((i + 1) == max)
                break;
        node.table.insert_row(mapL[i], mapL[i+1]);
    }

}
void Parser::updateEdgeTable(std::string prev_func,std::string id, std::vector<std::string> mapL)
{
    Edge& edge= this->g.get_edge(id);
    int max = mapL.size() ;
    
    for(int i=0; i < max; i+=2)
    {
        if((i + 1) == max)
                break;
        edge.table.insert_row(mapL[i], mapL[i+1]);
    }

}
void Parser::addNodeConnections(std::string prev_func, std::string node_id, std::string label, std::vector<std::string> ids)
{
    for(int i=0; i < ids.size(); ++i)
    {
        this->g.add_edge(node_id, ids[i], label, {});
    }
}
void Parser::delNodeConnections(std::string prev_func, std::string node_id, std::vector<std::string> ids)
{
    for(int i=0; i < ids.size(); ++i)
        this->g.delete_connection(node_id, ids[i]);
}

void Parser::outE(std::string prev_func, std::string labels)
{
   if(!this->isV){
        std::cout << "No nodes selected. \n";
        return;
   } 

    //do it in graph  (how tho) do for each, there should be an operation for each from graph
    std::set<Node> node_set;
    for (auto it = this->curr_nodes.begin(); it != this->curr_nodes.end(); ++it)
    {
        try{
            NodeList new_additions = this->g.get_out_nodes(*it);
            for (auto it2= new_additions.begin(); it != new_additions.end(); ++it2)
                node_set.insert(*it2);
        }
        catch(int e)
        {
            std::cout << "Invalid Node in Local set \n";
        }
    }

    this->curr_nodes = {};
    std::copy(node_set.begin(), node_set.end(), this->curr_nodes.begin());
}
