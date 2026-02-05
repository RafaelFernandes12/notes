# Operadores do MongoDB

--- 

- **Date:** 2026-01-28
- **Tags:** [Mongo](tags/mongo.md), [Nosql](tags/Nosql.md), [BancoDeDados](tags/BancoDeDados.md)
- **URL:**  ""

---

## Operadores de Comparação


| Operador      | Descrição                                 | Exemplo de Uso                      |
|---------------|-------------------------------------------|-------------------------------------|
| $eq           | Igual a                                   | { idade: { $eq: 18 } }              |
| $ne           | Diferente de                              | { idade: { $ne: 18 } }              |
| $gt           | Maior que                                 | { idade: { $gt: 18 } }              |
| $gte          | Maior ou igual a                          | { idade: { $gte: 18 } }             |
| $lt           | Menor que                                 | { idade: { $lt: 18 } }              |
| $lte          | Menor ou igual a                          | { idade: { $lte: 18 } }             |
| $in           | Em uma lista                              | { cor: { $in: ["azul", "verde"] } } |
| $nin          | Não está em uma lista                     | { cor: { $nin: ["azul", "verde"] }  |


## Operadores de Elemento e Tipo


| Operador      | Descrição                                 | Exemplo de Uso                      |
|---------------|-------------------------------------------|-------------------------------------|
| $exists       | Campo existe ou não                       | { nome: { $exists: true } }         |
| $type         | Tipo do campo                             | { idade: { $type: "int" } }         |


## Operadores Lógicos


| Operador      | Descrição                                 | Exemplo de Uso                      |
|---------------|-------------------------------------------|-------------------------------------|
| $and          | E lógico                                  | { $and: [ {a: 1}, {b: 2} ] }        |
| $or           | Ou lógico                                 | { $or: [ {a: 1}, {b: 2} ] }         |
| $not          | Negação                                   | { idade: { $not: { $gt: 18 } } }    |
| $nor          | Nem um nem outro                          | { $nor: [ {a: 1}, {b: 2} ] }        |


## Operadores de Array


| Operador      | Descrição                                 | Exemplo de Uso                      |
|---------------|-------------------------------------------|-------------------------------------|
| $size         | Tamanho de array                          | { tags: { $size: 3 } }              |
| $all          | Todos os valores em array                 | { tags: { $all: ["red", "blue"] } } |
| $elemMatch    | Elemento que corresponde à condição       | { notas: { $elemMatch: { $gt: 7 } } }|


## Operadores de Atualização


| Operador      | Descrição                                 | Exemplo de Uso                      |
|---------------|-------------------------------------------|-------------------------------------|
| $set          | Atualiza valor de campo                   | { $set: { nome: "João" } }          |
| $unset        | Remove campo                              | { $unset: { nome: "" } }            |
| $inc          | Incrementa valor                          | { $inc: { idade: 1 } }              |
| $push         | Adiciona valor em array                   | { $push: { tags: "novo" } }         |
| $pull         | Remove valor de array                     | { $pull: { tags: "velho" } }        |


## Outros Operadores


| Operador      | Descrição                                 | Exemplo de Uso                      |
|---------------|-------------------------------------------|-------------------------------------|
| $regex        | Expressão regular                         | { nome: { $regex: "^A" } }          |


## Operadores de Agregação


| Operador         | Descrição                                                                                                                                      |
|------------------|------------------------------------------------------------------------------------------------------------------------------------------------|
| $bucket          | Categoriza documentos em grupos (buckets) baseados em uma expressão específica e limites definidos.                                            |
| $bucketauto      | Categoriza documentos em um número específico de grupos, onde os limites são determinados automaticamente para distribuir os dados uniformemente.|
| $collstats       | Retorna estatísticas sobre uma coleção ou visualização (view).                                                                                 |
| $count           | Retorna a contagem total de documentos que chegam a este estágio do pipeline.                                                                  |
| $facet           | Processa múltiplos pipelines de agregação simultâneos no mesmo conjunto de documentos de entrada, permitindo análises multidimensionais.        |
| $geonear         | Retorna documentos ordenados por proximidade a um ponto geoespacial. Combina funcionalidades de $match, $sort e $limit.                        |
| $graphlookup     | Realiza uma busca recursiva em uma coleção, adicionando um array com os resultados da travessia ao documento de saída.                         |
| $group           | Agrupa documentos por uma expressão identificadora e aplica acumuladores (soma, média, etc.). Retorna um documento por grupo.                  |
| $indexstats      | Retorna estatísticas detalhadas sobre o uso de cada índice da coleção.                                                                         |
| $limit           | Passa apenas os primeiros n documentos para o próximo estágio do pipeline, descartando o restante.                                             |
| $listsessions    | Lista as sessões ativas que foram propagadas para a coleção system.sessions.                                                                   |


### Operadores de Expressão e Transformação

| Operador   | Descrição                                                                                        |
|------------|--------------------------------------------------------------------------------------------------|
| $map       | Aplica uma expressão a cada elemento de um array e retorna um novo array com os resultados.     |
| $filter    | Retorna um novo array contendo apenas os elementos que satisfazem uma condição.                 |
| $reduce    | Reduz um array a um único valor aplicando uma expressão acumuladora.                            |
| $concat    | Concatena strings em uma única string.                                                          |
| $toUpper   | Converte uma string para letras maiúsculas.                                                     |
| $toLower   | Converte uma string para letras minúsculas.                                                     |

#### Exemplos

- **$map**  
  Transforma todos os nomes para maiúsculo:
  ```js
  db.pessoas.aggregate([
    {
      $project: {
        nomes_maiusculos: {
          $map: {
            input: "$nomes",
            as: "nome",
            in: { $toUpper: "$$nome" }
          }
        }
      }
    }
  ])
  // Saída: { nomes_maiusculos: ["JOÃO", "MARIA", ...] }
  ```

- **$filter**  
  Filtra notas maiores ou iguais a 7:
  ```js
  db.alunos.aggregate([
    {
      $project: {
        notas_aprovadas: {
          $filter: {
            input: "$notas",
            as: "nota",
            cond: { $gte: ["$$nota", 7] }
          }
        }
      }
    }
  ])
  // Saída: { notas_aprovadas: [8, 9, 10] }
  ```

- **$reduce**  
  Soma todos os valores de um array:
  ```js
  db.pedidos.aggregate([
    {
      $project: {
        total: {
          $reduce: {
            input: "$valores",
            initialValue: 0,
            in: { $add: ["$$value", "$$this"] }
          }
        }
      }
    }
  ])
  // Saída: { total: 42 }
  ```

## Operadores de Agrupamento


| Operador         | Descrição                                                                                         |
|------------------|---------------------------------------------------------------------------------------------------|
| $sum             | Calcula a soma dos valores de um campo específico.                                                |
| $avg             | Calcula a média dos valores de um campo específico.                                               |
| $min             | Retorna o valor mínimo de um campo específico.                                                    |
| $max             | Retorna o valor máximo de um campo específico.                                                    |    
| $first           | Retorna o primeiro valor de um campo específico em cada grupo.                                    |
| $last            | Retorna o último valor de um campo específico em cada grupo.                                     |
| $push            | Adiciona valores de um campo específico a um array em cada grupo.                                |
| $addToSet       | Adiciona valores únicos de um campo específico a um array em cada grupo.                          |
| $stdDevPop      | Calcula o desvio padrão populacional dos valores de um campo específico.                          |
| $stdDevSamp     | Calcula o desvio padrão amostral dos valores de um campo específico.                           |
| $mergeObjects   | Combina múltiplos documentos em um único documento.                                               |
| $count          | Conta o número de documentos em cada grupo.                                                       |


### exemplos

Exemplo de coleção de vendas com datas:
```js
{ "_id" : 1, "item" : "abc", "price" : 10, "quantity" : 2, "date" : ISODate("2014-01-01T08:00:00Z") }
{ "_id" : 2, "item" : "jkl", "price" : 20, "quantity" : 1, "date" : ISODate("2014-02-03T09:00:00Z") }
{ "_id" : 3, "item" : "xyz", "price" : 5, "quantity" : 5, "date" : ISODate("2014-02-03T09:05:00Z") }
{ "_id" : 4, "item" : "abc", "price" : 10, "quantity" : 10, "date" : ISODate("2014-02-15T08:00:00Z") }
{ "_id" : 5, "item" : "xyz", "price" : 5, "quantity" : 10, "date" : ISODate("2014-02-15T09:12:00Z") }
```

#### avg
```js
db.sales.aggregate(
  [
    {
      $group:
        {
          _id: "$item",
          avgAmount: { $avg: { $multiply: [ "$price", "$quantity" ] } },
          avgQuantity: { $avg: "$quantity" }
        }
    }
  ]
)
// Resultado:
{ "_id" : "xyz", "avgAmount" : 37.5, "avgQuantity" : 7.5 }
{ "_id" : "jkl", "avgAmount" : 20, "avgQuantity" : 1 }
{ "_id" : "abc", "avgAmount" : 60, "avgQuantity" : 6 }

```

#### max
```js
db.sales.aggregate([
  {
    $group: {
      _id: "$item",
      maxTotalAmount: { $max: { $multiply: [ "$price", "$quantity" ] } },
      maxQuantity: { $max: "$quantity" }
    }
  }
])
// Resultado:
{ "_id" : "xyz", "maxTotalAmount" : 50, "maxQuantity" : 10 }
{ "_id" : "jkl", "maxTotalAmount" : 20, "maxQuantity" : 1 }
{ "_id" : "abc", "maxTotalAmount" : 100, "maxQuantity" : 10 }
```

#### min
```js
db.sales.aggregate([
  {
    $group: {
      _id: "$item",
      minQuantity: { $min: "$quantity" }
    }
  }
])

{ "_id" : "xyz", "minQuantity" : 5 }
{ "_id" : "jkl", "minQuantity" : 1 }
{ "_id" : "abc", "minQuantity" : 2 }
```

#### first
```js
db.sales.aggregate([
  { $sort: { item: 1, date: 1 } },
  {
    $group: {
      _id: "$item",
      firstSalesDate: { $first: "$date" }
    }
  }
])

{ "_id" : "xyz", "firstSalesDate" : ISODate("2014-02-03T09:05:00Z") }
{ "_id" : "jkl", "firstSalesDate" : ISODate("2014-02-03T09:00:00Z") }
{ "_id" : "abc", "firstSalesDate" : ISODate("2014-01-01T08:00:00Z") }
```

#### last
```js
db.sales.aggregate([
  { $sort: { item: 1, date: 1 } },
  {
    $group: {
      _id: "$item",
      lastSalesDate: { $last: "$date" }
    }
  }
])
{ "_id" : "xyz", "lastSalesDate" : ISODate("2014-02-15T14:12:12Z") }
{ "_id" : "jkl", "lastSalesDate" : ISODate("2014-02-03T09:00:00Z") }
{ "_id" : "abc", "lastSalesDate" : ISODate("2014-02-15T08:00:00Z") }
```

#### addToSet
```js
db.sales.aggregate([
  {
    $group: {
      _id: { day: { $dayOfYear: "$date" }, year: { $year: "$date" } },
      itemsSold: { $addToSet: "$item" }
    }
  }
])
{ "_id" : { "day" : 46, "year" : 2014 }, "itemsSold" : [ "xyz", "abc" ] }
{ "_id" : { "day" : 34, "year" : 2014 }, "itemsSold" : [ "xyz", "jkl" ] }
{ "_id" : { "day" : 1, "year" : 2014 }, "itemsSold" : [ "abc" ] }
```

#### push

```js
db.sales.aggregate([
  {
    $group: {
      _id: { day: { $dayOfYear: "$date" }, year: { $year: "$date" } },
      itemsSold: { $push: { item: "$item", quantity: "$quantity" } }
    }
  }
])
{ "_id" : { "day" : 46, "year" : 2014 }, "itemsSold" : [ "xyz", "abc" ] }
{ "_id" : { "day" : 34, "year" : 2014 }, "itemsSold" : [ "xyz", "jkl" ] }
{ "_id" : { "day" : 1, "year" : 2014 }, "itemsSold" : [ "abc" ] }
```
