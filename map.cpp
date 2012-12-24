#include <commandmap.hpp>
#include <string>
#include <utility>
#include <list>
#include <fstream>
#include <algorithm>
#include <vector>

#include <boost/serialization/list.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using namespace std;

enum {MAKE, DUMP, QUIT, SEARCH, WRITE};
enum {NEXT, DELETE};


class OpMap : public CommandMap<int> {
  OpMap(){
    map_["n"] = NEXT;
    map_["x"] = DELETE;
  }
public:
  static const OpMap &instance() {
    static const OpMap inst;
    return inst;
  }
};


class CMap : public CommandMap<int> {
  CMap(){
    map_["m"] = MAKE;
    map_["d"] = DUMP;
    map_["q"] = QUIT;
    map_["s"] = SEARCH;
    map_["w"] = WRITE;
  }
public:
  static const CMap &instance() {
    static const CMap inst;
    return inst;
  }
};


typedef std::list<std::pair<string,string> > Map;

void printline(const pair<string,string> &p){
  cout << p.first << " :: " << p.second << endl;
}

bool rettrue(const pair<string, string> &) {
  return true;
}

template <class TestElem>
void operate(Map &map, TestElem f){
  Map::iterator it, itend;
  itend = map.end();
  for(it=map.begin(); it!=itend;)
    if(f(*it)){
      printline(*it);
      cout << "> " << flush;
      switch(OpMap::instance().query()){
      case NEXT:
	++it;
	break;
      case DELETE:
	it = map.erase(it);
	break;
      }
    }
}

template <class DumpElem>
void dump(const Map &m, DumpElem f){
  Map::const_iterator it, itend;
  itend = m.end();
  for(it=m.begin(); it!=itend; ++it)
    if(f(*it))
      printline(*it);
}

void write_map(const char *filep, const Map &m){
  ofstream ofs(filep);
  if(ofs.fail())
    throw "error in opening ofstream!";
  boost::archive::text_oarchive archive(ofs);
  archive << m;
}


void interactive_mode(const char *filepath, Map &map){

  int ans;
  string key;
  string val;
  Map::iterator it, itend;

  while((ans = CMap::instance().query()) != QUIT){
    switch(ans){
    case MAKE:
      cout << "\tKey: " << flush;
      getline(cin, key);
      if(key.empty()){
	cout << "key empty!" << endl;
	break;
      }
      cout << "\tVal: " << flush;
      getline(cin, val);
      if(val.empty()){
	cout << "val empty!" << endl;
	break;
      }
      map.push_back(make_pair(key, val));
      break;
    case SEARCH:
      cout << "\tKey: " << flush;
      getline(cin, key);
      itend = map.end();
      for(it=map.begin(); it != itend; ++it)
	if(it->first.find(key) != string::npos)
	  printline(*it);
      break;
    case DUMP:
      dump(map, rettrue);
      break;
    case WRITE:
      write_map(filepath, map);
      break;
    }
  }
}

void query_mode(const Map &map, const char *query){
  typedef Map::const_iterator iter;
  iter it;
  const iter itend(map.end());
  vector<iter> candidates;

  for(it=map.begin(); it!=itend; ++it)
    if(it->first.find(query) != string::npos)
      candidates.push_back(it);

  switch(candidates.size()){
  case 0:
    cout << "no matching data" << endl;
    break;
  case 1:
    cout << candidates.front()->second << endl;
    break;
  default:
    for_each
      (candidates.begin(),
       candidates.end(),
       [](iter it){ printline(*it); });
    break;
  }
}

void insert_mode(const char *filepath, Map &map, string key, string value){
  map.push_back(make_pair(key,value));
  write_map(filepath, map);
}

int main(int argc, char **argv){

  if(argc == 1 || argc > 4){
    cout << "usage: " << argv[0] << " <archive> [<key> [<value>]]" << endl;
    return 0;
  }

  Map map;
  ifstream ifs(argv[1]);
  if(!ifs.fail()){
    boost::archive::text_iarchive archive(ifs);
    archive >> map;
  }

  switch(argc){
  case 2:
    interactive_mode(argv[1], map);
    break;
  case 3:
    query_mode(map, argv[2]);
    break;
  case 4:
    insert_mode(argv[1], map, argv[2], argv[3]);
    break;
  }
}
