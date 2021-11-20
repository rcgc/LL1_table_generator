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
  cout << endl;

  return 0;
}
