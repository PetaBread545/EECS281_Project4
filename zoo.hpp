// Project Identifier: 3E33912F8BAA7542FC4A1585D2DB6FE0312725B9

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <limits>
#include <cmath>
#include <vector>
#include <getopt.h>
#include <string>
#include <numeric>

enum Mode{
    MST,
    FASTTSP,
    OPTTSP,
    help
};

auto getMode(int argc, char * argv[])->Mode;

void printHelp();


//////////////////////////////////////
///             MST                ///
//////////////////////////////////////
enum animalType{
    Safe,
    Wild,
    Wall
};

struct Edge{
    int start;
    int end;
    double weight;

    friend std::ostream &operator<<(std::ostream & out, const Edge &edge);
    
};

struct CageMST{
    int x;
    int y;
    int ID;
    int parent;
    double dist;
    animalType type;
    bool visited;
    

    void checkType();
    friend std::ostream &operator<<(std::ostream & out, const CageMST &cage);
};

class GraphMST{
    public:

    void run();

    private:

    void print() const;


    void readInput();

    //euclidian distance calculation helper function
    double calcDist(const CageMST & cage1, const CageMST & cage2)const;

    void addCages();


    std::vector<CageMST> cages;
    int cageNum;
    double totalDist;
};





//////////////////////////////////////
///             FASTTSP            ///
//////////////////////////////////////

struct CageFAST{
    int x;
    int y;
    int ID;
    double dist;
    bool visited;
    
    friend std::ostream &operator<<(std::ostream & out, const CageFAST &cage);
};

class GraphFASTTSP{
    public:
    void run();

    private:
    void print();
    void readInput();

    //void twoOPTswap(size_t i, size_t j, double distDiff);

    //void greedyCycle();

    //void twoOPT();

    void randomInsertion();
    double calcDist(const CageFAST & cageOne, const CageFAST & cageTwo)const;
    double calcTotalDistance();

    std::vector<int> path;
    std::vector<CageFAST> cages;
    double totalDist;
    int numCages;

};





//////////////////////////////////////
///             OPTTSP             ///
//////////////////////////////////////


class GraphOPT{
    public:

    private:

    template <typename T>
    void genPerms(vector<T> &path, size_t permLength) {
        if (permLength == path.size()) {
        // Do something with the path
            return;
        }  // if ..complete path

        if (!promising(path, permLength)) {
            return;
        }  // if ..not promising

        for (size_t i = permLength; i < path.size(); ++i) {
            swap(path[permLength], path[i]);
            genPerms(path, permLength + 1);
            swap(path[permLength], path[i]);
        }  // for ..unpermuted elements
    }  // genPerms()

};