#ifndef QDigest_DOCSTRING
#define QDigest_DOCSTRING
#include <iostream>

namespace QDigestDocString
{
    const std::string DOC_STRING = "O algoritmo Q-Digest pode ser usado tanto para saber o rank de um determinado número dentro de uma stream de dados, quanto para saber o quantile de um valor.\n\n"
                                    "Execução:\n"
                                    "   qdig [build_options] data_file.csv [rank | quant] [query_args | --in query_args_file]\n\n"
                                    "Build options:\n"
                                    "    --val <int>: Especifica o número da coluna do arquivo de entrada a ser usada para alimentar o sketch.\n" 
                                    "    default: 0\n\n"
                                    "    --eps <double>: Especifica o valor do parâmetro ε do sketch (limite do erro relativo desejado).\n" 
                                    "    default: 0.1\n\n"
                                    "    --univ <int>: Especifica o valor do parâmetro U do sketch. Valores fora da ranage [0, U-1] serão ignorados.\n"
                                    "    default: 10000\n\n"
                                    "[rank | quant]:\n"
                                    "   Define a operação a ser executada pelo sketch.\n\n"
                                    "[query_args | --in query_args_file]:\n"
                                    "   Define os argumentos de entrada da operação escolhida.\n"
                                    "   Caso queira fornecer os argumentos por meio de um arquivo, utilize a segunda opção, passando um arquivo que contenha os números separados por linha. Caso contrário, passe os argumentos diretamente pelo terminal separados por espaço.\n"
                                    "   Além disso, caso a operação escolhida tenha sido rank, os números da query devem ser inteiros. Caso seja quant, os números devem ser decimais entre 0 e 1.\n";
}


#endif