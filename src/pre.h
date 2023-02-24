#ifndef PRE_H
#define PRE_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "tipos.h"

using namespace std;

Codigo processarLinhas(string conteudo);
Codigo processarEquates(const Codigo& codigo);
Codigo processarIfs(const Codigo& codigo);
vector<string> tokenize(const string& str);

#endif  // PRE_H