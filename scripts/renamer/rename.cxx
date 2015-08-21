#include <iostream>
#include <string>
#include "pattern.h"
#include "renamer.h"

using namespace std;

int main(int argn, char *args[]){

	if(argn<4){
		std::cout << "Display usage" << std::endl;
		return 1;
	}

	string instring = args[1];
	string outstring = args[2];
	pattern inpattern(instring);
	pattern outpattern(outstring);

	if(inpattern.compatible(outpattern)){
		renamer myrenamer = renamer(inpattern,outpattern);
		for(int i=3;i<argn;i++){
			cout << "mv " << args[i] << " " <<  myrenamer.dorename(args[i]) << endl;
			rename(args[i], myrenamer.dorename(args[i]).c_str());
		}
	}
	else
		cout << "Not compatible pattern (yet?)" << endl;

}
