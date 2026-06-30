# Modelo EER - Especializacao Generalizacao e Relacionamentos N-arios

______________________________________________________________________

**Date:** 2026-05-20
**Tags:** [BancoDeDados](../tags/BancoDeDados.md), [ModelagemDeDados](../tags/ModelagemDeDados.md), [ModeloER](../tags/ModeloER.md), [ModeloEER](../tags/ModeloEER.md)
**URL:** https://www.db-book.com/ | https://www.uml.org/

______________________________________________________________________

## Recapitulação mínima

- Entidade forte tem identificação própria.
- Entidade fraca depende da chave da entidade proprietária mais sua chave parcial.
- Atributos podem ser simples, compostos, monovalorados, multivalorados, armazenados ou derivados.
- Relacionamentos possuem cardinalidade máxima e participação mínima.

## DER alternativo e diagrama de classes

- Alguns livros representam entidade como uma caixa com atributos dentro dela.
- Essa representação lembra o diagrama de classes UML.
- Em UML, uma classe costuma ter três partes: nome, atributos e operações.
- ER foca em dados e relacionamentos; UML também pode representar comportamento/métodos.

## Relacionamentos com grau maior que 2

Um relacionamento ternário envolve três entidades ao mesmo tempo.

Exemplo:

```text
FORNECEDOR -- FORNECE -- PROJETO
                    |
                  PECA
```

O atributo `Quantidade` pertence ao relacionamento `FORNECE`, porque depende da combinação fornecedor-projeto-peça.

## Cardinalidade em relacionamento ternário

Em relacionamento ternário, a cardinalidade de uma entidade é lida em relação ao par das outras duas.

Exemplo com `FORNECE(FORNECEDOR, PROJETO, PECA)`:

- Se o lado `FORNECEDOR` tem cardinalidade `1`, então cada par `(PROJETO, PECA)` tem no máximo um fornecedor.
- Se o lado `PROJETO` tem cardinalidade `1`, então cada par `(FORNECEDOR, PECA)` tem no máximo um projeto.
- Se o lado `PECA` tem cardinalidade `1`, então cada par `(FORNECEDOR, PROJETO)` tem no máximo uma peça.

## Exemplo prático de leitura

```text
Fornecedor | Projeto | Peca
A          | X       | 1
B          | Y       | 2
A          | Y       | 1
B          | X       | 2
```

Se `Fornecedor` é o lado `1`, note que `(X,1)` aparece com fornecedor `A` uma vez, `(Y,2)` com `B`, e assim por diante.

## Posso trocar ternário por vários binários?

- Nem sempre.
- Separar `FORNECEDOR-PECA`, `PECA-PROJETO` e `FORNECEDOR-PROJETO` pode perder a associação exata dos três ao mesmo tempo.
- Se `Quantidade` depende da tripla, ela não pertence corretamente a nenhum relacionamento binário isolado.
- Uma alternativa é criar uma entidade associativa/fraca, como `SUPRIMENTO`, ligada às três entidades.

## Especialização

Especialização é definir subclasses a partir de uma superclasse.

Exemplo:

```text
FUNCIONARIO
  ├── SECRETARIO   (Velocidade_digitacao)
  ├── TECNICO      (Grau_tec)
  └── ENGENHEIRO   (Tipo_eng)
```

- `FUNCIONARIO` é superclasse.
- `SECRETARIO`, `TECNICO` e `ENGENHEIRO` são subclasses.
- Subclasses herdam atributos da superclasse.
- A especialização é útil quando subclasses possuem atributos ou relacionamentos específicos.

## Relacionamento específico de subclasse

Exemplo:

```text
FUNCIONARIO
  └── GERENTE -- GERENCIA -- PROJETO
```

Nem todo funcionário gerencia projetos, então o relacionamento fica na subclasse `GERENTE`.

## Generalização

Generalização é o processo inverso: criar uma entidade mais geral a partir de tipos específicos.

Exemplo:

```text
CARRO(Cod_veiculo, Placa, Preco, Velocidade_maxima)
CAMINHAO(Cod_veiculo, Placa, Preco, Numero_eixos)
```

Pode virar:

```text
VEICULO(Cod_veiculo, Placa, Preco)
  ├── CARRO(Velocidade_maxima)
  └── CAMINHAO(Numero_eixos)
```

## Restrições de especialização/generalização

- **Disjunção (`d`)**: uma entidade pertence a no máximo uma subclasse.
- **Sobreposição (`o`)**: uma entidade pode pertencer a várias subclasses.
- **Total**: toda entidade da superclasse deve estar em alguma subclasse.
- **Parcial**: uma entidade da superclasse pode não estar em nenhuma subclasse.

## Exemplos de restrições

- Disjunção total: todo `VEICULO` é exatamente um de `CARRO` ou `CAMINHAO`.
- Disjunção parcial: um `FUNCIONARIO` pode ser secretário, técnico, engenheiro ou nenhum desses.
- Sobreposição total: toda `PESSOA` é pelo menos uma das subclasses e pode ser várias.
- Sobreposição parcial: uma pessoa pode pertencer a várias subclasses ou a nenhuma.

## Hierarquia x reticulado

- Hierarquia de especialização: cada subclasse tem apenas uma superclasse.
- Reticulado de especialização: uma subclasse pode ter várias superclasses.
- Exemplo de reticulado: `GERENTE_ENGENHEIRO` herda de `GERENTE` e de `ENGENHEIRO`.

## Pegadinhas de prova

- Em ternário, cardinalidade não é lida como em binário; ela se refere ao par das outras entidades.
- Transformar ternário em binários pode perder informação.
- Especialização: de cima para baixo, superclasse para subclasses.
- Generalização: de baixo para cima, subclasses para superclasse.
- `d` significa disjoint/disjunção; `o` significa overlap/sobreposição.
- Excluir uma entidade da superclasse implica removê-la das subclasses.
- Inserir uma entidade na superclasse não significa inseri-la em todas as subclasses.
