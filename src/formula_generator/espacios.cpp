#include<iostream>
#include<fstream>
using namespace std;


int main(int argc, char* argv[])
{
	string entrada(argv[1]);
	string salida(argv[2]);

	ifstream fentrada(entrada.c_str());
	ofstream fsalida(salida.c_str());
	
	string linea;
	while (getline(fentrada,linea))
	{
		std::size_t found = linea.find_first_of(" ");
		while (found!=std::string::npos)
		{
			linea.erase(found,1);
			found = linea.find_first_of(" ");
			};
		fsalida.write(linea.c_str(),linea.length());
		fsalida << endl;
		};
	
	fentrada.close();
	fsalida.close();
	}
