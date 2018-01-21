#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>


std::vector< std::vector<int> > read_connectivity_graph(std::string file_name);
std::vector< std::vector<float> > read_positions(std::string file_name);


#endif
