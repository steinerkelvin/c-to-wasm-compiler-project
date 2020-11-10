# Relatório CP2 — Grupo DOPE

Para esta etapa, decidimos fazer parte da implementação da AST para ter uma
noção melhor de como deveriam ser as estruturas das tabelas de símbolos.

Devido a meu desgosto com soluções com pouca segurança de tipo, optei por
implementar uma AST com os nós tipados adequadamente. Isso também se provou um
desgosto pois - apesar da minha tentativa de evitá-lo o máximo possível - acabei
por ter que implementar uma hierarquia de classes relativamente complicada.

![Hierarquia de classes da AST](./docs/inherit_graph_1.png)

~~E isso aumentou minha apreciação pelo sistema de traits de Rust.~~
