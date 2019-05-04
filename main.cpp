//macros
#define m1 -1 //metal 1
#define m2 -2 //metal 2
#define m3 -3 //metal 3
#define v12 -4 //via m1-m2
#define v23 -5 //via m2-m3
#define v123 -6 //via m1-m2-m3

//includes
#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>
using namespace std;

//structs
struct coord{int x; int y; int z;};

//global variables
int cells = 0;
int vias;
int route = -10;
int s_value, t_value;

//function declarations
bool input(coord &source, coord &target, int x, int y); //takes in the input coordinates and validates input
void printMatrix (vector<vector<int>> m, int x, int y); //print a matrix
coord traverse(vector <vector<int>> &l,  int x, int y, coord s, coord t, bool isSource); //DFS part
bool flood(vector<vector<int>> &l1, vector<vector<int>> &l2, vector<vector<int>> &l3, int x, int y, coord newSource, coord target, int via, int count0, bool isSource); //BFS part
bool backTracking(vector<vector<int>> &l1, vector<vector<int>> &l2, vector<vector<int>> &l3, int x, int y, coord source, coord target, int via, coord source1); //To generate route
void backToLife(vector<vector<int>> &l1, vector<vector<int>> &l2, vector<vector<int>> &l3, int x, int y); //Erase flooding after route is complete
void undoTraversal(vector<vector<int>> &l, int x, int y, coord s, coord t); //remove route of traversal if no path is found
bool classicalImplementation(vector<vector<int>> &l1, vector<vector<int>> &l2, vector<vector<int>> &l3, int x, int y, coord source, coord target, int via, bool swapCoord, bool floodLessB); //full implementation
coord floodLess(vector<vector<int>> &l1, vector<vector<int>> &l2, vector<vector<int>> &l3, int x, int y, coord newSource, coord target); //DFS until flooding is needed
void multiPins(vector<vector<int>> &l1, vector<vector<int>> &l2, vector<vector<int>> &l3, int x, int y, coord source, vector<coord> target, int via);
void getDistance(coord source, vector<coord> targets, vector<pair<int,int>> &targetsDist);
void getCoords(vector<vector<int>> &l1, vector<vector<int>> &l2, vector<vector<int>> &l3, int x, int y, int route, vector<coord> &coords);

//main
int main(){

    int x = 11;
    int y = 13;
    int via = 10;
    coord source;
    source.x = 7;
    source.y = 1;
    source.z = 1;
    vector <coord> target;
    coord target1;
    
    target1.x = 9;
    target1.y = 7;
    target1.z = 3;
    target.push_back(target1);
    
    target1.x = 7;
    target1.y = 7;
    target1.z = 3;
    target.push_back(target1);
    
    target1.x = 4;
    target1.y = 4;
    target1.z = 1;
    target.push_back(target1);
    
    target1.x = 5;
    target1.y = 3;
    target1.z = 2;
    target.push_back(target1);
    
    target1.x = 10;
    target1.y = 12;
    target1.z = 3;
    target.push_back(target1);
    
//    target.push_back(target2);
//    target.push_back(target3);

    //initializations
    vector <int> rows(y);
    vector <vector <int>> l1(x,rows); //layer 1
    vector <vector <int>> l2(x,rows); //layer 2
    vector <vector <int>> l3(x,rows); //layer 3

    for (int i = 0; i<x; i++){
        for (int j = 0; j<y; j++){
            l1[i][j] = 0;
        }
    }

    l2 = l1;
    l3 = l1;

    multiPins(l1, l2, l3, x, y, source, target, via);

        return 0;
}

//function definitions
bool input(coord &source, coord &target, int x, int y){
    do{
        cout << "\nEnter x coordinate of source\n";
        cin >> source.x;
    } while((source.x >= 0) && (source.x >= x));
    
    if(source.x < 0 )
        return false;
    
    do {
        cout << "Enter y coordinate of source\n";
        cin >> source.y;
    } while ((source.y >= 0) && (source.y >= y));
    
    if(source.y < 0 )
        return false;
    
    do {
        cout << "Enter z coordinate of source\n";
        cin >> source.z;
    } while((source.z > 0) && (source.z != 1) && (source.z != 2) && (source.z != 3));
    
    if(source.z < 0 )
        return false;
    
    do{
        cout << "Enter x coordinate of target\n";
        cin >> target.x;
    } while((target.x >= 0) && (target.x >= x));
    
    if(target.x < 0 )
        return false;
    
    do {
        cout << "Enter y coordinate of target\n";
        cin >> target.y;
    } while ((target.y >= 0) && (target.y >= y));
    
    if(target.y < 0 )
        return false;
    
    do {
        cout << "Enter z coordinate of target\n";
        cin >> target.z;
    } while((target.z > 0) && (target.z != 1) && (target.z != 2) && (target.z != 3));
    
    if(target.z < 0 )
        return false;
    return true;
}

void printMatrix (vector<vector<int>> m, int x, int y){
    for (int i = 0; i<x; i++){
        for (int j = 0; j<y; j++){
            cout <<m[i][j] << "\t";
        }
        cout << endl;
    }
}

coord traverse(vector <vector<int>> &l, int x, int y, coord s, coord t, bool isSource){
    //vertical
    coord newSource;
    if (s.z  == 2){ //vertical
        if (isSource) //is s == initial source
            l[s.x][s.y] = route;
        else if ((s.x == t.x) && (s.y == t.y) && (s.z == t.z)){ //s is target
            l[s.x][s.y] = route;
            cells++;
            return t;
        }
        else if (l[s.x][s.y] == 0)
            l[s.x][s.y] = route;
        else //is s is not initial source and it is visited
            return s;
        cells ++; //increment cells (to account for s)
        if (t.x > s.x){
            for (int i = s.x + 1; i<=t.x; i++){ //iterate vertically from source to target
                if (l[i][s.y] == 0){ //if cell is empty
                    l[i][s.y] = route; //visit cell
                    cells ++;
                }
                else{ //else return
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
                    l[i][s.y] = route; //visit cell
                    cells ++;
                }
                else{ //else return
                    newSource.x = i+1;
                    newSource.y = s.y;
                    newSource.z = s.z;
                    return newSource;
                }
            }
        }
        newSource.x = t.x; //t.x is reached
        newSource.y = s.y; //same y as s
        newSource.z = s.z; //same z as s
    }
    //horizontal
    else{
        if (isSource) //is s == initial source
            l[s.x][s.y] = route;
        else if ((s.x == t.x) && (s.y == t.y) && (s.z == t.z)){ //s is target
            l[s.x][s.y] = route;
            cells++;
            return t;
        }
        else if (l[s.x][s.y] == 0)
            l[s.x][s.y] = route;
        else
            return s; //is s is not initial source and it is visited
        cells++; //increment cells (to account for s)
        if (t.y > s.y){
            for (int j = s.y + 1; j <= t.y; j++){ //iterate horizontally from source to target
                if (l[s.x][j] == 0){ //if cell is empty
                    l[s.x][j] = route; //visit cell
                    cells++;
                }
                else{ //else return
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
                    l[s.x][j] = route; //visit cell
                    cells++;
                }
                else{ //else return
                    newSource.x = s.x;
                    newSource.x = s.x;
                    newSource.y = j+1;
                    newSource.z = s.z;
                    return newSource;
                }
            }
        }
        newSource.x = s.x; //iteration was only horizontal so same x as s
        newSource.y = t.y; // y of target is reached
        newSource.z = s.z; //iteration was only horizontal so same z as s
    }
    return newSource;
}

bool flood(vector<vector<int>> &l1, vector<vector<int>> &l2, vector<vector<int>> &l3, int x, int y, coord newSource, coord target, int via, int count0, bool isSource){
    int count = count0; //initial count
    switch(newSource.z){
        case (1):{
            if (isSource) //if newSource is the initial source
                l1[newSource.x][newSource.y] = count; //set cell to initial count (even if it is not empty)
            else if (l1[newSource.x][newSource.y] == 0 || l1[newSource.x][newSource.y] == route) //else then cell must be empty
                l1[newSource.x][newSource.y] = count;
            else
                return false; //else there is no path
        }
            break;
        case (2):{
            if (isSource) //if newSource is the initial source
                l2[newSource.x][newSource.y] = count; //set cell to initial count (even if it is not empty)
            else if (l2[newSource.x][newSource.y] == 0 || l2[newSource.x][newSource.y] == route) //else then cell must be empty
                l2[newSource.x][newSource.y] = count;
            else
                return false; //else there is no path
        }
            break;
        case (3):{
            if (isSource) //if newSource is the initial source
                l3[newSource.x][newSource.y] = count; //set cell to initial count (even if it is not empty)
            else if (l3[newSource.x][newSource.y] == 0 || l3[newSource.x][newSource.y] == route) //else then cell must be empty
                l3[newSource.x][newSource.y] = count;
            else
                return false; //else there is no path
        }
            break;
    }
    //for optimization (not implemented)
    int imin, imax, jmin, jmax;
    imin = imax = newSource.x;
    jmin = jmax = newSource.y;
    
    while(count < x * y * via){
        //                for (int i= imin; i<= imax; i++){
        //                    for (int j = jmin; j <= jmax; j++){
        
        //     while (count < (x+y+2*via)){ //maximum count
        for (int i = 0; i<= imax; i++){
            for (int j = 0; j<= jmax; j++){
                if (l1[i][j] == count){ //check if cell in l1 == count
                    
                    if((i == target.x) && (j == target.y) && (target.z == 2)){ //check is cell above is target
                        l2[i][j] = count + via;
                        return true; //path found
                    }
                    if(l2[i][j] == 0){ //check if cell above is empty
                        l2[i][j] = count + via;
                    }
                    
                    if((i == target.x) && ((j+1) == target.y) && (target.z == 1)){ //check if cell to the right is target
                        l1[i][j+1] = count+1;
                        jmax = j+1;
                        return true; //path found
                    }
                    if((j+1<y) && (l1[i][j+1] == 0)){ //check if cell to the right is empty
                        jmax = j+1;
                        //                                i= imin;
                        l1[i][j+1] = count+1;
                    }
                    
                    if((i == target.x) && ((j-1) == target.y) && (target.z == 1)){ //check if cell to the left is target
                        l1[i][j-1] = count+1;
                        jmin = j-1;
                        return true; //path found
                    }
                    if((j-1>=0) && (l1[i][j-1] == 0)){  //check if cell to the left is empty
                        jmin = j-1;
                        //                                i = imin;
                        //                                j = jmin;
                        l1[i][j-1] = count+1;
                    }
                }
                
                
                if (l2[i][j] == count){  //check if cell in l2 == count
                    
                    if((j == target.y) && (i == target.x) && (target.z == 1)){ //check if cell below is target
                        l1[i][j] = count + via;
                        return true; //path found
                    }
                    if(l1[i][j] == 0){ //check if cell below is empty
                        l1[i][j] = count + via;
                        
                    }
                    
                    if((j == target.y) && (i == target.x) && (target.z == 3)){ //check if cell above is target
                        l3[i][j] = count + via;
                        return true; //path found
                    }
                    if(l3[i][j] == 0){ //check if cell above is empty
                        l3[i][j] = count + via;
                    }
                    
                    if((j == target.y) && ((i+1) == target.x) && (target.z == 2)){ //check if cell upwards (in same metal layer) is target
                        l2[i+1][j] = count+1;
                        imax = i + 1;
                        return true; //path found
                    }
                    if((i+1<x) && (l2[i+1][j] == 0)){ //check if cell upwards (in same metal layer) is empty
                        imax = i + 1;
                        //                                j = jmin;
                        l2[i+1][j] = count+1;
                    }
                    
                    if((j == target.y) && ((i-1) == target.x) && (target.z == 2)){ //check if cell downwards (in same metal layer) is target
                        imin = i-1;
                        l2[i-1][j] = count+1;
                        return true; //path found
                    }
                    if((i-1>=0) && (l2[i-1][j] == 0)){ //check is cell downwards (in same metal layer) is empty
                        imin = i-1;
                        //                                j = jmin;
                        //                                i = imin;
                        l2[i-1][j] = count+1;
                    }
                }
                
                if (l3[i][j] == count){ //check if cell in l3 == count
                    
                    if((i == target.x) && (j == target.y) && (target.z == 2)){ //check if cell below is target
                        l2[i][j] = count + via;
                        return true; //path found
                    }
                    if(l2[i][j] == 0){ //check if cell below is empty
                        l2[i][j] = count + via;
                    }
                    
                    if((i == target.x) && ((j+1) == target.y) && (target.z == 3)){ //check if cell to the right is target
                        l3[i][j+1] = count+1;
                        jmax = j + 1;
                        return true; //path found
                    }
                    if((j+1<y) && (l3[i][j+1] == 0)){ //check if cell to the right is empty
                        jmax = j + 1;
                        //                                i = imin;
                        l3[i][j+1] = count+1;
                    }
                    
                    if((i == target.x) && ((j-1) == target.y) && (target.z == 3)){ //check if cell to the left if target
                        l3[i][j-1] = count+1;
                        jmin = j-1;
                        return true; //path found
                    }
                    if((j-1>=0) && (l3[i][j-1] == 0)){ //check if cell to the left is empty
                        jmin = j-1;
                        //                                    i = imin;
                        //                                    j = jmin;
                        l3[i][j-1] = count+1;
                    }
                }
            }
        }
        count++;
        
    }
    return false; //path not found
}

bool backTracking(vector<vector<int>> &l1, vector<vector<int>> &l2, vector<vector<int>> &l3, int x, int y, coord source, coord target, int via, coord source1){
    int count = 0; //count we're looking for
    switch (target.z){
        case(1): {
            count = l1[target.x][target.y]; //initialize count with cost of target cell
            l1[target.x][target.y] = route; //set target cell to route
        }
            break;
        case(2): {
            count = l2[target.x][target.y]; //initialize count with cost of target cell
            l2[target.x][target.y] = route;  //set target cell to route
        }
            break;
        case(3): {
            count = l3[target.x][target.y]; //initialize count with cost of target cell
            l3[target.x][target.y] = route;  //set target cell to route
        }
            break;
    }
    
    if (count <= 0) //did not reach target and there is no route
        return false;
    int i = target.x;
    int j = target.y;
    int z = target.z;
    while(i!=source.x | j!=source.y | z!=source.z){
        //horizontal
        if(z==1){
            if((j-1 >= 0) && (l1[i][j-1] == (count - 1)) && (l1[i][j-1] >= 0)){ //checking if next cell in route is left cell
                l1[i][j-1] = route;
                j--;
                count--;
            }
            else if ((j+1 < y) && (l1[i][j+1] == (count - 1)) && (l1[i][j+1] >= 0)){ //checking if next cell in route is right cell
                l1[i][j+1] = route;
                j++;
                count--;
            }
            else if(l2[i][j] == (count-via) && (l2[i][j] >= 0)){ //checking if next cell in route is cell above
                z = 2;
                count -= via;
                l2[i][j] = route;
                l1[i][j] = route;
                //                l2[i][j] = v12;
                //                l1[i][j] = v12;
                vias ++;
            }
        }
        else if (z == 3){
            if((j-1 >= 0) && (l3[i][j-1] == (count - 1)) && (l3[i][j-1] >= 0)){ //checking if next cell in route is left cell
                l3[i][j-1] = route;
                j--;
                count--;
            }
            else if ((j+1 < y) && (l3[i][j+1] == (count - 1)) && (l3[i][j+1] >= 0)){ //checking if next cell in route is right cell
                l3[i][j+1] = route;
                j++;
                count--;
            }
            else if(l2[i][j] == (count-via) && (l2[i][j] >= 0)){ //checking if next cell in route is cell below
                z = 2;
                count-=via;
                //                l2[i][j] = v23;
                //                l3[i][j] = v23;
                l2[i][j] = route;
                l3[i][j] = route;
                vias ++;
            }
        }
        //vertical
        else {
            if((i-1 >= 0) && (l2[i-1][j] == (count - 1)) && (l2[i-1][j] >= 0)){ //checking if next cell in route is cell downwards (in same metal layer)
                l2[i-1][j] = route;
                i--;
                count--;
            }
            else if ((i+1 < x) && (l2[i+1][j] == (count - 1)) && (l2[i+1][j] >= 0)){ //checking if next cell in route is cell upwards (in same metal layer)
                l2[i+1][j] = route;
                i++;
                count--;
            }
            else if(l3[i][j] == (count-via) && (l3[i][j] >= 0)){ //checking if next cell in route is cell above
                z = 3;
                count-=via;
                //                l3[i][j] = v23;
                //                l2[i][j] = v23;
                l3[i][j] = route;
                l2[i][j] = route;
                vias ++;
            }
            else if(l1[i][j] == (count-via) && (l1[i][j] >= 0)){ //checking if next cell in route is cell below
                //                l1[i][j] = v12;
                //                l2[i][j] = v12;
                l1[i][j] = route;
                l2[i][j] = route;
                z = 1;
                count-=via;
                vias ++;
            }
        }
    }
    
    return true;
}

void backToLife(vector<vector<int>> &l1, vector<vector<int>> &l2, vector<vector<int>> &l3, int x, int y){
    for (int i=0; i<x; i++){
        for(int j=0; j<y; j++){
            // emptying flooded cells that are not part of route
            if(l1[i][j] > 0)
                l1[i][j] = 0;
            if(l2[i][j] > 0)
                l2[i][j] = 0;
            if(l3[i][j] > 0)
                l3[i][j] = 0;
            //getting # of cells
            if (l1[i][j] == route)
                cells++;
            if (l2[i][j] == route)
                cells++;
            if (l3[i][j] == route)
                cells++;
        }
    }
}

bool classicalImplementation(vector<vector<int>> &l1, vector<vector<int>> &l2, vector<vector<int>> &l3, int x, int y, coord source, coord target, int via, bool swapCoord, bool floodLessB){
    coord newSource;
    cells = 0; //# of cells
    vias = 0; //# of vias
    bool isSource = true; //source is same as newSource
    int swap;
    if (swapCoord) //if target and source will be swapped if no source
        swap = 2; //swap = 2 (to allow for 1 swap)
    else
        swap = 1; //swap = 1 (to allow for 0 swaps)
    
    while (swap > 0){
        switch (source.z){
            case(1): {
                s_value = l1[source.x][source.y]; //s_value is values of source cell (used if no path is available to retrieve old matrix)
                if (!floodLessB) //if implementation is not floodLess (more DFS)
                    newSource = traverse(l1, x, y, source, target, isSource);
            }
                break;
            case (2): {
                s_value = l2[source.x][source.y]; //s_value is values of source cell (used if no path is available to retrieve old matrix)
                if (!floodLessB) //if implementation is not floodLess (more DFS)
                    newSource = traverse(l2, x, y, source, target, isSource);
            }
                break;
            case (3): {
                s_value = l3[source.x][source.y]; //s_value is values of source cell (used if no path is available to retrieve old matrix)
                if (!floodLessB) //if implementation is not floodLess (more DFS)
                    newSource = traverse(l3, x, y, source, target, isSource);
            }
                break;
        }
        
        switch (target.z){
            case(1): {
                t_value = l1[target.x][target.y]; //t_value is values of target cell (used if no path is available to retrieve old matrix)
            }
                break;
            case (2): {
                t_value = l2[target.x][target.y]; //t_value is values of target cell (used if no path is available to retrieve old matrix)
            }
                break;
            case (3): {
                t_value = l3[target.x][target.y]; //t_value is values of target cell (used if no path is available to retrieve old matrix)
            }
                break;
        }
        
        if (floodLessB) //if implementation is floodLess (more DFS)
            newSource = floodLess(l1, l2, l3, x, y, source, target);
        
        int count0 = 1; //initial count for cell in flooding
        
        //if target is reached
        if ((newSource.x == target.x) && (newSource.y == target.y) && (newSource.z == target.z)){
            printMatrix(l1, x, y);
            cout << endl << endl;
            printMatrix(l2, x, y);
            cout << endl << endl;
            printMatrix(l3, x, y);
            cout << endl << endl;
            cout << "Cost = " << (cells + vias * via) << endl;
            return true;
        }
        else {
            cells = 0; //reset cells count
            if ((source.x != newSource.x) && (source.y != newSource.y) && (source.z != newSource.z))
                isSource = false;
        }
        flood(l1, l2, l3, x, y, newSource, target, via, count0, isSource);
        
        
        //        ///////////////////
        //        //remove before submission
        //        cout << "Flooding" << endl;
        //        printMatrix(l1, x, y);
        //        cout << endl << endl;
        //        printMatrix(l2, x, y);
        //        cout << endl << endl;
        //        printMatrix(l3, x, y);
        //        cout << endl << endl;
        //        ///////////////////
        //        //remove before submission
        
        
        if (backTracking(l1, l2, l3, x, y, newSource, target, via, source)){ //get route
            backToLife(l1, l2, l3, x, y); //empty flooded, non-routed cells
            cout << "BACKTRACKING" << endl;
            printMatrix(l1, x, y);
            cout << endl << endl;
            printMatrix(l2, x, y);
            cout << endl << endl;
            printMatrix(l3, x, y);
            cout << endl << endl;
            cout << "Cost = " << (cells + vias * via) << endl;
            swap = 0; //set swap to 0 (aka do not swap) because path is found
            return true;
        } else {
            swap--; //decrement swap
            backToLife(l1, l2, l3, x, y); //empty flooded cells
            cells = 0;
            switch (source.z){
                case(1): undoTraversal(l1, x, y, source, newSource); //remove traversed cells as no path is found
                    break;
                case (2): undoTraversal(l2, x, y, source, newSource); //remove traversed cells as no path is found
                    break;
                case (3): undoTraversal(l3, x, y, source, newSource); //remove traversed cells as no path is found
                    break;
            }
            switch (target.z){
                case(1): {
                    l1[target.x][target.y] = t_value; //set target cell to target value before flooding or traversal
                }
                    break;
                case(2): {
                    l2[target.x][target.y] = t_value; //set target cell to target value before flooding or traversal
                }
                    break;
                case(3): {
                    l3[target.x][target.y] = t_value; //set target cell to target value before flooding or traversal
                }
                    break;
            }
            //swap source and target
            coord temp = source;
            source = target;
            target = temp;
            if (swap == 0){ //if no more swaps, print grids anyway
                cout << "No Path available" << endl;
                route++;
                printMatrix(l1, x, y);
                cout << endl << endl;
                printMatrix(l2, x, y);
                cout << endl << endl;
                printMatrix(l3, x, y);
                cout << endl << endl;
                return false;
            }
        }
    }
    return false;
}

void undoTraversal(vector<vector<int>> &l,  int x, int y, coord s, coord t){
    if (s.z  == 2){
        if (t.x > s.x){
            for (int i = s.x + 1; i<=t.x; i++){ //vertical
                if (l[i][s.y] == route){ //if cell was part of route
                    l[i][s.y] = 0; //empty cell
                }
            }
        }
        else { //vertical
            for (int i = s.x + 1; i>=t.x; i--){
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
            for (int j = s.y + 1; j >= t.y; j--){
                if (l[s.x][j] == route){ //if cell was part of route
                    l[s.x][j] = 0; //empty cell
                }
            }
        }
    }
    l[s.x][s.y] = s_value; //set source to value before filling (might not be zero)
}

coord floodLess(vector<vector<int>> &l1, vector<vector<int>> &l2, vector<vector<int>> &l3, int x, int y, coord newSource, coord target){
    int count = 0;
    coord newSource1; //return of traverse
    coord newSource2 = newSource; //parameter of traverse
    coord newSourceBuffer; //to save last value of newSource1
    newSourceBuffer = newSource; //initially it is set to newSource
    while (count < 3){
        //check is initial newSource is the same as newSource2
        bool isSource = (newSource2.x == newSource.x) && (newSource2.y == newSource.y) && (newSource2.z == newSource.z);
        switch(newSource2.z){
            case 1: newSource1 = traverse(l1, x, y, newSource2, target, isSource);
                break;
            case 2:newSource1 = traverse(l2, x, y, newSource2, target, isSource);
                break;
            case 3:
                newSource1 = traverse(l3, x, y, newSource2, target, isSource);
                break;
            default:
                break;
        }
        if ((newSource1.x == newSource2.x) && (newSource1.y == newSource2.y) && (newSource1.z == newSource2.z)){ //no traverse
            if ((newSource1.x == target.x) && (newSource1.y == target.y) && (newSource1.z == target.z)) //target reached
                return target;
            if(newSourceBuffer.z != newSource1.z) //if no traverse, reduce vias
                vias--;
            return newSourceBuffer; //return saved newSource1 value
        }
        else {
            newSource2 = newSource1;
        }
        
        if ((newSource2.x == target.x) && (newSource2.y == target.y) && (newSource2.z == target.z)){ //target reached
            return target;
        }
        else {
            if(newSource2.z < target.z){ //z of target is greater
                newSource2.z = ((newSource2.z) % 3 )+ 1; //increment z sent to traverse
                vias++;
            }
            else if (newSource2.z > target.z){ //z of target is less
                newSource2.z = ((newSource2.z - 1) % 3 ); //decrement z sent to traverse
                vias++;
            }
            else if ((newSource2.z == target.z) && ((target.z == 1) || (target.z == 3)) && (newSource2.x != target.x)){ //horizontal layer and there is a difference vertically
                newSource2.z = 2;
                vias++;
            }
            else if ((newSource2.z == target.z) && (target.z == 2) && (newSource2.y != target.y)){ //vertical layer and their is a difference horizontally
                newSource2.z = ((newSource2.z) % 3 )+ 1;
                vias++;
            }
        }
        newSourceBuffer = newSource1; //update buffer
        count ++;
    }
    return newSource1;
}

void multiPins(vector<vector<int>> &l1, vector<vector<int>> &l2, vector<vector<int>> &l3, int x, int y, coord source, vector<coord> target, int via){
    int routeNo = route;
    int i = 0;
    while(!classicalImplementation(l1, l2, l3, x, y, source, target[0], via, 0, 1))
    {
        coord temp = target[0];
        target[0] = target[i+1];
        target[i+1] = temp;
        i++;
    }
    route = routeNo;
    for (int i=1; i<target.size(); i++){
        vector<coord> coords;
        getCoords(l1, l2, l3, x, y, routeNo, coords);
        coord targetPin = target[i];
        vector<pair<int,int>> targetsDist;
        getDistance(targetPin, coords, targetsDist);
        for(int i=0; i<targetsDist.size(); i++){
            cout << targetsDist[i].first << "\t\t" << coords[targetsDist[i].second].x << " " << coords[targetsDist[i].second].y << " " << coords[targetsDist[i].second].z <<endl;
        }
        int j = 0;
        while( !classicalImplementation(l1, l2, l3, x, y, targetPin, coords[targetsDist[j].second], via, 0, 1))
            j++;
    }

}

void getDistance(coord source, vector<coord> targets, vector<pair<int,int>> &targetsDist){
    for(int i = 0; i<targets.size(); i++){
        if(source.z == 2){ //vertical
            int distance = abs(targets[i].x - source.x);
            targetsDist.push_back(make_pair(distance, i));
        }else{ //horizontal
            int distance = abs(targets[i].y - source.y);
            targetsDist.push_back(make_pair(distance, i));
        }
    }
    sort(targetsDist.begin(), targetsDist.end());
}


void getCoords(vector<vector<int>> &l1, vector<vector<int>> &l2, vector<vector<int>> &l3, int x, int y, int route, vector<coord> &coords){
    for (int i=0; i<x; i++){
        for(int j=0; j<y; j++){
            // emptying flooded cells that are not part of route
            if(l1[i][j] == route){
                coord temp;
                temp.x = i;
                temp.y = j;
                temp.z = 1;
                coords.push_back(temp);
            }
            if(l2[i][j] == route){
                coord temp;
                temp.x = i;
                temp.y = j;
                temp.z = 2;
                coords.push_back(temp);
            }
            if(l3[i][j] == route){
                coord temp;
                temp.x = i;
                temp.y = j;
                temp.z = 3;
                coords.push_back(temp);
            }
        }
    }
}
