#ifndef UTILS_CC_INCLUDED_
#define UTILS_CC_INCLUDED_

#include <string>
#include <vector>
#include <unordered_map>
#include <vector>

#include "types.hpp"

namespace utils
{
    std::vector<std::string> split(std::string str, std::string delim=" ")
    {
        std::vector<std::string> ret = {};
        std::size_t start = str.find_first_not_of(delim), end=start;

        while (start != std::string::npos)
        {
            end = str.find(delim, start); // Find next occurence of delimiter
            ret.push_back(str.substr(start, end-start)); // Push back the token found into vector
            start = str.find_first_not_of(delim, end); // Skip all occurences of the delimiter to find new start
        }
        return ret;
    }

    std::string trim_spaces(std::string str)
    {
        const char* typeOfWhitespaces = " ";
        str.erase(str.find_last_not_of(typeOfWhitespaces) + 1);
        str.erase(0,str.find_first_not_of(typeOfWhitespaces));
        return str;
    }

    template <typename T, typename U>
    std::vector<T> umap_keys_to_vec(std::unordered_map<T,U> umap)
    {
        std::vector<T> ret = {};
        for(auto it = umap.begin(); it != umap.end(); ++it)
            ret.push_back(it->first);
        return ret;
    }

    ExprDetails get_expr_map(std::string expr)
    {
        ExprDetails ret;
        std::string sep = ".";
        std::vector<std::string> m = utils::split(expr, (std::string)"."); // split 

        for(int i=0; i< m.size(); ++i)
        {
            std::string sep2 = "(";
            std::vector<std::string> k = utils::split(m.at(i), sep2);

            ret.functions.push_back(k.at(0)); //push func into func vec
        
            std::string sep3 = ",";
            std::vector<std::string> h = utils::split(k.at(1), sep3);

            const int len = h.size()-1;
            h[len] = h[len].substr(0, h[len].length()-1);

            for(int i=0; i<h.size(); ++i)
            {
                h[i] = utils::trim_spaces(h[i]);
                ret.umap[k.at(0)].push_back(h[i]); //save arg into map w/ func name
            }
        }
        return ret;
    }
}

#endif //UTILS_CC_INCLUDED_

