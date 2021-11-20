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
  Prints all items into terminals set in the form
  Terminales: a, b, c
  @param terminals: Set with all string terminals
*/
void print_terminals(set<string> &terminals){
  set<string> :: iterator it;
  int i=0;
  cout << "Terminales: ";
  for(it = terminals.begin(); it != terminals.end(); ++it, i++){
    i==terminals.size()-1 ? cout << *it : cout << *it << ", ";
  }
}

/**
  Prints all items into nonterminals set in the form
  No terminales: A, B, C
  @param nonterminals: Set with all string nonterminals
*/
void print_nonterminals(set<string> &nonterminals){
  set<string> :: iterator it;
  int i=0;
  cout << "No terminales: ";
  for(it = nonterminals.begin(); it != nonterminals.end(); ++it, i++){
    i==nonterminals.size()-1 ? cout << *it : cout << *it << ", ";
  }
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

  cout << endl;
  print_map(productions);
  get_terminals(productions, terminals);
  get_nonterminals(productions, nonterminals);
  cout << endl;

  print_terminals(terminals);
  cout << endl;
  print_nonterminals(nonterminals);
  cout << endl;

  return 0;
}
