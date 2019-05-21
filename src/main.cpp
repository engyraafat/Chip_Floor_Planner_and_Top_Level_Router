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

void split(string str, vector<string>& vec)
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

gridpt grid [6000][6000];

struct pin
{
    int x;
    int y;
    int z;
    int to;
    int v;
    int value;
    string name;
    string to_name;
};

vector<string> hard_names;
vector<string> soft_names;
vector<vector<int>> hard_macrosC;
vector<vector<int>> soft_macrosC;
vector<vector<string>> temp_net;
vector<vector<pin>> pins_map;

vector<vector<pin>> pins_soft;
vector<vector<pin>> pins_soft_2;
vector<pin> temp_vec;
vector<vector<pin>> pins_hard; // sontains the soft pins connected to hard pins

int pins_count=1;
int soft_macrosn=0;
int hard_macrosn=0;
//vector<string> vec_string;

int find_index_soft(string name) // won't be connected to anything but softmacros
{
    for (int i=0; i<soft_names.size();i++)
    {
        if(soft_names[i]==name)
        {
            return i;
            
        }
    }

    
    
    return -1;
}

void initialize_int_vec(vector<int>& vec)
{
    while (!vec.empty())
        vec.pop_back();
}
void initialize_string_vec(vector<string>& vec)
{
    while (!vec.empty())
        vec.pop_back();
}
void initialize_vec()
{
    while (!temp_vec.empty())
        temp_vec.pop_back();
}
void parsing_gl()
{
    ifstream input;
    ofstream output;
    int grid_width;
    int grid_height;
    int len;
    int i;
    int pos;
    vector<string> vec;
    vector<int> vec_coordinates;
    string line_1;
    string str;
    pin temp;
    int macro;
    
    
    input.open("test_g_2.txt");
    if (!input.fail())
    {
        string line;
        
        while(!input.eof())
        {
            getline(input, line);
            // len=line.length();
            split(line, vec);
            //line_1=line.substr(0, 3);
           
            if(vec[0]=="M1")
            {
                if(vec[1]=="H")
                {
                     M1=1;
                }
                else M1=0;
                initialize_string_vec(vec);
            }
            else if(vec[0]=="M2")
            {
                if(vec[1]=="H")
                {
                    M2=1;
                }

                else M2=0;
                initialize_string_vec(vec);
            }

           
           else if (vec[0]!="name:" && vec[0]!="pins:")
            {
               // pos=line.find(":");
                soft_names.push_back(vec[0]);
                vec_coordinates.push_back(stoi(vec[1])); // x coordinates
                vec_coordinates.push_back(stoi(vec[2])); //y coordinates
                
                vec_coordinates.push_back(stoi(vec[3]));//h
                vec_coordinates.push_back(stoi(vec[4]));//w
                soft_macrosC.push_back(vec_coordinates);
                initialize_string_vec(vec);
                initialize_int_vec(vec_coordinates);
                soft_macrosn++;
                
                getline(input, line);
                while(line!=";")
                {
                    if (line!="pins:")
                    {
                    split(line, vec);
                    temp.x=0;
                    temp.y=0;
                    temp.z=0;
                   // macro=find_index_soft(vec[9]);
                    temp.to_name= vec[3];
                    temp.value=pins_count;
                    temp.v=0;
                    temp.name=vec[1];
                    temp_vec.push_back(temp);
                    initialize_string_vec(vec);
                   // grid[temp.x][temp.y].value=pins_count;
                    //grid[temp.x][temp.y].connected=temp.to;
                    pins_count++;
                    }
                    getline(input, line);
                }
                if (temp_vec.size()>0)
                pins_soft.push_back(temp_vec);
                initialize_vec();
            }
        }
    }
    else cout << "failed to open gl file" << endl;

}
void parsing_floor_out()
{
   ifstream input;
   ofstream output;
    int grid_width;
    int grid_height;
    int len;
    int i;
    int pos;
    vector<string> vec;
    vector<int> vec_coordinates;
    string line_1;
    string str;
    pin temp;
    int macro;
    

input.open("test_f_2.txt");
if (!input.fail())
{
    string line;

    while(!input.eof())
    {
        getline(input, line);
       // len=line.length();
        split(line, vec);
        
        //line_1=line.substr(0, 3);
        if (vec[0][0]=='W')
        {
            grid_width=stoi(line.substr(1,len-2));
            cout << "grid width" <<grid_width << endl;
        }
        else if (vec[0][0]=='H')
        {
            grid_height=stoi(line.substr(1, len-2));
            cout << "grid height" <<grid_height << endl;
        }
        
        else if(vec[0]!="name:" && vec[0]!="pins:")
        {
            //pos=line.find(":");
            hard_names.push_back(vec[0]); // taking the coordinates and the names
            
            vec_coordinates.push_back(stoi(vec[1])); // x coordinates
            vec_coordinates.push_back(stoi(vec[2])); //y coordinates
            
            vec_coordinates.push_back(stoi(vec[3]));
            vec_coordinates.push_back(stoi(vec[4]));
            hard_macrosC.push_back(vec_coordinates);
            initialize_string_vec(vec);
            initialize_int_vec(vec_coordinates);
            hard_macrosn++;
            
            getline(input, line);
            
            while(line!=";" )
            {
                if (line!="pins:")
                {
                    split(line, vec);
                    temp.x=stoi(vec[5]);
                    temp.y=stoi(vec[7]);
                    str=vec[3][5];
                    temp.z=stoi(str);
                    //macro=find_index_soft(vec[9]);
                    //temp.to= macro;
                    temp.to_name= vec[9];
                    temp.name=vec[1];
                    temp.value=pins_count;
                    temp.v=0;
                    temp_vec.push_back(temp);
                    initialize_string_vec(vec);
                    grid[temp.x][temp.y].value=pins_count;
                    //grid[temp.x][temp.y].connected=temp.to;
                    pins_count++;
                    
                }
                getline(input, line);
            }
            if (temp_vec.size()>0)
            pins_map.push_back(temp_vec);
            initialize_vec();
        }
    }
  }
else cout << "failed to open floor planner file" << endl;

}

void final_parsing()
{
    int macro;
    parsing_floor_out();
    parsing_gl();
    for (int i=0; i<pins_soft.size(); i++)
    {
        for (int j=0; j<pins_soft[i].size();j++)
        {
            macro=find_index_soft(pins_soft[i][j].to_name);
            pins_soft[i][j].to=macro+1;
            grid[pins_soft[i][j].x][pins_soft[i][j].y].connected=macro+1;
        }
    }
    
    for (int i=0; i<pins_map.size(); i++)
    {
        for (int j=0; j<pins_map[i].size();j++)
        {
            macro=find_index_soft(pins_map[i][j].to_name);
            pins_map[i][j].to=macro+1;
            grid[pins_map[i][j].x][pins_map[i][j].y].connected=macro+1;
           // cout << "wrote on grid " << macro+1 << endl;
        }
    }
}
//
//vector<vector<pin>> pins_map(hard_macrosn);
//
//struct coordinates
//{
//    int x;
//    int y;
//};
//vector<pin> pins;


void output()
{
    ofstream output ("out_2.txt", ios::app);
    //ofstream.open("out.txt");
    if (!output)
        cout <<"output file failed to open"<< endl;
    
    for (int i=0; i<hard_names.size();i++)
    {
        output<<"L:"<<hard_macrosC[i][2]<<endl;
        output<<"W:"<<hard_macrosC[i][3]<<endl;
        for(int j=0; j<pins_map[i].size();j++)
        {
            output<<"( " <<pins_map[i][j].x <<" "<<pins_map[i][j].y << " "<< pins_map[i][j].z<< " )" << endl;
//                    for(int s=0; s<pins_hard.size();j++)
//                    {
//                        for (int k=0; k<pins_hard[k].size();k++)
//                        {
//                            if (pins_hard[s][k].value==pins_map[i][j].value)
//                            {
//                              output<<"( " <<pins_hard[i][j].x <<" "<<pins_map[i][j].y << " "<< pins_map[i][j].z<< " )" << endl;
//                            }
//                        }
//
//
//
        }
    }
    
    for (int i=0; i<soft_names.size(); i++)
    {
        output<<"L:"<<soft_macrosC[i][2]<<endl;
        output<<"W:"<<soft_macrosC[i][3]<<endl;
        for(int j=0; j<pins_soft[i].size();j++) // looping on current macro
        {
            if(i<pins_soft.size())
            {
           output<<"( " <<pins_soft[i][j].x <<" "<<pins_soft[i][j].y << " "<< pins_soft[i][j].z<< " )" << endl;
            }
            for(int s=0; s<pins_soft_2.size();s++) // looping on all other to check if connected to this macro
            {
                for (int k=0; k<pins_soft_2[k].size();k++)
                {
                    if (pins_soft_2[s][k].to==i+1)
                    {
                        output<<"( " <<pins_soft_2[s][k].x <<" "<<pins_soft_2[s][k].y << " "<< pins_soft_2[s][k].z<< " )" << endl;
                        
                        cout << i << s << endl;
                    }
                }
            }
        }
    
    }

    
    
}

void output_macros()
{
    ofstream output ("out_macros.txt", ios::app);
    //ofstream.open("out.txt");
    if (!output)
        cout <<"output file failed to open"<< endl;
    else{
        for (int i=0; i <hard_macrosC.size(); i++)
            output<< hard_macrosC[i][0]<< "," << hard_macrosC[i][1]+hard_macrosC[i][2] << "," << hard_macrosC[i][2]<< "," << hard_macrosC[i][3]<<endl;
        
        for (int i=0; i <soft_macrosC.size(); i++)
            output<< soft_macrosC[i][0]<< "," << soft_macrosC[i][1]+soft_macrosC[i][2] << "," << soft_macrosC[i][2]<< "," << soft_macrosC[i][3]<<endl;
    }
}


bool looping(dposition dir, int softmacro, int h_soft, int w_soft, int originx, int originy, stack<int> &used, int soft)
{
    int x=0;
    long s=0;
    int start;
    int z;
    pin temp;
    if (dir==R)
    {
        
        if(used.size()<h_soft)
            start=originy+h_soft-used.size()+1;
        else start=originy;
        cout << "used size" <<used.size()<<endl;
        cout<< "start y"<<start << endl;
        for(int i=originy; i<=originy+h_soft; i++)
        {
            x=originx+w_soft;
            cout << " R_for" << endl;
            if(grid[x+1][i].value==-1)
            {
                cout << " R _if" << endl;
                if(!used.empty())
                {
                    s=used.top();
                    grid[x+1][i].value=s;
                    temp.x=x+1;
                    temp.y=i;
                    if(M1==0)
                        z=1;
                    else z=2;
                    temp.z=z;
                    temp.to=softmacro+1;
                    temp.value=s;// didn't push back to
                    
                    temp_vec.push_back(temp);
                    if(soft==0)
                    {
                        pins_hard.push_back(temp_vec);
                        initialize_vec();
                    }
                    else{
                        pins_soft_2.push_back(temp_vec);
                        initialize_vec();
                    }
                    cout << "pushing back s"<< s <<endl;
                    cout << "dir " << R << endl;
                    
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
        for(int i=originy; i<=originy+h_soft; i++)
        {
            x=originx;
            cout << " L_for" << endl;
            cout << "grid value " << grid[x-1][i].value<< endl;
            cout << "x value " << x-1 <<endl;
            if(grid[x-1][i].value==-1)
            {
                cout << " L_if" << endl;
                
                if(!used.empty())
                {
                    s=used.top();
                    grid[x-1][i].value=s;
                    temp.x=x-1;
                    temp.y=i;
                    if(M1==0)
                        z=1;
                    else z=2;
                    temp.z=z;
                    temp.value=s;// didn't push back to
                    temp_vec.push_back(temp);
                    temp.to=softmacro+1;
                    if(soft==0)
                    {

                        pins_hard.push_back(temp_vec);
                        initialize_vec();
                    }
                    else{
                        pins_soft_2.push_back(temp_vec);
                        initialize_vec();
                    }
                    cout << "pushing back s"<< s <<endl;
                    cout << "dir " << L << endl;
                    cout<<"originx" << originx << endl;
                    cout<< "orgin x + w_soft"<< originx+w_soft<< endl;
                    cout<< "wdith "<< w_soft;
                    cout << " x " <<x<< " i "<< i << "origin y "<< originy<< endl;
                    cout << "start y" << start << endl;
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
        for(int i=originx; i<=w_soft; i++)
        {
            x=originx;
            cout << " u_for" << endl;
            
            if(grid[x+i][originy+h_soft+1].value==-1)
            {
                cout << " u_if" << endl;
                if(!used.empty()){
                    s=used.top();
                    grid[x+i][originy+h_soft+1].value=s;
                    temp.x=x+i;
                    temp.y=originy+h_soft+1;
                    if(M1==1)
                        z=1;
                    else z=2;
                    temp.z=z;
                    temp.value=s;// didn't push back to
                    temp.to=softmacro+1;
                    temp_vec.push_back(temp);
                    if (soft==0){

                        
                        pins_hard.push_back(temp_vec);
                        initialize_vec();
                    }
                    else{
                        pins_soft_2.push_back(temp_vec);
                        initialize_vec();
                    }
                    cout << "pushing back s"<< s <<endl;
                    cout << "dir " << U << endl;
                    cout<<"originx" << originx << endl;
                    cout<< "orgin x+w_soft"<< originx+w_soft<< endl;
                    cout<< "wdith "<< w_soft;
                    cout << " x " <<x<< " i "<< i << "origin y "<< originy<< endl;
                    cout << "start x" << start << endl;
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
            cout << " D _for" << endl;
            if(grid[x+i][originy-1].value==-1)
            {
                cout << " D_if" << endl;
                if(!used.empty()){
                    cout << "D_if_used" << endl;
                    s=used.top();
                    grid[x+i][originy-1].value=s;
                    temp.x=x+i;
                    temp.y=originy-1;
                    if(M1==1)
                        z=1;
                    else z=2;
                    temp.z=z;
                    temp.value=s;// didn't push back to
                    temp.to=softmacro+1;
                    temp_vec.push_back(temp);
                    if(soft==0){

                        pins_hard.push_back(temp_vec);
                        cout << "pushing back s"<< s <<endl;
                        cout << "dir " << R << endl;
                        initialize_vec();
                        
                    }
                    
                    else{
                        pins_soft_2.push_back(temp_vec);
                        initialize_vec();
                    }
                    
                    cout << "pushing back s"<< s <<endl;
                    cout << "dir " << D << endl;
                    cout<<"originx" << originx << endl;
                    cout<< "orgin x+w_soft"<< originx+w_soft<< endl;
                    cout<< "wdith "<< w_soft;
                    cout << " x " <<x<< " i "<< i << "origin y "<< originy<< endl;
                    cout << "start x" << start << endl;
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
            cout << " R_ reverse _for" << endl;
            if(grid[x+1][i].value==-1)
            {
                cout << " R_ reverse _if" << endl;
                if(!used.empty())
                {
                    s=used.top();
                    grid[x+1][i].value=s;
                    temp.x=x+1;
                    temp.y=i;
                    if(M1==0)
                        z=1;
                    else z=2;
                    temp.z=z;
                    temp.to=softmacro+1;
                    temp.value=s;// didn't push back to
                    temp_vec.push_back(temp);
                    
                    if(soft==0)
                    {
                        cout << "pushing back s"<< s <<endl;
                        cout << "dir " << R << endl;
                        pins_hard.push_back(temp_vec);
                        initialize_vec();
                        
                    }
                    else{
                    pins_soft_2.push_back(temp_vec);
                    initialize_vec();
                    }
                    cout << "pushing back s"<< s <<endl;
                    cout << "dir reverse" << R << endl;
                    cout<<"originx" << originx << endl;
                    cout<< "orgin x + w_soft"<< originx+w_soft<< endl;
                    cout<< "wdith "<< w_soft;
                    cout << " x " <<x<< " i "<< i << "origin y "<< originy<< endl;
                    cout << "start x" << originy+h_soft << endl;
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
            cout << " L_ reverse _for" << endl;
            if(grid[x-1][i].value==-1)
            {
                cout << " L_ reverse _if" << endl;
                if(!used.empty())
                {
                    s=used.top();
                    grid[x-1][i].value=s;
                    temp.x=x-1;
                    temp.y=i;
                    if(M1==1)
                        z=1;
                    else z=2;
                    temp.z=z;
                    temp.value=s;// didn't push back to
                    temp.to=softmacro+1;
                    temp_vec.push_back(temp);
                    if(soft==0)
                    {

                        cout << "pushing back s"<< s <<endl;
                        cout << "dir " << R << endl;
                        pins_hard.push_back(temp_vec);
                        initialize_vec();
                        
                    }
                    else{
                        pins_soft_2.push_back(temp_vec);
                        initialize_vec();
                    }
                    cout << "pushing back s"<< s <<endl;
                    cout << "dir reverse" << L << endl;
                    cout<<"originx" << originx << endl;
                    cout<< "orgin x+w_soft"<< originx+w_soft<< endl;
                    cout<< "wdith "<< w_soft;
                    cout << " x " <<x<< " i "<< i << "origin y "<< originy<< endl;
                    cout << "start y" << originy+h_soft << endl;
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
            cout << " u_ reverse _for" << endl;
            if(grid[i][originy+h_soft+1].value==-1)
            {
                cout << " u_ reverse _if" << endl;
                if(!used.empty())
                {
                    s=used.top();
                    grid[i][originy+h_soft+1].value=s;
                    temp.x=x+1;
                    temp.y=i;
                    if(M1==1)
                        z=1;
                    else z=2;
                    temp.z=z;
                    temp.value=s;// didn't push back to
                    temp.to=softmacro+1;
                    temp_vec.push_back(temp);
                    if(soft==0)
                    {

                        cout << "pushing back s"<< s <<endl;
                        cout << "dir " << R << endl;
                        pins_hard.push_back(temp_vec);
                        initialize_vec();
                        
                    }
                    else{
                        pins_soft_2.push_back(temp_vec);
                        initialize_vec();
                    }
                    cout << "pushing back s"<< s <<endl;
                    cout << "dir reverse" << U << endl;
                    cout<<"originx" << originx << endl;
                    cout<< "orgin x+w_soft"<< originx+w_soft<< endl;
                    cout<< "wdith "<< w_soft;
                    cout << " x " <<x<< " i "<< i << "origin y "<< originy<< endl;
                    cout << "start x" << originx+w_soft << endl;
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
            cout << " D_ reverse _for" << endl;
            if(grid[i][originy-1].value==-1)
            {
                cout << " D_ reverse _if" << endl;
                if(!used.empty())
                {
                    s=used.top();
                    grid[i][originy-1].value=s;
                    temp.x=x+1;
                    temp.y=i;
                    if(M1==1)
                        z=1;
                    else z=2;
                    temp.z=z;
                    temp.value=s;// didn't push back to
                    temp.to=softmacro+1;
                    temp_vec.push_back(temp);
                    if(soft==0)
                    {

                        cout << "pushing back s"<< s <<endl;
                        cout << "dir " << R << endl;
                        pins_hard.push_back(temp_vec);
                        initialize_vec();
                    }
                    else{
                        pins_soft_2.push_back(temp_vec);
                        initialize_vec();
                    }
                    cout << "pushing back s"<< s <<endl;
                    cout << "dir reverse" << D << endl;
                    cout<<"originx" << originx << endl;
                    cout<< "orgin x+w_soft"<< originx+w_soft<< endl;
                    cout<< "wdith "<< w_soft;
                    cout << " x " <<x<< " i "<< i << "origin y "<< originy<< endl;
                    cout << "start x" << originx+w_soft << endl;
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
    cout << "placement d pos " << dmax_pos << endl;
    cout << "placement up size" << up.size()<<endl;
    cout << "placement doen size" << down.size()<<endl;
    if (dmax_pos==D)
    {
        if (xs<xh)
        {
            flag=looping(L, softmacro, h_soft, w_soft, originx_soft, originy_soft, up, soft);
            if(!flag) // spaces weren't enough
                looping_reverse(D, softmacro, h_soft, w_soft, originx_soft, originy_soft, up, soft);
            
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
            cout << "entered less than " << endl;
            flag=looping(U, softmacro, h_soft, w_soft, originx_soft, originy_soft, up, soft);
            if(!flag) // spaces weren't enough
                looping_reverse(R, softmacro, h_soft, w_soft, originx_soft, originy_soft, up, soft);
            
            flag=looping_reverse(L, softmacro, h_soft, w_soft, originx_soft, originy_soft, down, soft);
            if(!flag)
                looping(D, softmacro, h_soft, w_soft, originx_soft, originy_soft, down, soft);
            
        }
        else
        {
            cout << "entered greater tahn" << endl;
            flag=looping_reverse(L, softmacro, h_soft, w_soft, originx_soft, originy_soft, up, soft);
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
        
        cout<< "intersection " << dminx << " "<<dminy <<endl;
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
        
        cout<< "intersection " << dminx << " "<<dminy <<endl;
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
        
        cout<< "intersection " << dminx << " "<<dminy <<endl;
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
        cout<< "intersection " << dminx << " "<<dminy <<endl;
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
                    if(pins_soft[index].size()>0 && index<pins_soft.size()){
                        if(pins_soft[index][pins_index].to==macrono && pins_soft[index][pins_index].v==0 && pins_soft[index][pins_index].value!=0)
                        {
                            if (pins_soft[index][pins_index].value!=0){
                                grid[startingx-i][startingy].value=pins_soft[index][pins_index].value;
                                used.push(grid[startingx-i][startingy].value);
                                pins_soft[index][pins_index].x=startingx-i;
                                pins_soft[index][pins_index].y=startingy;
                                pins_soft[index][pins_index].v=1;
                                if(M1==0)// m1 is vertical
                                    pins_soft[index][pins_index].z=1;
                                else
                                    pins_soft[index][pins_index].z=2;
                                
                                
                                cout << "pins index" << pins_index << endl;
                                cout<< "index " << index << endl;
                                cout<<"macrono"<< macrono<<endl;
                                cout<<"starting x"<< startingx-i<<endl;
                                cout << "pushed " << pins_soft[index][pins_index].value << endl;
                                cout << "to " << pins_soft[index][pins_index].to << endl;
                            }
       
                        }
                        pins_index++;
                        
                    }
                }
            }
            else
                if(grid[startingx-i][startingy].value!=-1)
                {
                    
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
                if(grid[startingx+i][startingy].value==-1){
                if(pins_soft[index].size()>0 && index<pins_soft.size()){
                
                    {
                        if(pins_soft[index][pins_index].to==macrono && pins_soft[index][pins_index].v==0 && pins_soft[index][pins_index].value!=0)
                        {
                            grid[startingx+i][startingy].value=pins_soft[index][pins_index].value;
                            used.push(grid[startingx+i][startingy].value);
                            pins_soft[index][pins_index].x=startingx+i;
                            pins_soft[index][pins_index].y=startingy;
                            pins_soft[index][pins_index].v=1;
                            if(M1==0)// m1 is vertical
                                pins_soft[index][pins_index].z=1;
                            else
                                pins_soft[index][pins_index].z=2;
                            
                            cout << "pins index" << pins_index << endl;
                            cout<< "index " << index << endl;
                            cout << "pushed " << pins_soft[index][pins_index].value << endl;
                            cout << "macro no "<< macrono << endl;
                            cout << "to " << pins_soft[index][pins_index].to << endl;
                            
                        }
                        pins_index++;
                    }
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
                    if(pins_soft[index].size()>0 && index<pins_soft.size()){
                        if(pins_soft[index][pins_index].to==macrono && pins_soft[index][pins_index].v==0 && pins_soft[index][pins_index].value!=0)
                        {
                            grid[startingx][startingy+i].value=pins_soft[index][pins_index].value;
                            used.push(grid[startingx][startingy+i].value);
                            pins_soft[index][pins_index].x=startingx;
                            pins_soft[index][pins_index].y=startingy+i;
                            pins_soft[index][pins_index].v=1;
                            if(M1==1)// m1 is vertical
                                pins_soft[index][pins_index].z=1;
                            else
                                pins_soft[index][pins_index].z=2;
                            
                            cout << "pins index" << pins_index << endl;
                            cout<< "index " << index << endl;
                            cout << "pushed " << pins_soft[index][pins_index].value << endl;
                            cout << "macro no "<< macrono << endl;
                            cout << "to " << pins_soft[index][pins_index].to << endl;
                            
                        }
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
                    if(pins_soft[index].size()>0 && index<pins_soft.size()){
                        if(pins_soft[index][pins_index].to==macrono && pins_soft[index][pins_index].v==0 && pins_soft[index][pins_index].value!=0)
                        {
                            grid[startingx][startingy-i].value=pins_soft[index][pins_index].value;
                            used.push(grid[startingx][startingy-i].value);
                            pins_soft[index][pins_index].x=startingx;
                            pins_soft[index][pins_index].y=startingy-i;
                            pins_soft[index][pins_index].v=1;
                            if(M1==1)// m1 is vertical
                                pins_soft[index][pins_index].z=1;
                            else
                                pins_soft[index][pins_index].z=2;
                            // pins_index++;
                            cout << "pins index" << pins_index << endl;
                            cout<< "index " << index << endl;
                            cout << "pushed " << pins_soft[index][pins_index].value << endl;
                            cout << "macro no "<< macrono << endl;
                            cout << "to " << pins_soft[index][pins_index].to << endl;
                        }
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
        traverse(U, h, w, xh, yh, found, maxdx+1, maxdy, macrono, dx-1, dy+1, up, lastx, lasty, soft,index_2,temp);
        cout << lastx<<endl;
        cout << lasty<< endl;
        if (found!=true)
        {
            traverse(L, h, w, xh, yh, found, lastx-1, lasty, macrono, dx-1, dy+1, up, lastx, lasty,soft,index_2,temp);
            cout << lastx<<endl;
            cout << lasty<< endl;
            if(found!=true)
            {
                traverse(D, h, w, xh, yh, found, lastx-1, lasty-1, macrono, dx-1, dy+2, up, lastx, lasty,soft,index_2,temp);
                cout << lastx<<endl;
                cout << lasty<< endl;
            }
        }
        
        
        
        //traversing from dmin to dmax
        traverse(D,h,w,xh,yh,found,maxdx+1, maxdy-1,macrono,dx-1, dy+1,down,lastx,lasty,soft,index_2, temp);
        {
            if (found!=true)
            {
                traverse(L,h,w,xh,yh,found,lastx-1,lasty,macrono,dx-1, dy+1,down,lastx,lasty,soft,index_2,temp);
                if (found!=true)
                {
                    traverse(U,h,w,xh,yh,found,lastx-1,lasty+1,macrono,dx-1, dy+1,down,lastx,lasty,soft,index_2,temp);
                    
                }
                
            }
        }
        
        
    }
    // end of maxdos at R
    if(maxdpos==D) //-1 in maxdy to loop around the macro
    {
        temp=0;
        traverse(R, h, w, xh, yh, found, maxdx, maxdy-1, macrono, dx+1, dy-1, up, lastx, lasty,soft,index_2,temp);
        if(found!=true)
        {
            traverse(U, h, w, xh, yh, found, lastx, lasty+1, macrono, dx+1, dy, up, lastx, lasty,soft,index_2,temp);
            if (found!=true)
            {
                traverse(L, h, w, xh, yh, found, lastx-1, lasty+1, macrono, dx+1, dy+1, up, lastx, lasty,soft,index_2,temp);
            }
        } /// traversed up
        // dy-1 to account en we loop up
        traverse(L, h, w, xh, yh, found, maxdx-1, maxdy-1, macrono, dx, dy-1, down, lastx, lasty,soft,index_2,temp);
        if(found!=true)
        {
            traverse(U, h, w, xh, yh, found, lastx, lasty+1, macrono, dx, dy, down, lastx, lasty,soft,index_2,temp);
            if (found!=true)
            {
                traverse(R, h, w, xh, yh, found, lastx+1, lasty+1, macrono, dx, dy+1, down, lastx, lasty,soft,index_2,temp);
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
                traverse(D, h, w, xh, yh, found, lastx+1, lasty-1, macrono, dx+1, dy+1, up, lastx, lasty,soft,index_2,temp);
            }
        }// traversed up
        //start of down traversal
        traverse(D, h, w, xh, yh, found, maxdx-1, maxdy-1, macrono, dx, dy+1, down, lastx, lasty,soft,index_2,temp);
        if(!found)
        {
            traverse(R, h, w, xh, yh, found, lastx+1, lasty, macrono, dx, dy+1, down, lastx,lasty,soft,index_2,temp);
            if(found!=true)
            {
                traverse(U, h, w, xh, yh, found, lastx+1, lasty+1, macrono, dx+1, dy+1, down, lastx, lasty,soft,index_2,temp);
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
                traverse(L, h, w, xh, yh, found,lastx-1, lasty-1, macrono, dx+1, dy-1, up, lastx, lasty,soft,index_2,temp);
                
            }
        }// end of up traversal
        
        traverse(L, h, w, xh, yh, found, maxdx-1, maxdy+1, macrono, dx, dy, down, lastx, lasty,soft,index_2,temp);
        if (found!=true)
        {
            traverse(D, h, w, xh, yh, found, lastx, lasty-1, macrono, dx, dy, down, lastx, lasty,soft,index_2,temp);
            if (found!=true)
            {
                traverse(R, h, w, xh, yh, found, lastx+1, lasty-1, macrono, dx, dy-1, down, lastx, lasty,soft,index_2,temp);
                
                
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
    
   final_parsing();
    
    cout << "this is the value of M1" << M1 << endl;
    cout << "this is the value of M2 " << M2<< endl;
        for (int i=0; i<pins_map.size(); i++)
            {
                cout << "hardmacros of " << i << endl;
                for (int j=0; j<pins_map[i].size();j++)
                {
                    cout << " hard macros output x, y, z, value" << endl;
                    cout <<pins_map[i][j].x <<endl;
                    cout <<pins_map[i][j].y <<endl;
                    cout <<pins_map[i][j].z <<endl;
                    cout<<pins_map[i][j].to << endl;
                     cout<<pins_map[i][j].to_name << endl;
                    cout <<pins_map[i][j].value <<endl;
                    cout << endl;
                    
                }
            }
    
            for (int i=0; i<pins_soft.size(); i++)
            {
                cout << "softmacros of " << i << endl;
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
        for (int j=0; j<soft_macrosn; j++)
            if(i!=j)
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


                cout << "grid i " << i <<endl;
                cout << "grid j "<< j << endl;
                cout << "placement xh" << xh << endl;
                cout << "placement yh" << yh << endl;
                cout << "placement xs" << xs << endl;
                cout << "placement ys" << ys << endl;
                cout<<  "placement h_soft" << soft_macrosC[j][2] << endl;
                cout << "placement w_soft" << soft_macrosC[j][3]<<endl;
                cout << "placement originx_soft" << j << "   "<< soft_macrosC[j][0]<<endl;
                cout<< "placement originy_soft"<< j<<"   "<<soft_macrosC[j][1] << endl;

                for (int i=25; i>=0; i--){
                    for (int j=0;j<25; j++)
                    {
                        cout <<" " << grid[j][i].value << " " ;
                    }
                    cout<< endl;
                } cout << endl<<endl<<endl;

                //            cout <<"i " << i << endl;
                //            cout << "j "<<j <<endl;
                //
                //                        cout << "up elements ";
                //                        while(!up.empty())
                //                        {
                //                            s=up.top();
                //                            cout<<s;
                //                            up.pop();
                //
                //                        }
                //                        cout << "down elements";
                //                        while (!down.empty())
                //                        {
                //                            s=down.top();
                //                            cout<<s;
                //                            down.pop();
                //                       }
                //
                placement(j, soft_macrosC[j][2],soft_macrosC[j][3] , soft_macrosC[j][0], soft_macrosC[j][1], xh, yh,  xs,  ys,  dmax_pos, 1);


                cout << "grid i " << i <<endl;
                cout << "grid j "<< j << endl;
                cout << "placement xh" << xh << endl;
                cout << "placement yh" << yh << endl;
                cout << "placement xs" << xs << endl;
                cout << "placement ys" << ys << endl;
                cout<<  "placement h_soft" << soft_macrosC[j][2] << endl;
                cout << "placement w_soft" << soft_macrosC[j][3]<<endl;
                cout << "placement originx_soft" << j << "   "<< soft_macrosC[j][0]<<endl;
                cout<< "placement originy_soft"<< j<<"   "<<soft_macrosC[j][1] << endl;

                for (int i=25; i>=0; i--){
                    for (int j=0;j<25; j++)
                    {
                        cout <<" " << grid[j][i].value << " " ;
                    }
                    cout<< endl;
                } cout << endl<<endl<<endl;

            }

        cout << "up elements ";
        while(!up.empty())
        {
            s=up.top();
            cout<<s;
            up.pop();

        }
        cout << "down elements";
        while (!down.empty())
        {
            s=down.top();
            cout<<s;
            down.pop();
        }



    }

    //    for (int i=15; i>=0; i--){
    //        for (int j=0;j<15; j++)
    //        {
    //            cout <<"    " << grid[j][i].value << "   " ;
    //        }
    //        cout<< endl;
    //    }

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
   output();
    output_macros();
    return 0;
}
