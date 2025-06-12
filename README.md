Especificação do Projeto

O sistema foi desenvolvido para atender às seguintes especificações:

"Uma planta industrial inteligente realiza medições constantes por meio de sensores distribuídos na linha de produção. As leituras desses sensores são registradas sequencialmente em um arquivo bruto, sem distinção de tipo de sensor nem garantia de ordem temporal.Com o crescimento do volume de dados, tornou-se necessário implementar um sistema que permita consultas rápidas e precisas com base em instantes específicos de tempo."

Programa 1 – Organização de Dados

Processa um arquivo de entrada no formato <ID_SENSOR> .
Identifica os diferentes sensores presentes no arquivo.
Separa os dados em arquivos distintos para cada sensor.
Em cada arquivo de sensor, ordena as leituras por TIMESTAMP em ordem crescente.
Programa 2 – Consulta

Recebe o nome de um sensor e uma data/hora como argumentos.
Localiza, obrigatoriamente usando "busca binária", a leitura registrada com o timestamp mais próximo daquele instante.
Exibe as informações da leitura encontrada.
Programa 3 – Geração de Arquivo de Teste

Recebe um intervalo de tempo (início e fim), nomes de sensores e seus tipos como argumentos.
Gera um arquivo de teste com 2000 leituras aleatórias para cada sensor, com timestamps e valores sorteados.
O arquivo gerado segue o formato <ID_SENSOR> .
Solução:

A solução é feita em C e dividida nos seguintes arquivos:

leitura_sensor.h: Arquivo de cabeçalho que define a estrutura LeituraSensor, usada por todos os programas para padronizar a manipulação dos dados.
gerador_de_testes.c (Implementação do Programa 3): Responsável por criar um arquivo dados_brutos.txt com dados de teste aleatórios e misturados.
organizador_dados.c (Implementação do Programa 1): Processa o arquivo bruto, separando os dados por sensor em arquivos individuais e os organuzando por tempo.
consultor_dados.c (Implementação do Programa 2): Realiza a busca binária em um arquivo de sensor específico para encontrar a leitura mais próxima de um instante de tempo.
Compilação:

Abra o terminal e compile cada programa individualmente. Os comandos abaixo criarão três arquivos executáveis: gerador, organizador e consultor.

Compilar o gerador de testes: "gcc gerador_de_testes.c -o gerador"

Compilar o organizador de dados "gcc organizador_dados.c -o organizador"

Compilar o consultor de dados "gcc consultor_dados.c -o consultor"