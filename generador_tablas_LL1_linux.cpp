/**
  TC3048.1: Compilers Design
  @file generador_tablas_LL1_linux.cpp
  @author Roberto Carlos Guzman Cortes
  @version 1.0 10/21/2021
*/

#include <iostream>
#include <algorithm>
#include <string>
#include <iterator>
#include <map>
#include <set>
#include <list>
#include <fstream>

using namespace std;

#define MAX_PRODUCTION_LEN 64
#define MAX_STRING_LEN 255

/**
  Verifies that the string is epsilon ' '.
  @param sub_string: String which length is less than 64 characters
  @return boolean True if epsiolon, False otherwise
*/
bool isEpsilon(string sub_string){
  if(sub_string.length()==3 && sub_string[0] == 39 && sub_string[1] == 32 && sub_string[2] == 39){
    return true;
  }
  return false;
}

/**
  If the string contains spaces different
  from epsilon ' ', it will reformat str in the form
  token1|token2|token3|...|tokenN|' '.
  @param str: String to be reformatted
  @return String reformatted
*/
string replace_spaces_with_pipes(string str){
  int space_index;

  if(str.find(' ') < str.length()){
    for(int k=1; k<str.length()-1; k++){
      space_index = str.find(' ');
      if(space_index < str.length() && str[space_index-1]!=39 && str[space_index+1]!=39){ // If not epsilon will replace spaces with |
        str[space_index] = '|';
      }
    }
  }
  return str;
}

/**
  Gets and returns production's header only.
  @param production: String formatted in the way
  header -> body
  @return production header
*/
string get_header(string production){
  string header;
  int limit;

  limit = production.find(' ');
  header = production.substr(0, limit);

  return header;
}

/**
  Separates production's header from production's body, uses
  production's header as productions map key and production's body as its map
  value. If production's header already exists as a map key, production's
  body is concatenated.
  @param productions: Map with productio's headers as string keys and
  production's bodies as a list of string values
  @param production: 64-length char array in format
  production's header -> production's body
*/
void separate_header_body(map<string, list<string>> &productions, char production[MAX_PRODUCTION_LEN]){
    string str = production;
    int limit = 0;

    string header;
    limit = str.find(' ');
    header = str.substr(0, limit);

    string body;
    limit = str.find('>');
    body = str.substr(limit+2, str.length()-1);

    auto itr = productions.find(header);

    if(itr == productions.end()){ // New header
      list<string> aux;

      aux.push_back(body);
      productions.insert({header, aux});
      aux.pop_front();
    }else{ // Header already exists
      itr->second.push_back(body);
    }
}

/**
  Receives two sets and merges them into a new one.
  @param set1: Set of Strings
  @param set2: Set of Strings
  @return new Set
*/
set<string> merge_sets(set<string> &set1, set<string> &set2){
  set<string> smallest, largest;
  set<string> :: iterator it;

  if(set1.size() >= set2.size()){
    smallest = set2;
    largest = set1;
  }else{
    smallest = set1;
    largest = set2;
  }
  for(it = smallest.begin(); it != smallest.end(); ++it){
    largest.insert(*it);
  }
  return largest;
}

/**
  Checks if set contains epsilon.
  @param set1: Set of Strings
  @return boolean Ture if contains epsilon, False otherwise
*/
bool contains_epsilon(set<string> &set1){
  set<string> :: iterator it;
  for(it = set1.begin(); it != set1.end(); ++it){
      if(isEpsilon(*it)) return true;
  }
  return false;
}

/**
  Removes epsilon from Set.
  @param set1: Set of Strings
*/
void remove_epsilon(set<string> &set1){
  set<string> :: iterator it;
  for(it = set1.begin(); it != set1.end(); ++it){
    if(isEpsilon(*it)){
      set1.erase(it);
      return;
    }
  }
  return;
}

/**
  Gets all values from productions map.
  After that, it will check tokens that are not productions map key and put them
  into terminals set.
  @param productions: Map with production's headers as astring keys and
  production's bodies as a list of string values
  @param terminals: EMpty set that will be filled with all string terminals
*/
void get_terminals(map<string, list<string>> &productions, set<string> &terminals){
  list<string> bodies;
  string sub_string;

  for(auto itr1 = productions.begin(); itr1 != productions.end(); ++itr1){
    bodies = itr1->second;
    while(!bodies.empty()){
      string body = bodies.front();
      body = replace_spaces_with_pipes(body);
      for(int i=0, j=0; i<=body.length(); i++){
        if(body[i]=='|' || body[i]=='\0'){
          sub_string = body.substr(j, i-j);
          j=i+1;
          //is not a map key nor epsilon nor null
          if( productions.find(sub_string) == productions.end() && !isEpsilon(sub_string) && sub_string[0]!='\0' ){
            terminals.insert(sub_string);
          }
        }
      } bodies.pop_front();
    }
  }
}

/**
  Gets all keys from productions map and adds them to terminals set.
  @param productions: Map with production's headers as string keys and
  production's bodies as a list of string values
  @param nonterminals: Empty set that will be filled with all string
  nonterminals
*/
void get_nonterminals(map<string, list<string>> &productions, set<string> &nonterminals){
  for(auto itr1 = productions.begin(); itr1 != productions.end(); ++itr1){
    nonterminals.insert(itr1->first);
  }
}

/**
  Prints all items into set in the form
  Terminales: a, b, c
  or
  No terminales: A, B, C
  @param terminals: Set with all string terminals or nonterminals
  @param flag: Integer value which could be 0 or 1
*/
void print_set(set<string> &terminals, int flag){
  set<string> :: iterator it;
  int i=0;
  flag==0 ? cout << "Terminales: " : cout << "No terminales: ";
  for(it = terminals.begin(); it != terminals.end(); ++it, i++){
    i==terminals.size()-1 ? cout << *it : cout << *it << ", ";
  }
}

/**
  Gets all terminals at the beginning of every production body.
  @param productions: Map with production's headers as string keys
  and production's bodies as a list of string values
  @param body: String production body
  @return String first at the very beginning of body
*/
string get_direct_firsts(map<string, list<string>> &productions, string body){
  string sub_string, first;
  for(int i=0, j=0; i <= body.length(); i++){
    if(body[i]=='|' || body[i]=='\0'){
      sub_string = body.substr(j, i-j);
      j=i+1;

      if( productions.find(sub_string) == productions.end() && !isEpsilon(sub_string)){
        return sub_string;
      } else if(isEpsilon(body)){
        return sub_string;
      } break;
    }
  }
  first = " ";
  return first;
}

/**
  Gets set of derived firsts when nonterminal at the beginning of production body.
  @param firsts: Empty map that will be filled with all productions
  @param key : String production key
  @param body: String production body
  @return Set of derived firsts
*/
set<string> get_derived_firsts(map<string, set<string>> &firsts, string key, string body){
  set<string> aux_set;
  string sub_string;

  for(int i=0, j=0; i<=body.length(); i++){
    if(body[i]=='|' || body[i]=='\0'){
      sub_string = body.substr(j, i-j);
      j=i+1;
      auto itr = firsts.find(sub_string);
      if( itr != firsts.end() && sub_string != key){
        aux_set = itr->second;
        break;
      }
      break;
    }
  }
  return aux_set;
}

/**
  Gets set of firsts when epsilon at the starting nonterminals.
  @param firsts: Empty map that will be filled with all productions
  @param key : String production key
  @param body: String production body
  @return Set of found firsts
*/
set<string> get_firsts_when_epsilon(map<string, set<string>> &firsts, string key, string body){
  bool flag = false;
  set<string> set1;
  string sub_string;

  for(int i=0, j=0; i<=body.length(); i++){
    if(body[i]=='|' || body[i]=='\0'){
      sub_string = body.substr(j, i-j);
      j=i+1;
      auto itr = firsts.find(sub_string);
      if(itr != firsts.end() && sub_string != key){
        set<string> set2 = get_derived_firsts(firsts, key, sub_string);

        if(contains_epsilon(set2)){
            flag = true;
            set1.empty() ? set1 = set2 : set1 = merge_sets(set1, set2);
            continue;
        }else{
          break;
        }

      }else if(sub_string == key){
        break;
      }else if(itr == firsts.end()){
        set1.insert(sub_string);
        flag = false;
        break;
      }
    }
  }

  if(contains_epsilon(set1) && flag==false){
    remove_epsilon(set1);
  }
  return set1;
}

/**
  Gets all firsts for every nonterminal in production map, and
  will put them into firsts maps.
  @param productions: Map with production's headers as string keys
  and production's bodies as a list of string values
  @param firsts: Empty map that will be filled with all productions
  firsts into a new list for every nonterminal
*/
void get_firsts(map<string, list<string>> &productions, map<string, set<string>> &firsts){
  // Gets direct firsts
  for(auto itr = productions.begin(); itr != productions.end(); ++itr){
    string f, key = itr->first;
    list<string> bodies = itr->second;
    while(!bodies.empty()){
      string body = bodies.front();
      body = replace_spaces_with_pipes(body);
      if(get_direct_firsts(productions, body) != " "){
        f = get_direct_firsts(productions, body);
        auto itr2 = firsts.find(key);
        if(itr2 == firsts.end()){
          set<string> aux_set;
          aux_set.insert(f);
          firsts.insert({key, aux_set});
        }else{
          itr2->second.insert(f);
        }
      }
      bodies.pop_front();
    }
  }

  // Gets derived firsts
  for(int k=0; k<2; k++){
    for(auto itr = productions.begin(); itr != productions.end(); ++itr){
      set<string> aux_set;
      string key = itr->first;
      list<string> bodies = itr->second;
      while(!bodies.empty()){
        string aux_body = bodies.front(), body = bodies.front();
        body = replace_spaces_with_pipes(body);
        aux_set = get_derived_firsts(firsts, key, body);

        if(!aux_set.empty()){
          auto itr2 = productions.find(aux_body);
          if(contains_epsilon(aux_set) && itr2 == productions.end()){
            remove_epsilon(aux_set);
          }
          auto itr3 = firsts.find(key);
          set<string> aux_set2;
          if(!itr3->second.empty()){
            aux_set2 = merge_sets(itr3->second, aux_set);
          }else{
            aux_set2 = aux_set;
          }
          if(itr3 != firsts.end()){
            itr3->second = aux_set2;
          }else{
            firsts.insert({key, aux_set2});
          }
        }
        bodies.pop_front();
      }
    }
  }
  // Gets firsts when epsilon
  for(auto itr = productions.begin(); itr != productions.end(); ++itr){
    set<string> aux_set;
    string key = itr->first;
    list<string> bodies = itr->second;
    while(!bodies.empty()){
      string body = bodies.front();
      body = replace_spaces_with_pipes(body);
      aux_set = get_firsts_when_epsilon(firsts, key, body);

      if(!aux_set.empty()){
        auto itr2 = firsts.find(key);
        set<string> aux_set2;
        if(!itr2->second.empty()){
          aux_set2 = merge_sets(itr2->second, aux_set);
        }else{
          aux_set2 = aux_set;
        }
        if(itr2 != firsts.end()){
          itr2->second = aux_set2;
        }else{
          firsts.insert({key, aux_set2});
        }
      }
      bodies.pop_front();
    }
  }

}

/**
  Gets follows at the right side of every nonterminal
  @param key: String nonterminal key which follows will be gotten
  @param productions: Map with productio's headers as string keys and
  production's bodies as a list of string values
  @param firsts: Map with production's header a string keys and
  production's firsts as a set of string values
  @param initial_header: String nonterminal key which corresponds to
  the header of the very first production
  @return Set of Strings fillwd with all gotten follows by the given key
*/
set<string> get_follows_into_bodies(string key, map<string, list<string>> &productions, map<string, set<string>> &firsts, string initial_header){
  set<string> follows;

  return follows;
}

/**
  Gets all follows for every nonterminal in production map.
  @param productions: Map with productio's headers as string keys and
  production's bodies as a list of string values
  @param firsts: Map with production's header a string keys and
  production's firsts as a set of string values
  @param follows: Map with production's header as string keys and
  production's follows as a set of string values
  @param initial_header: String nonterminal key which corresponds to
  the header of the very first production
*/
void get_follows(map<string, list<string>> &productions, map<string, set<string>> &firsts, map<string, set<string>> &follows, string initial_header){
  for(auto itr = productions.begin(); itr != productions.end(); ++itr){
    set<string> aux;
    string key = itr->first;

    aux = get_follows_into_bodies(key, productions, firsts, initial_header);

    if(aux.empty()){
      //aux = get_follows_end_bodies(key, productions, follows);
    }
    follows.insert({key, aux});
  }
}

/**
  Prints all firsts and follows for every nonterminal in the form
  goal => FIRST = { (, a, b}, FOLLOW = {$}
  @param nonterminals: Set with all string get_nonterminals
  @param firsts: Map with production's header a string keys and
  production's firsts as a set of string values
  @param follows: Map with production's header as string keys and
  production's follows as a set of string values
*/
void print_firsts_follows(set<string> &nonterminals, map<string, set<string>> &firsts, map<string, set<string>> &follows){
  set<string> :: iterator it1, it2, it3;
  int i, j;
  for(it1 = nonterminals.begin(); it1 != nonterminals.end(); ++it1, i++){
    i=0;
    j=0;
    string res = "", res2 = "";
    auto itr1 = firsts.find(*it1);

    if(itr1 != firsts.end()){
      set<string> aux = itr1->second;
      for(it2 = aux.begin(); it2 != aux.end(); ++it2, i++){
        i==aux.size()-1 ? res+=*it2 : res+=*it2+", ";
      }

      auto itr2 = follows.find(*it1);
      if(itr2 != follows.end()){
        set<string> aux2 = itr2->second;
        for(it3 = aux2.begin(); it3 != aux2.end(); ++it3, j++){
          j==aux2.size()-1 ? res2+=*it3 : res2+=*it3+", ";
        }
      }
    } cout << * it1 << " => " << "FIRST" << " = " << "{" << res << "}, " << "FOLLOW" << "{" << res2 << "}" << endl;
  }
}

/**
  Prints productions map items in the form
  A -> ( A ) a
  B -> b ' '
  C -> goal
  goal -> A
  @param productions: Map with productio's headers as string keys and
  production's bodies as a list of string values
*/
void print_map(map<string, list<string>> &productions){
  for(auto itr = productions.begin(); itr != productions.end(); ++itr){
    list<string> bodies = itr->second;
    while(!bodies.empty()){
      cout << itr->first << " -> " << bodies.front() << endl;
      bodies.pop_front();
    }
  }
}


int main(){
  int n, m=0;
  char production[MAX_PRODUCTION_LEN], str[MAX_STRING_LEN];

  string initial_production, initial_header;

  map<string, list<string>> productions;
  map<string, map<string, string>> LL1_table;
  map<string, set<string>> firsts, follows;
  set<string> terminals, nonterminals;
  list<string> strings_input;

  cin >> n;// >> m;
  cin.ignore(); // Cleans end of line from last cin

  for(int i=0; i<n; i++){
    cin.getline(production, MAX_PRODUCTION_LEN);
    separate_header_body(productions, production);

    if(i==0){
      initial_production = production;
      initial_header = get_header(initial_production);
    }
  }

  // cout << endl;
  // print_map(productions);
  get_terminals(productions, terminals);
  get_nonterminals(productions, nonterminals);
  get_firsts(productions, firsts);
  get_follows(productions, firsts, follows, initial_header);
  cout << endl;

  // print_set(terminals, 0);
  // cout << endl;
  // print_set(nonterminals, 1);
  // cout << endl;
  print_firsts_follows(nonterminals, firsts, follows);

  return 0;
}
