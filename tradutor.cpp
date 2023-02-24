#include <iostream>

#include "src/pre.h"
#include "src/tipos.h"
#include "src/utilitarios.h"

using namespace std;
int main(int argc, char *argv[])
{
  string nome_arquivo = argv[1];
  string conteudo = lerArquivo(nome_arquivo);
  Codigo codigo = processarLinhas(conteudo); // Processa as linhas do arquivo
#ifdef DEBUG
  cout << "\nCodigo original:" << endl;
  dumpCodigo(codigo);
#endif

  codigo = processarEquates(codigo);
#ifdef DEBUG
  cout << "\nCodigo depois de processar equates:" << endl;
  dumpCodigo(codigo);
#endif
  codigo = processarIfs(codigo);
#ifdef DEBUG
  cout << "\nCodigo depois de processar ifs:" << endl;
  dumpCodigo(codigo);
#endif

  Codigo secao_texto_IA32, secao_dados_IA32, secao_bss_IA32;

  secao_texto_IA32.push_back({"SECTION", ".text"}); // Adiciona a seção .text
  secao_texto_IA32.push_back({"_start", ":"});      // Adiciona a label _start
  
  secao_dados_IA32.push_back({"SECTION", ".data"}); // Adiciona a seção .data

  secao_dados_IA32.push_back({"output_msg", "db", "0xa", ",", "'Quantidade de Bytes lidos/escritos = '", ",", "0x0"});
  secao_dados_IA32.push_back({"len_output_msg", "equ", "$", "-", "output_msg"});
  secao_dados_IA32.push_back({"new_line", "db", "0xa", ",", "0x0"});
  secao_dados_IA32.push_back({"minus_str", "db", "'-',", "0x0"});

  secao_bss_IA32.push_back({"SECTION", ".bss"}); // Adiciona a seção .bss
  secao_bss_IA32.push_back({"store_char", "resb", "1"});
  secao_bss_IA32.push_back({"buffer_number", "resb", "10"});
  secao_bss_IA32.push_back({"buffer_number_read", "resb", "10"});

  // Registrador utilizado apenas o EAX
  int tamanho = codigo.size();
  for (int i = 0; i < tamanho; i++)
  {

    LinhaCodigo linha = codigo[i];
    // verificar se a linha[1] [text+1]
    // Iterar sobre a linha e verificar se tem um valor
    for (int j = 0; j < linha.size(); j++)
    {
      if (regex_match(linha[j], reSymbolPlusValue))
      {
        linha[j] = linha[j] + "*4";
      }
    }
    if (regex_match(linha[0], reSection)) {
      continue; // Se a linha for uma seção, pula para a próxima
    }
    else if (regex_match(linha[0], reAdd))
    {
      // ADD ONE vire ADD EAX, [ONE] || ADD 1 vire ADD EAX, 1 || ADD ONE+1 vire ADD EAX, [ONE+1]
      // Verifica se o segundo elemento da linha é um valor
      if (regex_match(linha[1], reValue))
      {
        secao_texto_IA32.push_back({"ADD", "EBX", ",", linha[1]}); // Adiciona a linha ADD EBX, 1
      }
      else
      { // Se não for um valor, é uma variável
        secao_texto_IA32.push_back({"ADD", "EBX", ",", "[", linha[1], "]"}); // Adiciona a linha ADD EBX, [ONE]
      }
    }
    else if (regex_match(linha[0], reSub))
    {
      // Verifica se o segundo elemento da linha é um valor
      if (regex_match(linha[1], reValue))
      {
        secao_texto_IA32.push_back({"SUB", "EBX", ",", linha[1]}); // Adiciona a linha SUB EBX, 1
      }
      else
      { // Se não for um valor, é uma variável
        secao_texto_IA32.push_back({"SUB", "EBX", ",", "[", linha[1], "]"}); // Adiciona a linha SUB EBX, [ONE]
      }
    }

    else if (regex_match(linha[0], reMul))
    {
      // Verifica se o segundo elemento da linha é um valor
      if (regex_match(linha[1], reValue))
      {
        secao_texto_IA32.push_back({"push", "EAX"}); // Adiciona a linha PUSH EAX 
        secao_texto_IA32.push_back({"mov", "EAX", ",", "EBX"}); // Adiciona a linha MOV EAX, EBX
        secao_texto_IA32.push_back({"mov", "EBX", ",", linha[1]}); // Adiciona a linha MOV EBX, 1
        secao_texto_IA32.push_back({"imul", "EBX"}); // Adiciona a linha IMUL EBX
        secao_texto_IA32.push_back({"mov", "EBX", ",", "EAX"});  // Adiciona a linha MOV EBX, EAX
        secao_texto_IA32.push_back({"pop", "EAX"}); // Adiciona a linha POP EAX
      }
      else
      { // Se não for um valor, é uma variável
        secao_texto_IA32.push_back({"push", "EAX"}); // Adiciona a linha PUSH EAX
        secao_texto_IA32.push_back({"mov", "EAX", ",", "EBX"}); // Adiciona a linha MOV EAX, EBX
        secao_texto_IA32.push_back({"imul", "dword", "[", linha[1], "]"}); // Adiciona a linha IMUL EBX, [ONE]
        secao_texto_IA32.push_back({"mov", "EBX", ",", "EAX"}); // Adiciona a linha MOV EBX, EAX
        secao_texto_IA32.push_back({"pop", "EAX"}); // Adiciona a linha POP EAX
      }
    }
    else if (regex_match(linha[0], reDiv))
    {
      // Verifica se o segundo elemento da linha é um valor
      if (regex_match(linha[1], reValue))
      {
        secao_texto_IA32.push_back({"push", "EAX"}); // Adiciona a linha PUSH EAX
        secao_texto_IA32.push_back({"push", "EDX"}); // Adiciona a linha PUSH EDX
        secao_texto_IA32.push_back({"mov", "EAX", ",", "EBX"}); // Adiciona a linha MOV EAX, EBX
        secao_texto_IA32.push_back({"mov", "EBX", ",", linha[1]}); // Adiciona a linha MOV EBX, 1
        secao_texto_IA32.push_back({"mov", "EDX", ",", "0"}); // Adiciona a linha MOV EDX, 0
        secao_texto_IA32.push_back({"idiv", "EBX"}); // Adiciona a linha IDIV EBX
        secao_texto_IA32.push_back({"mov", "EBX", ",", "EAX"}); // Adiciona a linha MOV EBX, EAX
        secao_texto_IA32.push_back({"push", "EBX"}); // Adiciona a linha PUSH EBX
        secao_texto_IA32.push_back({"call", "modulo"}); // Adiciona a linha CALL modulo -> retorna o resultado em EAX
        secao_texto_IA32.push_back({"pop", "EBX"}); // Adiciona a linha POP EBX
        secao_texto_IA32.push_back({"mov", "EBX", ",", "EAX"}); // Adiciona a linha MOV EBX, EAX -> EBX recebe o resultado da divisão
        secao_texto_IA32.push_back({"pop", "EDX"}); // Adiciona a linha POP EDX
        secao_texto_IA32.push_back({"pop", "EAX"}); // Adiciona a linha POP EAX
      }
      else
      { // Se não for um valor, é uma variável
        secao_texto_IA32.push_back({"push", "EAX"}); // Adiciona a linha PUSH EAX
        secao_texto_IA32.push_back({"push", "EDX"}); // Adiciona a linha PUSH EDX
        secao_texto_IA32.push_back({"mov", "EAX", ",", "EBX"}); // Adiciona a linha MOV EAX, EBX
        secao_texto_IA32.push_back({"mov", "EDX", ",", "0"}); // Adiciona a linha MOV EDX, 0
        secao_texto_IA32.push_back({"div", "dword", "[", linha[1], "]"}); // Adiciona a linha DIV [ONE]
        secao_texto_IA32.push_back({"mov", "EBX", ",", "EAX"}); // Adiciona a linha MOV EBX, EAX
        secao_texto_IA32.push_back({"pop", "EDX"}); // Adiciona a linha POP EDX
        secao_texto_IA32.push_back({"pop", "EAX"}); // Adiciona a linha POP EAX
      }
    }
    // JMP - JUMP
    else if (regex_match(linha[0], reJmp))
    {
      secao_texto_IA32.push_back({"jmp", linha[1]});
    }
    // JMPN - JUMP IF NEGATIVE
    else if (regex_match(linha[0], reJmpn))
    {
      secao_texto_IA32.push_back({"cmp", "EBX", ",", "0"});
      secao_texto_IA32.push_back({"jl", linha[1]});
    }
    // JMPP - JUMP IF POSITIVE
    else if (regex_match(linha[0], reJmpp))
    {
      secao_texto_IA32.push_back({"cmp", "EBX", ",", "0"});
      secao_texto_IA32.push_back({"jg", linha[1]});
    }

    else if (regex_match(linha[0], reJmpz))
    {
      secao_texto_IA32.push_back({"cmp", "EBX", ",", "0"});
      secao_texto_IA32.push_back({"jz", linha[1]});
    }
    else if (regex_match(linha[0], reCopy))
    {
      secao_texto_IA32.push_back({"push", "EBX"});
      secao_texto_IA32.push_back({"mov", "EBX", ",", "dword", "[", linha[1], "]"});
      secao_texto_IA32.push_back({"mov", "[", linha[3], "]", ",", "dword", "EBX"});
      secao_texto_IA32.push_back({"pop", "EBX"});
    }
    else if (regex_match(linha[0], reLoad))
    {
      secao_texto_IA32.push_back({"mov", "EBX", ",", "dword", "[", linha[1], "]"});
    }
    else if (regex_match(linha[0], reStore))
    {
      secao_texto_IA32.push_back({"mov", "dword", "[", linha[1], "]", ",", "dword", "EBX"});
    }
    else if (regex_match(linha[0], reInput))
    {
      secao_texto_IA32.push_back({"push", "EBX"}); // Adiciona a linha PUSH EBX
      secao_texto_IA32.push_back({"push", linha[1]}); // Adiciona a linha PUSH 1
      secao_texto_IA32.push_back({"call", "input"}); // Adiciona a linha CALL input
      secao_texto_IA32.push_back({"pop", "ebx"}); // Adiciona a linha POP EBX
      secao_texto_IA32.push_back({"pop", "ebx"}); // Adiciona a linha POP EBX
    }
    else if (regex_match(linha[0], reOutput))
    {
      secao_texto_IA32.push_back({"push", "EBX"}); // Adiciona a linha PUSH EBX
      secao_texto_IA32.push_back({"push", linha[1]}); // Adiciona a linha PUSH 1
      secao_texto_IA32.push_back({"call", "output"}); // Adiciona a linha CALL output
      secao_texto_IA32.push_back({"pop", "ebx"}); // Adiciona a linha POP EBX
      secao_texto_IA32.push_back({"pop", "ebx"}); // Adiciona a linha POP EBX
    }

    else if (regex_match(linha[0], reInputS))
    {
      secao_texto_IA32.push_back({"push", "EBX"}); // Adiciona a linha PUSH EBX
      secao_texto_IA32.push_back({"push", linha[1]}); // Adiciona a linha PUSH [STR]
      secao_texto_IA32.push_back({"push", linha[3]});  // Adiciona a linha PUSH 1
      secao_texto_IA32.push_back({"call", "input_s"}); // Adiciona a linha CALL input_s
      secao_texto_IA32.push_back({"pop", "ebx"}); // Adiciona a linha POP EBX
      secao_texto_IA32.push_back({"pop", "ebx"}); // Adiciona a linha POP EBX
      secao_texto_IA32.push_back({"pop", "ebx"}); // Adiciona a linha POP EBX
    }
    else if (regex_match(linha[0], reOutputS))
    {
      secao_texto_IA32.push_back({"push", "EBX"});
      secao_texto_IA32.push_back({"push", linha[1]});
      secao_texto_IA32.push_back({"push", linha[3]});
      secao_texto_IA32.push_back({"call", "output_s"});
      secao_texto_IA32.push_back({"pop", "ebx"});
      secao_texto_IA32.push_back({"pop", "ebx"});
      secao_texto_IA32.push_back({"pop", "ebx"});
    }

    else if (regex_match(linha[0], reInputC))
    {
      secao_texto_IA32.push_back({"push", "EBX"});
      secao_texto_IA32.push_back({"push", linha[1]});
      secao_texto_IA32.push_back({"call", "input_c"});
      secao_texto_IA32.push_back({"pop", "ebx"});
      secao_texto_IA32.push_back({"pop", "ebx"});
    }
    else if (regex_match(linha[0], reOutputC))
    {
      secao_texto_IA32.push_back({"push", "EBX"});
      secao_texto_IA32.push_back({"push", linha[1]});
      secao_texto_IA32.push_back({"call", "output_c"});
      secao_texto_IA32.push_back({"pop", "ebx"});
      secao_texto_IA32.push_back({"pop", "ebx"});
    }
    else if (regex_match(linha[0], reStop))
    {
      secao_texto_IA32.push_back({"mov", "EAX", ",", "1"});
      secao_texto_IA32.push_back({"mov", "EBX", ",", "0"});
      secao_texto_IA32.push_back({"int", "80h"});
    }
    else if (linha[1] == ":" & linha.size() >= 3)
    {
      if (regex_match(linha[2], reSpace) & linha.size() > 3)
      {
        secao_bss_IA32.push_back({linha[0], "resd", linha[3]});
      }
      else if (regex_match(linha[2], reSpace))
      {
        secao_bss_IA32.push_back({linha[0], "resd", "1"});
      }
      else if (regex_match(linha[2], reConst))
      {
        // verificar se linha[3] é uma letra e transformar em hexadecimal
        if (regex_match(linha[3], reChar))
        {
          int letra = linha[3][1];
          stringstream ss;
          ss << hex << letra;
          string letraHexa = ss.str();
          secao_dados_IA32.push_back({linha[0], "dd", "0x" + letraHexa});
        }
        else
        {
          secao_dados_IA32.push_back({linha[0], "dd", linha[3]});
        }
      }
    }
    else if (regex_match(linha[0], reLabel))
    {
      secao_texto_IA32.push_back({linha[0], linha[1]});
    }
    else
    {
      cout << "Erro: Código inválido" << endl;
      cout << linha[0] << endl;
      return 1;
    }
  }

  // concatena secao_dados_IA32, secao_bss_IA32 e secao_texto_IA32
  secao_texto_IA32.insert(secao_texto_IA32.begin(), {"global", "_start"});
  secao_texto_IA32.insert(secao_texto_IA32.begin(), secao_bss_IA32.begin(), secao_bss_IA32.end());
  secao_texto_IA32.insert(secao_texto_IA32.begin(), secao_dados_IA32.begin(), secao_dados_IA32.end());
  // retirar .asm do nome do arquivo
  nome_arquivo = nome_arquivo.substr(0, nome_arquivo.size() - 4);

  salvarArquivo(nome_arquivo + ".s", secao_texto_IA32);
  // Char
  string conteudo_io_c = lerArquivo("./src/io_c.asm");
  string conteudo_io_s = lerArquivo("./src/io_s.asm");
  string conteudo_io = lerArquivo("./src/io.asm");
  ofstream myfile;
  myfile.open(nome_arquivo + ".s", ios::app);
  myfile << conteudo_io_c;
  myfile << conteudo_io_s;
  myfile << conteudo_io;
  myfile.close();

  return 0;
}
