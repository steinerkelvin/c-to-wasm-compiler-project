
# Relatório CP1 — Grupo DOPE

Tivemos um problema que surgiu devido a versões diferentes do Bison sendo usadas
no desenvolvimento. Como é possível ver no [changelog do Bison][1], recentemente
(aparentemente na versão 3.6) foi adicionado um alias para o tipo que deve ser
retornado pelo scanner `enum yytokentype`:

```c
typedef enum yytokentype yytoken_kind_t;
```

Para que esse nome (`yytoken_kind_t`) possa ser utilizado normalmente com a
versão anterior do Bison, adicionamos essa declaração, em `parsing.h` e
explicitamos no arquivo README quais versões das ferramentas foram utilizadas
para desenvolvimento e testes.

## Suporte CRLF

O caractere de controle que representa a quebra de linha em arquivos pode ser
diferente dependendo do sistema operacional, esses caracteres são CR (carriage
return) e LF (line feed) respectivamente 0x0D (13 decimal, representado por
"\n") e 0x0A (10 decimal, representado por "\r"). Sistemas baseados em Unix
utilizam a terminação LF, e o Windows utiliza dois caracteres CR LF. O antigo
MacOS (pré-OSX MacIntosh) utilizava apenas CR, porém escolhemos não dar suporte
a essa formatação em nosso scanner.

Para a maioria dos casos, não é necessário explicitar o caso \r\n como quebra de
linha, já que o caractere \r está representado no _pattern_ "." ,que representa
qualquer byte exceto LF (0x0A), desta forma, só precisamos explicitar o caso
CRLF nos comentários de linha (marcados por //) que possuem quebra de linha.

[1]: https://fossies.org/linux/bison/ChangeLog
