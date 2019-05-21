#include <iostream>
#include <vector>

#include <cmath>
#include <stack>
#include <queue>
#include <fstream>
#include <string>


using namespace std;
//assuming that pins connected to hard macros won't be put again while in the soft macros loop
//assuming that pin connected having multiple connections will be mentioned twice
// do i assume en el hardc macros el pins bta3thom 2d
stack<int> up;
stack<int>down;
//queue <int> up;
//queue <int> down;
int s;
int M1;
int M2;

void split(string str, vector<string> & vec)
{
    string word = "";
    bool flag = false;
    for (auto x : str)
    {
        if (x != ' ')
            flag = true;
        if (flag)
        {
            if (x == ' ')
            {
                vec.push_back(word);
                word = "";
            }
            else
            {
                word = word + x;
            }
        }
    }
    vec.push_back(word);
}

enum dposition {N,L,R,U,D};
struct gridpt
{
    int value;
    int connected;
};

gridpt grid [50][50];

struct pin
{
    int x;
    int y;
    int z;
    int to;
    
    int value;
    string name;
};

vector<string> hard_names;
vector<string> soft_names;
vector<vector<int>> hard_macrosC;
vector<vector<int>> soft_macrosC;
vector<vector<string>> temp_net;
vector<vector<pin>> pins_map;

vector<vector<pin>> pins_soft;
vector<pin> temp_vec;
vector<vector<pin>> pins_hard; // sontains the soft pins connected to hard pins
vector<int> vec;
int pins_count=1;
int soft_macrosn=0;
int hard_macrosn=0;
//vector<string> vec_string;



void initialize_int_vec(vector<int>& vec)
{
    while (!vec.empty())
        vec.pop_back();
}

//void parsing()
//{
//   ifstream input;
//   ofstream output;
//    int grid_width;
//    int grid_height;
//    int len;
//    int i;
//    int pos;
//    string line_1;
//
//input.open("input.txt");
//if (!input.fail())
//{
//    string line;
//
//    while(!input.eof())
//    {
//        getline(input, line);
//        len=line.length();
//        line_1=line.substr(0, 3);
//        if (line.at(0)=='W')
//        {
//            grid_width=stoi(line.substr(1,len-2));
//        }
//        else if (line.at(0)=='H')
//        {
//            grid_width=stoi(line.substr(1, len-2));
//        }
//
//        else if (line_1=="IO")
//        {
//            while (line!="Hard Modules:")
//            {
//
//            }
//        }
//        else if(line=="Hard Modules:")
//        {
//            while(line!="Soft Modules")
//            {
//                pos=line.find(":");
//                hard_names.push_back(line.substr(0,line.length()-pos));
//                input>>line;
//                vec.push_back(stoi(line));
//                input>>line;
//                vec.push_back(stoi(line));
//                input>>line;
//                vec.push_back(stoi(line));
//                input>>line;
//                vec.push_back(stoi(line));
//                hard_macrosC.push_back(vec);
//                initialize_int_vec( vec);
//
//                hard_macrosn++;
//            }
//        }
//        else if (line=="Soft Modules:")
//        {
//            while(!input.eof())
//            {
//                pos=line.find(":");
//                soft_names.push_back(line.substr(0,line.length()-pos));
//                input>>line;
//                vec.push_back(stoi(line));
//                input>>line;
//                vec.push_back(stoi(line));
//                input>>line;
//                vec.push_back(stoi(line));
//                input>>line;
//                vec.push_back(stoi(line));
//                soft_macrosC.push_back(vec);
//                initialize_int_vec(vec);
//                soft_macrosn++;
//            }
//        }
//    }
//  }
//
//}
//
//vector<vector<pin>> pins_map(hard_macrosn);
//
//void gl_parsing()
//{
//    ifstream input;
//    ofstream output;
//    int grid_width;
//    int grid_height;
//    int len;
//    string line;
//    string name;
//    int i;
//    int pos;
//    bool found_hard=false;
//    bool found_soft=false;
//    string line_1;
//    int index=0;
//    vector<string> splitted;
//
//    input.open("gl.txt");
//    if(!input.fail())
//    {
//       // input >> line;
//        while (!input.eof())
//        {
//            getline(input, line);
//            len=line.length();
//            found_hard=false;
//            found_soft=false;
//            while(line.at(len-1)!=':')
//            {
//                getline(input,line);
//                split (line, splitted);
//                temp_net.push_back(splitted);
//            }
//            for (int s=0; s<temp_net.size(); s++)
//            {
//                for (int i=0; i<hard_names.size(); i++)
//                    if(hard_names[i]==temp_net[s][0])
//                    {
//                        for (int k=0;k<hard_names[i].size();k++)
//                        {
//                            if(hard_names[i][5]==temp_net[i][1])
//                        }
//                        index=i;
//                        pins_hard[i].name=split
//                        found_hard==true;
//                    }
//                for(int j=0;j<soft_names.size();j++)
//                    if(soft_names[j]==temp_net[s][0])
//                    {
//                        index=j;
//                        found_soft==true;
//                    }
//            }
//        }
//    }
//}
//struct coordinates
//{
//    int x;
//    int y;
//};
//vector<pin> pins;



//bool freecells(dposition dir, int softmacro, int h, int w, int originx, int originy, int val)
//{`
//    int x=0;
//
//    if (dir==R)
//    {
//        for(int i=0; i<h; i++)
//        {
//            x=originx+w;
//
//            if(grid[i+1][x].value==-1)
//            {
//                grid[i+1][x].value=val;
//                return true;
//            }
//        }
//    }
//
//    if (dir==L)
//    {
//        for(int i=0; i<h; i++)
//        {
//            x=originx;
//
//            if(grid[i-1][x].value==-1)
//            {
//                grid[i-1][x].value=val;
//                return true;
//            }
//        }
//    }
//
//    if (dir==U)
//    {
//        for(int i=0; i<h; i++)
//        {
//            x=originx;
//
//            if(grid[originy+h][x+i+1].value==-1)
//            {
//                grid[originy+h][x+i+1].value=val;
//                return true;
//            }
//        }
//    }
//
//    if (dir==D)
//    {
//        for(int i=0; i<h; i++)
//        {
//            x=originx+w;
//
//            if(grid[originy][x+i-1].value==-1)
//            {
//                grid[originy][x+i-1].value=val;
//                return true;
//            }
//        }
//    }
//    return false;
//}

void output()
{
    ofstream output ("out.txt", ios::app);
    //ofstream.open("out.txt");
    if (!output)
        cout <<"output file failed to open"<< endl;
    
            for (int i=0; i<pins_hard.size(); i++)
            {
                for (int j=0; j<pins_hard[i].size();j++)
                {
                    cout <<pins_hard[i][j].x <<endl;
                    cout <<pins_hard[i][j].y <<endl;
                    cout <<pins_hard[i][j].z <<endl;
                    cout <<pins_hard[i][j].value <<endl;
                }
            }
    
    for (int i=0; i<pins_soft.size(); i++)
    {
        for (int j=0; j<pins_soft[i].size();j++)
        {
            cout << " soft macros output x, y, z, value" << endl;
            cout <<pins_soft[i][j].x <<endl;
            cout <<pins_soft[i][j].y <<endl;
            cout <<pins_soft[i][j].z <<endl;
            cout<<pins_soft[i][j].to << endl;
            cout <<pins_soft[i][j].value <<endl;
        }
    }
    

    
}

void initialize_vec()
{
    while (!temp_vec.empty())
        temp_vec.pop_back();
}
bool looping(dposition dir, int softmacro, int h_soft, int w_soft, int originx, int originy, stack<int> &used, int soft)
{
    int x=0;
    int s=0;
    int start;
    int z;
    pin temp;
    if (dir==R)
    {

        if(used.size()<h_soft)
            start=originy+h_soft-used.size()+1;
        else start=originy;
        for(int i=start; i<=originy+h_soft; i++)
        {
            x=originx+w_soft;
            
            if(grid[x+1][i].value==-1)
            {
                 if(!used.empty())
                {
                    s=used.top();
                    grid[x+1][i].value=s;
                    if(soft==0)
                    {
                    temp.x=x+1;
                    temp.y=i;
                    if(M1==0)
                        z=1;
                    else z=2;
                    temp.z=z;
                    temp.value=s;// didn't push back to
                    temp_vec.push_back(temp);
                    pins_hard.push_back(temp_vec);
                    initialize_vec();}
                    used.pop();
                }
            }
        }
    }
    
    if (dir==L)
    {
        if(used.size()<h_soft)
            start=originy+h_soft-used.size()+1;
        else start=originy;
        for(int i=start; i<=originy+h_soft; i++)
        {
            x=originx;
            
            if(grid[x-1][i].value==-1)
            {
                
                if(!used.empty())
                {
                    s=used.top();
                    grid[x-1][i].value=s;
                    if(soft==0)
                    {
                    temp.x=x-1;
                    temp.y=i;
                    if(M1==0)
                        z=1;
                    else z=2;
                    temp.z=z;
                    temp.value=s;// didn't push back to
                    temp_vec.push_back(temp);
                    pins_hard.push_back(temp_vec);
                    initialize_vec();
                    }
                    used.pop();
                }
  
            }
        }
    }
    
    if (dir==U)
    {
        if(used.size()<w_soft)
            start=originx+w_soft-used.size()+1;
        else start=originx;
        for(int i=start; i<=w_soft; i++)
        {
            x=originx;
            
            if(grid[x+i][originy+h_soft+1].value==-1)
            {
                if(!used.empty()){
                    s=used.top();
                    grid[x+i][originy+h_soft+1].value=s;
                    if (soft==0){
                    temp.x=x+i;
                    temp.y=originy+h_soft+1;
                    if(M1==1)
                        z=1;
                    else z=2;
                    temp.z=z;
                    temp.value=s;// didn't push back to
                    temp_vec.push_back(temp);
                    pins_hard.push_back(temp_vec);
                        initialize_vec();}
                    used.pop();
                }
            }
        }
    }
    
    if (dir==D)
    {
        if(used.size()<w_soft)
            start=originx+w_soft-used.size()+1;
        else start=originx;
        for(int i=0; i<=w_soft; i++)
        {
            x=originx;
            if(grid[x+i][originy-1].value==-1)
            {
                if(!used.empty()){
                    s=used.top();
                    grid[x+i][originy+h_soft+1].value=s;
                    if(soft==0){
                    temp.x=x+i;
                    temp.y=originy-1;
                    if(M1==1)
                        z=1;
                    else z=2;
                    temp.z=z;
                    temp.value=s;// didn't push back to
                    temp_vec.push_back(temp);
                    pins_hard.push_back(temp_vec);
                        initialize_vec();}
                    grid[x+i][originy-1].value=s;
                    
                    used.pop();
                }
            }
        }
    }
 
    if(used.empty())
        return true;
    else return false;
}

bool looping_reverse(dposition dir, int softmacro, int h_soft, int w_soft, int originx, int originy, stack<int> &used, int soft)
{
    int x=0;
    int s=0;
    pin temp;
    int z=0;
    if (dir==R)
    {
        for(int i=originy+h_soft; i>=originy; i--)
        {
            x=originx+w_soft;
            
            if(grid[x+1][i].value==-1)
            {
                if(!used.empty())
                {
                    s=used.top();
                    grid[x+1][i].value=s;
                    if(soft==1)
                    {
                        temp.x=x+1;
                        temp.y=i;
                        if(M1==0)
                            z=1;
                        else z=2;
                        temp.z=z;
                        temp.value=s;// didn't push back to
                        temp_vec.push_back(temp);
                        pins_hard.push_back(temp_vec);
                        initialize_vec();}
                    
                    used.pop();
                    
                }
            }
        }
    }
    
    if (dir==L)
    {
        for(int i=originy+h_soft; i>=originy; i--)
        {
            x=originx;
            
            if(grid[x-1][i].value==-1)
            {
                if(!used.empty())
                {
                    s=used.top();
                    grid[x-1][i].value=s;
                    if(soft==0)
                    {
                        temp.x=x-1;
                        temp.y=i;
                        if(M1==1)
                            z=1;
                        else z=2;
                        temp.z=z;
                        temp.value=s;// didn't push back to
                        temp_vec.push_back(temp);
                        pins_hard.push_back(temp_vec);
                        initialize_vec();}
                
                    used.pop();
                }
            }
        }
    }
    
    if (dir==U)
    {
        for(int i=originx+w_soft; i>=originx; i--)
        {
            x=originx;
            
            if(grid[i][originy+h_soft+1].value==-1)
            {
                if(!used.empty())
                {
                    s=used.top();
                    grid[i][originy+h_soft+1].value=s;
                    if(soft==0)
                    {
                        temp.x=x+1;
                        temp.y=i;
                        if(M1==1)
                            z=1;
                        else z=2;
                        temp.z=z;
                        temp.value=s;// didn't push back to
                        temp_vec.push_back(temp);
                        pins_hard.push_back(temp_vec);
                        initialize_vec();
                        
                    }
                    used.pop();
                }
            }
        }
    }
    
    if (dir==D)
    {
        for(int i=originx+w_soft; i>=originx; i--)
        {
            x=originx;
            if(grid[i][originy-1].value==-1)
            {
                if(!used.empty())
                {
                    s=used.top();
                    grid[i][originy-1].value=s;
                    if(soft==0)
                    {
                        temp.x=x+1;
                        temp.y=i;
                        if(M1==1)
                            z=1;
                        else z=2;
                        temp.z=z;
                        temp.value=s;// didn't push back to
                        temp_vec.push_back(temp);
                        pins_hard.push_back(temp_vec);
                        initialize_vec();
                    }
                    used.pop();
                }
            }
        }
    }
    
    if(used.empty())
        return true;
    else return false;
}


void placement(int softmacro, int h_soft, int w_soft, int originx_soft, int originy_soft, int xh, int yh, int xs, int ys, dposition dmax_pos, int soft)
{
    bool flag;
   if (dmax_pos==D)
    {
       if (xs<xh)
       {
           flag=looping(L, softmacro, h_soft, w_soft, originx_soft, originy_soft, up, soft);
           if(!flag) // spaces weren't enough
               looping_reverse(D, softmacro, h_soft, w_soft, originx_soft, originy_soft, up, soft );
           
           flag=looping(U, softmacro, h_soft, w_soft, originx_soft, originy_soft, down, soft);
           if(!flag)
               looping_reverse(D, softmacro, h_soft, w_soft, originx_soft, originy_soft, down, soft);
       }
      else
       {

           
           flag=looping(D, softmacro, h_soft, w_soft, originx_soft, originy_soft, up, soft);
           if(!flag) // spaces weren't enough
               looping(R, softmacro, h_soft, w_soft, originx_soft, originy_soft, up, soft);
           
           flag=looping_reverse(L, softmacro, h_soft, w_soft, originx_soft, originy_soft, down, soft);
           if(!flag)
               looping(U, softmacro, h_soft, w_soft, originx_soft, originy_soft, down, soft);
       }
    }
    
    //// d pos is up
    else if (dmax_pos==U)
    {
        if(xs<xh)
        {
            flag=looping(R, softmacro, h_soft, w_soft, originx_soft, originy_soft, up, soft);
            if(!flag) // spaces weren't enough
                looping_reverse(D, softmacro, h_soft, w_soft, originx_soft, originy_soft, up, soft);
            
            flag=looping(U, softmacro, h_soft, w_soft, originx_soft, originy_soft, down, soft);
            if(!flag)
                looping_reverse(L, softmacro, h_soft, w_soft, originx_soft, originy_soft, down, soft);
        }
        else{
            flag=looping(U, softmacro, h_soft, w_soft, originx_soft, originy_soft, up, soft);
            if(!flag) // spaces weren't enough
                looping_reverse(R, softmacro, h_soft, w_soft, originx_soft, originy_soft, up, soft);
            
            flag=looping_reverse(L, softmacro, h_soft, w_soft, originx_soft, originy_soft, down, soft);
            if(!flag)
                looping(D, softmacro, h_soft, w_soft, originx_soft, originy_soft, down, soft);
        }
    }
    
    // dpos is right
    else if (dmax_pos==R)
    {
        if(ys<yh) // lower part
        {
            flag=looping_reverse(U, softmacro, h_soft, w_soft, originx_soft, originy_soft, up, soft);
            if(!flag) // spaces weren't enough
                looping_reverse(L, softmacro, h_soft, w_soft, originx_soft, originy_soft, up, soft);
            
            flag=looping_reverse(R, softmacro, h_soft, w_soft, originx_soft, originy_soft, down, soft);
            if(!flag)
                looping_reverse(D, softmacro, h_soft, w_soft, originx_soft, originy_soft, down, soft);
        
        }
        else {
            flag=looping(R, softmacro, h_soft, w_soft, originx_soft, originy_soft, up, soft);
            if(!flag) // spaces weren't enough
                looping_reverse(L, softmacro, h_soft, w_soft, originx_soft, originy_soft, up, soft);
            
            flag=looping_reverse(D, softmacro, h_soft, w_soft, originx_soft, originy_soft, down, soft);
            if(!flag)
                looping(U, softmacro, h_soft, w_soft, originx_soft, originy_soft, down, soft);
            
        }
    }
    
    // dpos is left
    else if (dmax_pos==L)
    {
        if(ys<yh) // lower part
        {
            flag=looping(U, softmacro, h_soft, w_soft, originx_soft, originy_soft, up, soft);
            if(!flag) // spaces weren't enough
                looping_reverse(R, softmacro, h_soft, w_soft, originx_soft, originy_soft, up, soft);
            
            flag=looping_reverse(L, softmacro, h_soft, w_soft, originx_soft, originy_soft, down, soft);
            if(!flag)
                looping(D, softmacro, h_soft, w_soft, originx_soft, originy_soft, down, soft);
            
        }
        else
        {
            flag=looping(L, softmacro, h_soft, w_soft, originx_soft, originy_soft, up, soft);
            if(!flag) // spaces weren't enough
                looping(U, softmacro, h_soft, w_soft, originx_soft, originy_soft, up, soft);
            
            flag=looping(D, softmacro, h_soft, w_soft, originx_soft, originy_soft, down, soft);
            if(!flag)
                looping(R, softmacro, h_soft, w_soft, originx_soft, originy_soft, down, soft);
            
        }
    }
       
}





void initialize_grid()
{
    for (int i=0; i<50; i++)
        for(int j=0; j<50;j++)
        {
            grid[i][j].value=-1;
            grid[i][j].connected=-1;
        }
}
void solve_quations(int xh, int yh, int xs, int ys, int leftx, int rightx, int uppery, int lowery, int xtoedge, int ytoedge, dposition& dpos, dposition& maxdpos, double& dminx, double& dminy, double& maxdx, double& maxdy )
{
    //leftx ->x=     uppery ->y=
    //xh and yh and ys and xs here are the center
    // when you use any of x or y here as indecies make sure to cast them as integers
    double m;
    double b;
    double maxdistance=0;
    long double distance=0.0;
    double intersectionL;
    double intersectionR;
    double intersectionU;
    double intersectionD;
    double dx=0.0;
    double dy=0.0;
   

    
    m=double(yh-ys)/double(xh-xs);
    b=double(yh)-double(m*xh);
    
    intersectionL=double(m*leftx)+b;
    intersectionR=double(m*rightx)+b;
    intersectionU=double(uppery-b)/m;
    intersectionD=double(lowery-b)/m;
    
    // unsolvable left x

    if((yh-ytoedge)<=intersectionL && intersectionL<=(yh+ytoedge))
    {
        dpos=L;
        dx=leftx;
        dy=intersectionL;
        distance=sqrt(((dx-xs)*(dx-xs)+(dy-ys)*(dy-ys)));
        if(distance>maxdistance)
        {
            maxdpos=L;
            maxdx=dx;
            maxdy=dy;
            maxdistance=distance;
            dminy=intersectionR;
            dminx=rightx;
            dpos=R;

        }

        cout<< "intersection " << dx << " "<<dy <<endl;
    }
    if((yh-ytoedge)<=intersectionR && intersectionR<=(yh+ytoedge))
    {
        dpos=R;
        dx=rightx;
        dy=intersectionR;
        distance=sqrt(((dx-xs)*(dx-xs)+(dy-ys)*(dy-ys)));
        if(distance>maxdistance)
        {
            maxdpos=R;
            maxdx=dx;
            maxdy=dy;
             maxdistance=distance;
            dminy=intersectionL;
            dminx=leftx;
            dpos=L;
            
        }

        cout<< "intersection " << dx << " "<<dy <<endl;
    }
    if(xh-xtoedge<=intersectionU && intersectionU<=xh+xtoedge)
    {
        dpos=U;
        dy=uppery;
        dx=intersectionU;
        distance=sqrt(((dx-xs)*(dx-xs)+(dy-ys)*(dy-ys)));
        if(distance>maxdistance)
        {
            maxdpos=U;
            maxdx=dx;
            maxdy=dy;
            dminy=lowery;
            dminx=intersectionD;
            maxdistance=distance;
            dpos=D;
        }

        cout<< "intersection " << dx << " "<<dy <<endl;
    }
    if(xh-xtoedge<=intersectionD && intersectionD<=xh+xtoedge)
    {
        dpos=D;
        dy=lowery;
        dx=intersectionD;
        distance=sqrt(((dx-xs)*(dx-xs))+((dy-ys)*(dy-ys)));
        if(distance>maxdistance)
        {
            maxdpos=D;
            maxdx=dx;
            maxdy=dy;
            maxdistance=distance;
            dminx=intersectionU;
            dminy=uppery;
            dpos=U;
        }
        cout<< "intersection " << dx << " "<<dy <<endl;
    }
    
    
}
void traverse(dposition direction, int h, int w, int xh, int yh, bool& found, int startingx, int startingy, int macrono, int dx, int dy, stack<int>& used, int& lastdx, int& lastdy, int soft, int index, int& pins_index )
{
    //xh and yh are the center
    // handle if alreadu occupied
    int vertexL=xh-(w/2);
    int vertexR=xh+(w/2);
    int vertexU=yh+(h/2);
    int vertexD=yh-(h/2);
    found=false;
    
    if(direction==L)
    {
        for (int i=0;i<=w; i++)
        {

            if (startingx-i<vertexL)
            {
                lastdx=startingx-i;
                lastdy=startingy;
                return;
            }
            if (soft==1)
            {
                if(grid[startingx-i][startingy].value==-1)
                {
                    if(pins_soft[index].size()>0)
                    if(pins_soft[index][pins_index].to==macrono)
                    {
                        
                        grid[startingx-i][startingy].value=pins_soft[index][pins_index].value;
                        used.push(grid[startingx-i][startingy].value);
                        pins_soft[index][pins_index].x=startingx-i;
                        pins_soft[index][pins_index].y=startingy;
                        if(M1==0)// m1 is vertical
                            pins_soft[index][pins_index].z=1;
                        else
                            pins_soft[index][pins_index].z=2;
                        
                        
                        pins_index++;
                        
                    }
                  
                }
            }
            else
            if(grid[startingx-i][startingy].value!=-1)
            {
                if(pins_soft[index].size()>0)
                if(grid[startingx-i][startingy].connected==macrono)
                {
                    used.push(grid[startingx-i][startingy].value);

                }
            }
            lastdx=startingx-i;
            lastdy=startingy;
            if ((startingx-i==dx) &&(startingy==dy))
            {
                found=true;
                return;
            }
        }
    }
    else if(direction==R)
    {
        for (int i=0; i<=w; i++)
        {

            if(startingx+i>vertexR){
                lastdx=startingx+i;
                lastdy=startingy;
                return;
            }
            if (soft==1)
            {
                if(pins_soft[index].size()>0)
                if(grid[startingx+i][startingy].value==-1)
                {
                    if(pins_soft[index][pins_index].to==macrono)
                    {
                        grid[startingx+i][startingy].value=pins_soft[index][pins_index].value;
                        used.push(grid[startingx+i][startingy].value);
                        pins_soft[index][pins_index].x=startingx+i;
                        pins_soft[index][pins_index].y=startingy;
                        if(M1==0)// m1 is vertical
                            pins_soft[index][pins_index].z=1;
                        else
                            pins_soft[index][pins_index].z=2;
                        pins_index++;
                        
                    }
                }
            }
            else
            if (grid[startingx+i][startingy].value!=-1)
            {
                if (grid[startingx+i][startingy].connected==macrono)
                {
                    used.push(grid[startingx+i][startingy].value);

                }
            }
            lastdx=startingx+i;
            lastdy=startingy;
            if ((startingx+i==dx) && (startingy==dy))
            {
                found=true;
                return;
            }
        }
    }
    else if (direction==U)
    {
        for (int i=0; i<=h ; i++)
        {


            if (startingy+i>vertexU)
            {
                lastdx=startingx;
                lastdy=startingy+i;
                return;
            }
            
            if (soft==1)
            {
                if(grid[startingx][startingy+i].value==-1)
                {
                    if(pins_soft[index][pins_index].to==macrono)
                    {
                        grid[startingx][startingy+i].value=pins_soft[index][pins_index].value;
                        used.push(grid[startingx][startingy+i].value);
                        pins_soft[index][pins_index].x=startingx;
                        pins_soft[index][pins_index].y=startingy+i;
                        if(M1==1)// m1 is vertical
                            pins_soft[index][pins_index].z=1;
                        else
                            pins_soft[index][pins_index].z=2;
                        pins_index++;
                        
                    }
                }
            }
            else
            if (grid[startingx][startingy+i].value!=-1)
            {
                if(grid[startingx][startingy+i].connected==macrono)
                    used.push(grid[startingx][startingy+i].value);
            }
            lastdx=startingx;
            lastdy=startingy+i;
            if ((startingx==dx) && (startingy+i==dy))
            {
                found=true;
                return;
            }
        }
    }
    else if (direction==D)
    {
        for (int i=0; i<=h ; i++)
        {
            

            if (startingy-i<vertexD)
            {
                lastdx=startingx;
                lastdy=startingy-i;
                return;
            }
            if (soft==1)
            {
                if(grid[startingx][startingy-i].value==-1)
                {
                    if(pins_soft[index][pins_index].to==macrono)
                    {
                        grid[startingx][startingy-i].value=pins_soft[index][pins_index].value;
                        used.push(grid[startingx][startingy-i].value);
                        pins_soft[index][pins_index].x=startingx;
                        pins_soft[index][pins_index].y=startingy-i;
                        if(M1==1)// m1 is vertical
                            pins_soft[index][pins_index].z=1;
                        else
                            pins_soft[index][pins_index].z=2;
                        pins_index++;
                        
                    }
                }
            }
            else
            
            if (grid[startingx][startingy-i].value!=-1)
            {
                if(grid[startingx][startingy-i].connected==macrono)
                    used.push(grid[startingx][startingy-i].value);

            }
            
            lastdx=startingx;
            lastdy=startingy-i;
            if ((startingx==dx) && (startingy-i==dy))
            {
                found=true;
                return;
            }
        }

    }
}

void handle_hard_macros(int h, int w, int xh, int yh, int xs, int ys, int leftx, int rightx, int uppery, int lowery, int xtoedge, int ytoedge, int macrono, dposition& maxdpos, int soft, int index)
{
    dposition dpos;
    //dposition maxdpos;
    double dx=0;
    double dy=0;
    double maxdx;
    double maxdy=0;
    int lastx=0;
    int lasty=0;
    int temp=0;
    int index_2=0;
    
    if(soft==1)
    {
     solve_quations(xh, yh, xs, ys, leftx, rightx, uppery, lowery, xtoedge, ytoedge, dpos, maxdpos, dx, dy,  maxdx,  maxdy);
        index_2=index;
        swap(dx, maxdx);
        swap(dy, maxdy);
        swap(dpos, maxdpos);
       
        
    }
    else{
    solve_quations(xh, yh, xs, ys, leftx, rightx, uppery, lowery, xtoedge, ytoedge, dpos, maxdpos, dx, dy, maxdx, maxdy );
        index_2=0;
    }
    
    maxdx=ceil(maxdx);
    maxdy=ceil(maxdy);
    
    bool found;
    if(maxdpos==R)
    {
        temp=0;
        //traversing from dmax to dmin
        traverse(U, h, w, xh, yh, found, maxdx+1, maxdy, macrono, dx, dy+1, up, lastx, lasty, soft,index_2,temp);
        if (found!=true)
        {
           traverse(L, h, w, xh, yh, found, lastx-1, lasty, macrono, dx, dy+1, up, lastx, lasty,soft,index_2,temp);
            if(found!=true)
            {
                traverse(D, h, w, xh, yh, found, lastx, lasty-1, macrono, dx, dy+1, up, lastx, lasty,soft,index_2,temp);
                if(found!=true)
                    traverse(R, h, w, xh, yh, found, lastx+1, lasty, macrono, dx, dy+1, up, lastx, lasty,soft,index_2,temp);
            }
        }


        
        //traversing from dmin to dmax
        traverse(D,h,w,xh,yh,found,maxdx+1, maxdy-1,macrono,dx, dy,down,lastx,lasty,soft,index_2, temp);
        {
             if (found!=true)
             {
                traverse(L,h,w,xh,yh,found,lastx-1,lasty,macrono,dx, dy,down,lastx,lasty,soft,index_2,temp);
                 if (found!=true)
                 {
                   traverse(U,h,w,xh,yh,found,lastx,lasty+1,macrono,dx, dy,down,lastx,lasty,soft,index_2,temp);
                     if(found!=true)
                     {
                         traverse(R,h,w,xh,yh,found,lastx-1,lasty,macrono,dx, dy,down,lastx,lasty,soft,index_2,temp);
                     }
                 }
                 
             }
        }
       
            
    }
// end of maxdos at R
    if(maxdpos==D) //-1 in maxdy to loop around the macro
    {
        temp=0;
        traverse(R, h, w, xh, yh, found, maxdx, maxdy-1, macrono, dx+1, dy, up, lastx, lasty,soft,index_2,temp);
        if(found!=true)
        {
            traverse(U, h, w, xh, yh, found, lastx, lasty+2, macrono, dx+1, dy, up, lastx, lasty,soft,index_2,temp);
            if (found!=true)
            {
                traverse(L, h, w, xh, yh, found, lastx-1, lasty, macrono, dx+1, dy, up, lastx, lasty,soft,index_2,temp);
                if (found!=true)
                    traverse(D, h, w, xh, yh, found, lastx, lasty-2, macrono, dx+1, dy, up, lastx, lasty,soft,index_2,temp);

            }
        } /// traversed up
        
        traverse(L, h, w, xh, yh, found, maxdx-1, maxdy-1, macrono, dx, dy, down, lastx, lasty,soft,index_2,temp);
        if(found!=true)
        {
            traverse(U, h, w, xh, yh, found, lastx, lasty+1, macrono, dx, dy, down, lastx, lasty,soft,index_2,temp);
            if (found!=true)
            {
                traverse(R, h, w, xh, yh, found, lastx+1, lasty, macrono, dx, dy, down, lastx, lasty,soft,index_2,temp);
                if (found!=true)
                    traverse(D, h, w, xh, yh, found, lastx, lasty-1, macrono, dx, dy, down, lastx, lasty,soft,index_2,temp);
                
            }
        }
        // traversed down
    }
// end of maxdpos at D
    
    if (maxdpos==L)
    {
        traverse(U, h, w, xh, yh, found, maxdx-1, maxdy, macrono, dx, dy+1, up, lastx, lasty,soft,index_2,temp);
        if(found!=true)
        {
            traverse(R, h, w, xh, yh, found, lastx+1, lasty, macrono, dx, dy+1, up, lastx, lasty,soft,index_2,temp);
            if(found!=true)
            {
                traverse(D, h, w, xh, yh, found, lastx, lasty-1, macrono, dx, dy+1, up, lastx, lasty,soft,index_2,temp);
                if (found!=true)
                    traverse(L, h, w, xh, yh, found, lastx-1, lasty, macrono, dx, dy+1, up, lastx, lasty,soft,index_2,temp);
            
            }
        }// traversed up
        //start of down traversal
        traverse(D, h, w, xh, yh, found, maxdx-1, maxdy-1, macrono, dx, dy, down, lastx, lasty,soft,index_2,temp);
        if(!found)
        {
            traverse(R, h, w, xh, yh, found, lastx+1, lasty, macrono, dx, dy, down, lastx, lasty,soft,index_2,temp);
           if(found!=true)
           {
               traverse(U, h, w, xh, yh, found, lastx, lasty+1, macrono, dx, dy, down, lastx, lasty,soft,index_2,temp);
               if(found!=true)
                   traverse(L, h, w, xh, yh, found, lastx-1, lasty, macrono, dx, dy, down, lastx, lasty,soft,index_2,temp);
           }
        }// end of down traversal
    } // end of dpos at L
    
    if (maxdpos==U)
    {
        traverse(R, h, w, xh, yh, found, maxdx, maxdy+1, macrono, dx+1, dy, up, lastx, lasty,soft,index_2,temp);
        if (found!=true)
        {
            traverse(D, h, w, xh, yh, found,lastx, lasty-1, macrono, dx+1, dy, up, lastx, lasty,soft,index_2,temp);
            if (found!=true)
            {
                traverse(L, h, w, xh, yh, found,lastx-1, lasty, macrono, dx+1, dy, up, lastx, lasty,soft,index_2,temp);
                if (found!=true)
                    traverse(U, h, w, xh, yh, found,lastx, lasty+1, macrono, dx+1, dy, up, lastx, lasty,soft,index_2,temp);
            }
        }// end of up traversal
        
        traverse(L, h, w, xh, yh, found, maxdx-1, maxdy+1, macrono, dx, dy, down, lastx, lasty,soft,index_2,temp);
        if (found!=true)
        {
            traverse(D, h, w, xh, yh, found, lastx, lasty-1, macrono, dx, dy, down, lastx, lasty,soft,index_2,temp);
            if (found!=true)
            {
                traverse(R, h, w, xh, yh, found, lastx+1, lasty, macrono, dx, dy, down, lastx, lasty,soft,index_2,temp);
                if (found!=true)
                    traverse(U, h, w, xh, yh, found, lastx, lasty+1, macrono, dx, dy, down, lastx, lasty,soft,index_2,temp);
            
            }
        }// end of down traversal
    }// maxdpos at U
    
    if (soft==1)
    {
        swap(dx, maxdx);
        swap(dy, maxdy);
        swap(dpos, maxdpos);
    }
}

//void soft_macro(int soft1, int soft2, int xs1, int ys1, int xs2, int ys2, int leftx, int rightx, int uppery, int lowery, int xtoedge, int ytoedge, dposition& dpos, dposition& maxdpos, double& dminx, double& dminy, double& maxdx, double& maxdy, int originy1, int originx1, int h1, int w1, int h2, int w2, int originx2, int originy2)
//{
//    bool flag;
//
//    solve_quations(xs1, ys1, xs2, ys2, leftx, rightx, uppery, lowery, xtoedge, ytoedge, dpos, maxdpos, dminx, dminy,  maxdx,  maxdy);
//    for(int k=0; k<pins_soft[soft1].size(); k++)
//    {
//        if (dpos==R)
//        {
//            flag=freecells(L, soft1, h1, w1, originx1,  originy1, pins_soft[soft1][k].value);
//            if(!flag)
//                flag=freecells(D, soft1, h1, w1, originx1,  originy1, pins_soft[soft1][k].value);
//            if (!flag)
//                flag=freecells(<#dposition dir#>, <#int softmacro#>, <#int h#>, <#int w#>, <#int originx#>, <#int originy#>, <#int val#>)
//        }
//        else if (dpos==U)
//        {
//
//        }
//        else if (dpos==L)
//        {
//
//        }
//        else if (dpos==D)
//        {
//
//        }
//    }
//
//
//}
//---------------------------------------------------------------------------------
int main(int argc, const char * argv[]) {
  //  int macrosn=0;

    int x=0;
    int y=0;
    int h=0;
    int w=0;
    int z=0;
    int connectedto=0;
    int pins_num=0;
    int xh;
    int yh;
    int xs;
    int ys;
    int rightx;
    int uppery;
    int xtoedge;
    int ytoedge;
    dposition dmax_pos;
    
    initialize_grid();

    cout << " enter meatl layer 1 direction 0 for vertical and one for horizontal "<< endl;
    cin >> M1;
    cout << "enter the dircetion of metal layer 2 0 for vetical and 1 for horizontal" << endl;
    cin >> M2;
    
    cout << "enter the number of hard macros "<< endl;
    cin >> hard_macrosn;
    cout << "enter the numebr of soft macros "<< endl;
    cin>> soft_macrosn;
    
    for (int i=0; i<hard_macrosn; i++)
    {
        cout << "enter the coordinates of the  hard macros x,y,h,w";
        cin>>x;
        cin>>y;
        cin>>h;
        cin>>w;
        
        vec.push_back(x);
        vec.push_back(y);
        vec.push_back(h);
        vec.push_back(w);
        vec.push_back(i);
        hard_macrosC.push_back(vec);
        initialize_int_vec(vec);

        cout << "enter the number of pins for this macro" << endl;
        cin >> pins_num;
        
        cout<<"enter the coordinates of these pins x,y,z" << endl;
        
        for (int j=0;j<pins_num;j++)
        {
            pin temp;
            cin>>x;
            cin>>y;
            cin>>z;
            temp.x=x;
            temp.y=y;
            temp.z=z;
            cout << " enter the number of the macro this pin is connected to " << endl;
            cin>> connectedto;
            temp.to=connectedto;
            temp_vec.push_back(temp);
            grid[x][y].value=pins_count;
            grid[x][y].connected=connectedto;
            pins_count++;
        }
       // pins_map.insert(pair <int,vector<pin>>(i,temp_vec));
        pins_map.push_back(temp_vec);
        initialize_vec();
    
    }



   
    for (int i=0; i<soft_macrosn; i++)
    {
        cout << "Enter the coordinates of the soft macros" << endl;

        cin>>x;
        cin>>y;
        cin>>h;
        cin>>w;
        vec.push_back(x);
        vec.push_back(y);
        vec.push_back(h);
        vec.push_back(w);
        vec.push_back(i);
        soft_macrosC.push_back(vec);
         initialize_int_vec(vec);
        
        cout << "enter the number of pins for this macro" << endl;
        cin >> pins_num;
        
        for (int j=0; j<pins_num; j++)
        {
            pin temp;
//            cin>>x;
//            cin>>y;
//            cin>>z;
            temp.x=0;
            temp.y=0;
            temp.z=0;
            cout << " enter the number of the macro this pin is connected to " << endl;
            cin>> connectedto;
            temp.to=connectedto;
            temp.value=pins_count;
            temp_vec.push_back(temp);
            pins_count++;
        }
        pins_soft.push_back(temp_vec);
        initialize_vec();
    }
    
    //------------------ end of taking the input-----------------------------
    
    // ------------------start of handling the connection---------------------
    for (int i=0; i<hard_macrosn;i++)
    {
        for (int j=0; j<soft_macrosn;j++)
        {
            
            xh=hard_macrosC[i][0]+(hard_macrosC[i][3]/2);
            yh=hard_macrosC[i][1]+(hard_macrosC[i][2]/2);
    
            xs=soft_macrosC[j][0]+(soft_macrosC[j][3]/2);
           
            ys=soft_macrosC[j][1]+(soft_macrosC[j][2]/2);
            rightx=hard_macrosC[i][0]+hard_macrosC[i][3];
            uppery=hard_macrosC[i][1]+hard_macrosC[i][2];
            xtoedge=(hard_macrosC[i][3]/2);
            ytoedge=(hard_macrosC[i][2]/2);

            handle_hard_macros(hard_macrosC[i][2],hard_macrosC[i][3], xh, yh, xs, ys, hard_macrosC[i][0], rightx, uppery, hard_macrosC[i][1],xtoedge, ytoedge, j+1, dmax_pos,0,0);
            
            placement(j, soft_macrosC[j][2],soft_macrosC[j][3] , soft_macrosC[j][0], soft_macrosC[j][1], xh, yh,  xs,  ys,  dmax_pos,0);
            
//            cout << "up elements ";
//            while(!up.empty())
//            {
//                s=up.top();
//                cout<<s;
//                up.pop();
//
//            }
//            cout << "down elements";
//            while (!down.empty())
//            {
//                s=down.top();
//                cout<<s;
//                down.pop();
//           }
//            for (int i=15; i>=0; i--){
//                for (int j=0;j<15; j++)
//                {
//                    cout <<"     " << grid[j][i].value << "   " ;
//                }
//            cout<< endl;
//              }
        }
    }
// handling soft macros
    
    
    
    for (int i=0; i<soft_macrosn; i++)
    {
        for (int j=i+1; j<soft_macrosn; j++)
        {
            xh=soft_macrosC[i][0]+(soft_macrosC[i][3]/2);
            yh=soft_macrosC[i][1]+(soft_macrosC[i][2]/2);
            
            xs=soft_macrosC[j][0]+(soft_macrosC[j][3]/2);
            
            ys=soft_macrosC[j][1]+(soft_macrosC[j][2]/2);
            rightx=soft_macrosC[i][0]+soft_macrosC[i][3];
            uppery=soft_macrosC[i][1]+soft_macrosC[i][2];
            xtoedge=(soft_macrosC[i][3]/2);
            ytoedge=(soft_macrosC[i][2]/2);
            
            handle_hard_macros(soft_macrosC[i][2],soft_macrosC[i][3], xh, yh, xs, ys, soft_macrosC[i][0], rightx, uppery, soft_macrosC[i][1],xtoedge, ytoedge, j+1, dmax_pos,1,i);
            placement(j, soft_macrosC[j][2],soft_macrosC[j][3] , soft_macrosC[j][0], soft_macrosC[j][1], xh, yh,  xs,  ys,  dmax_pos, 1);
            
        }
        
 
        
    }
    
    for (int i=15; i>=0; i--){
        for (int j=0;j<15; j++)
        {
            cout <<"    " << grid[j][i].value << "   " ;
        }
        cout<< endl;
    }
    
//    for (int i=0; i<pins_hard.size(); i++)
//    {
//        for (int j=0; j<pins_hard[i].size();j++)
//        {
//            cout << " hard macros output x, y, z, value" << endl;
//            cout <<pins_hard[i][j].x <<endl;
//            cout <<pins_hard[i][j].y <<endl;
//            cout <<pins_hard[i][j].z <<endl;
//            cout <<pins_hard[i][j].value <<endl;
//        }
//    }
//    
//    for (int i=0; i<pins_soft.size(); i++)
//    {
//        for (int j=0; j<pins_soft[i].size();j++)
//        {
//            cout << " soft macros output x, y, z, value" << endl;
//            cout <<pins_soft[i][j].x <<endl;
//            cout <<pins_soft[i][j].y <<endl;
//            cout <<pins_soft[i][j].z <<endl;
//            cout<<pins_soft[i][j].to << endl;
//            cout <<pins_soft[i][j].value <<endl;
//        }
//    }

    return 0;
}
