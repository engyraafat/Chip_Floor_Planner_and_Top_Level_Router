#include <ilcplex/ilocplex.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include<string>
#include<vector>
#include<istream>
#include<unordered_map>
#include<math.h>
#include <algorithm>
#include <ctime>
#include<fstream>
#include<time.h>


//#include "lp_lib.h"
//#include <stdio.h>
using namespace std;
struct Pin {
	string name;
	string metal;
	int xcor;
	int ycor;

};
struct HardR {
	string name;
	int height;
	int width = 0;
	int varc = 0;
	double xcor = 0;
	double ycor = 0;
	vector <Pin> pins;
};


struct SoftR {
	string name;
	int area;
	double width;
	double height;
	double xcor;
	double ycor;

};


unordered_map< int, HardR > NewHM;

int legalize(int x, int grid)
{

	float divy, divx;
	float idiffy, idiffx, diffy, diffx;
	int newx;
	divx = x / grid;
	idiffx = x - int(divx)*grid;
	diffx = (grid - idiffx);
	cout << "diff:" << diffx << endl;
	newx = x + diffx;
	return newx;
	cout << "newx:" << newx << endl;

}

bool LegalizeSoft( int a, int w , int h, int & new_w , int & new_h )
{
	int j;
	for (int i = 1; i <= a; i++)
		if (a % i == 0)
		{
			j = a / i;
			if (i < w && j < h)
			{
				new_w = i;
				new_h = j;
				return true;
			}
		}
	return false;

}
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

void DEFParser(string filename, string & designname, int & width, int & height, vector<Pin> & pinv)
{
	ifstream input;
	string txt;
	int counter = 0;
	bool pinsFlag = false;
	bool Npins;
	string pinName;
	string pinLayer;
	int xcor;
	int ycor;
	int x1, y1, x2, y2;
	input.open(filename);

	if (!input.is_open()) {
		cout << "error in opening file\n ";
	}
	while (!input.eof())
	{
		vector<string> components;
		getline(input, txt);
		split(txt, components);
		if (components[0] == "DESIGN")
		{
			designname = components[1];
		}
		else if (components[0] == "DIEAREA")
		{
			if (components.size() == 10)
			{
				x1 = stoi(components[2]);
				y1 = stoi(components[3]);
				x2 = stoi(components[6]);
				y2 = stoi(components[7]);
				width = stoi(components[6]) - stoi(components[2]);
				height = stoi(components[7]) - stoi(components[3]);
			}

			if (components.size() == 18)
			{
				x1 = stoi(components[2]);
				y1 = stoi(components[3]);
				x2 = stoi(components[11]);
				y2 = stoi(components[12]);
				width = stoi(components[10]) - stoi(components[2]);
				height = stoi(components[11]) - stoi(components[3]);
			}
		}
		else if (components[0] == "PINS")
		{
			pinsFlag = true;
			Npins = stoi(components[1]);
		}


		else if (components[0] == "-" && pinsFlag == true)
		{
			pinName = components[1];
		}

		else if (components[0] == "+" && components[1] == "LAYER" && pinsFlag == true)
		{
			pinLayer = components[2];
		}

		else if (components[0] == "+" && components[1] == "PLACED" && pinsFlag == true)
		{
			xcor = stoi(components[3]) - x1;
			ycor = stoi(components[4]) - y1;

			Pin p;
			p.name = pinName;
			p.metal = pinLayer;
			p.xcor = xcor;
			p.ycor = ycor;

			pinv.push_back(p);


		}

		else if (components[0] == "END" && components[1] == "PINS")
		{
			pinsFlag = false;
		}


	}
}

void GetHard(string directory , unordered_map<int , HardR> & outHM , int & count)
{

	ifstream inp;
	inp.open("directory.txt");
	vector<string> files;
	count = 0;
	while (!inp.eof())
	{
		string txt;
		getline(inp, txt);
		files.push_back(txt);
		count += 1;
	}

	inp.close();

	for (int i = 0; i < files.size(); i++)
	{
		string fname = files[i];
		string designname;
		int height;
		int width;
		vector <Pin> pins;
		HardR modu;
		DEFParser(fname, designname, width, height, pins);

		modu.name = designname;
		modu.width = width;
		modu.height = height;
		//modu.pins = pins;

		for (int i = 0; i < pins.size(); i++)
		{
			Pin temp;
			temp.metal = pins[i].metal;
			temp.name = pins[i].name;
			temp.xcor = pins[i].xcor;
			temp.ycor = pins[i].ycor;
			modu.pins.push_back(temp);
		}

		outHM[i] = modu;
	}

}

void parser(string filename, unordered_map<int, HardR> & hard_map, unordered_map<int, SoftR> & soft_map, int & pin_count, int & pin_width, int & pin_height, int &hard_counter , int &soft_counter)
{

	string mod_text;
	ifstream input;
	 hard_counter = 0;
	 soft_counter = 0;
	int counter = 0;
	input.open(filename);

	if (!input.is_open()) {
		cout << "error in opening file\n ";
	}
	while (!input.eof())
	{
		vector<string> components;
		getline(input, mod_text);
		split(mod_text, components);

		if (components.at(0) == ".hard_module")
		{
			HardR temp;
			temp.name = components.at(1);
			temp.width = stoi(components.at(2));
			temp.height = stoi(components.at(3));
			hard_map[hard_counter] = temp;
			hard_counter++;
		}
		else if (components.at(0) == ".soft_module")
		{
			SoftR temp;
			temp.name = components.at(1);
			temp.area = stoi(components.at(2));
			soft_map[soft_counter] = temp;
			soft_counter++;
		}
		else if (components.at(0) == ".pins")
		{
			pin_count = stoi(components.at(1));
			pin_width = stoi(components.at(2));
			pin_height = stoi(components.at(3));

		}
		else
			cout << "invalid input";
	}
}
void diePerimeter(int count, int width, int height, int & outer_dim, int & inner_dim)
{
	int rectPerside = ceil(count / 4.0);	// the number or rectangles per side rounded up
	int max_dim = max(width, height);		//the maximim dimension to get a square shape
	inner_dim = max_dim * rectPerside;
	outer_dim = inner_dim + 2 * max_dim;

}
void HardRectangles(unordered_map < int, HardR> hmp , unordered_map < int, SoftR> smp , int pin_count , int pin_height ,int pin_width , int hard_count , int soft_ount) {
	ofstream myfile;
	myfile.open("example.txt");
	int area = 0;
	int W;
	bool core_const = false;
	bool die_cosnt = false;
	float initialW;
	for (auto it = hmp.begin(); it != hmp.end(); ++it)
	{
		int h = it->second.height;
		int w = it->second.width;
		area += (h*w);
	}
	for (auto it = smp.begin(); it != smp.end(); ++it)
	{
		int a = it->second.area;
		area += a;
	}
	cout << "area:" << area << "sqrt" << sqrt(area);
	initialW = sqrt(area);
	/*int pps = (pin_count / 4);
	if ( (initialW / pps) >= (min (pin_height, pin_width) ) )
		core_const = 1;
	else
		die_cosnt = 1;

	if (die_cosnt)
		W = 4 * min(pin_height, pin_width);
	else
		W = initialW;

	cout << "w:" << W;
	*/
	//W = initialW;
	W = sqrt(area);
	cout << "W:" << W;

	IloEnv env;
	
		IloModel model(env);
		IloNumVarArray vars(env);
		//IloNumVarArray x(env);
		//IloNumVarArray y(env);
		//IloNumVarArray z(env);
		//IloNumVarArray xc(env);
		//IloNumVarArray yc(env);
		//IloNumVarArray res(env);
		IloNumVar a(env);
		//res.add(IloNumVar(env));
		vars.add(IloNumVar(env));
		model.add(IloMinimize(env, vars[0]));
		IloRangeArray c;
		for (int i = 1; i <= hard_count; i++)
		{
			//x.add(IloNumVar(env, 0, 40));
			//x.add(IloNumVar(env));

			//y.add(IloNumVar(env, 0, 40));
			//y.add(IloNumVar(env));
			//z.add(IloNumVar(env));
			/*vars.add(IloNumVar(env));
			vars.add(IloNumVar(env));
			vars.add(IloNumVar(env));
			model.add(vars[i] >= 0);
			myfile << "x" << i << " >= 0" << endl;
			model.add(vars[i+1] >= 0);
			myfile << "y" << i << " >=0" << endl;
			model.add(vars[i] + vars[i+2] * (hmp[i-1].height) + (1 - vars[i+2])*(hmp[i-1].width) <= W);
			myfile << "x" << i << " + "<< hmp[i-1].height << " z"<< i << " + " << hmp[i-1].width <<" - " << hmp[i-1].width << " z" << i << " <= " << W<<endl ;
			model.add(vars[i+1] + vars[i+2] * (hmp[i-1].width) + (1 - vars[i+2])*(hmp[i-1].height) <= vars[0]);
			myfile << "y" << i << " + " << hmp[i - 1].width << " z" << i << " + " << hmp[i - 1].height << " - " << hmp[i - 1].height << " z" << i << " <= " << "Y" << endl;
			cout << i << endl;*/
			vars.add(IloNumVar(env));
			vars.add(IloNumVar(env));
			model.add(vars[i] >= 0);
			myfile << "x" << i << " >= 0" <<";"<< endl;
			model.add(vars[i + 1] >= 0);
			myfile << "y" << i << " >= 0" << ";" << endl;
			model.add(vars[i] + hmp[i - 1].width <= W);
			myfile << "x" << i << " + "  << hmp[i - 1].width <<" <= " << W << ";" << endl;
			model.add(vars[i + 1] + hmp[i - 1].height <= vars[0]);
			myfile << "y" << i << " + " << hmp[i - 1].height <<  " <= " << "Y" << ";" << endl;
			cout << i << endl; 
		}
		int ct = hard_count;
		/*for (auto it1 = hmp.begin(); it1 != hmp.end(); ++it1)
			for (auto it2 = hmp.begin(); it2 != hmp.end(); ++it2)*/
		for (int i = 1; i <= hard_count - 1; i++)
			for (int j = i+1; j <= hard_count; j++)
			{
				vars.add(IloNumVar(env));
				vars.add(IloNumVar(env));
				//xc.add(IloNumVar(env));
				//yc.add(IloNumVar(env));
				/*model.add(x[i] + z[i] * (hmp[i].height) + (1 - z[i])*(hmp[i].width) <= x[j] + W * (xc[ct] + yc[ct]));
				model.add(y[i] + z[i] * (hmp[i].width) + (1 - z[i])*(hmp[i].height) <= y[j] + W * (1 + xc[ct] - yc[ct]));
				model.add(x[j] + z[j] * (hmp[j].height) + (1 - z[j])*(hmp[j].width) <= x[i] + W * (1 - xc[ct] + yc[ct]));
				model.add(y[j] + z[j] * (hmp[j].width) + (1 - z[j])*(hmp[j].height) <= y[i] + W * (2 - xc[ct] - yc[ct]));*/
			/*	model.add(vars[i] +vars[i+2] * (hmp[i].height) + (1 - vars[i+2])*(hmp[i].width) <= vars[j] + W * (vars[ct] + vars[ct+1]));
				myfile << "x" << i << " + " << hmp[i - 1].height << " z" << i << " + " << hmp[i - 1].width << " - " << hmp[i - 1].width << " z" << i << " <= " << "x" <<j << " + " << W << " x" <<i<<j << " + " << W << " y" << i << j << endl;
				model.add(vars[i+1] + vars[i+2] * (hmp[i].width) + (1 - vars[i+2])*(hmp[i].height) <= vars[j+1] + W * (1 + vars[ct] - vars[ct+1]));
				myfile << "y" << i << " + " << hmp[i - 1].width << " z" << i << " + " << hmp[i - 1].height << " - " << hmp[i - 1].height << " z" << i << " <= " << "y" << j << " + "<< W <<" + " << W << " x" << i << j << " - "<< W << " y" << i << j << endl;
				model.add(vars[j] + vars[j+2] * (hmp[j].height) + (1 - vars[j+2])*(hmp[j].width) <= vars[i] + W * (1 - vars[ct] + vars[ct+1]));
				myfile << "x" << j << " + " << hmp[j - 1].height << " z" << j << " + " << hmp[j - 1].width << " - " << hmp[j - 1].width << " z" << j << " <= " << "x" << i << " + " << W << " - "<< W << " x" << i << j << " + " << W << " y" << i << j << endl;
				model.add(vars[j+1] + vars[j+2] * (hmp[j].width) + (1 - vars[j+2])*(hmp[j].height) <= vars[i+1] + W * (2 - vars[ct] - vars[ct+1]));
				myfile << "y" << j << " + " << hmp[j - 1].width << " z" << j << " + " << hmp[j - 1].height << " - " << hmp[j - 1].height << " z" << i << " <= " << "y" << i << " + " << "2 "<< W << " - " << W << " x" << i << j << " - " << W << " y" << i << j << endl;
				ct++;
*/
				model.add(vars[i] + hmp[i-1].width <= vars[j] + W * (vars[ct] + vars[ct + 1]));
				myfile << "x" << i << " + " << hmp[i - 1].width << " <= " << "x" << j << " + " << W << " x" << i << j << " + " << W << " y" << i << j << ";" << endl;
				model.add(vars[i+1] + hmp[i-1].height<= vars[j + 1] + W * (1 + vars[ct] - vars[ct + 1]));
				myfile << "y" << i << " + " << hmp[i - 1].height <<  " <= " << "y" << j << " + " << W << " + " << W << " x" << i << j << " - " << W << " y" << i << j << ";" << endl;
				model.add(vars[j] + hmp[j-1].width <= vars[i] + W * (1 - vars[ct] + vars[ct + 1]));
				myfile << "x" << j << " + " << hmp[j - 1].width << " <= " << "x" << i << " + " << W << " - " << W << " x" << i << j << " + " << W << " y" << i << j << ";" << endl;
				model.add(vars[j + 1] + hmp[j-1].height<= vars[i + 1] + W * (2 - vars[ct] - vars[ct + 1]));
				myfile << "y" << j << " + " << hmp[j - 1].height << " <= " << "y" << i << " + " << "2 " << W << " - " << W << " x" << i << j << " - " << W << " y" << i << j << ";" << endl;
				ct++;
			}

		
		IloCplex cplex(model);
		cplex.solve();
		//cout << " min =" << cplex.getObjValue() << endl;
		cout << cplex.getStatus();
		if (cplex.getStatus() == IloAlgorithm::Optimal) {
			IloNumArray vals(env);
			//cplex.getValues(vals, vars);
			//env.out() <<"value: "<< vars[0];
			//env.out() << "Values = " << vals << endl;
		}
	
	env.end();
}

//ILOSTLBEGIN

int main()
{
	//IloEnv env;
	//IloModel model(env);
	//IloNumVarArray x(env);
	//x.add(IloNumVar(env, 0, 40));
	//x.add(IloNumVar(env)); // default : between 0 and +∞
	//x.add(IloNumVar(env));
	//model.add(-x[0] + x[1] + x[2] <= 20);
	//model.add(x[0] - 3 * x[1] + x[2] <= 30);
	//model.add(IloMaximize(env, x[0] + 2 * x[1] + 3 * x[2]));
	//IloCplex cplex(model);
	//cplex.solve();
	//cout << " Max =" << cplex.getObjValue() << endl;
	//if (cplex.getStatus() == IloAlgorithm::Optimal) {
	//	IloNumArray vals(env);
	//	cplex.getValues(vals, x);
	//	env.out() << "Values = " << vals << endl;
	//	env.out() << x;
	//}
	//env.end();
	//system("pause");
	/*string file_name = "modules.txt";
	unordered_map < int, HardR> HM;
	unordered_map < int, SoftR> SM;
	int hard_count = 0, soft_count = 0;
	int pin_count = 0, pin_height = 0, pin_width = 0, inner_dim = 0, outer_dim = 0;
	parser(file_name, HM, SM, pin_count, pin_width, pin_height, hard_count, soft_count);
	for (auto it = HM.begin(); it != HM.end(); it++)
		cout << it->first << "," << it->second.name << "," << it->second.height << "," << it->second.width << endl;
	for (auto it = SM.begin(); it != SM.end(); it++)
		cout << it->first << "," << it->second.name << "," << it->second.area << endl;
	cout << "pins:" << pin_count << "," << pin_height << "," << pin_width << endl;
	HardRectangles(HM, SM, pin_count, pin_height , pin_width, hard_count , soft_count);
	//diePerimeter(pin_count, pin_width, pin_height, outer_dim, inner_dim);
	//cout << outer_dim << "," << inner_dim;
	system("pause");
	return 0;*/
	float utilization = 0.7;
	float suti = 0.6;
	float aspect_min = 0.5;
	float aspect_max = 1 / 0.5;
	int WafterU;
	int pin_max, pin_min;
	int pins_side;
	int pins_min_width;
	int init_W;
	bool io_del = 0;
	string file_name = "inpp.txt";
	unordered_map < int, HardR> HM;
	unordered_map < int, SoftR> SM;
	double area = 0;
	float totalw = 0;
	float totalh = 0;
	int totalmax = 0;
	int W;
	int arrsize = 0;
	int hard_count = 0, soft_count = 0;
	int pin_count = 0, pin_height = 0, pin_width = 0, inner_dim = 0, outer_dim = 0;
	int hfinal;
	//parser(file_name, HM, SM, pin_count, pin_width, pin_height, hard_count, soft_count);
	GetHard("directory.txt", HM , hard_count);
	cout << "hard:" << hard_count << endl;
	cout << "soft:" << soft_count << endl;
	int gmin = 1;
	int pinmar = 10;
	int margin = 2*pinmar + 2*gmin;
	//int margin = 0;
	
	for (auto it = HM.begin(); it != HM.end(); ++it)
	{
		long long h = it->second.height;
		long long w = it->second.width;
		area += (h*w);
		totalw += w;
		totalh += h;
	}
	for (auto it = SM.begin(); it != SM.end(); ++it)
	{
		float wmin_i, wmax_i, hmin_i, hmax_i;
		int iarea;
		area += it->second.area;
		iarea = it->second.area;
		wmin_i = sqrt(iarea*aspect_min);
		wmax_i = ceil(sqrt(iarea*aspect_max));
		hmax_i = iarea / wmax_i + (wmax_i - wmin_i) * (iarea / (wmax_i*wmax_i));
		hmin_i = iarea / wmax_i + (wmax_i - wmax_i) * (iarea / (wmax_i*wmax_i));
		totalw += wmax_i;
		totalh += hmax_i;
	}
	totalmax = max(totalh, totalw);
	init_W = ceil(sqrt(area));
	WafterU = init_W / utilization + hard_count*margin + soft_count*margin;
	pin_max = max(pin_height, pin_width);
	pin_min = min(pin_height, pin_width);
	pins_side = ceil(pin_count / 4.0);
	pins_min_width = pins_side * pin_max;
	if (pins_min_width >= WafterU)
		io_del = 1;
	W = max(WafterU, pins_min_width);

	int Na = hard_count + soft_count;
	int** conn = new int*[Na];
	for (int i = 0; i < Na; ++i)
		conn[i] = new int[Na];

	for (int i = 0; i < Na; ++i)
		for (int j = 0; j < Na; ++j)
			conn[i][j] =  rand() % 2;

	for (int i = 0; i < Na; ++i)
	{
		for (int j = 0; j < Na; ++j)
			cout << conn[i][j] << "	";
		cout << endl;
	}

	int HS = hard_count + soft_count;
	int IO = pin_count;
	int** connPM = new int*[Na];
	for (int i = 0; i < Na; ++i)
		connPM[i] = new int[Na];

	for (int i = 0; i < Na; ++i)
		for (int j = 0; j < Na; ++j)
			connPM[i][j] = rand() % 2;

	for (int i = 0; i < Na; ++i)
	{
		for (int j = 0; j < Na; ++j)
			cout << connPM[i][j] << "	";
		cout << endl;
	}

	//W = totalw;
	//W = totalh;
	cout << "W = " << W;
	int wid = W;
	IloEnv env;
	stringstream logfile;
	cout << "total" << totalw << " , from area:" << wid;
	try {
		IloModel model(env);
		IloNumVarArray vars(env);
		IloRangeArray con(env);
		IloNumVar maxX(env);
		IloCplex cplex(model);
		ofstream LogFile("LogFile.txt");
		ofstream Results("newres.txt");
		ofstream def("DEF.txt");
		ofstream temp("temp.txt");
		
		cplex.setOut(LogFile);
		// Set options of IP solver
		cplex.setParam(IloCplex::TiLim, 100.000);
		cplex.setParam(IloCplex::Threads, 1);
		cplex.setParam(IloCplex::EpGap, 0.0);
		cplex.setParam(IloCplex::EpAGap, 0.0);
		vars.add(IloNumVar(env));
		IloRangeArray c(env);
		arrsize += 1;
		// Variables
		for (int i = 1; i <= hard_count*3; i+=3)
		{
			//cout << "ii=" << i << endl;
			//cout << "h=" << HM[i / 2].height<<endl;
			//cout << "w=" << HM[i / 2].width<<endl;
			auto ww = (HM[i / 3].width + margin);
			auto hh = (HM[i / 3].height + margin);
			vars.add(IloNumVar(env));
			vars.add(IloNumVar(env));
			vars.add(IloNumVar(env));
			arrsize += 3;
			c.add(vars[i] >= 0);
			c.add(vars[i + 1] >= 0);
			//c.add(vars[i] + HM[i/2].width <= W);
			c.add(vars[i] + ww * (1- vars[i+2]) + hh*vars[i+2] <= W);
			c.add(vars[i + 1] + hh*(1 - vars[i + 2]) + ww*vars[i + 2] - vars[0]  <= 0);
			c.add(((vars[i+2] == 1) + (vars[i+2] == 0)) == 1);

		}
		
		int ct = hard_count*3+1;
		for (int i = 1; i <= hard_count*3 - 3; i+=3)
			for (int j = i + 3; j <= hard_count * 3; j += 3)
			{
				vars.add(IloNumVar(env));
				vars.add(IloNumVar(env));
				arrsize += 2;
				int wwi = (HM[i / 3].width + margin);
				int hhi = (HM[i / 3].height + margin);
				int wwj = (HM[j / 3].width + margin);
				int hhj = (HM[j / 3].height + margin);
			
				c.add(vars[i] + wwi*(1 - vars[i + 2]) + hhi*vars[i + 2] - (vars[j] + W * (vars[ct] + vars[ct + 1])) <= 0);
				c.add(vars[i + 1] + hhi* (1 - vars[i + 2]) + wwi*vars[i + 2] - (vars[j + 1] + W * (1 + vars[ct] - vars[ct + 1])) <= 0);
				c.add(vars[j] + wwj*(1 - vars[j + 2]) + hhj*vars[j + 2] - (vars[i] + W * (1 - vars[ct] + vars[ct + 1])) <= 0);
				c.add(vars[j + 1] + hhj*(1 - vars[j + 2]) + wwj * vars[j + 2] - (vars[i + 1] + W * (2 - vars[ct] - vars[ct + 1])) <= 0);

				c.add(((vars[ct] == 1) + (vars[ct] == 0)) == 1);
				c.add(((vars[ct+1] == 1) + (vars[ct+1] == 0)) == 1);
			//	c.add(IloAbs(vars[i] - vars[j]) - IloAbs(vars[i + 1] - vars[j + 1]) - maxX <= 0);

			//	c.add( (vars[i]-vars[j]) - vars[ct+2]  == 0);
			//	c.add((vars[i+1] - vars[j+1]) - vars[ct + 3] == 0);
				//c.add(IloMinimize(env, vars[ct+2]));
				//model.add(IloMinimize(env, vars[ct + 3]));
				ct+=2; 
				cout << "ct=" << ct << endl; 

			}

	/*	for (int c = hard_count * 3 + 1; c < ct; c += 4)
		{
			auto x = vars[c];
			vars[ct] =  (vars[c] + x) ;
		}
		ct += 1;*/
		cout << "final_ct" << ct;
		// x -> i , y -> i+1 , w -> i+2 , delta -> i+3 , c -> i+4
		for (int i = ct; i < (ct+ soft_count * 4); i += 4)
		{
			int iarea;
			float wmin_i, wmax_i, hmin_i, hmax_i, deltai , ci;
			cout << "ii=" << i << endl;
			iarea = SM[(i - ct)/4].area/suti;
			wmin_i = sqrt(iarea*aspect_min);
			wmax_i = ceil(sqrt(iarea*aspect_max));
			hmax_i = ceil(iarea / wmax_i + (wmax_i - wmin_i) * (iarea / (wmax_i*wmax_i)));
			hmin_i = iarea / wmax_i + (wmax_i - wmax_i) * (iarea / (wmax_i*wmax_i));
			deltai = ceil( (hmin_i - hmax_i) / (wmax_i - wmin_i));
			ci = ceil( hmax_i - deltai * wmin_i);
			vars.add(IloNumVar(env));
			vars.add(IloNumVar(env));
			vars.add(IloNumVar(env));
			vars.add(IloNumVar(env));
			arrsize += 4;
			c.add(vars[i] >= 0);
			c.add(vars[i + 1] >= 0);
			c.add(vars[i] + vars[i+2] - W <= 0 );
			c.add(vars[i + 1] + vars[i+3] - vars[0]  <= 0);
		//	c.add(vars[i + 3] - (iarea / wmin_i + (vars[i + 2] - wmin_i) * (iarea / (wmin_i*wmin_i))) == 0);
	    //	c.add(vars[i + 3] - (iarea / wmax_i + (wmax_i - vars[i+2]) * (iarea / (wmax_i*wmax_i))) == 0);
			c.add((vars[i + 3] - vars[i + 2] * deltai - ci) == 0);
			c.add(vars[i + 2] >= wmin_i);
			c.add(vars[i + 2] <= wmax_i);
		//	c.add((vars[i + 2] * deltai + ci) >= hmin_i);
		//	c.add((vars[i + 2] * deltai + ci) <= hmax_i);
		//	c.add((vars[i + 3] - vars[i + 2] * deltai - ci) == 0);
		}
		
		int cts = ct + soft_count*4 ;
		cout << "cts:" << cts;
		for (int i = ct; i < (ct + soft_count*4 - 4); i += 4)
			for (int j = i + 4; j < (ct + soft_count * 4); j += 4)
			{
			/*	int iarea;
				float wmin_i, wmax_i, hmin_i, hmax_i, deltai, ci;
				int wmin, wmax, hmin, hmax;
				iarea = SM[(i - ct)/4].area / utilization;
				wmin_i = sqrt(iarea*aspect_min);
				wmax_i = sqrt(iarea*aspect_max);
				hmax_i = iarea / wmax_i + (wmax_i - wmin_i) * ( iarea/ (wmax_i*wmax_i) );
				hmin_i = iarea / wmax_i + (wmax_i - wmax_i) * (iarea / (wmax_i*wmax_i));
				deltai = (hmin_i - hmax_i) / (wmax_i - wmin_i);
				ci = hmax_i - deltai * wmin_i;

				int jarea;
				float wmin_j, wmax_j, hmin_j, hmax_j, deltaj, cj;
				jarea = SM[(j - ct)/4].area;
				wmin_j = sqrt(jarea*aspect_min);
				wmax_j = sqrt(jarea*aspect_max);
				hmax_j = jarea / wmax_j + (wmax_j - wmin_j) * (jarea / (wmax_j*wmax_j));
				hmin_j = jarea / wmax_j + (wmax_j - wmax_j) * (jarea / (wmax_j*wmax_j));
				deltaj = (hmin_j - hmax_j) / (wmax_j - wmin_j);
				cj = hmax_j - deltaj * wmin_j;*/

				arrsize += 2;
				vars.add(IloNumVar(env));
				vars.add(IloNumVar(env));
				cout << "i=" << i << endl;
				cout << "j=" << j << endl;
				cout << "ct=" << ct << endl;
				c.add(vars[i] + vars[i+2] - (vars[j] + wid * (vars[cts] + vars[cts + 1])) <= 0);
				c.add(vars[i + 1] + vars[i+3] - (vars[j + 1] + wid * (1 + vars[cts] - vars[cts + 1])) <= 0);
			   // c.add(vars[j] + vars[j+2] - (vars[i] + totalmax * (1 - vars[cts] + vars[cts + 1])) <= 0);
				//c.add(vars[j + 1] + vars[j+3] - (vars[i + 1] + totalmax * (2 - vars[cts] - vars[cts + 1])) <= 0);
				c.add(vars[j] + vars[j + 2] - (vars[i] + wid * ( 1 - vars[cts] + vars[cts + 1])) <= 0);
				c.add(vars[j + 1] + vars[j+3] - (vars[i + 1] + wid * (2 - vars[cts] - vars[cts + 1])) <= 0);
				c.add(((vars[cts] == 1) + (vars[cts] == 0)) == 1);
				c.add(((vars[cts + 1] == 1) + (vars[cts + 1] == 0)) == 1);
				cts += 2;
			}
		for (int i = 1; i <= hard_count * 3; i += 3)

			for (int j = ct; j < (ct + soft_count * 4); j += 4) {


				int jarea;
				float wmin_j, wmax_j, hmin_j, hmax_j, deltaj, cj;
				jarea = SM[(j - ct) / 4].area / suti;
				wmin_j = sqrt(jarea*aspect_min);
				wmax_j = sqrt(jarea*aspect_max);
				hmax_j = jarea / wmax_j + (wmax_j - wmin_j) * (jarea / (wmax_j*wmax_j));
				hmin_j = jarea / wmax_j + (wmax_j - wmax_j) * (jarea / (wmax_j*wmax_j));
				deltaj = (hmin_j - hmax_j) / (wmax_j - wmin_j);
				cj = hmax_j - deltaj * wmin_j;

				arrsize += 2;
				vars.add(IloNumVar(env));
				vars.add(IloNumVar(env));
				cout << "i=" << i << endl;
				cout << "j=" << j << endl;
				cout << "ct=" << ct << endl;
				c.add(vars[i] + vars[i + 2] * HM[i / 3].height + (1 - vars[i + 2])*HM[i / 3].width - (vars[j] + W * (vars[cts] + vars[cts + 1]) ) <= 0 );
				c.add( vars[i] - vars[j+2] - vars[j] + W*(1-vars[cts] + vars[cts+1]) >= 0  );
				c.add(vars[i + 1] + vars[i + 2] * HM[i / 3].width + (1 - vars[i + 2])* HM[i / 3].height - vars[j + 1] - W * (1 + vars[cts] - vars[cts + 1]) <= 0);
				c.add(vars[i+1] - vars[j+3] - vars[j+1] + W*(2 - vars[cts] - vars[cts + 1]) >= 0);
				c.add(((vars[cts] == 1) + (vars[cts] == 0)) == 1);
				c.add(((vars[cts + 1] == 1) + (vars[cts + 1] == 0)) == 1);
				cts += 2;
			}

		model.add(c);
		model.add(IloMinimize(env, vars[0] ));
		//model.add(IloMinimize(env, vars[15]));
		// Solve
		cplex.solve();
		
		
		if (cplex.getStatus() == IloAlgorithm::Optimal) {
			
			IloNumArray vals(env);
			cplex.getValues(vals, vars);
			vector<int> realval;
			for (int i = 0; i < arrsize; i++)
			{
				temp << vals[i] << endl;
			}
			temp.close();
			ifstream tempin("temp.txt");
			while (!tempin.eof())
			{
				float tv;
				tempin >> tv;
				realval.push_back(ceil(tv));
			}

			for (int i = 0; i < arrsize; i++)
			{
				cout << realval[i] << endl;
			}

			Results << "W:" << W + 2*pin_max << endl;
			if (io_del)
			{
				hfinal = pins_min_width + 2 * pin_max;
				Results << "H:" << pins_min_width + 2 * pin_max << endl;
				Results << "Total Area:" << W * pins_min_width << endl;
				Results << "Final Utilization:" << area / (W * pins_min_width) << endl;
			}
			else
			{
				hfinal = realval[0] + 2 * pin_max;
				Results << "H:" << realval[0] + 2 * pin_max << endl;
				Results << "Total Area:" << W * hfinal << endl;
				long long divv = long long(W) * long long(hfinal);
				float utiliz = area / divv;
				cout<<"utilization: " << utiliz << endl;
				Results << "Final Utilization:" << utiliz << endl;
			}
			
			Results << "I/O pads:" << endl;
			for (int i = 1; i <= pins_side; i++)
			{
				Results << "0 , " << i * pin_min<<endl;
			}

			for (int i = 1; i <= pins_side; i++)
			{
				Results << i * pin_min << " , 0" << endl;
			}

			for (int i = 1; i <= pins_side; i++)
			{
				Results << W << " , " << i * pin_min << endl;
			}
			for (int i = 1; i <= pins_side; i++)
			{
				Results << i * pin_min << " , "<< realval[0] << endl;
			}

			Results << "Hard Modules:" << endl;
			double temp;
			for (int i = 1; i <= hard_count * 3; i += 3) 
			{
				Results<< HM[i/3].name<<":		" << realval[i] + pin_max << "," <<vals[i+1]+pin_max<<"		";
				HM[i / 3].xcor = legalize(	(realval[i] + pin_max) , gmin);
				HM[i / 3].ycor = legalize( (realval[i+1] + pin_max) , gmin);
				if (vals[i + 2])
				{
					Results << "rotated";
					temp = HM[i / 3].height;
					HM[i / 3].height = HM[i / 3].width;
					HM[i / 3].width = temp;
				}

				Results << endl;
			}

			def << "Components " <<hard_count+soft_count<< " ;"<< endl;
			for (int i = 1; i <= hard_count * 3; i += 3)
			{
				def << HM[i / 3].name << "_1 " << HM[i/3].name << " + PLACED "<< "( " << legalize( (realval[i] + pin_max) , gmin) << " " << legalize( (vals[i + 1] + pin_max) , gmin) << " ) ";
				
			
				if (vals[i + 2])
				{
					def << "N ;"<<endl;
				}
				else
				{
					def << "W ;"<<endl;
				}
				Results << endl;
			}


			Results << "Soft Modules:" << endl;
			for (int i = ct; i < (ct + soft_count * 4); i += 4)
			{
				
				Results <<SM[i/4 - hard_count ].name<<":		"<< realval[i]+pin_max << "		" << realval[i + 1]+pin_max <<"	" << realval[i+2] << "		" << realval[i+3]<<endl;
				SM[i / 4 - hard_count  ].xcor = legalize(realval[i] + pin_max, gmin);
				SM[i / 4 - hard_count  ].ycor = legalize(realval[i+1] + pin_max,gmin);
				SM[i / 4 - hard_count  ].width = legalize(realval[i+2] , gmin);
				SM[i / 4 - hard_count  ].height = legalize( realval[i+3] , gmin);

			}
			cout << "Values = " << vals << endl;
		}
		else
		{
			cout << "couldn't find a solution, please change the utilization factor"<<endl;
		}
	}
	catch (IloException& e) {
		cerr << "C-Exp: " << e << endl;
	}
	catch (...) {
		cerr << "Unknown Exception" << endl;
	}



	const string str = logfile.str();
	cout << str << endl;

	//grid:
	
	int N = (W + 2 * pin_max)/gmin;
	int M = hfinal/gmin;

	int** ary = new int*[N+1];
	for (int i = 0; i <= N; ++i)
		ary[i] = new int[M+1];

	for (int i = 0; i <= N; ++i)
		for (int j = 0; j <= M; ++j)
			ary[i][j] = 0;

	for (auto it = HM.begin(); it != HM.end(); ++it)
	{
		int x = it->second.xcor/gmin;
		int y = it->second.ycor/gmin;
		int w = it->second.width / gmin;
		int h = it->second.height / gmin;
		for(int i = x ; i < x+w ; i++ )
			for (int j = y; j < y + h; j++)
			{
				ary[i+2][j+2] = 1;
			}
	}
	for (auto it = SM.begin(); it != SM.end(); ++it)
	{
		int x = it->second.xcor / gmin;
		int y = it->second.ycor / gmin;
		int w = it->second.width / gmin;
		int h = it->second.height / gmin;
		for (int i = x; i < x + w; i++)
			for (int j = y; j < y + h; j++)
			{
				ary[i][j] = 1;
			}
	}

	

	ofstream grd;
	grd.open("outgrid.txt");

	for (int j = 0; j < M; ++j)
	{
		for (int i = 0; i < N; ++i)
		{
			grd << ary[i][M-j-1];
		}
		grd << endl;
	}
	env.end();

	system("pause");
	return 0;
}

void adjustDimensions(float x , float y , float grid , float & newx , float & newy , int pinmar)
{

	float divy, divx;
	float idiffy, idiffx, diffy, diffx;
	divy = y / grid;
	cout << "x:" << divy << endl;
	cout << "conv:" << int(divy) << endl;
	idiffy = y - int(divy)*grid;
	diffy = grid - idiffy;
	cout << "diff:" << diffy << endl;
	newy = y + diffy + pinmar;
	cout << "newy:" << newy << endl;

	divx = x / grid;
	idiffx = x - int(divx)*grid;
	diffx = (grid - idiffx);
	cout << "diff:" << diffx << endl;

	newx = x + diffx + pinmar;
	cout << "newx:" << newx << endl;

	system("pause");
}

