# stream-algorithms
stream algorithms implementation for TAA course 2021.2


# Como instalar:
    Abra o terminal na root do projeto e rode `make install`. Este comando copiará os arquivos necessários para que seja possível rodar os algoritmos apenas com os comandos `cmin` e `kmv` (os comandos funcionarão de qualquer diretório).

# Execução:

##Count Min:
`cmin [options] input_file.csv`.

Opções:
    
    --id <int>: Especifica número da coluna do arquivo de entrada a ser usado como identificador. Esta coluna pode conter valores repetidos.
    default: 0


    --weight <int>: Especifica o número da coluna do arquivo de entrada a ser usado como peso. Esta coluna deve conter inteiros positivos.
    default: 3


    --eps <double>: Especifica o valor do parametro epsilon (e) do estimador (limite do erro relativo desejado).
    default: 0.01


    --delta <double>: Especifica o valor do parametro δ do estimador (limite para a probabilidade que o erro relativo da estimação seja superior ao limite especificado). 
    default: 0.05


    --query <int> <int> ...: Especifica os identificadores dos fluxos cujos pesos devem ser estimados.
    
    
    --qryfile query_file_name: Alternativamente, os identificadores dos fluxos consultados podem ser fornecidos num arquivo com um identificador por linha atraves desta opção.
    (se nenhuma das duas opções acima forem passadas, o código quebra)

##Count Min:
`kmv [options] input_file.csv`.

Opções:

    --target <int>: Especifica o número da coluna alvo (coluna ≥ 0) do arquivo de entrada cuja quantidade de valores únicos deve ser estimada. 
    default: 0

    --eps <double>: Especifica o valor do parâmetro ε e do estimador (limite do erro relativo desejado). 
    default: 0.05

    --delta <double>: Especifica o valor do parâmetro δ do estimador (limite para a probabilidade que o erro relativo da estimação seja superior ao limite especificado).
    default: 0.01

# Como desinstalar:
Para desinstalar, basta voltar para a root do projeto e rodar `make uninstall`.