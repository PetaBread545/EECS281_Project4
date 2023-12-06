// Project Identifier: 3E33912F8BAA7542FC4A1585D2DB6FE0312725B9

#include "zoo.hpp"


using namespace std;

int main(int argc, char * argv[]){
    ios_base::sync_with_stdio(false);
    cout << std::setprecision(2); //Always show 2 decimal places
    cout << std::fixed; //Disable scientific notation for large numbers
    Mode mode = getMode(argc, argv);
    if(mode == MST){
        GraphMST graphMST;
        graphMST.run();
    } else if(mode == FASTTSP){
        GraphFASTTSP graphFAST;
        graphFAST.run();
    } else if(mode == OPTTSP){
        GraphFASTTSP graphOne;
        graphOne.run2();

        GraphOPT graphTwo(graphOne.getPath(), graphOne.getCages(), graphOne.getDist());
        //GraphOPT graphTwo(graphOne.getCages(), graphOne.getDist());
        graphTwo.run();

    }

}