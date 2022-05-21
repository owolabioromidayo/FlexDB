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
        //    throw std::invalid_argument("File does not exist.\n");
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

    //return empty expr
    if(functions.size() < 2){
        return false;
    }

    //do a simple pass to check if all ops are there before digging further 
    for(int i=1; i < functions.size(); ++i){
        std::string curr = functions.at(i);
    
        if(*std::find(all_ops.begin(), all_ops.end(), curr) != curr){
            return false;
        }
    }

    //check starting ops
    if(*std::find(starting_ops.begin(), starting_ops.end(), functions.at(0)) != functions.at(0)){
        return false;
    }

    //check ending ops
    if(*std::find(end_ops.begin(), end_ops.end(), functions.back()) != functions.back()){
        return false;
    }

    //check middle ops

    //check args, neglect g.
    // for(int i=0; i< functions.size(); i++){
    //     std::string curr = functions.at(i);

    //     std::vector<std::string> args = umap[curr];
    //     std::vector<std::string> correct = this->f_args[curr];

    //     //return if arg lengths dont match  
    //     if(args.size() != correct.size()){
    //         return false;
    //     }
    // }


    //get function list chaining and check each fn args

    //iter -> check ssquencing

    return true;

}

void Parser::resolve_query(std::string query){

}