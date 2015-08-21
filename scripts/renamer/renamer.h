/*
 *  renamer.h
 *  
 *
 *  Created by Javier Montejo on 12/18/11.
 *  Copyright 2011 IFAE. All rights reserved.
 *
 */
#ifndef _RENAMER_H_
#define _RENAMER_H_

#include <string>
#include "pattern.h"

using namespace std;

class renamer {

  public:
	vector<pair<string,string> > changes;
	
	renamer(pattern in, pattern out){
		vector<int> wildin = in.get_wildcards();
		vector<int> wildout = out.get_wildcards();
		string stringin = in.get_original();
		string stringout = out.get_original();
		
		if(wildin.size() != wildout.size())
			return;
		
		int posin=0;
		int posout=0;
		string tempin;
		string tempout;
		for(unsigned int i=0; i<wildin.size(); i++){
			tempin = stringin.substr(posin,wildin.at(i)-posin);
			tempout = stringout.substr(posout,wildout.at(i)-posout);
			changes.push_back(pair<string,string>(tempin,tempout));
			posin=wildin.at(i)+1;
			posout=wildout.at(i)+1;
			//cout << changes.at(i).first << " " << changes.at(i).second << endl;
		}
		tempin = stringin.substr(posin,stringin.size());
		tempout = stringout.substr(posout,stringout.size());
		changes.push_back(pair<string,string>(tempin,tempout));

	}

	string dorename(string in){
		string out(in);
		int pos;
		int size;
		for(unsigned int i=0; i<changes.size(); ++i){
			pos = out.find(changes.at(i).first);
			out.replace(pos,changes.at(i).first.size(),changes.at(i).second);
		}
		return out;
	}

};

#endif

