# Relatório CP2 — Grupo DOPE

Para esta etapa, decidimos começar parte da implementação da AST para ter uma
noção melhor de como deveriam ser as estruturas das tabelas de símbolos.

Devido a meu desgosto com soluções com pouca segurança de tipo, optei por tentar
implementar uma AST com os nós tipados adequadamente. Isso acabou resultando em
uma hierarquia interessante de classes para representear os nós da AST.

![Hierarquia de classes da AST](./docs/inherit_graph_1.png)

Isso facilita a manipulação dos nós através de polimorfismo, uma vez que, por
exemplo, os nós envolvidos em operações com expressões tem o tipo `Expr` e por
tanto tem uma representação de tipo associada.

Decidimos resumir os tipos primitivos a apenas 4 possibilidades de
implementação:

- "`void`";
- "`char`";
- "`integer`, que emgloba os tipos `short`,
  `int`, `long`, `long long`, com sinal e sem sinal;
- e um tipo "`real`" que
  representa os tipos de ponto flutuante `float` e `double`.

bastando usar na implementação as representações correnpondentes com maior
quantidade de bits.

Também decidimos limitar o suporte a vetores apenas a vetores de tamanho
conhecido em tempo de compilação.
