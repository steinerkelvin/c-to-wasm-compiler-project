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

Os caracteres de controle que representam uma quebra de linha em arquivos de
texto podem ser diferente dependendo do SO. Esses caracteres são CR (_carriage
return_) e LF (_line feed_), respectivamente `0x0D` (representado por "\n") e
`0x0A` (representado por "\r"). Sistemas Unix utilizam a terminação `LF`,
enquanto o Windows utiliza dois caracteres: `CR LF`. O antigo MacOS (pré-OSX
Macintosh) utilizava apenas `CR`, porém escolhemos não dar suporte a essa
formatação em nosso scanner por ser extremamente legado.

Para a maioria dos casos, não é necessário explicitar o caso `\r\n` como quebra
de linha, já que o caractere `\r` está representado no _pattern_ `.`, que
corresponde a qualquer byte exceto `\n` (`0x0A`), desta forma, só precisamos
explicitar o caso CRLF nos comentários de linha (marcados por `//`) que possuem
quebra de linha.

[1]: https://fossies.org/linux/bison/ChangeLog
