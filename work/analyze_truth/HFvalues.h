#include <sparsehash/sparse_hash_map>
//#include <ext/hash_set>
#include "stl_hash.h"
#include <map>

using google::sparse_hash_map;      // namespace where class lives by default
//using ext::hash;  // or __gnu_cxx::hash, or maybe tr1::hash, depending on your OS
//using __gnu_cxx::hash;  // or __gnu_cxx::hash, or maybe tr1::hash, depending on your OS

class HFvalues{

  public:
    struct Values{
      int HFtype;
      float var1, var2;
      //Values( int a, float b, float c ) : HFtype( a ), var1( b ), var2( c ) {};
    };
    bool static initialize();
    bool static getHFvalues(int evnum, int *HFtype, float *var1, float *var2);
  //static std::map<int,Values > HFmap;
  static sparse_hash_map<int,Values, hash<int> > HFmap;

};
