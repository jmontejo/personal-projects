#include <iostream>
#include <string>
#include <vector>
#include "Monitor.h"
#include "Partitura.h"
#include "Solucion.h"
#include "InterfazOut.h"
#include "Monitor.h"
#define MAX 35

int main (int argc, char * const argv[]) {

	std::cout << "main: Init" << std::endl;
	std::string acordes ("acordes.txt");
	std::string instrumentos("instrumentos.txt");
	std::cout << "main: Ready" << std::endl;
	Monitor *monitor = new Monitor(MAX);
	Partitura partitura(acordes,instrumentos,MAX);
	partitura.Resolver();
	std::vector<Solucion> soluciones = partitura.GetSoluciones();
	InterfazOut iout(soluciones);
	monitor->Analyze(soluciones);
	iout.Print();
	//iout.Dump(acordes);
	//monitor.Show();
	std::cout << "main: End" << std::endl;

    return 0;
}
