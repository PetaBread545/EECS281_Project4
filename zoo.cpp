// Project Identifier: 3E33912F8BAA7542FC4A1585D2DB6FE0312725B9

#include "zoo.hpp"

using namespace std;


auto getMode(int argc, char * argv[])->Mode {
  // These are used with getopt_long()
  opterr = false; // Let us handle all error output for command line options
  int32_t choice = 0;
  int32_t index = 0;

  struct option long_options[] = {

    // TODO: Fill in two lines, for the "mode" ('m') and
    // the "help" ('h') options.
    {"help" , no_argument, nullptr, 'h'},
    {"mode", required_argument, nullptr, 'm'},
    { nullptr, 0, nullptr, '\0' }
    
  };  // long_options[]
  string modeIn;
  Mode mode;

  //Fill in the double quotes, to match the mode and help options.
  while ((choice = getopt_long(argc, argv, "hm:", long_options, &index)) != -1) {
    switch (choice) {
      case 'h':
        printHelp();
        exit(0);
        break;

      case 'm':  // Need a block here to declare a variable inside a case
        modeIn = optarg;
        break;

      default:
        cerr << "Error: invalid option\n";
        exit(1);
    }  // switch ..choice
  }  // while

  if(modeIn == "MST"){
    mode = MST;
  }else if(modeIn == "FASTTSP"){
    mode = FASTTSP;
  }else if(modeIn == "OPTTSP"){
    mode = OPTTSP;
  }else{
    cerr << "Error: invalid option\n";
    exit(1);
  }
  return mode;// if ..mode
}  // getMode()


void printHelp(){
    cout << "--mode [-m]: specify mode (MST, FASTTSP, OPTTSP)\n--help [-h]: get help";
}


//////////////////////////////////
//         CAGE STUFF          //
//////////////////////////////////

GraphMST::GraphMST(){
    cageNum = 0;
    totalDist = 0;
}

void CageMST::checkType(){
    if((this->x > 0 && this->y > 0) || (this->x <= 0 && this->y > 0) || (this->x > 0 && this->y <= 0)){
        this->type = Safe;
        return;
    }else if(this->x < 0 && this->y < 0){
        this->type = Wild;
        return;
    }else if((this->x == 0 && this->y <= 0) || (this->x <= 0 && this->y == 0)){
        this->type = Wall;
        return;
    }
}

ostream & operator<<(ostream & out, const CageMST & cage){
    if(cage.parent < cage.ID){
        out << cage.parent << " " << cage.ID;
        return out;
    }
    out << cage.ID << " " << cage.parent;
    return out;
}

//////////////////////////////////
//         GRAPH STUFF          //
//////////////////////////////////




void GraphMST::readInput(){
    bool hasWild = false;
    bool hasWall = false;
    bool hasSafe = false;
    cin >> cageNum;
    int xCord;
    int yCord;
    cages.reserve(cageNum);
    for(int i = 0; i < cageNum; ++i){
        cin >> xCord >> yCord;
        CageMST cage{xCord, yCord, i, 0, numeric_limits<double>::infinity(), Safe, false};
        cage.checkType();
        cages.push_back(cage);
        switch(cage.type){
            case Safe:
            hasSafe = true;
            break;

            case Wall:
            hasWall = true;
            break;

            case Wild:
            hasWild = true;
            break;
        }
    }
    if(hasSafe && hasWild && !hasWall){
        cerr << "Cannot construct MST\n";
        exit(1);
    }

}

double GraphMST::calcDist(const CageMST & cageOne, const CageMST & cageTwo)const{
    if((cageOne.type != cageTwo.type) && (!(cageOne.type == Wall || cageTwo.type == Wall))){
        return numeric_limits<double>::infinity();
    }
    uint64_t xDiff = cageOne.x - cageTwo.x;
    uint64_t yDiff = cageOne.y - cageTwo.y;
    uint64_t xDist = xDiff * xDiff;
    uint64_t yDist = yDiff * yDiff;
    return sqrt(xDist + yDist);
}

void GraphMST::addCages(){
    int nextV = 0, currentV = 0;
    cages[0].dist = 0;
    //outer loop
    for(int i = 0; i < cageNum; ++i){
        //step 1: identify lowest distance
        double minDist = numeric_limits<double>::infinity();
        for(int k = 0; k < cageNum; ++k){
            if(cages[k].visited){
                continue;
            }
            if(cages[k].dist < minDist){
                nextV = k;
                minDist = cages[k].dist;
            }
        }
        
        //step 2: lowest distance set to true
        auto & nextItr = cages[nextV];
        nextItr.visited = true;
        //make new edge, add to path
        totalDist += minDist;
        //define new parent for step 3
        currentV = nextV;

        //step 3: update unvisited distances
        for(int j = 0; j < cageNum; ++j){
            auto & nodePtr = cages[j];
            //if already visited, skip
            if(nodePtr.visited){
                continue;
            }
            double min = nodePtr.dist;
            double temp = calcDist(cages[currentV], nodePtr);
            //If distance from current parent is lower than current distance, update
            if(temp < min){
                nodePtr.dist = temp;
                nodePtr.parent = currentV;
            }
        }
    }
}

void GraphMST::print()const{
    //print total distance
    cout << totalDist << "\n";
    for(int i = 1; i < cageNum; ++i){
        cout << cages[i] << "\n";
    }
}


void GraphMST::run(){
    readInput();
    addCages();
    print();
}



//////////////////////////////////
//        FASTTSP STUFF         //
//////////////////////////////////


GraphFASTTSP::GraphFASTTSP(){
    totalDist = 0;
    numCages = 0;
}

ostream & operator<<(ostream & out, const CageTSP & cage){
    out << cage.ID;
    return out;
}

void GraphFASTTSP::run(){
    readInput();
    //greedyCycle();
    //twoOPT();
    randomInsertion();
    print();
}

void GraphFASTTSP::print()const{
    cout << totalDist << "\n";
    for(size_t i = 0; i < path.size(); ++i){
        cout << path[i] << " ";
    }
}


void GraphFASTTSP::readInput(){
    cin >> numCages;
    int xCord;
    int yCord;
    cages.reserve(numCages);
    path.reserve(numCages + 1);
    for(int i = 0; i < numCages; ++i){
        cin >> xCord >> yCord;
        CageTSP cage{xCord, yCord, i, 0,  false, numeric_limits<double>::infinity()};
        cages.push_back(cage);
    }
}

void GraphFASTTSP::run2(){
    readInput();
    //greedyCycle();
    //twoOPT();
    randomInsertion();
}

vector<int> GraphFASTTSP::getPath(){
    return path;
}

vector<CageTSP> GraphFASTTSP::getCages(){
    return cages;
}

double GraphFASTTSP::getDist()const{
    return totalDist;
}
/*void GraphFASTTSP::greedyCycle(){
    
    int nextV = 0, currentV = 0;
    cages[0].dist = 0;
    //outer loop
    for(int i = 0; i < numCages; ++i){
        //step 1: identify lowest distance
        double minDist = numeric_limits<double>::infinity();
        for(int k = 0; k < numCages; ++k){
            if(cages[k].visited){
                continue;
            }
            if(cages[k].dist < minDist){
                nextV = k;
                minDist = cages[k].dist;
            }
        }
        //step 2: lowest distance set to true
        auto & nextItr = cages[nextV];
        path.push_back(nextV);
        nextItr.visited = true;
        //make new edge, add to path
        totalDist += minDist;
        //define new parent for step 3
        currentV = nextV;

        //step 3: update unvisited distances
        for(int j = 0; j < numCages; ++j){
            auto & nodePtr = cages[j];
            //if already visited, skip
            if(nodePtr.visited){
                continue;
            }
            //If distance from current parent is lower than current distance, update
            nodePtr.dist = calcDist(cages[currentV], nodePtr);
        }
    }
    //complete cycle;
    double last = calcDist(cages[0], cages[currentV]);
    totalDist += last;
    path.push_back(0);
}

void GraphFASTTSP::twoOPTswap(size_t i, size_t j, double distDiff){
    reverse(path.begin() + i + 1, path.begin() + j + 1);
    totalDist += distDiff;
}

void GraphFASTTSP::twoOPT(){
    bool improved = true;
    size_t size = path.size();
    while(improved){
        improved = false;
        for(size_t i = 0; i < size - 1; ++i){
            for(size_t j = i + 1; j < size; ++j){
                double newRouteDist = - calcDist(cages[path[i]], cages[path[(i + 1)]]) 
                                    - calcDist(cages[path[j % size]], cages[path[(j + 1) % size]])
                                    + calcDist(cages[path[i]], cages[path[j % size]]) 
                                    + calcDist(cages[path[(i + 1)]], cages[path[(j + 1) % size]]);
                if(newRouteDist < 0){
                    improved = true;
                    twoOPTswap(i, j, newRouteDist);
                }
            }
        }
    }
}*/

double GraphFASTTSP::calcDist(const CageTSP & cageOne, const CageTSP & cageTwo)const{
    uint64_t xDiff = cageOne.x - cageTwo.x;
    uint64_t yDiff = cageOne.y - cageTwo.y;
    uint64_t xDist = xDiff * xDiff;
    uint64_t yDist = yDiff * yDiff;
    return sqrt(xDist + yDist);
}



void GraphFASTTSP::randomInsertion(){
    vector<int> order;
    order.resize(numCages - 1);
    iota(order.begin(), order.end(), 1);
    //shuffle(order.begin(), order.end(), g);
    path.push_back(0);
    path.push_back(order[0]);
    path.push_back(order[1]);
    for(size_t i = 2; i < order.size(); ++i){
        double min = numeric_limits<double>::infinity();
        int minIndex = -1;
        int currentIndex = order[i];
        for(size_t j = 0; j < path.size(); ++j){
            double temp = 0;
            int pathJ = path[j];
            if(j + 1 == path.size()){
                temp = calcDist(cages[pathJ], cages[currentIndex]) 
                    + calcDist(cages[currentIndex], cages[path[0]]) 
                    - calcDist(cages[pathJ], cages[path[0]]);
            }else{
                temp = calcDist(cages[pathJ], cages[currentIndex]) 
                    + calcDist(cages[currentIndex], cages[path[j+1]]) 
                    - calcDist(cages[pathJ], cages[path[j+1]]);
            }
            if(temp < min){
                minIndex = static_cast<int>(j + 1);
                min = temp;
            }
        }
        if(minIndex == 0){
            path.push_back(currentIndex);
        }else{
            path.insert(path.begin() + minIndex, currentIndex);
        }   
    }
    totalDist = calcTotalDistance();
}

double GraphFASTTSP::calcTotalDistance()const{
    double ans = 0;
    for(size_t i = 0; i < path.size() - 1; ++i){
        ans += calcDist(cages[path[i]], cages[path[i + 1]]);
    }
    ans += calcDist(cages[path[0]], cages[path[path.size()-1]]);
    return ans;
}


////////////////////////////////////////
//              OPTTSP                //
////////////////////////////////////////


GraphOPT::GraphOPT(vector<int> path_in, vector<CageTSP> cages_in, double upperBound_in)
:path(path_in), cages(cages_in)
{
    numCages = path_in.size();
    bestPath = path;
    upperBound = upperBound_in;
    costMatrix = vector<vector<double>>(numCages, vector<double>(numCages, 0));
}

void GraphOPT::genCostMat(){
    costMatrix.reserve(numCages);
    for(size_t i = 0; i < numCages; ++i){
        vector<double> temp;
        for(size_t j = 0; j <= i; ++j){
            costMatrix[i][j] = (calcDist(cages[i], cages[j]));
        }
    }
}

double GraphOPT::calcDist(const CageTSP & cageOne, const CageTSP & cageTwo)const{
    uint64_t xDiff = cageOne.x - cageTwo.x;
    uint64_t yDiff = cageOne.y - cageTwo.y;
    uint64_t xDist = xDiff * xDiff;
    uint64_t yDist = yDiff * yDiff;
    return sqrt(xDist + yDist);
}




double GraphOPT::calcDistTotal()const{
    /*double ans = 0;
    for(size_t i = 0; i < path.size() - 1; ++i){
        ans += calcDist(cages[path[i]], cages[path[i + 1]]);
    }
    ans += calcDist(cages[path[0]], cages[path[numCages-1]]);
    return ans;*/
    double ans = 0;
    int parent = 0;
    for(size_t i = 0; i < numCages; ++i){
        if(parent > path[i]){
            ans += costMatrix[parent][path[i]];
        }else{
            ans += costMatrix[path[i]][parent];
        }
        parent = path[i];
    }
    ans += costMatrix[parent][0];
    return ans;
}


void GraphOPT::run(){
    genCostMat();
    size_t permLength = 1; 
    genPerms(permLength);
    print();
}

void GraphOPT::print()const{
    cout << upperBound << '\n';
    for(auto&itr:bestPath){
        cout << itr << " ";
    }
}

bool GraphOPT::promising(size_t permLength){
    double subCost = 0;
    int parent = 0;
    //fixed permute cost
    for(size_t i = 0; i < permLength; ++i){
        if(parent > path[i]){
            subCost += costMatrix[parent][path[i]];
        }else{
            subCost += costMatrix[path[i]][parent];
        }
        parent = path[i];
    }

    //connection costs and mst of the rest of the vertices.
    double lowerBound = connectMSTback(permLength) + connectMSTfront(permLength) + calcMST(permLength) + subCost;
    if(lowerBound > upperBound){
        return false;
    }
    return true;
}



void GraphOPT::genPerms(size_t permLength){
    if (permLength == path.size()) {
        double temp = calcDistTotal();
        if(temp < upperBound){
            upperBound = temp;
            bestPath = path;
        }
    return;
    }  // if ..complete path

  if (!promising(permLength)) {
    return;
  }  // if ..not promising

  for (size_t i = permLength; i < numCages; ++i) {
    swap(path[permLength], path[i]);
    genPerms(permLength + 1);
    swap(path[permLength], path[i]);
  }  // for ..unpermuted elements
}

double GraphOPT::calcMST(size_t permLength){
    struct tracker{
        bool visited = false;
        double dist = numeric_limits<double>::infinity();
    };

    int pathI = path[permLength];
    size_t nextV = 0, currentV = 0;
    double totalDist = 0;
    
    //initialize tracking vector, visitedCages[i] -> cages[i]
    tracker temp;
    vector<tracker> visitedCages(numCages, temp);

    //first cage after the permutation is starter
    visitedCages[pathI].dist = 0;
    //outer loop
    for(size_t i = permLength; i < numCages; ++i){
        //step 1: identify lowest distance
        double minDist = numeric_limits<double>::infinity();
        for(size_t k = permLength; k < numCages; ++k){
            //this is the path index after the fixed perm
            int pathK = path[k];
            //if the cage has been visited,skip it
            if(visitedCages[pathK].visited){
                continue;
            }
            // if the cage has a lower distance, store the lower distance and track the index of the cage on the path
            if(visitedCages[pathK].dist < minDist){
                nextV = pathK;
                minDist = visitedCages[pathK].dist;
            }
        }
        
        //step 2: lowest distance set to true
        visitedCages[nextV].visited = true;
        //make new edge, add to path
        totalDist += minDist;
        //define new parent for step 3
        currentV = nextV;

        //step 3: update unvisited distances
        for(size_t j = permLength; j < numCages; ++j){
            int pathJ = path[j];
            //if already visited, skip
            if(visitedCages[pathJ].visited){
                continue;
            }
            //current cage min to beat
            double min = visitedCages[pathJ].dist;
            //current cage to the other cages
            double temp = calcDist(cages[currentV], cages[pathJ]);
            //If distance from current parent is lower than current distance, update
            if(temp < min){
                visitedCages[pathJ].dist = temp;
                cages[pathJ].parent = currentV;
            }
        }
    }
    return totalDist;
}


double GraphOPT::connectMSTfront(size_t permLength){
    double min = numeric_limits<double>::infinity();
    for(size_t i = permLength; i < numCages; ++i){
        double temp = calcDist(cages[path[0]], cages[path[i]]);
        if(temp < min){
            min = temp;
        }
    }
    return min;
}

double GraphOPT::connectMSTback(size_t permLength){
    double min = numeric_limits<double>::infinity();
    for(size_t i = permLength; i < numCages; ++i){
        double temp = calcDist(cages[path[permLength - 1]], cages[path[i]]);
        if(temp < min){
            min = temp;
        }
    }
    return min;
}
