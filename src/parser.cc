#include <string>
#include <unordered_map>
#include <iostream>
#include <stdexcept>
#include <algorithm>

#include "node.hpp"
#include "utils.cc"
#include "types.hpp"
#include "parser.hpp"


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
            this->readGraph();
            done = true;
        }
        catch(std::invalid_argument& e){
            std::cout << "File could not be loaded.";
        }
    }

    this->init_all_ops();
   
}

void Parser::init_all_ops(){
    all_ops.insert(
    end_ops.begin(), end_ops.end(), 
    collection_ops.begin(), collection_ops.end(), 
    inference_ops.begin(), inference_ops.end(),
    traversal_ops.begin(), traversal_ops.end());

    for(int i=0; i<all_ops.size(); ++i){
        std::cout << m.at(i) << "\n";
    }
}

void Parser::readGraph(std::string filename){
       try{
            Graph g("__");
            g.deserialize(filename);
            this->g = g;
            done = true;
        } catch(int e){
           throw std::invalid_argument("File does not exist.\n");
        }
}

NodeList Parser::V(){
    return this->g.get_nodes();
}

EdgeList Parser::E(){
    return this->g.get_edges();
}


bool Parser::is_valid_expr(std::string expr){
    bool is_valid = false;
    std::vector<std::string> functions = {};
    std::unordered_map<std::string, std::vector<std::string>> mapped_expr = {};


    //decompose expr
    ExprDetails expr = utils::get_expr_map(expr);
    std::vector<std::string> functions = expr.functions;
    std::map<std::string, std::vector<std::string>> umap = expr.umap;

    //return empty expr
    if(functions.size() < 2){
        return false;
    }

    //do a simple pass to check if all ops are there before digging further
    for(int i=0; i < functions.size(); ++i){
        std::string curr = functions.at(i);
        if(*std::find(all_ops.begin(), all_ops.end(), curr) != curr){
            return false;
        }
    }

    //check starting ops
    if (functions.at(0) != "g"){
        return false;
    }

    if(*std::find(all_ops.begin(), all_ops.end(), functions.at(1)) != functions.at(1)){
        return false;
    }

    //check ending ops
    if(*std::find(end_ops.begin(), end_ops.end(), functions.at(1)) != functions.at(1)){
        return false;
    }

    //check args, neglect g.
    for(int i=0; i< functions.size(); i++){
        std::string curr = functions.at(i);

        std::vector<std::string> args = umap[curr];
        std::vector<std::string> correct = f_args[curr];

        //return if arg lengths dont match  
        if(arg.size() != correct.size()){
            return false
        }
    }


    //get function list chaining and check each fn args

    //iter -> check ssquencing

    return is_valid;

}

void Parser::resolve_query(std::string query){

}