## Título do Projeto
Estruturas de Dados Probabilísticas e Compactas

## Objetivo

A iniciação científica tem como objetivo geral estudar estruturas de dados probabilísticas e compactas, buscando compreender como diferentes técnicas de representação e aleatoriedade nos permitem resolver problemas de forma mais eficiente em espaço e tempo mantendo uma probabilidade aceitável de erros.

O primeiro semestre será dedicado principalmente ao estudo da fundamentação básica para o desenvolvimento da pesquisa. Serão estudados conceitos introdutórios de teoria da informação, estruturas de dados compactas, probabilidade e hashing, assim como o estudo de algoritmos e estruturas probabilísticas relevantes, como estruturas para resolução de problemas de pertencimento (bloom filter e variantes), estruturas para aproximação de cardinalidade (hyperloglog e variantes), estruturas para problemas de contagem de elementos (count-min sketch), entre outras.

Também serão realizadas implementações e experimentos simples de algumas estruturas estudadas, visando analisar na prática os trade-offs descritos nos artigos teóricos.

Trabalhos recentes como Iceberg Hashing, Tiny Pointers e Succinct and Fast Tiny Pointer Hash Tables foram as principais motivações para a escolha do tema. No entanto, o estudo aprofundado desses trabalhos será deixado principalmente para uma etapa posterior da iniciação científica, após a consolidação da base teórica.

Ao final do semestre, espera-se que o aluno tenha construído repertório suficiente para compreender a literatura sobre estruturas de dados probabilísticas e compactas, reproduzir experimentalmente estruturas básicas da área e delimitar questões mais específicas para investigação durante a segunda metade da iniciação científica.

# Cronograma

O cronograma de estudos foi planejado semanalmente, buscando gastar 6 horas por semana para o estudo de diferentes tópicos.

## Agosto
6 horas por semana, totalizando 24 horas.

Semana 1 (03/08 a 09/08): Foi realizada uma leitura bibliográfica inicial de artigos da área de algoritmos e estruturas de dados. Entre os artigos vistos, destacam-se: "Tiny Pointers", "Iceberg Hashing", "Succint and Fast Tiny Pointer Hash Tables". O objetivo foi compreender a motivação geral da pesquisa, e não compreender a fundo as técnicas empregadas nos artigos. A partir disso consegui decidir o tema da pesquisa e identificar conceitos que precisam ser estudados para melhor compreensão da literatura atual.

Semana 2 (10/08 a 16/08): Estudo de desigualdades de cauda de Markov, Chebyshev e Chernoff. Realizei a aplicação dessas desigualdades para resolução de alguns problemas probabilísticos. Também vi a aplicação da desigualdade de Chernoff para bons limitantes em tentativas de Poisson, que será relevante para a pesquisa futuramente.

Semana 3 (17/08 a 23/08): Estudo do modelo probabilístico de Balls into Bins e variáveis aleatórias de Poisson. Com isso, foi possível analisar hash tables a partir desse modelo. Estudo da estrutura de dados probabilística Bloom-Filter, utilizada para checar o pertencimento de um elemento em um conjunto de forma rápida.

Semana 4 (24/08 a 30/08): Estudo de duas variantes do Bloom Filter voltadas para otimização de espaço e rede, Compressed Bloom Filter e XOR Filter.

## Setembro
6 horas por semana, totalizando 24 horas.

Semana 5 (31/08 a 06/09): Estudo de outra alternativa para o Bloom Filter, o Cuckoo Filter, que utiliza realocação dinâmica de elementos com cuckoo hashing.

Semana 6(07/09 a 13/09): Estudo da técnica de quotienting, utilizada em outra variante do bloom filter, o Quotient Filter. Estudo da estrutura de dados Skip-List, estrutura baseada em listas ligadas com ligações adicionais aleatorizadas.

Semana 7 (14/09 a 20/09): Implementações em C++ de Bloom Filters, XOR Filters e Skip-Lists.

Semana 8 (21/09 a 27/09): Implementações de Cuckoo Filters e Quotient Filters

## Outubro
6 horas por semana, totalizando 30 horas

Semana 9 (28/09 a 04/10): Estudo de algoritmos de streaming para o problema de estimar a cardinalidade de um conjunto, como Count-Distinct e Flajolet-Martin.

Semana 10 (05/10 a 11/10): Estudo do algoritmo Hyperloglog para o problema de cardinalidade. Estudar quais as melhorias realizadas no algoritmo de Hyperloglog++.

Semana 11 (12/10 a 18/10): Estudo de algoritmos para contagem de frequência em streaming, como Count-Sketch e Count-Min Sketch.

Semana 12 (19/10 a 25/10): Implementações em C++ de Hyperloglog, Count-Sketch e Count-Min Sketch

Semana 13 (26/10 a 01/11): Estudo de Fuzzy Hashing/Similarity Hashing, onde queremos que elementos semelhantes sejam destinados a posições semelhantes. Algoritmos incluem Locality Sensitive Hashing e Min-Hash.

## Novembro
6 horas por semana, totalizando 24 horas

Semana 14 (02/11 a 08/11): Estudo inicial de estruturas de dados compactas, com uma introdução a entropia e arrays compactos

Semana 15 (09/11 a 15/11): Estudo de Bit-Vectors, estruturas compactas com operações de rank/select.

Semana 16 (16/11 a 22/11): Implementação de arrays compactos e bit-vectors.

Semana 17 (23/11 a 29/11): Estudo de wavelet-trees, estruturas que generalizam as operações rank-select dos bitvectors.
