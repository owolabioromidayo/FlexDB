#ifndef TYPES_HPP_INCLUDED_
#define TYPES_HPP_INCLUDED_

typedef std::map<std::string, std::string> TableData;

class Table{

    public:
        Table(){}
        Table(TableData itable){
            table = itable;
        }

         TableData get_table() const{ return table; }

        void insert_row(std::string key, std::string value){
            TableData::iterator it;
            it = table.find(key);
            if(it == table.end()) 
                table[key] = value;
            else{
                throw "Key already exists in Table";
            }
        }

        void edit_row(std::string key, std::string value){
            table[key] = value;
        }

        void delete_row(std::string key, std::string value){
            table.erase(key);
        }

        std::string get_row(std::string key){
            TableData::iterator it;
            it = table.find(key);
            if(it == table.end()) 
                return "";
            else
                return it->second;
        }

        void clear_row(std::string key){
            TableData::iterator it;
            it = table.find(key);
            if(it != table.end()) 
                table[key] = "";
        }

        void clear_table(){
            table.clear();
        }

    private:
        TableData table;

};



#endif //TYPES_HPP_INCLUDED_