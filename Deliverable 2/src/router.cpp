//includes
#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>
#include <map>
#include <fstream>
#include <string>
#include <cstring>
using namespace std;

//structs
struct layer{vector<vector<int>> l; int dir; int num; float pitch; int ratio; };
struct coord{int x; int y; int z;};
struct layerDir{int direction; float pitch;};

//global variables
int cells = 0;
int vias = 0;
int route = -10;
int s_value, t_value;

//function declarations
void printMatrices(vector <layer> &manyLayers, int x, int y, int m);
bool input(vector <coord> &pins, int x, int y, int m);
coord traverse(vector <vector<int>> &l, int dir, int x, int y, int m, coord s, coord t, bool isSource, string &def);
bool backTracking(vector<layer> &manyLayers, int x, int y, int m, coord source, coord target, int via, coord source1, string def, vector<string> &defs);
void backToLife(vector<layer> &manyLayers, int x, int y, int m);
bool flood(vector<layer> &manyLayers, int x, int y, int m, coord newSource, coord target, int via, int count0, bool isSource);
bool classicalImplementation(vector<layer> &manyLayers, int x, int y, int m, coord source, coord target, int via, bool floodLessB, vector <string> &defNets, string metalO, ofstream &out);
void undoTraversal(vector<vector<int>> &l, int dir, int x, int y, coord s, coord t);
coord floodLess(vector<layer> &manyLayers, int x, int y, int m, coord newSource, coord target, vector <string> &defs);
void multiPins(vector<layer> &manyLayers, int x, int y, int m, vector<coord> target, int via, bool floodLessB, vector <string> &defNets, ofstream &out, int netCount);
void getDistance(int dir, coord source, vector<coord> targets, vector<pair<int,int>> &targetsDist);
void getCoords(vector<layer> &manyLayers, int x, int y, int m, int route, vector<coord> &coords);
int countCells(vector<layer> manyLayers, int x, int y, int m);
void computeRatio(vector<layer> &manyLayers);
void parseTechLef(string filename, map<int, int> &layerDirection, map<int, float> &layerPitch);
void parseInput(string filename, string outputName, vector<coord> &pins, vector<layer> &manyLayers, vector <string> &defNets, int x, int y, int m, int via, bool floodLessB);
void parseInput2(string filename, int &width, int &length, vector <coord> &obstacles);
void obstructRoute(vector <layer> &manyLayers, int x, int y, int m, vector <coord> obstacles);

//main
int main(){
    
//    string inputFile1 = "test1.txt";
//    string inputFile2 = "test1.1.txt";
//    string outputFile = "out_test1.txt";
    
    string inputFile1 = "test2.txt";
    string inputFile2 = "test2.2.txt";
    string outputFile = "out_test2.txt";
    
    int x, y, m; //coordinates of grid
    
    map<int, int> layerDirection; //map of metal index and metal direction
    map<int, float> layerPitch; //map of metal index and metal pitch
    parseTechLef("osu035.lef", layerDirection, layerPitch);
    
    m = layerDirection.size();
//    for (int i=0; i < m; i++){
//        cout << "Metal Layer " << i << endl;
//        cout << layerDirection[i] << endl;
//        cout << layerPitch[i] << endl;
//    }
    
    int via = 10; //via cost
    bool floodLessB = 1; //DFS until flooding is needed
    
    int width, length;
    vector <coord> obstacles;
    parseInput2(inputFile2, width, length, obstacles);
    
    if(layerDirection[0] == 0){ //vertical
        y = width / layerPitch[0];
        x = length / layerPitch[1];
        cout << x << endl;
        cout << y << endl;
    }
    else {
        x = length / layerPitch[0];
        y = width / layerPitch[1];
        cout << x << endl;
        cout << y << endl;
    }
//
//    x = 10;
//    y = 10;
    
    vector<layer> manyLayers;
    
    //initializations
    vector <int> rows(y);
    vector <vector <int>> l1(x,rows); //layer 1
    
    for (int i = 0; i<x; i++){
        for (int j = 0; j<y; j++){
            l1[i][j] = 0;
        }
    }
    for (int i=0; i<m; i++){
        layer tempL;
        vector<vector<int>> temp = l1;
        tempL.l = temp;
        tempL.num = i;
        tempL.pitch = layerPitch[i];
        tempL.dir = layerDirection[i]; //1: horizontal, 0: vertical
        manyLayers.push_back(tempL);
    }
    computeRatio(manyLayers);
    
    obstructRoute(manyLayers, x, y, m, obstacles);
    
    vector<coord> pins2;
    vector <string> defNets;
    parseInput(inputFile1, outputFile,pins2, manyLayers, defNets, x, y, m, via, floodLessB);

    

    
//    cout << "Please enter cost of via\n";
//    cin >> via;
    
//    cout << "\nPlease enter the x-dimension of the plane\n";
//    cin >> x;
//    cout << "Please enter the y-dimension of the plane\n";
//    cin >> y;
    
//    cout << "How many metal layers are there?\n";
//    cin >> m;
//
//    //1: horizontal, 0: vertical
//    for (int i=0; i<m; i++){
//        layerDirection[i] = (i+1) % 2;
//        layerPitch[i] = 0.6;
////        if (i==0)
////            layerPitch[i] = 0.6;
////        else
////            layerPitch[i] =  0.6 * (i);
//    }
//
    
    
    
//    for (int i=0; i<m; i++){
//        layer tempL;
//        vector<vector<int>> temp = l1;
//        tempL.l = temp;
//        tempL.num = i;
//        tempL.pitch = layerPitch[i];
//        tempL.dir = layerDirection[i]; //1: horizontal, 0: vertical
//        manyLayers.push_back(tempL);
//    }
//    computeRatio(manyLayers);
    
//    cout << "\nDo you want to do DFS in more than one layer and thus flood less?\nIf yes enter 1, if no enter 0" << endl;
//    cin >> floodLessB;
//
//    vector <coord> pins;
//
//    while (true){
//        pins.erase(pins.begin(), pins.end());
//        if (!input(pins, x, y, m))
//            return 0;
//        //function to route
//        multiPins(manyLayers, x, y, m, pins, via, floodLessB, defNets);
//        route--;
//        cells = 0;
//        vias = 0;
//    }

    
    return 0;
}

//function definitions
void printMatrices(vector <layer> &manyLayers, int x, int y, int m){
    for(int z = 0; z< m; z++){
        cout << "\nMetal Layer: " << z << ", Direction: ";
        if (manyLayers[z].dir == 1)
            cout << "horizontal";
        else if (manyLayers[z].dir == 0)
            cout << "vertical";
        cout << " Pitch: " << manyLayers[z].pitch;
        cout << " Ratio: " << manyLayers[z].ratio << endl;
        for (int i = 0; i<x; i++){
            for (int j = 0; j<y; j++){
                cout << manyLayers[z].l[i][j] << "\t";
            }
            cout << endl;
        }
        cout << endl << endl << endl;
    }
}
bool input(vector <coord> &pins, int x, int y, int m){
    coord pin;
    int n;
    do {
        cout << "Number of pins to connect in this net\n";
        cin>> n;
    }   while(n == 1);
    if (n<=0)
        return false;
    
    while (n>0){
        do{
            cout << "Enter x coordinate of target\n";
            cin >> pin.x;
        } while((pin.x < 0) || (pin.x >= x));
        
        
        do {
            cout << "Enter y coordinate of target\n";
            cin >> pin.y;
        } while ((pin.y < 0) || (pin.y >= y));
        
        
        do {
            cout << "Enter z coordinate of target\n";
            cin >> pin.z;
        } while((pin.z < 0) || (pin.z >= m));
        
        pins.push_back(pin);
        
        n--;
    }
    return true;
}
coord traverse(vector <vector<int>> &l, int dir, int ratio, int x, int y, coord s, coord t, bool isSource, string &def){
    //vertical
    def = def + to_string(s.z + 1);
    coord newSource;
    coord prev;
    if(dir == 0){ // vertical
        s.y -= (s.y % ratio);
        t.y -= (t.y % ratio);
    }
    else {
        s.x -= (s.x % ratio);
        t.x -= (t.x % ratio);
    }
    if (dir  == 0){ //vertical
        if (isSource){ //is s == initial source
            int j = s.y;
            if(s.y % ratio == 0){
                int index = 0;
                while(index < ratio && j+index<y){
                    l[s.x][j+index] = route;
                    index++;
                }
//                def = def + " ( " + to_string(s.x) + " " + to_string(s.y) + " ) ";
                prev.x = s.x;
                prev.y = s.y;
            }
        }
        else if ((s.x == t.x) && (s.y == t.y) && (s.z == t.z)){ //s is target
            int j = s.y;
            if(s.y % ratio == 0){
                int index = 0;
                while(index < ratio && j+index<y){
                    l[s.x][j+index] = route;
                    index++;
                }
//                def = def + " ( " + to_string(s.x) + " " + to_string(s.y) + " ) ";
                prev.x = s.x;
                prev.y = s.y;
            }
            return t;
        }
        else if (l[s.x][s.y] == 0){
            int j = s.y;
            if(s.y % ratio == 0){
                int index = 0;
                while(index < ratio && j+index<y){
                    l[s.x][j+index] = route;
                    index++;
                }
            }
//            def = def + " ( " + to_string(s.x) + " " + to_string(s.y) + " ) ";
            prev.x = s.x;
            prev.y = s.y;
        }
        else //is s is not initial source and it is visited
            return s;
        if (t.x > s.x){
            for (int i = s.x + 1; i<=t.x; i++){ //iterate vertically from source to target
                if (l[i][s.y] == 0){ //if cell is empty
                    int j = s.y;
                    if(s.y % ratio == 0){
                        int index = 0;
                        while(index < ratio && j+index<y){
                            l[i][j+index] = route;
                            index++;
                        }
                    }
                }
                else{ //else return
//                    cout <<" (" << i-1 << " " << s.y << " ) " << endl;
                    
                    def = def + " ( " + to_string(prev.x) + " " + to_string(prev.y) + " ) ";
                    def = def + " ( " + to_string(i-1) + " " + to_string(s.y) + " ) ";
                    newSource.x = i-1;
                    newSource.y = s.y;
                    newSource.z = s.z;
                    return newSource;
                }
            }
        }
        else {
            for (int i = s.x - 1; i >= t.x; i--){ //iterate vertically from source to target
                if (l[i][s.y] == 0){ //if cell is empty
                    int j = s.y;
                    if(s.y % ratio == 0){
                        int index = 0;
                        while(index < ratio && j+index<y){
                            l[i][j+index] = route;
                            index++;
                        }
                    }
                }
                else{ //else return
//                    cout <<" (" << i+1 << " " << s.y << " ) " << endl;
                    
                    def = def + " ( " + to_string(prev.x) + " " + to_string(prev.y) + " ) ";
                    def = def + " ( " + to_string(i+1) + " " + to_string(s.y) + " ) ";
                    newSource.x = i+1;
                    newSource.y = s.y;
                    newSource.z = s.z;
                    return newSource;
                }
            }
        }
//        cout <<" (" << t.x << " " << s.y << " ) " << endl;
        
        def = def + " ( " + to_string(prev.x) + " " + to_string(prev.y) + " ) ";
        def = def + " ( " + to_string(t.x) + " " + to_string(s.y) + " ) ";
        newSource.x = t.x; //t.x is reached
        newSource.y = s.y; //same y as s
        newSource.z = s.z; //same z as s
    }
    //horizontal
    else{
        if (isSource){ //is s == initial source
//            l[s.x][s.y] = route;
            int i = s.x;
            if(i % ratio == 0){
                int index = 0;
                while(index < ratio && i+index<x){
                    l[i+index][s.y] = route;
                    index++;
                }
//                def = def + " ( " + to_string(s.x) + " " + to_string(s.y) + " ) ";
                prev.x = s.x;
                prev.y = s.y;
            }
        }
        else if ((s.x == t.x) && (s.y == t.y) && (s.z == t.z)){ //s is target
//            l[s.x][s.y] = route;
            int i = s.x;
            if(i % ratio == 0){
                int index = 0;
                while(index < ratio && i+index<x){
                    l[i+index][s.y] = route;
                    index++;
                }
//                def = def + " ( " + to_string(s.x) + " " + to_string(s.y) + " ) ";
                prev.x = s.x;
                prev.y = s.y;
            }
            return t;
        }
        else if (l[s.x][s.y] == 0){
//            l[s.x][s.y] = route;
            int i = s.x;
            if(i % ratio == 0){
                int index = 0;
                while(index < ratio && i+index<x){
                    l[i+index][s.y] = route;
                    index++;
                }
//                def = def + " ( " + to_string(s.x) + " " + to_string(s.y) + " ) ";
                prev.x = s.x;
                prev.y = s.y;
            }
        }
        else
            return s; //is s is not initial source and it is visited
        if (t.y > s.y){
            for (int j = s.y + 1; j <= t.y; j++){ //iterate horizontally from source to target
                if (l[s.x][j] == 0){ //if cell is empty
//                    l[s.x][j] = route; //visit cell
                    int i = s.x;
                    if(i % ratio == 0){
                        int index = 0;
                        while(index < ratio && i+index<x){
                            l[i+index][j] = route;
                            index++;
                        }
                    }
                }
                else{ //else return
                    def = def + " ( " + to_string(prev.x) + " " + to_string(prev.y) + " ) ";
                    def = def + " ( " + to_string(s.x) + " " + to_string(j-1) + " ) ";
                    newSource.x = s.x;
                    newSource.y = j-1;
                    newSource.z = s.z;
                    return newSource;
                }
            }
        }
        else {
            for (int j = s.y - 1; j >= t.y; j--){ //iterate horizontally from source to target
                if (l[s.x][j] == 0){ //if cell is empty
//                    l[s.x][j] = route; //visit cell
                    int i = s.x;
                    if(i % ratio == 0){
                        int index = 0;
                        while(index < ratio && i+index<x){
                            l[i+index][j] = route;
                            index++;
                        }
                    }
                }
                else{ //else return
                    def = def + " ( " + to_string(prev.x) + " " + to_string(prev.y) + " ) ";
                    def = def + " ( " + to_string(s.x) + " " + to_string(j+1) + " ) ";
                    newSource.x = s.x;
                    newSource.x = s.x;
                    newSource.y = j+1;
                    newSource.z = s.z;
                    return newSource;
                }
            }
        }
        
        def = def + " ( " + to_string(prev.x) + " " + to_string(prev.y) + " ) ";
        def = def + " ( " + to_string(s.x) + " " + to_string(t.y) + " ) ";
        newSource.x = s.x; //iteration was only horizontal so same x as s
        newSource.y = t.y; // y of target is reached
        newSource.z = s.z; //iteration was only horizontal so same z as s
    }
    return newSource;
}
bool flood(vector<layer> &manyLayers, int x, int y, int m, coord newSource, coord target, int via, int count0, bool isSource){
    int count = count0; //initial count
    
    if(manyLayers[newSource.z].dir == 0) // vertical
        newSource.y -= (newSource.y % manyLayers[newSource.z].ratio);
    else
        newSource.x -= (newSource.x % manyLayers[newSource.z].ratio);
    
    if(manyLayers[target.z].dir == 0) // vertical
        target.y -= (target.y % manyLayers[target.z].ratio);
    else
        target.x -= (target.x % manyLayers[target.z].ratio);
    
    if (isSource)
        manyLayers[newSource.z].l[newSource.x][newSource.y] = count;
    else if (manyLayers[newSource.z].l[newSource.x][newSource.y] == 0 ||  manyLayers[newSource.z].l[newSource.x][newSource.y] == route)
        manyLayers[newSource.z].l[newSource.x][newSource.y] = count;
    else
        return false;
    
    //for optimization (not implemented)
    int imin, imax, jmin, jmax;
    imin = imax = newSource.x;
    jmin = jmax = newSource.y;
    
    while(count < x * y * via * m){
        for (int i = 0; i< x; i++){
            for (int j = 0; j< y; j++){
                for (int k = 0; k < m; k++){
                    if(manyLayers[k].l[i][j] == count){
                        
                        //flooding on different metal layer.....
                        if((i == target.x) && (j == target.y) && (target.z == k+1)){
                            //check if cell above (vertical) is target
                            if(manyLayers[k+1].dir == 0){ //vertical
                                if(j % manyLayers[k+1].ratio == 0){
                                    int index = 0;
                                    while(index < manyLayers[k+1].ratio && j+index<y){
                                        manyLayers[k+1].l[i][j+index] = count + via;
                                        index++;
                                    }
                                }
                            }
                            //check if cell above (horizontal) is target
                            if(manyLayers[k+1].dir == 1){ //horizontal
                                if(i % manyLayers[k+1].ratio == 0){
                                    int index = 0;
                                    while(index < manyLayers[k+1].ratio && i+index<x){
                                        manyLayers[k+1].l[i+index][j] = count + via;
                                        index++;
                                    }
                                }
                            }
                            return true; //path found
                        }
                        if((i == target.x) && (j == target.y) && (target.z == k - 1)){
                            //check is cell below (vertical) is target
                            if(manyLayers[k-1].dir == 0){ //vertical
                                if(j % manyLayers[k-1].ratio == 0){
                                    int index = 0;
                                    while(index < manyLayers[k-1].ratio && j+index<y){
                                        manyLayers[k-1].l[i][j+index] = count + via;
                                        index++;
                                    }
                                }
                            }
                            //check is cell below (horizontal) is target
                            else if(manyLayers[k-1].dir == 1){ //horizontal
                                if(i % manyLayers[k-1].ratio == 0){
                                    int index = 0;
                                    while(index < manyLayers[k-1].ratio && i+index<x){
                                        manyLayers[k-1].l[i+index][j] = count + via;
                                        index++;
                                    }
                                }
                            }
                            return true; //path found
                        }
                        if (k-1 >= 0)//check if cell below is empty
                            if(manyLayers[k-1].l[i][j] == 0){
                                
                                //cell below is on vertical layer
                                if(manyLayers[k-1].dir == 0){ //vertical
                                    if(j % manyLayers[k-1].ratio == 0){
                                        int index = 0;
                                        while(index < manyLayers[k-1].ratio && j+index<y){
                                            manyLayers[k-1].l[i][j+index] = count + via;
                                            index++;
                                        }
                                    }
                                }
                                //cell below is on horizontal layer
                                else if(manyLayers[k-1].dir == 1){ //horizontal
                                    if(j % manyLayers[k-1].ratio == 0){
                                        int index = 0;
                                        while(index < manyLayers[k-1].ratio && i+index<x){
                                            manyLayers[k-1].l[i+index][j] = count + via;
                                            index++;
                                        }
                                    }
                                }
                            }
                        if (k+1 < m){//check if cell above is empty
                            if(manyLayers[k+1].l[i][j] == 0){
                                if(manyLayers[k+1].dir == 0){ //vertical
                                    if(j % manyLayers[k+1].ratio == 0){
                                        int index = 0;
                                        while(index < manyLayers[k+1].ratio && j+index<y){
                                            manyLayers[k+1].l[i][j+index] = count + via;
                                            index++;
                                        }
                                    }
                                }
                                else if(manyLayers[k+1].dir == 1){ //horizontal
                                    if(i % manyLayers[k+1].ratio == 0){
                                        int index = 0;
                                        while(index < manyLayers[k+1].ratio && i+index<x){
                                            manyLayers[k+1].l[i+index][j] = count + via;
                                            index++;
                                        }
                                    }
                                }
                            }
                        }
                        
                        //flooding on same metal layer
                        if(manyLayers[k].dir == 1){ //horizontal
                            if((i == target.x) && ((j+1) == target.y) && (target.z == k)){ //check if cell to the right is target
//                                manyLayers[k].l[i][j+1] = count+ 1;
                                
                                if(i % manyLayers[k].ratio == 0){
                                    int index = 0;
                                    while(index < manyLayers[k].ratio && i+index<x){
                                       manyLayers[k].l[i+index][j+1] = count + 1;
                                        index++;
                                    }
                                }
                                
                                if ((j+1) > jmax)
                                    jmax = j+1;
                                return true; //path found
                            }
                            if((j+1<y) && ((manyLayers[k].l[i][j+1] == 0))){ //check if cell to the right is empty
                                if ((j+1) > jmax)
                                    jmax = j+1;
                                
                                if(i % manyLayers[k].ratio == 0){
                                    int index = 0;
                                    while(index < manyLayers[k].ratio && i+index<x){
                                        manyLayers[k].l[i+index][j+1] = count + 1;
                                        index++;
                                    }
                                }
                            }
                            
                            if((i == target.x) && ((j-1) == target.y) && (target.z == k)){ //check if cell to the left is target
                                
                                if(i % manyLayers[k].ratio == 0){
                                    int index = 0;
                                    while(index < manyLayers[k].ratio && i+index<x){
                                        manyLayers[k].l[i+index][j-1] = count + 1;
                                        index++;
                                    }
                                
                                jmin = j-1;
                                return true; //path found
                            }
                            }
                            if((j-1>=0) && ((manyLayers[k].l[i][j-1] == 0))){  //check if cell to the left is empty
                                jmin = j-1;
                                
                                if(i % manyLayers[k].ratio == 0){
                                    int index = 0;
                                    while(index < manyLayers[k].ratio && i+index<x){
                                        manyLayers[k].l[i+index][j-1] = count + 1;
                                        index++;
                                    }
                                 }
        
                            }
                        }
                        if(manyLayers[k].dir == 0){ //vertical
                           
                            if((j == target.y) && ((i+1) == target.x) && (target.z == k)){ //check if cell upwards (in same metal layer) is target
                                
                                if(j % manyLayers[k].ratio == 0){
                                    int index = 0;
                                    while(index < manyLayers[k].ratio && j+index<x){
                                        manyLayers[k].l[i+1][j+index] = count + 1;
                                        index++;
                                    }
                                }
                                
                                if ((i+1) > imax)
                                    imax = i + 1;
                                return true; //path found
                            }
                            if((i+1<x) && ((manyLayers[k].l[i+1][j] == 0))){ //check if cell upwards (in same metal layer) is empty
                                if ((i+1) > imax)
                                    imax = i + 1;
                                
                                if(j % manyLayers[k].ratio == 0){
                                    int index = 0;
                                    while(index < manyLayers[k].ratio && j+index<x){
                                        manyLayers[k].l[i+1][j+index] = count + 1;
                                        index++;
                                    }
                                }
                            }
                            if((j == target.y) && ((i-1) == target.x) && (target.z == k)){ //check if cell downwards (in same metal layer) is target
                                imin = i-1;
//                                manyLayers[k].l[i-1][j] = count+1;
                                
                                if(j % manyLayers[k].ratio == 0){
                                    int index = 0;
                                    while(index < manyLayers[k].ratio && j+index<x){
                                        manyLayers[k].l[i-1][j+index] = count + 1;
                                        index++;
                                    }
                                }
                                
                                return true; //path found
                            }
                            if((i-1>=0) && ((manyLayers[k].l[i-1][j] == 0))){ //check is cell downwards (in same metal layer) is empty
                                imin = i-1;
                                
                                if(j % manyLayers[k].ratio == 0){
                                    int index = 0;
                                    while(index < manyLayers[k].ratio && j+index<x){
                                        manyLayers[k].l[i-1][j+index] = count + 1;
                                        index++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        count++;
    }
    return false; //path not found
}
bool backTracking(vector<layer> &manyLayers, int x, int y, int m, coord source, coord target, int via, coord source1, string def, vector<string> &defs){
    int count = 0; //count we're looking for
    count = manyLayers[target.z].l[target.x][target.y];
    manyLayers[target.z].l[target.x][target.y] = -5;
    //start of route
    
    if (count <= 0) //did not reach target and there is no route
        return false;
    
    int i = target.x;
    int j = target.y;
    int k = target.z;
    
    coord prev;
//    cout << "ROUTED metal" << k << " ( " << i << " " << j << " )" ;
    
//    cout << "Routed metal" << k ;
//    def = def + to_string(k) + " ( " + to_string(i) + " " + to_string(j) + " ) ";
    def = def + to_string(k + 1);
    prev.x = i;
    prev.y = j;
    while(i!=source.x | j!=source.y | k!=source.z){
        //horizontal
        if(manyLayers[k].dir == 1){
            i -= (i%manyLayers[k].ratio);
            if ((j-1 >= 0) && (manyLayers[k].l[i][j-1] == route)){
                manyLayers[source.z].l[source.x][source.y] = s_value;
                return true;
            }
            else if((j+1 < y) && (manyLayers[k].l[i][j+1] == route)){
                manyLayers[source.z].l[source.x][source.y] = s_value;
                return true;
            }
            else {
                if(k+1 < m){
                    if(manyLayers[k+1].l[i][j] == route){
                        manyLayers[source.z].l[source.x][source.y] = s_value;
                        return true;
                    }
                }
                if (k-1 >= 0){
                    if(manyLayers[k-1].l[i][j] == route){
                        manyLayers[source.z].l[source.x][source.y] = s_value;
                        return true;
                    }
                }
            }
            if((j-1 >= 0) && ((manyLayers[k].l[i][j-1] == (count - 1)) || (manyLayers[k].l[i][j-1] == count )) && (manyLayers[k].l[i][j-1] >= 0)){ //checking if next cell in route is left cell
                if (manyLayers[k].l[i][j-1] != count )
                    count--;
                manyLayers[k].l[i][j-1] = -5;
                
                j--;
            }
            else if ((j+1 < y) && ((manyLayers[k].l[i][j+1] == (count - 1)) ||(manyLayers[k].l[i][j+1] == count)) && (manyLayers[k].l[i][j+1] >= 0)){ //checking if next cell in route is right cell
                if(manyLayers[k].l[i][j+1] != count)
                    count--;
                manyLayers[k].l[i][j+1] = -5;
                j++;
            }
            else
            {
                if(k+1 < m){
                    if(manyLayers[k+1].l[i][j] == (count-via) && (manyLayers[k+1].l[i][j] >= 0)){ //checking if next cell in route is cell above
                        k++;
                        count -= via;
                        manyLayers[k].l[i][j] = -5;
                        manyLayers[k-1].l[i][j] = -5;
                        vias ++;
//                        cout << " ( " << i << " " << j << " ) M" << k-1 << "_M" << k << endl;
//                        cout << "NEW metal" << k;
//                        def = def + " ( " + to_string(i) + " " + to_string(j) + " ) ( " + to_string(prev.x) + " " + to_string(prev.y) + " ) M" + to_string(k) + "_M" + to_string(k-1) + "\n";
                        def = def + " ( " + to_string(prev.x) + " " + to_string(prev.y) + " ) ( " + to_string(i) + " " + to_string(j) + " ) M" + to_string(k-1 + 1) + "_M" + to_string(k + 1);
                        defs.push_back(def);
//                        def = "NEW metal" + to_string(k) + " ( " + to_string(i) + " " + to_string(j) + " ) ";
                        def = "NEW metal" + to_string(k + 1);
                        prev.x = i;
                        prev.y = j;
                        //leaving metal layer
                        continue;
                    }
                }
                if (k-1 >= 0){
                    if(manyLayers[k-1].l[i][j] == (count-via) && (manyLayers[k-1].l[i][j] >= 0)){ //checking if next cell in route is cell above
                        k--;
                        count -= via;
                        manyLayers[k].l[i][j] = -5;
                        manyLayers[k+1].l[i][j] = -5;
                        vias ++;
//                        cout << " ( " << i << " " << j << " ) M" << k+1 << "_M" << k << endl;
//                        cout << "NEW metal" << k;
                        
//                        def = def + " ( " + to_string(i) + " " + to_string(j) + " ) ( " + to_string(prev.x) + " " + to_string(prev.y) + " ) M" + to_string(k) + "_M" + to_string(k+1) + "\n";
                        def = def + " ( " + to_string(prev.x) + " " + to_string(prev.y) + " ) ( " + to_string(i) + " " + to_string(j) + " ) M" + to_string(k+1 + 1) + "_M" + to_string(k + 1);
//                        def = def + " ( " + to_string(i) + " " + to_string(j) + " ) M" + to_string(k+1) + "_M" + to_string(k) + "\n";
                        defs.push_back(def);
//                        def = "NEW metal" + to_string(k) + " ( " + to_string(i) + " " + to_string(j) + " ) ";
                        def = "NEW metal" + to_string(k + 1);
                        prev.x = i;
                        prev.y = j;
                        //leaving metal layer
                        continue;
                    }
                }
            }
        }
        //vertical
        else {
            
            j -= (j%manyLayers[k].ratio);
            if((i-1 >= 0) && (manyLayers[k].l[i-1][j] == route)){
                manyLayers[source.z].l[source.x][source.y] = s_value;
                return true;
            }
            else if ((i+1 < x) && (manyLayers[k].l[i+1][j] == route)){
                manyLayers[source.z].l[source.x][source.y] = s_value;
                return true;
            }
            else{
                if(k+1 < m){
                    if (manyLayers[k+1].l[i][j] == route){
                        manyLayers[source.z].l[source.x][source.y] = s_value;
                        return true;
                    }
                }
                if(k-1 >= 0){
                    if (manyLayers[k-1].l[i][j] == route){
                        manyLayers[source.z].l[source.x][source.y] = s_value;
                        return true;
                    }
                }
            }
                
            
            if((i-1 >= 0) && ((manyLayers[k].l[i-1][j] == (count - 1)) || (manyLayers[k].l[i-1][j] == count)) && (manyLayers[k].l[i-1][j] >= 0)){ //checking if next cell in route is cell downwards (in same metal layer)
                if(manyLayers[k].l[i-1][j] != count)
                    count--;
                manyLayers[k].l[i-1][j] = -5;
                i--;
            }
            else if ((i+1 < x) && ((manyLayers[k].l[i+1][j] == (count - 1)) || (manyLayers[k].l[i+1][j] == count)) && (manyLayers[k].l[i+1][j] >= 0)){ //checking if next cell in route is cell upwards (in same metal layer)
                if(manyLayers[k].l[i+1][j] != count)
                    count--;
                manyLayers[k].l[i+1][j] = -5;
                i++;
            }
            else{
                if(k+1 < m){
                    if (manyLayers[k+1].l[i][j] == (count-via) && (manyLayers[k+1].l[i][j] >= 0)){ //checking if next cell in route is cell above
                        k++;
                        count-=via;
                        manyLayers[k].l[i][j] = -5;
                        manyLayers[k-1].l[i][j] = -5;
                        vias ++;
//                        cout << " ( " << i << " " << j << " ) M" << k-1 << "_M" << k << endl;
//                        cout << "NEW metal" << k;
                        
                        
//                        def = def + " ( " + to_string(i) + " " + to_string(j) + " ) ( " + to_string(prev.x) + " " + to_string(prev.y) + " ) M" + to_string(k) + "_M" + to_string(k-1) + "\n";
                        
                        def = def + " ( " + to_string(prev.x) + " " + to_string(prev.y) + " ) ( " + to_string(i) + " " + to_string(j) + " ) M" + to_string(k-1 + 1) + "_M" + to_string(k + 1);
                        
//                        def = def + " ( " + to_string(i) + " " + to_string(j) + " ) M" + to_string(k-1) + "_M" + to_string(k) + "\n";
                        defs.push_back(def);
//                        def = "NEW metal" + to_string(k) + " ( " + to_string(i) + " " + to_string(j) + " ) ";
                        def = "NEW metal" + to_string(k + 1);
                        prev.x = i;
                        prev.y = j;
                        //leaving metal layer
                    }
                }
                if(k-1 >= 0){
                    if(manyLayers[k-1].l[i][j] == (count-via) && (manyLayers[k-1].l[i][j] >= 0)){ //checking if next cell in route is cell below
                        k--;
                        manyLayers[k].l[i][j] = -5;
                        manyLayers[k+1].l[i][j] = -5;
                        count-=via;
                        vias ++;
//                        cout << " ( " << i << " " << j << " ) M" << k+1 << "_M" << k << endl;
//                        cout << "NEW metal" << k;
                        
//                        def = def + " ( " + to_string(i) + " " + to_string(j) + " ) ( " + to_string(prev.x) + " " + to_string(prev.y) + " ) M" + to_string(k) + "_M" + to_string(k+1) + "\n";
                        
                        def = def + " ( " + to_string(prev.x) + " " + to_string(prev.y) + " ) ( " + to_string(i) + " " + to_string(j) + " ) M" + to_string(k+1 + 1) + "_M" + to_string(k + 1);
                        
//                        def = def + " ( " + to_string(i) + " " + to_string(j) + " ) M" + to_string(k+1) + "_M" + to_string(k) + "\n";
                        defs.push_back(def);
//                        def = "NEW metal" + to_string(k) + " ( " + to_string(i) + " " + to_string(j) + " ) ";
                        def = "NEW metal" + to_string(k + 1);
                        prev.x = i;
                        prev.y = j;
                        //leaving metal layer
                    }
                    
                }
            }
        }
    }
//    cout << " ( " << i << " " << j << " ) ;" << endl;
    //source found
    return true;
}
void backToLife(vector<layer> &manyLayers, int x, int y, int m){
    for (int i=0; i<x; i++){
        for(int j=0; j<y; j++){
            for(int k=0; k<m; k++){
                // emptying flooded cells that are not part of route
                if(manyLayers[k].l[i][j] > 0)
                    manyLayers[k].l[i][j] = 0;
                
                if(manyLayers[k].l[i][j] == -5){
                    if(manyLayers[k].dir == 0){ //vertical
                        j -= (j % manyLayers[k].ratio);
                        int index = 0;
                        while(index < manyLayers[k].ratio && j+index<x){
                            manyLayers[k].l[i][j+index] = route;
                            index++;
                        }
                    }
                    else if (manyLayers[k].dir == 1){ //horizontal
                        i -= (i % manyLayers[k].ratio);
                        int index = 0;
                        while(index < manyLayers[k].ratio && j+index<x){
                            manyLayers[k].l[i+index][j] = route;
                            index++;
                        }
                    }
                }
                
                //getting # of cells
                if (manyLayers[k].l[i][j] == route)
                    cells++;
            }
        }
    }
}
bool classicalImplementation(vector<layer> &manyLayers, int x, int y, int m, coord source, coord target, int via, bool floodLessB, vector <string> &defNets, string metalO, ofstream &out){
    coord newSource;
    bool isSource = true; //source is same as newSource
    cells = 0;
    s_value = manyLayers[source.z].l[source.x][source.y];
    string def;
//    def = "NEW metal";
//    def = "ROUTED metal";
    vector<string> defs2;
    if(!floodLessB){
        newSource = traverse(manyLayers[source.z].l, manyLayers[source.z].dir, manyLayers[source.z].ratio, x, y, source, target, isSource, def);
        defs2.push_back(def);
    }
//    cout << def;
    t_value = manyLayers[target.z].l[target.x][target.y];

    if(floodLessB){
        newSource = floodLess(manyLayers, x, y, m, source, target, defs2);
    }
    int count0 = 1; //initial count for cell in flooding
    
    //if target is reached
    if ((newSource.x == target.x) && (newSource.y == target.y) && (newSource.z == target.z)){
        for(int i=0; i<defNets.size(); i++){
//            cout << defNets[i] << endl;
            out << defNets[i] << endl;
        }
        defNets.erase(defNets.begin(), defNets.end());
//        cout << metalO << defs2[0] << endl;
        if (defs2.size() > 0)
            out << metalO << defs2[0] << endl;
        for (int i=1; i<defs2.size()-1; i++){
//            cout << defs2[i] << endl;
            out << defs2[i] << endl;
        }
//        cout << defs2[defs2.size()-1] ;
        if (defs2.size() > 1)
            out << defs2[defs2.size()-1] ;
        
        printMatrices(manyLayers, x, y, m);
        cells = countCells(manyLayers, x, y, m);
        cout << "Cells = " << cells << endl;
        cout << "Vias = " << vias << endl;
        cout << "Cost = " << (cells + vias * via) << endl;
        return true;
    }
    else {
        cells = 0; //reset cells count
        if ((source.x != newSource.x) && (source.y != newSource.y) && (source.z != newSource.z))
            isSource = false;
    }
//    vector<coord> changed;
//    printMatrices(manyLayers, x, y, m);
    flood(manyLayers, x, y, m, newSource, target, via, count0, isSource);
//    printMatrices(manyLayers, x, y, m);
    vector <string> defs;
    if (backTracking(manyLayers, x, y, m, newSource, target, via, source, metalO, defs)){ //get route
//        changed.erase(changed.begin(), changed.end());
        for(int i=0; i<defNets.size(); i++){
//            cout << defNets[i] << endl;
            out << defNets[i] << endl;
        }
        defNets.erase(defNets.begin(), defNets.end());
        for (int i=0; i<defs.size(); i++){
//            cout << defs[i] << endl;
            out << defs[i] << endl;
        }
        for (int i=defs2.size()-1; i>=1; i--){
//            cout << defs2[i] << endl;
            out << defs2[i] << endl;
        }
//        cout << metalO << defs2[0];
        if (defs2.size() > 0)
            out << metalO << defs2[0];
//        for (int i=defs2.size()-1; i>=0; i--)
//            cout << defs2[i];
        cells = 0;
        backToLife(manyLayers, x, y, m); //empty flooded, non-routed cells
        cout << "BACKTRACKING" << endl;
        printMatrices(manyLayers, x, y, m);
        cout << "Cells = " << cells << endl;
        cout << "Vias = " << vias << endl;
        cout << "Cost = " << (cells + vias * via) << endl;
        return true;
    } else {
        backToLife(manyLayers, x, y, m); //empty flooded cells
        cells = 0;

        undoTraversal(manyLayers[source.z].l, manyLayers[source.z].dir, x, y, source, newSource);
        manyLayers[target.z].l[target.x][target.y] = t_value;
        cout << "No Path available" << endl;
        route++;
        printMatrices(manyLayers, x, y, m);
        return false;
    }
    
    return false;
}
void undoTraversal(vector<vector<int>> &l, int dir, int x, int y, coord s, coord t){
    if (dir  == 0){ //vertical
        if (t.x > s.x){
            for (int i = s.x + 1; i<=t.x; i++){ //vertical
                if (l[i][s.y] == route){ //if cell was part of route
                    l[i][s.y] = 0; //empty cell
                }
            }
        }
        else { //vertical
            for (int i = s.x; i>=t.x; i--){
                if (l[i][s.y] == route){ //if cell was part of route
                    l[i][s.y] = 0; //empty cell
                }
            }
        }
    }
    //horizontal
    else{
        if (t.y > s.y){
            for (int j = s.y + 1; j <= t.y; j++){
                if (l[s.x][j] == route){ //if cell was part of route
                    l[s.x][j] = 0; //empty cell
                }
            }
        }
        else {
            for (int j = s.y; j >= t.y; j--){
                if (l[s.x][j] == route){ //if cell was part of route
                    l[s.x][j] = 0; //empty cell
                }
            }
        }
    }
    l[s.x][s.y] = s_value; //set source to value before filling (might not be zero)
}
coord floodLess(vector<layer> &manyLayers, int x, int y, int m, coord newSource, coord target, vector <string> &defs){
    int count = 0;
    coord newSource1; //return of traverse
    coord newSource2 = newSource; //parameter of traverse
    coord newSourceBuffer; //to save last value of newSource1
    newSourceBuffer = newSource; //initially it is set to newSource
    int s_value;
    string def;
//    def = "ROUTED metal";
    while (count < m){
        //check is initial newSource is the same as newSource2
        bool isSource = (newSource2.x == newSource.x) && (newSource2.y == newSource.y) && (newSource2.z == newSource.z);
        s_value = manyLayers[newSource2.z].l[newSource2.x][newSource2.y];
        newSource1 = traverse(manyLayers[newSource2.z].l, manyLayers[newSource2.z].dir, manyLayers[newSource2.z].ratio, x, y, newSource2, target, isSource, def);
        if ((newSource1.x == newSource2.x) && (newSource1.y == newSource2.y) && (newSource1.z == newSource2.z)){ //no traverse
            if ((newSource1.x == target.x) && (newSource1.y == target.y) && (newSource1.z == target.z)){ //target reached
                def = def + " ( " + to_string(target.x) + " " + to_string(target.y) + " ) ";
//                cout << def;
                defs.push_back(def);
                return target;
            }
            if(newSourceBuffer.z != newSource1.z) //if no traverse, reduce vias
                vias--;
            else
//                cout << def;
                defs.push_back(def);
            manyLayers[newSource1.z].l[newSource1.x][newSource1.y] = s_value;
            return newSourceBuffer; //return saved newSource1 value
        }
        else {
            newSource2 = newSource1;
        }
        
        if ((newSource2.x == target.x) && (newSource2.y == target.y) && (newSource2.z == target.z)){ //target reached
//            cout << def;
            defs.push_back(def);
            return target;
        }
        else {
            if(newSource2.z < target.z){ //z of target is greater
                
                def = def + " M" + to_string(newSource2.z + 1) + "_M";
                int tempz = newSource2.z;
                newSource2.z = ((newSource2.z) % m ) + 1; //increment z sent to traverse
                vias++;
                def = def + to_string(newSource2.z + 1);
                defs.push_back(def);
//                cout << def;
            }
            else if (newSource2.z > target.z){ //z of target is less
                newSource2.z = ((newSource2.z - 1) % m); //decrement z sent to traverse
                vias++;
//                cout << def;
            }
            else if ((newSource2.z == target.z) && (manyLayers[target.z].dir == 1) && (newSource2.x != target.x)){ //horizontal layer and there is a difference vertically
                int tempz = newSource2.z;
//                def = def + " M" + to_string(newSource2.z) + "_M";
                newSource2.z = ((newSource2.z +1) % m );
//                def = def + to_string(newSource2.z) + "\n";
                def = def + " M" + to_string(newSource2.z + 1) + "_M";
                def = def + to_string(tempz + 1);
                defs.push_back(def);
                vias++;
//                cout << def;
            }
            else if ((newSource2.z == target.z) && (manyLayers[target.z].dir == 0) && (newSource2.y != target.y)){ //vertical layer and there is a difference horizontally
                int tempz = newSource2.z;
//                def = def + to_string(newSource2.z) + "\n";
                newSource2.z = ((newSource2.z +1) % m);
                def = def + " M" + to_string(newSource2.z + 1) + "_M";
                def = def + to_string(tempz + 1);
                defs.push_back(def);
//                def = def + to_string(newSource2.z) + "\n";
                vias++;
//                cout << def;
            }
        }
        newSourceBuffer = newSource1; //update buffer
        count ++;
        def = "NEW metal";
    }
    return newSource1;
}
void multiPins(vector<layer> &manyLayers, int x, int y, int m, vector<coord> target, int via, bool floodLessB, vector <string> &defNets, ofstream &out, int netCount){
    int routeNo = route;
    int i = 1;
    for (int i=0; i< target.size(); i++){
        if (manyLayers[target[i].z].dir == 0){ //vertical
            target[i].y -= target[i].y % manyLayers[target[i].z].ratio;
        }
        else{ //horizontal
            target[i].x -= target[i].x % manyLayers[target[i].z].ratio;
        }
    }
    out << "_" << netCount << endl;
    while(!classicalImplementation(manyLayers, x, y, m, target[0], target[1], via, floodLessB, defNets, "ROUTED metal", out))
    {
        route--;
        coord temp = target[1];
        if (i >= (target.size()-1)){
            cout << "No route\n";
            return;
        }
        target[1] = target[i+1];
        target[i+1] = temp;
        i++;
    }
    out << endl;
    cout << endl;
    route = routeNo;
    for (int i=2; i<target.size(); i++){
        if (i > 2){
            out << endl;
            cout << endl;
        }
        vector<coord> coords;
        getCoords(manyLayers, x, y, m, routeNo, coords);
        coord targetPin = target[i];
        vector<pair<int,int>> targetsDist;
        getDistance(manyLayers[targetPin.z].dir, targetPin, coords, targetsDist);
        for(int i=0; i<targetsDist.size(); i++){
            cout << targetsDist[i].first << "\t\t" << coords[targetsDist[i].second].x << " " << coords[targetsDist[i].second].y << " " << coords[targetsDist[i].second].z <<endl;
        }
        int j = 0;
        if (targetPin.x == 0)
            cout << "hey" << endl;
        cout << "TARGET "<<targetPin.x << targetPin.y << targetPin.z << endl;
        while(!classicalImplementation(manyLayers, x, y, m, coords[targetsDist[j].second], targetPin, via, floodLessB, defNets, "NEW metal", out)){
            j++;
            route--;
            if (j>(targetsDist.size()-1)){
                cout << "No route\n";
                return;
            }
        }
    }
    out << " ;" << endl;
//    cout << " ;" << endl;
    
}

void getDistance(int dir, coord source, vector<coord> targets, vector<pair<int,int>> &targetsDist){
    for(int i = 0; i<targets.size(); i++){
        if(dir == 0){ //vertical
            int distance = abs(targets[i].x - source.x);
            targetsDist.push_back(make_pair(distance, i));
        }else{ //horizontal
            int distance = abs(targets[i].y - source.y);
            targetsDist.push_back(make_pair(distance, i));
        }
    }
    sort(targetsDist.begin(), targetsDist.end());
}
void getCoords(vector<layer> &manyLayers, int x, int y, int m, int route, vector<coord> &coords){
    for (int i=0; i<x; i++){
        for(int j=0; j<y; j++){
            for(int k = 0; k<m; k++){
                if(manyLayers[k].l[i][j] == route){
                    coord temp;
                    temp.x = i;
                    temp.y = j;
                    temp.z = k;
                    coords.push_back(temp);
                }
            }
        }
    }
}
int countCells(vector<layer> manyLayers, int x, int y, int m){
    int count = 0;
    for (int i=0; i<x; i++){
        for(int j=0; j<y; j++){
            for (int k=0; k<m; k++){
                if(manyLayers[k].l[i][j] == route)
                    count++;
            }
        }
    }
    return count;
}
void computeRatio(vector<layer> &manyLayers){
    bool isFirstV = true, isFirstH = true;
    float pitchV, pitchH;
    for (int i=0; i<manyLayers.size(); i++){
        if(manyLayers[i].dir == 0){ //vertical
            if(isFirstV){  //first vertical
                pitchV = manyLayers[i].pitch;
                isFirstV = false;
            }
            manyLayers[i].ratio = ceil(manyLayers[i].pitch / pitchV);
        }
        if (manyLayers[i].dir == 1){ //horizontal
            if (isFirstH){ //first horizontal
                pitchH = manyLayers[i].pitch;
                isFirstH = false;
            }
            manyLayers[i].ratio = ceil(manyLayers[i].pitch / pitchH);
        }
    }
}
void parseTechLef(string filename, map<int, int> &layerDirection, map<int, float> &layerPitch){
    ifstream inFile;
    inFile.open(filename);
    if(!inFile){
        cout << "Failure to open file" << endl;
        return;
    } else{
        string s;
        getline(inFile, s);
        while(!inFile.eof()){
            if (s.length() >= 5){
                string temp;
                temp = s.substr(0, 5);
                if (temp == "LAYER"){
                    temp = s.substr(6, 5);
                    if (temp == "metal"){
                        temp = s.substr(11);
                        int layerNum = stoi(temp);
                        getline(inFile, s);
                        temp = s.substr(0, 3);
                        while (temp != "END"){
                            temp = s.substr(2, 9);
                            if(temp == "DIRECTION"){
                                temp = s.substr(12);
                                int dir;
                                if (temp == "HORIZONTAL ;"){
                                    dir = 1;
                                    layerDirection[layerNum - 1] = dir;
                                }
                                else if (temp == "VERTICAL ;"){
                                    dir = 0;
                                    layerDirection[layerNum - 1] = dir;
                                }
                            }
                            temp = s.substr(2, 5);
                            if(temp == "PITCH"){
                                temp = s.substr(9);
                                size_t pos = temp.find(" ");
                                temp = s.substr(9, pos);
                                layerPitch[layerNum - 1] = stof(temp);
                            }
                            getline(inFile, s);
                            temp = s.substr(0, 3);
                        }
                    }
                    
                }
                
            }
            getline(inFile, s);
        }
    }
}
void parseInput(string filename, string outputName, vector<coord> &pins, vector<layer> &manyLayers, vector <string> &defNets, int x, int y, int m, int via, bool floodLessB){
    ofstream out;
    out.open(outputName, fstream::app);
    if (!out)
        cout << "failure to open output file" << endl;
    int netCount = 1;
    ifstream inFile;
    int width, length, nets=0;
    inFile.open(filename);
    if(!inFile){
        cout << "Failure to open file" << endl;
        return;
    } else{
        while(!inFile.eof()){
            string temp;
            getline(inFile, temp);
            if (temp == ";")
                nets++;
        }
        inFile.clear();
        inFile.seekg(0, ios::beg);
        out<< "nets "<< nets << " ;\n";
        string s;
        getline(inFile, s);
        while(!inFile.eof()){
            if (s == ";"){
                cout << "ha2olak" << endl;
                multiPins(manyLayers, x, y, m, pins, via, floodLessB, defNets, out, netCount);
                pins.erase(pins.begin(), pins.end());
                netCount++;
                getline(inFile, s);
                continue;
            }
            else {
                if (s == ""){
                    getline(inFile, s);
                    continue;
                }
                size_t pos = s.find(':');
                if (pos != NULL){
                    string def = s.substr(0,pos);
                    defNets.push_back(def);
                    string temp = s.substr(pos);
                    coord hehe;
                    string stemp = temp.substr(2);
                    size_t postemp = stemp.find(' ');
                    hehe.x = stoi(temp.substr(2,postemp));
                    
                    int index = 2 + postemp + 1;
                    stemp = temp.substr(index);
                    
                    postemp = stemp.find(' ');
                    hehe.y = stoi(temp.substr(index,postemp));
                    
                    index+= postemp + 1;
                    
                    stemp = temp.substr(index);
                    postemp = stemp.find(' ');
                    hehe.z = stoi(temp.substr(index,postemp));
                    pins.push_back(hehe);
                }
                getline(inFile, s);
            }
        }
        inFile.close();
        out.close();
    }
}
void parseInput2(string filename, int &width, int &length, vector <coord> &obstacles){
    ifstream inFile;
    inFile.open(filename);
    
    if(!inFile){
        cout << "Failure to open file" << endl;
        return;
    } else{
        string s;
        getline(inFile, s);
        while(!inFile.eof()){
            if (s[0] == 'W'){
                width = stoi(s.substr(3));
                cout << "Width is: " << width << endl;
                getline(inFile, s);
            }
            else if (s[0] == 'L'){
                length = stoi(s.substr(3));
                cout << "Length is: " << length << endl;
                getline(inFile, s);
            }
            else{
                coord temp;
                string stemp = s.substr(1);
                size_t pos = stemp.find(' ');
                temp.x = stoi(s.substr(1, pos));
                int index = 1 + pos + 1;
                stemp = s.substr(index);
                pos = stemp.find(' ');
                temp.y = stoi(s.substr(index, pos));
                index += pos + 1;
                stemp = s.substr(index);
                pos = stemp.find(' ');
                temp.z = stoi(s.substr(index, pos));
                obstacles.push_back(temp);
                getline(inFile, s);
            }
        }
        inFile.close();
    }
}

void obstructRoute(vector <layer> &manyLayers, int x, int y, int m, vector <coord> obstacles){
    for (int i=0; i<obstacles.size(); i++){
        manyLayers[obstacles[i].z].l[obstacles[i].x][obstacles[i].y] = -1;
    }
}
