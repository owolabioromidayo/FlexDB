#include <string>
#include <unordered_map>
#include <iostream>
#include <stdexcept>
#include <algorithm>

#include "parser.hpp"
#include "utils.cc"


Parser::Parser(Graph g){
    this->g = g;
    this->init_all_ops();
}

Parser::Parser(){

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

void Parser::init_all_ops(){
    all_ops.insert(starting_ops.begin(), starting_ops.end()); 
    all_ops.insert(end_ops.begin(), end_ops.end()); 
    all_ops.insert(collection_ops.begin(), collection_ops.end()); 
    all_ops.insert(inference_ops.begin(), inference_ops.end()); 
    all_ops.insert(traversal_ops.begin(), traversal_ops.end()); 

}

bool Parser::readGraph(std::string filename){
       try{
            this->g.deserialize(filename);
            return true;
        } catch(int e){
            throw std::invalid_argument("File does not exist.\n");
            return false;
        }
}

NodeList Parser::V(){
    return this->g.get_nodes();
}

EdgeList Parser::E(){
    return this->g.get_edges();
}


bool Parser::is_valid_expr(std::string expr){
   
    //decompose expr
    ExprDetails n_expr = utils::get_expr_map(expr);
    std::vector<std::string> functions = n_expr.functions;
    std::unordered_map<std::string, std::vector<std::string>> umap = n_expr.umap;

    std::cout << "Validating expr " << expr << std::endl;
    //return empty expr
    if(functions.size() < 2){
        std::cout << "Query is not long enough" << std::endl;
        return false;
    }

    //do a simple pass to check if all ops are there before digging further 
    for(int i=1; i < functions.size(); ++i){
        std::string curr = functions.at(i);
    
        if(*std::find(all_ops.begin(), all_ops.end(), curr) != curr){
            std::cout << "Function " << curr << " is not in argument list" << std::endl;
            return false;
        }
    }

    //check starting ops
    if(*std::find(starting_ops.begin(), starting_ops.end(), functions.at(0)) != functions.at(0)){
        std::cout << "Cannot start query with function " << functions.at(0) << std::endl ;
        return false;
    }

    //check middle ops not starting or ending
    //we really need to think about the networking of these functions

    //check ending ops
    if(*std::find(end_ops.begin(), end_ops.end(), functions.back()) != functions.back()){
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
    for(int i=0; i< functions.size(); i++){
        std::string curr = functions.at(i);

        std::vector<std::string> args = umap[curr];
        std::vector<std::string> correct = this->f_args[curr];

        //return if arg lengths dont match  
        if(correct.size() == 0 ){
            std::cout << "Neglecting args for function " << curr << std::endl;
            continue;
        }else if(args.size() != correct.size()){
            std::cout << "Argument lists at function " << curr << " dont match you:func " << args.size() <<" : " << correct.size() << std::endl;
            // return false;
        }

        //arg comparison
        for(int i=0; i< args.size(); ++i ){ 
            //here we try to see if we can convert the types going down from least likely
            std::string curr_arg = args[i];
            std::string curr_type = correct[i];

            if (curr_type.compare("int") == 0)
            {
                    //try convert string -> int
                    try
                    {
                        int x = stoi(curr_arg);
                    }
                    catch(int e)
                    { 
                        std::cout << "Cannot convert " << curr_arg << " to type int" << std::endl;
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
                    for (int i = 1; i < curr_arg.length() -1 ; i++){ //neglect braces
                        without_brackets += curr_arg[i];
                    } 

                    std::vector<std::string> ls = utils::split(without_brackets, "|");
                    for (int i = 0; i < ls.size(); i++){ //neglect braces
                        ls[i] = utils::trim_spaces(ls[i]);
                        //check if all are of type int
                        try{
                            int x = stoi(ls[i]);
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
                    for (int i = 1; i < curr_arg.length() -1 ; i++){ //neglect braces
                        without_brackets += curr_arg[i];
                    } 
                    std::cout << "Without brackets" <<  without_brackets << std::endl;

                    std::vector<std::string> ls = utils::split(without_brackets, "|");
                    for (int i = 0; i < ls.size(); i++){ //neglect braces
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
            {
                continue;
            }else
            {
                //unaccepted type
                std::cout << "Unaccepted type " << curr_type << std::endl; 
                return false;
            }
        }
    }

    return true;

}

void Parser::resolve_query(std::string query){

}