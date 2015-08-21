#ifndef _PATTERN_H_
#define _PATTERN_H_

#include <string>
#include <vector>

using namespace std;

class pattern {

private:
	string original;
	int key;
	vector<int> wildcards;
	int parse(string in){
		int pos = in.find("*");
		if(pos==string::npos)
			return 0;
		while(pos!=string::npos){
			//cout << "push wildcard: " << pos << endl;
			wildcards.push_back(pos);
			pos = in.find("*",pos+1);
		}
		return wildcards.size();
	}
	
public:
	
	pattern(string toparse){
		original = toparse;
		key = parse(toparse);
	}
	
	int get_key(){
		return key;
	}
	
	vector<int> get_wildcards(){
		return wildcards;
	}
	
	string get_original(){
		return original;
	}
	
	bool compatible(pattern other){
		if(get_key()==other.get_key())
			return true;
		else
			return false;
	}
	
};

#endif

