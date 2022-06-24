#include <iostream>
#include <string>

#include "./node.cc"
#include "./graph.cc"
#include "./parser.cc"

int main(){

    bool dbLoaded = false;

    std::string prompt = "\
                       ('-.  ) (`-.            _ .-') _ .-. .-')   \n\
                     _(  OO)  ( OO ).         ( (  OO) )\\  ( OO )  \n\
   ,------.,--.     (,------.(_/.  \\_)-.       \\     .'_ ;-----.\\  \n\
('-| _.---'|  |.-')  |  .---' \\  `.'  /        ,`'--..._)| .-.  |  \n\
(OO|(_\\    |  | OO ) |  |      \\     /\\        |  |  \\  '| '-' /_) \n\
/  |  '--. |  |`-' |(|  '--.    \\   \\ |        |  |   ' || .-. `.  \n\
\\_)|  .--'(|  '---.' |  .--'   .'    \\_)       |  |   / :| |  \\  | \n\
  \\|  |_)  |      |  |  `---. /  .'.  \\        |  '--'  /| '--'  / \n\
   `--'    `------'  `------''--'   '--'       `-------' `------'   ";

std::cout << prompt << std::endl;

    Graph db("database");
    std::string dbName = "";
    std::string query = "";

    while(!dbLoaded)
    {
        std::cout << "Enter path to database for loading.\n";
        std::cout << ">>>";
        std::cin >> dbName; 
        try{
            db.deserialize(dbName); 
            dbLoaded = true;
        } catch(int e){
            continue;
        }
    }

    Parser p(db);

    while(1)
    {
        std::cout << "Enter query to be resolved.\n";
        std::cout << ">>>";
        std::cin >> query; 
        try{

            p.resolve_query(query);
        } catch(int e){
            continue;
        }
    }    
    return 0;
}

