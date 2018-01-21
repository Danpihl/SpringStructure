#include "functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>

using namespace std;

std::vector< vector<float> > read_positions(std::string file_name) {

  ifstream myReadFile;
  myReadFile.open(file_name);

  std::string s;
  std::string delimiter = ",";
  std::string token;
  size_t pos = 0;
  std::vector<std::string> substrings;
  std::vector<float> floatcols;
  std::vector< vector<float> > floatdata;

  if (myReadFile.is_open()) {
    while (!myReadFile.eof()) {

      myReadFile >> s;

      if(myReadFile.eof())
        break;
      substrings.clear();
      floatcols.clear();

      while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        substrings.push_back(token);
        s.erase(0, pos + delimiter.length());

      }
      substrings.push_back(s);

      for(int i = 0; i < substrings.size(); i++) {
        floatcols.push_back(atof(substrings[i].c_str()));
      }

      floatdata.push_back(floatcols);

    }
  }
  myReadFile.close();

  return floatdata;

}

std::vector< vector<int> > read_connectivity_graph(std::string file_name) {

  ifstream myReadFile;
  myReadFile.open(file_name);
  //char output[100];

  std::string s;
  std::string delimiter = ",";
  std::string token;
  size_t pos = 0;
  std::vector<std::string> substrings;
  std::vector<int> intcols;
  std::vector< vector<int> > intdata;

  if (myReadFile.is_open()) {
    while (!myReadFile.eof()) {

      myReadFile >> s;

      if(myReadFile.eof())
        break;
      substrings.clear();
      intcols.clear();

      while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        substrings.push_back(token);
        s.erase(0, pos + delimiter.length());

      }
      substrings.push_back(s);

      for(int i = 0; i < substrings.size(); i++) {
        intcols.push_back(atoi(substrings[i].c_str()));
      }

      intdata.push_back(intcols);

    }
  }
  myReadFile.close();


  return intdata;

}

std::vector<int> split_string(std::string s, std::string delimiter) {

  std::vector<int> o;
  

  int l = s.size();

  for(int i = 0; i < l; i++) {

    

  }

  return o;

}