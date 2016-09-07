#include <iostream>
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TString.h"
#include "TKey.h"
#include <stdarg.h>

using namespace std;
int chain(int count, ...)
{
	if (count<1){
		cout << "Usage: chain filename [more files]" << endl;
		exit(1);
	}
  va_list ap;
  int j;
  vector<TString> files;
  va_start(ap, count); //Requires the last fixed parameter (to get the address)
  for(j=0; j<count; j++)
      files.push_back (va_arg(ap, const char*)); //Requires the type to cast to. Increments ap to the next argument.
  va_end(ap);

	TString treename="";
	TFile *test = TFile::Open(files.at(0));
  TIter nextkey( test->GetListOfKeys() );
  TKey *key=0;
  while ( (key = (TKey*)nextkey())) {
	 	//cout << key->GetName() <<endl;
     TObject *obj = key->ReadObj();
     if ( obj->IsA()->InheritsFrom( TTree::Class() ) ) {
	 		treename = key->GetName();
	 		break;
	 	}
	}

	test->Close();
	TChain *chain = new TChain(treename);
	for(int i=0;i<files.size();i++){
		chain->AddFile(files[i]);
		//cout << i << " " << files[i] << " " << chain->GetEntries() <<endl;
	}
}
