#include "pre.h"

// remove comentarios, espaços, quebra linhas e transforma no formato de lista
Codigo processarLinhas(string conteudo) {
  // Imprima o conteúdo linha a linha
  istringstream stream(conteudo);
  // mostrar o conteudo do arquivo
  string linha;
  Codigo codigo;
  while (getline(stream, linha)) {
    // se não for vazio, adicionar ao codigo
    LinhaCodigo linha_codigo = tokenize(linha);
    // quebrar label e instrucaao em linhas diferentes
    while (linha_codigo.size() > 2 && linha_codigo[1] == ":" &&
           !regex_match(linha_codigo[2], reSpace) && !regex_match(linha_codigo[2], reConst)) {
      LinhaCodigo linha_label;
      linha_label.push_back(linha_codigo[0]);
      linha_label.push_back(linha_codigo[1]);
      codigo.push_back(linha_label);
      linha_codigo.erase(linha_codigo.begin(), linha_codigo.begin() + 2);
    }

    if (!linha_codigo.empty()) {
      codigo.push_back(linha_codigo);
    }
  }

  return codigo;
}

// quebrar linha em tokens
LinhaCodigo tokenize(const string& str) {
  LinhaCodigo tokens;
  string token;

  for (char c : str) {
    if (c == ';') {
      break;
    }
    if (c == ' ' || c == ',' || c == ':') {
      if (!token.empty()) {
        tokens.push_back(token);
        token.clear();
      }
      if (c != ' ') {
        tokens.push_back(string(1, c));
      }
    } else {
      token += c;
    }
  }

  if (!token.empty()) {
    tokens.push_back(token);
  }

  return tokens;
}

Codigo processarEquates(const Codigo& codigo) {
  Codigo codigoProcessado;
  map<string, string> equMap;  // map de equates -> label_equate: valor
  vector<int> linhasIgnorar;   // indices das linhas que devem ser ignoradas

  for (auto linha = codigo.begin(); linha != codigo.end(); ++linha) {
    LinhaCodigo linha_codigo = *linha;
    // percerrer até encontrar SECTION TEXT
    if (linha_codigo.size() >= 2 && regex_match(linha_codigo[0], reSection) &&
        regex_match(linha_codigo[1], reText)) {
      break;
    }
    // se for label
    if (linha_codigo.size() == 2 && regex_match(linha_codigo[0], reLabel) &&
        linha_codigo[1] == ":") {
      string label = linha_codigo[0];           // possivel label
      int linhaLabel = linha - codigo.begin();  // linha do label
      ++linha;
      if (linha == codigo.end()) {
        break;
      }

      linha_codigo = *linha;  // possivel equ
      // se for equ
      if (linha_codigo.size() == 2 && regex_match(linha_codigo[0], reEqu) &&
          regex_match(linha_codigo[1], reValue)) {
        equMap[label] = linha_codigo[1];                  // adicionar ao map -> label_equate: valor
        linhasIgnorar.push_back(linhaLabel);              // adicionar linha do label para ignorar
        linhasIgnorar.push_back(linha - codigo.begin());  // adicionar linha do equate para ignorar
      }
    }
  }
  // percorrer o codigo e substituir equates e remover linhas ignoradas
  for (int i = 0; i < codigo.size(); ++i) {
    // se for linha a ser ignorada pular
    if (find(linhasIgnorar.begin(), linhasIgnorar.end(), i) != linhasIgnorar.end()) {
      continue;
    }
    LinhaCodigo linha_codigo = codigo[i];                  // linha do codigo
    for (int j = 0; j < linha_codigo.size(); ++j) {        // percorrer tokens da linha
      if (equMap.find(linha_codigo[j]) != equMap.end()) {  // se for equate substituir
        linha_codigo[j] = equMap[linha_codigo[j]];         // substituir pelo valor
      }
    }
    codigoProcessado.push_back(linha_codigo);  // adicionar linha ao codigo processado
  }

  return codigoProcessado;
}

Codigo processarIfs(const Codigo& codigo) {
  Codigo codigoProcessado;
  vector<int> linhasIgnorar;  // indices das linhas que devem ser ignoradas

  for (auto linha = codigo.begin(); linha != codigo.end(); ++linha) {
    LinhaCodigo linha_codigo = *linha;
    // se for if
    if (linha_codigo.size() >= 2 && regex_match(linha_codigo[0], reIf)) {
      int linhaIf = linha - codigo.begin();  // linha do if
      linhasIgnorar.push_back(linhaIf);      // adicionar linha do if para ignorar
      // se for valor valido e for 0 ignorar proxima linha
      if (regex_match(linha_codigo[1], reValue) && stoi(linha_codigo[1]) == 0) {
        ++linha;
        if (linha == codigo.end()) {
          break;
        }
        linhasIgnorar.push_back(linha - codigo.begin());
      }
    }
  }

  // percorrer o codigo e remover linhas ignoradas
  for (int i = 0; i < codigo.size(); ++i) {
    // se for linha a ser ignorada pular
    if (find(linhasIgnorar.begin(), linhasIgnorar.end(), i) != linhasIgnorar.end()) {
      continue;
    }
    codigoProcessado.push_back(codigo[i]);
  }

  return codigoProcessado;
}