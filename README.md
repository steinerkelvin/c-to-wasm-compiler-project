# compiladores-trabalho

C to WebAssemply compiler

ATENÇÃO:

1- Incluí as regras dos comentários em C e já está funcionando para todos os tipos de comentários existentes, inclusive com comentários que terminam em EOF (o comentário é a última coisa do arquivo);

2- O arquivo scanner.l já está com várias regras do C. Ele compila no flex, mas o .c gerado não compila. Provavelmente pq está havendo conflito entre os tokens e palavras reservadas. Ainda não me concentrei em corrigir essa parte; entretanto:

3- O arquivo comment.l está rodando. ele é uma cópia da (minha) resolução do primeiro laboratório (para a EZlang). Eu só alterei a parte dos comentários. Por isso, todas as demais regras são da EZlang e ele continua imprimindo tudo que encontra. Quando um comentário é encontrado, seu conteúdo é imprimido, sem constar o número da linha. Não me dei o trabalho de alterar tudo, uma vez que já fiz isso no scanner.l;

4- Abram o comment.l. Deixei comentários explicando as regras em detalhes;

5- Façam o teste usando o test_comment.txt como entrada. Ele tem os três tipos de comentários aceitos pela linguagem;

6- Todas as palavras reservadas do C já estão no scanner.l;

7- Se tiverem alguma solução mais elegante para qualquer uma das regras, não hesitem em alterá-las

Me desculpem caso esteja fazendo alguma besteira, sou noob no GIT.

Que a luz esteja conosco, pq vai dar dor de cabeça!
