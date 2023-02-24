#ifndef UTILITARIOS_H
#define UTILITARIOS_H

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "tipos.h"

using namespace std;

int verificarArgumentos(int argc, char* argv[]);
string lerArquivo(string arquivo);
void salvarArquivo(string arquivo, const Codigo& codigo);
void salvarArquivoObjeto(string nome_arquivo, const vector<string>& codigoObjeto);
void dumpMap(const LinhaMap& linhas);
void dumpMnt(const MNTMap& mnt);
void dumpCodigo(const Codigo& codigo);
void dumpLinhaCodigo(const LinhaCodigo& linha);
void dumpTabSimbolos(const TabSimbolos& tabSimbolos);

#endif  // UTILITARIOS_H