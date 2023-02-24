#include "utilitarios.h"

using namespace std;

int verificarArgumentos(int argc, char* argv[]) {
  // argv[0] = ./montador
  // argv[1] = -<op>
  // argv[2] = <arquivo>
  // O arquivo de entrada é um arquivo .asm a extensão não será passada como
  // argumento Verifica se o número de argumentos é válido
  if (argc != 3) {
    throw runtime_error("Número de argumentos inválido");
  }
  // Verifica se o segundo argumento é válido
  if (argv[1][0] != '-') {
    throw runtime_error("Segundo argumento inválido");
  }
  if (argv[1][1] != 'p' && argv[1][1] != 'm' && argv[1][1] != 'o') {
    throw runtime_error("Segundo argumento inválido");
  }
  // Verifica se o terceiro argumento é válido, não deve conter extensão
  string arquivo = argv[2];
  if (arquivo.find('.') != string::npos) {
    throw runtime_error("Terceiro argumento inválido");
  }

  return 0;
}

string lerArquivo(string arquivo) {
  // Abra o arquivo em modo de leitura texto
  ifstream file(arquivo);
  if (!file.is_open()) {
    throw runtime_error("Error opening file");
  }

  // Crie um buffer de memória para armazenar o conteúdo do arquivo
  stringstream buffer;

  // Copie o conteúdo do arquivo para o buffer
  buffer << file.rdbuf();

  // Feche o arquivo
  file.close();

  // Obtenha o conteúdo do buffer como uma string
  string conteudo = buffer.str();

  return conteudo;
}

void salvarArquivo(string arquivo, const Codigo& codigo) {
  // Abra o arquivo em modo de escrita texto
  ofstream file(arquivo);
  if (!file.is_open()) {
    throw runtime_error("Error opening file");
  }

  // Escreva o conteúdo do código no arquivo
  for (int i = 0; i < codigo.size(); i++) {
    LinhaCodigo linha;
    for (auto token : codigo[i]) {
      if (linha.empty()) { // Primeiro token
        linha.push_back(token);
      } else if (token == ":" || token == ",") { // Token especial
        linha.push_back(token);
      } else { // Outros tokens
        if (linha.back() == ",") { // Token anterior é uma vírgula
          linha.push_back(token);
        } else { // Token anterior não é uma vírgula
          linha.push_back(" " + token);
        }
      }
    }
    for (auto token : linha) {
      file << token;
    }
    file << endl;
  }

  // Feche o arquivo
  file.close();
}

void salvarArquivoObjeto(string nome_arquivo, const vector<string>& codigoObjeto) {
  // Salvar arquivo
  ofstream file(nome_arquivo);
  if (!file.is_open()) {
    throw runtime_error("Error opening file");
  }
  for (int i = 0; i < codigoObjeto.size(); i++) {
    file << codigoObjeto[i] << " ";
  }
  file.close();
}

void dumpMap(const LinhaMap& linhas) {
  for (auto it = linhas.begin(); it != linhas.end(); ++it) {
    cout << it->first << " => ";
    for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
      cout << *it2 << " ";
    }
    cout << endl;
  }
}

void dumpMnt(const MNTMap& mnt) {
  for (auto it = mnt.begin(); it != mnt.end(); ++it) {
    cout << it->first << " => N args: " << it->second.numArgs << " Linha:" << it->second.linha
         << endl;
  }
}

void dumpCodigo(const Codigo& codigo) {
  int linha = 1;
  for (auto it = codigo.begin(); it != codigo.end(); ++it) {
    cout << linha++ << ": ";
    for (auto it2 = it->begin(); it2 != it->end(); ++it2) {
      cout << *it2 << " ";
    }
    cout << endl;
  }
}

void dumpLinhaCodigo(const LinhaCodigo& linha) {
  for (auto it = linha.begin(); it != linha.end(); ++it) {
    cout << *it << " ";
  }
  cout << endl;
}

void dumpTabSimbolos(const TabSimbolos& tabSimbolos) {
  for (auto it = tabSimbolos.begin(); it != tabSimbolos.end(); ++it) {
    cout << it->first << " => " << it->second << endl;
  }
}