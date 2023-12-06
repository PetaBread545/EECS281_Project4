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

    GraphMST();

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

struct CageTSP{
    int x;
    int y;
    int ID;
    size_t parent;
    bool visited;
    double dist;
};

class GraphFASTTSP{
    public:
    void run();

    GraphFASTTSP();

    void run2();
    std::vector<CageTSP> getCages();
    double getDist()const;
    std::vector<int> getPath();

    private:
    void print()const;
    void readInput();

    void randomInsertion();
    double calcDist(const CageTSP & cageOne, const CageTSP & cageTwo)const;
    double calcTotalDistance()const;

    std::vector<int> path;
    std::vector<CageTSP> cages;
    double totalDist;
    int numCages;

};





//////////////////////////////////////
///             OPTTSP             ///
//////////////////////////////////////

class GraphOPT{
    public:
    void run();

    GraphOPT(std::vector<int> path_in, std::vector<CageTSP> cages_in, double upperBound_in);
    //GraphOPT(std::vector<CageTSP> cages_in, double upperBound_in);

    private:
    void print()const;

    double calcDist(const CageTSP & cageOne, const CageTSP & cageTwo)const;
    double calcDistTotal()const;
    double connectMSTfront(size_t permLength);
    double connectMSTback(size_t permLength);

    double calcLowerBound(size_t permLength);

    bool promising(size_t permLength);

    void genPerms(size_t permLength);

    void genCostMat();

    double calcMST(size_t permLength);

    std::vector<int> path;
    std::vector<int> bestPath;
    std::vector<CageTSP> cages;
    std::vector<std::vector<double>> costMatrix;
    double upperBound;
    size_t numCages;    
};