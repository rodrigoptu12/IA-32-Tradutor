#ifndef TIPOS_H
#define TIPOS_H

#include <map>
#include <regex>
#include <string>
#include <vector>

using namespace std;

typedef map<int, vector<string>> LinhaMap;

typedef vector<string> LinhaCodigo;

typedef vector<LinhaCodigo> Codigo;

typedef map<string, int> TabSimbolos;

// Macro Name Table
typedef struct {
  int numArgs;
  int linha;
} Macro;

typedef map<string, Macro> MNTMap;

extern string valuesToSintaticAnalysis;
extern string uppercaseInsts1Arg;
extern string lowercaseInsts1Arg;
extern string uppercaseInsts;
extern string lowercaseInsts;
extern string uppercaseDirectives;
extern string lowercaseDirectives;
extern string uppercaseSections;
extern string lowercaseSections;
extern string labels;
extern string values;
extern string separators;

extern regex reBeforeValue;
extern regex reValuesNoLabel;
extern regex reInst1Args;
extern regex reValues;
extern regex secDataValues;

extern regex reDeveTerEspacoNaDireita;

extern regex reAdd;
extern regex reSub;
extern regex reMul;
extern regex reDiv;
extern regex reJmp;
extern regex reJmpn;
extern regex reJmpp;
extern regex reJmpz;
extern regex reCopy;
extern regex reLoad;
extern regex reStore;
extern regex reInput;
extern regex reOutput;
extern regex reStop;
extern regex reSpace;
extern regex reConst;
extern regex reSection;
extern regex rePublic;
extern regex reExtern;
extern regex reEqu;
extern regex reIf;
extern regex reMacro;
extern regex reEndmacro;
extern regex reBegin;
extern regex reEnd;
extern regex reText;
extern regex reData;
extern regex reLabel;
extern regex reValue;
extern regex reSymbolPlusValue;
extern regex reSeparator;
extern regex reComment;
extern regex reMacroParam;
extern regex reParametro;
extern regex reOutputC;
extern regex reInputC;
extern regex reInputS;
extern regex reOutputS;
extern regex reOutput;
extern regex reInput;
extern regex reChar;
#endif  // TIPOS_H