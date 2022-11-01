#include <array>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <chrono>
#include <SWI-Prolog.h>
#include <SWI-Stream.h>
#include <SWI-cpp.h>

#include <fstream>
#include <getopt.h>

using namespace std;



int num = -1;
bool is_beep = false;
float sigma = 2.034;
std::string write_file = "default_file.txt";

void check_query(PlQuery* q)
{

	if (q->next_solution()) {
       std::cout << "Command OK\n";
	} else {
     std::cout << "Command failed\n";
	}

};

void load(string program,string lib)
{
    
    
	 PlTermv* term = new PlTermv(1);
     (*term)[0] = program.c_str();	 
	 PlQuery* query = new PlQuery("consult",*term);
		while( query->next_solution());
	 
	 delete term;
	 delete query;
	
	term = new PlTermv(1);
	(*term)[0] = lib.c_str();
	query = new PlQuery("load_foreign_library",*term);
		while( query->next_solution());
		
	delete term;
	delete query;
	
	};
	
std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}


void PrintHelp()
{
    std::cout <<
            "--num <n>:           Set number of program\n"
            "--beep:              Beep the user\n"
            "--sigma <val>:       Set sigma of program\n"
            "--writeFile <fname>: File to write to\n"
            "--help:              Show help\n";
    exit(1);
}

void ProcessArgs(int argc, char** argv)
{
 
}

int main(int argc, char** argv)
{
    cout << "Main" << endl;
	string demostrador,indexlib,set,type,basefile;
	int limit=-1;
	
	const char* const short_opts = "d:i:s:t:l:f:h";
    const option long_opts[] = {
            {"dem", required_argument, nullptr, 'd'},
            {"indexlib", required_argument, nullptr, 'i'},
            {"set", required_argument, nullptr, 's'},
            {"type", optional_argument, nullptr, 't'},
            {"limit", optional_argument, nullptr, 'l'},
            {"basefile", required_argument, nullptr, 'f'},
            {"help", optional_argument, nullptr, 'h'},

            {nullptr, no_argument, nullptr, 0}
    };

    while (true)
    {
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

        if (-1 == opt)
            break;

        switch (opt)
        {
        case 'd':
            demostrador=optarg;
	        break;

        case 'i':
            indexlib=optarg;
	        break;

        case 's':
            set=optarg;
            break;

        case 't':
            type=optarg;
            break;
		case 'l':
			limit=atoi(optarg);
			break;
		case 'f':
			basefile=optarg;
			break;
        case 'h': // -h or --help
        case '?': // Unrecognized option
        default:
            PrintHelp();
            break;
        }
    }
	

	
    cout << "cargo parametros" << endl;
  if (!PL_initialise(1, argv))
	 {
		 PL_halt(1);
		 };	 
	

	 load(demostrador,indexlib);
	 
    cout << "Cargo demostrador" << endl;
	string command = "cd ./tests/"+set+"/ && ls -Sr";
	string output = exec(command.c_str()); 
    istringstream iss(output);
    string element;
    
    //FILE *file = fopen(filepath.c_str(),"w");
    
    fstream filestream(basefile);
    int n=1;
    string line;
    while (getline(filestream,line))
    {
		cout << "saltea " << line << endl;
		getline(iss,element);
		};
	filestream.close();
	
	 std::ofstream outfile;

  outfile.open(basefile, std::ios_base::app);
  
	
    while (getline(iss, element) && (limit==-1 || n<=limit))
    {
		cout << "procesa " << element << endl;
		PlTermv* term = new PlTermv(1);
	
		string file = "./tests/"+ set+"/"+element;
		
		//get size of bytes
		string cmd_consult_size = "wc -c "+ file +" | cut -d' ' -f1";
		string size_in_string = exec(cmd_consult_size.c_str());
		//size_in_string[size_in_string.length()-1]=' ';
		size_in_string.erase(size_in_string.end()-1);
		(*term)[0] = file.c_str();
		
		PlQuery* query = new PlQuery("see",*term);
		while( query->next_solution());
        cout << "query finished" << endl;
		delete term;
		delete query;
	
		term = new PlTermv(1);
		PlTermv* out = new PlTermv(2);
		
		query = new PlQuery(type.c_str(),*out);
	

		
		auto start = chrono::steady_clock::now();
	
		int ns = query->next_solution(); //si ns es 0, no es solucion. Asi verifico si es true o false la respuesta de prolog
		//cout << "reglas aplicadas:" << (long) (*out)[1] << endl;
		
		auto end = chrono::steady_clock::now();
		auto diff = end - start;
		//fprintf(file,"%s,%i,%l\n",element.c_str(),ns,chrono::duration <double, milli> (diff).count());
		outfile << element <<"," << size_in_string << "," << ns << "," << chrono::duration <double> (diff).count();
		if (ns!=0)
		{
			outfile << "," << (long) (*out)[1];
			};
		outfile << endl;
		//cout << chrono::duration <double, milli> (diff).count() << " ms" << endl;
		
		//query = new PlQuery("Pl_cleani,*out);
		// query->next_solution(); 
		n++;
	};
	outfile.close();
	
	
	
    return 0;
}
