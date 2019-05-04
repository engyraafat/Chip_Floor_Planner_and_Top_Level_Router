//Floor Planner


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



//#include "lp_lib.h"
//#include <stdio.h>
using namespace std;
struct HardR {
	string name;
	int height;
	int width;
	int varc;
	double xcor;
	double ycor;
};
struct SoftR {
	string name;
	int area;
	double width;
	double height;
	double xcor;
	double ycor;

};
void split(string str, vector<string> & vec)
{
	string word = "";
	for (auto x : str)
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
	vec.push_back(word);
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
/*void HardRectangles(unordered_map < int, HardR> hmp , unordered_map < int, SoftR> smp , int pin_count , int pin_height ,int pin_width , int hard_count , int soft_ount) {
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

	W = sqrt(area);
	cout << "W:" << W;

	IloEnv env;
	
		IloModel model(env);
		IloNumVarArray vars(env);
		IloNumVar a(env);
		vars.add(IloNumVar(env));
		model.add(IloMinimize(env, vars[0]));
		IloRangeArray c;
		for (int i = 1; i <= hard_count; i++)
		{
		
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
	
		for (int i = 1; i <= hard_count - 1; i++)
			for (int j = i+1; j <= hard_count; j++)
			{
				vars.add(IloNumVar(env));
				vars.add(IloNumVar(env));
				
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
		cout << cplex.getStatus();
		if (cplex.getStatus() == IloAlgorithm::Optimal) {
			IloNumArray vals(env);
		
		}
	
	env.end();
}
*/
//ILOSTLBEGIN

int main()
{
	
	float utilization = 0.9;
	float suti = 0.72;
	float aspect_min = 0.5;
	float aspect_max = 1/0.5;
	int WafterU;
	int pin_max, pin_min;
	int pins_side;
	int pins_min_width;
	int init_W;
	bool io_del = 0;
	string file_name = "test9.txt";
	unordered_map < int, HardR> HM;
	unordered_map < int, SoftR> SM;
	double area = 0;
	float totalw = 0;
	float totalh = 0;
	int totalmax = 0;
	int W;
	int hard_count = 0, soft_count = 0;
	int pin_count = 0, pin_height = 0, pin_width = 0, inner_dim = 0, outer_dim = 0;
	int hfinal;
	parser(file_name, HM, SM, pin_count, pin_width, pin_height, hard_count, soft_count);
	cout << "hard:" << hard_count << endl;
	cout << "soft:" << soft_count << endl;
	int gmin = 1;
	for (auto it = HM.begin(); it != HM.end(); ++it)
	{
		int h = it->second.height;
		int w = it->second.width;
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
	WafterU = init_W / utilization;
	pin_max = max(pin_height, pin_width);
	pin_min = min(pin_height, pin_width);
	pins_side = ceil(pin_count / 4.0);
	pins_min_width = pins_side * pin_max;
	if (pins_min_width >= WafterU)
		io_del = 1;
	W = max(WafterU, pins_min_width);
	
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
		IloCplex cplex(model);
		ofstream LogFile("LogFile.txt");
		ofstream Results("Results.txt");
		cplex.setOut(LogFile);
		// Set options of IP solver
		cplex.setParam(IloCplex::TiLim, 100.000);
		cplex.setParam(IloCplex::Threads, 1);
		cplex.setParam(IloCplex::EpGap, 0.0);
		cplex.setParam(IloCplex::EpAGap, 0.0);
		vars.add(IloNumVar(env));
		IloRangeArray c(env);
		
		for (int i = 1; i <= hard_count*3; i+=3)
		{
			
			vars.add(IloNumVar(env));
			vars.add(IloNumVar(env));
			vars.add(IloNumVar(env));

			c.add(vars[i] >= 0);
			c.add(vars[i + 1] >= 0);
			c.add(vars[i] + HM[i / 3].width * (1- vars[i+2]) + HM[i / 3].height*vars[i+2] <= W);
			c.add(vars[i + 1] + HM[i/3].height*(1 - vars[i + 2]) + HM[i / 3].width*vars[i + 2] - vars[0]<= 0);
			c.add(((vars[i+2] == 1) + (vars[i+2] == 0)) == 1);

		}
		
		int ct = hard_count*3+1;
		for (int i = 1; i <= hard_count*3 - 3; i+=3)
			for (int j = i + 3; j <= hard_count * 3; j += 3)
			{
				vars.add(IloNumVar(env));
				vars.add(IloNumVar(env));

				//cout << "i=" << i<<endl;
				//cout << "j=" << j<<endl;
			//	c.add(vars[i] + HM[i/2].width - (vars[j] + W * (vars[ct] + vars[ct + 1]))  <= 0);
				c.add(vars[i] + HM[i / 3].width*(1-vars[i+2]) + HM[i / 3].height*vars[i+2] - (vars[j] + W * (vars[ct] + vars[ct + 1])) <= 0);
			//	c.add(vars[i + 1] + HM[i/2].height - (vars[j + 1] + W * (1 + vars[ct] - vars[ct + 1])) <= 0 );
				c.add(vars[i + 1] + HM[i / 3].height* (1-vars[i+2]) + HM[i / 3].width*vars[i+2] - (vars[j + 1] + W * (1 + vars[ct] - vars[ct + 1])) <= 0);
			//	c.add(vars[j] + HM[j/2].width - (vars[i] + W * (1 - vars[ct] + vars[ct + 1])) <= 0 );
				c.add(vars[j] + HM[j / 3].width*(1-vars[j+2]) + HM[j / 3].height*vars[j+2] - (vars[i] + W * (1 - vars[ct] + vars[ct + 1])) <= 0);
			//	c.add(vars[j + 1] + HM[j/2].height - (vars[i + 1] + W * (2 - vars[ct] - vars[ct + 1])) <= 0 );
				c.add(vars[j + 1] + HM[j / 3].height*(1-vars[j+2]) + HM[j / 3].width * vars[j+2] - (vars[i + 1] + W * (2 - vars[ct] - vars[ct + 1])) <= 0);
				c.add(((vars[ct] == 1) + (vars[ct] == 0)) == 1);
				c.add(((vars[ct+1] == 1) + (vars[ct+1] == 0)) == 1);
				ct+=2;
				cout << "ct=" << ct << endl;
			}
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
			c.add(vars[i] >= 0);
			c.add(vars[i + 1] >= 0);
			c.add(vars[i] + vars[i+2] - W <= 0 );
			c.add(vars[i + 1] + vars[i+3] - vars[0] <= 0);
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


				vars.add(IloNumVar(env));
				vars.add(IloNumVar(env));
				cout << "i=" << i << endl;
				cout << "j=" << j << endl;
				cout << "ct=" << ct << endl;
				c.add(vars[i] + vars[i+2] - (vars[j] + wid * (vars[cts] + vars[cts + 1])) <= 0);
				c.add(vars[i + 1] + vars[i+3] - (vars[j + 1] + wid * (1 + vars[cts] - vars[cts + 1])) <= 0);
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
		model.add(IloMinimize(env, vars[0]));

		// Solve
		cplex.solve();
		
		
		if (cplex.getStatus() == IloAlgorithm::Optimal) {
			IloNumArray vals(env);
			cplex.getValues(vals, vars);
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
				hfinal = vals[0] + 2 * pin_max;
				Results << "H:" << vals[0] + 2 * pin_max << endl;
				Results << "Total Area:" << W * vals[0] << endl;
				Results << "Final Utilization:" << area / (W * vals[0]) << endl;
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
				Results << i * pin_min << " , "<< vals[0] << endl;
			}

			Results << "Hard Modules:" << endl;
			double temp;
			for (int i = 1; i <= hard_count * 3; i += 3) 
			{
				Results<< HM[i/3].name<<":		" << vals[i] + pin_max << "," <<vals[i+1]+pin_max<<"		";
				HM[i / 3].xcor = vals[i] + pin_max;
				HM[i / 3].ycor = vals[i+1] + pin_max;
				if (vals[i + 2])
				{
					Results << "rotated";
					temp = HM[i / 3].height;
					HM[i / 3].height = HM[i / 3].width;
					HM[i / 3].width = temp;
				}

				Results << endl;
			}

			Results << "Soft Modules:" << endl;
			for (int i = ct; i < (ct + soft_count * 4); i += 4)
			{
				Results <<SM[i/4 - hard_count - 1].name<<":		"<< vals[i]+pin_max << "		" << vals[i + 1]+pin_max <<"	" << vals[i+2] << "		" << vals[i+3]<<endl;
				SM[i / 4 - hard_count - 1].xcor = vals[i];
				SM[i / 4 - hard_count - 1].ycor = vals[i+1];
				SM[i / 4 - hard_count - 1].width = vals[i+2];
				SM[i / 4 - hard_count - 1].height = vals[i+3];

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

	int** ary = new int*[N];
	for (int i = 0; i < N; ++i)
		ary[i] = new int[M];

	for (int i = 0; i < N; ++i)
		for (int j = 0; j < M; ++j)
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
				ary[i][j] = 1;
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

void adjustDimensions(float x , float y , float grid , float & newx , float newy)
{

	float divy, divx;
	float idiffy, idiffx, diffy, diffx;
	divy = y / grid;
	cout << "x:" << divy << endl;
	cout << "conv:" << int(divy) << endl;
	idiffy = y - int(divy)*grid;
	diffy = grid - idiffy;
	cout << "diff:" << diffy << endl;
	newy = y + diffy;
	cout << "newy:" << newy << endl;

	divx = x / grid;
	idiffx = x - int(divx)*grid;
	diffx = (grid - idiffx);
	cout << "diff:" << diffx << endl;

	newx = x + diffx;
	cout << "newx:" << newx << endl;

}
