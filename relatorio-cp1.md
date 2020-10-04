
## Relatório CP1 — Grupo DOPE

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

[1]: https://fossies.org/linux/bison/ChangeLog
