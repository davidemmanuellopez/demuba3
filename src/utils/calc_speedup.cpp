#include<iostream>
#include<string>
#include<fstream>
#include<getopt.h>
using namespace std;

int main(int argc, char** argv)
{
	string pathfileA,pathfileB,pathfileC,op;
	
	const char* const short_opts = "a:b:c:o";
    const option long_opts[] = {
            {"a", required_argument, nullptr, 'a'},
            {"b", required_argument, nullptr, 'b'},
            {"output", required_argument, nullptr, 'c'},
            {nullptr, no_argument, nullptr, 0}
    };

    while (true)
    {
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

        if (-1 == opt)
            break;

        switch (opt)
        {
        case 'a':
            pathfileA=optarg;
	        break;

        case 'b':
            pathfileB=optarg;
	        break;

        case 'c':
            pathfileC=optarg;
            break;

        default:
            break;
        }
    }
	
	
	ifstream fileA(pathfileA);
	ifstream fileB(pathfileB);
	ofstream fileC(pathfileC);
	cout << "FileC:" << pathfileC << endl;
	
	string name,TMP,size_A,size_B,time_A,time_B,rules_A,rules_B;
	while (getline(fileA, name, ',') && getline(fileB,TMP,',')) {
	
	
	getline(fileA,size_A,',');
	getline(fileB,size_B,',');
	
	getline(fileA,TMP,',');
	getline(fileB,TMP,',');
	
	bool rules=true;
	char n_sep=',';
	if (TMP=="0")
	{
		 rules=false;
		n_sep='\n';
		};
		
	getline(fileA,time_A,n_sep);
	getline(fileB,time_B,n_sep);
	
	cout << "caso:" << name << endl;
	
	cout << "time_A:" << time_A << "fin time_A "  << endl;
	cout << "time_B:" << time_B << endl;
	
	cout << "comparar TMP:" << TMP << endl;
	

	
	if (rules)
	{
			getline(fileA,rules_A,'\n');
			getline(fileB,rules_B,'\n');
	
		};
	
	
	double speedup_r,speedup_t;
	
	double time_A_d = std::stod(time_A);
	double time_B_d = std::stod(time_B);
	speedup_t = time_A_d/time_B_d;
	
	if (rules)
	{	
	long long rules_A_l = std::stoll(rules_A);
	cout << "rules A" << rules_A_l << endl;
	long long rules_B_l = std::stoll(rules_B);
	cout << "rules B" << rules_B_l << endl;
	speedup_r = (double) rules_A_l/rules_B_l;
	}
	fileC.precision(4);
	
	if (rules) fileC << name << "," << size_A << "," << speedup_t << "," << speedup_r << endl; else
	fileC << name << "," << size_A << "," << speedup_t << endl;
	
	//cout << "size A:" << size_A << ",size B:" << size_B << endl;
	};
	
	fileC.close();
	};
