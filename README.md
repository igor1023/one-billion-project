DESAFIO DE 1 BILHÃO DE LINHAS

Este projeto foi realizado na disciplina de Programação II, no curso Engenharia de Telecomunicações - IFSC câmpus São José.

O desafio de 1 bilhão de linhas trata de processar no menor tempo possível um arquivo contendo 1 bilhão de medições meteorológicas. Uma amostra do arquivo é esta:

Hamburg;12.0
-
Bulawayo;8.9
-
Palembang;38.8
-
St. John's;15.2
-
Cracow;12.6
-

Cada linha do arquivo contém uma medição de temperatura no seguinte formato:

nome da estação meteorológica;temperatura
-

O resultado do processamento deve ser uma listagem das estações meteorológicas com respectivas estatísticas de temperatura:

nome da estação meteorológica=menor/média/maior
-

... sendo menor, média e maior, respectivamente, a menor temperatura, a temperatura média, e a maior temperatura naquela estação meteorológica.
A referência para a implementação do aluno proposta pelo Professor deve ser um tempo de 100 segundos.
Obs.: armazenou-se as informações em uma tabela hash, desenvolvida pelo professor, conforme proposta do docente da disciplina neste projeto.