# compiladores-trabalho

C to WebAssemply compiler

ATENÇÃO:

1- Incluí as regras dos comentários em C e já está funcionando para todos os tipos de comentários existentes, inclusive com comentários que terminam em EOF (o comentário é a última coisa do arquivo);

2- Já está compilando o parser, mas com uma série de avisos de "regras inúteis", devido a ausência de uma hierarquia de regras;

3- Só fiz as regras que envolvem funções. Comentei algumas regras vindas da EZlang que podem ser úteis para servirem de referência posteriormente;

4- Ainda não fiz nenhum teste além dos comentários. Façam-nos;

5- Precisamos estabelecer uma hirarquia para as regras, o que demanda considerável raciocínio;

6- Se tiverem alguma solução mais elegante para qualquer uma das regras, não hesitem em alterá-las;

Me desculpem caso esteja fazendo alguma besteira, sou noob no GIT.

## Requisitos para compilação

sudo apt install flex bison clang -y