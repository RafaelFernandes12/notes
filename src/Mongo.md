# Mongo

______________________________________________________________________

**Date:** 2026-02-08
**Tags:** [Mongo](tags/Mongo.md), [Nosql](tags/Nosql.md), [BancoDeDados](tags/BancoDeDados.md)
**URL:** [Notion](https://gustavoleitao.notion.site/Banco-de-dados-NoSQL-1049da677dce804c927ae69e8fd93173)

______________________________________________________________________

## Setup

```bash
docker run -p 27017:27017 --name mongo -v mongo-data:/data/ -d mongo
wget https://raw.githubusercontent.com/gustavoleitao/mongo-dataset/refs/heads/master/primer-dataset.json
docker exec -it mongo /bin/bash
mongoimport --db test --collection restaurants --drop --file /var/tmp/primer-dataset.json
docker exec -it mongo mongosh
```

Selecionar o banco de dados

```bash
use test
```

## Crud

Inserir um documento, não é necessário criar a coleção antes, ela será criada automaticamente.

```js
db.users.insertOne( // users é o nome da coleção
    {
        nome: 'newton', 
        email: 'newton@gmail.com', 
        idade: 53
    }
);
```

Consultar documentos na coleção

```js
> db.users.find();

test> db.users.find();
[
    {
        _id: ObjectId('67489f103360e3729df7c614'), // ID gerado automaticamente
        nome: 'newton',
        email: 'newton@gmail.com',
        idade: 53
    }
]
```

Atualizar documentos na coleção

```js
db.users.updateMany(
    {nome: "newton"}, 
    {
        $set: {email: "newton@outlook.com"}
    }
);
```

Deletar documentos na coleção

```js
db.users.deleteMany({nome: "newton"})
```

## Find

Filtro para encontrar documentos em uma coleção.

```js
db.restaurants.find({ "borough": "Manhattan" });
```

Buscando por um campo em um documento interno

```js
db.restaurants.find({"address.zipcode": "10075"});
```

Contando a quantidade de retorno

```js
db.restaurants.find({"address.zipcode": "10075"}).count();
```

Limitando a quantidade de retorno

```js
db.restaurants.find().limit(5);
```

### Operadores

```js
db.restaurants.find({ "grades.score": { $gt: 30 } }) // maior que 30
db.restaurants.find({ "grades.score": { $lt: 10 } }) // menor que 10
```

| Nome | Descrição |
| :--- | :--- |
| **$eq** | Corresponde a valores que são iguais a um valor especificado. |
| **$gt** | Corresponde a valores que são maiores que um valor especificado. |
| **$gte** | Corresponde a valores que são maiores ou iguais a um valor especificado. |
| **$in** | Corresponde a qualquer um dos valores especificados em um array. |
| **$lt** | Corresponde a valores que são menores que um valor especificado. |
| **$lte** | Corresponde a valores que são menores ou iguais a um valor especificado. |
| **$ne** | Corresponde a todos os valores que não são iguais a um valor especificado. |
| **$nin** | Corresponde a nenhum dos valores especificados em um array. |

### Projeção

Inclui os campos restaurant_id, name e borough, exclui o campo \_id

```js
db.restaurants.find(
  { "borough": "Manhattan" },
  { "restaurant_id": 1, "name": 1, "borough": 1, "_id": 0 } 
);
```

### Operadores lógicos

| Nome | Descrição |
| :--- | :--- |
| **$and** | Junta condições e retorna documentos que atendem a todas as condições especificadas. |
| **$or** | Junta condições e retorna documentos que atendem a pelo menos uma das condições especificadas. |
| **$not** | Inverte a condição especificada, retornando documentos que não atendem à condição. |
| **$nor** | Junta condições e retorna documentos que não atendem a nenhuma das condições especificadas. |

and

```js
db.restaurants.find({"cuisine": "Italian", "borough": "Manhattan"}, {"cuisine": 1, "borough": 1})
```

or

```js
db.restaurants.find({ $or: [ { "cuisine": "Italian" }, { "cuisine": "Chinese" } ] }, { "cuisine": 1, "_id": 0 })
```

not

```js
db.restaurants.find({ "cuisine": { $not: { $eq: "Italian" } } }, { "cuisine": 1, "_id": 0 })
```

nor

```js
db.restaurants.find({ $nor: [ { "cuisine": "Italian" }, { "cuisine": "Chinese" } ] }, { "cuisine": 1, "_id": 0 })
```

### Ordernação

```js
// Ordena por nome em ordem crescente (A-Z)
db.restaurants.find().sort({ name: 1 })

// Ordena por nome em ordem decrescente (Z-A)
db.restaurants.find().sort({ name: -1 })

// Ordena por borough crescente e depois por name decrescente
db.restaurants.find().sort({ borough: 1, name: -1 })
```

### Paginação

```js
// Pula os primeiros 10 documentos e retorna os próximos 5
db.restaurants.find().skip(10).limit(5)
```

### Verificação de existência de campo

```js
// Encontra documentos onde o campo "cuisine" existe
db.restaurants.find({ "cuisine": { $exists: true } })
```

### Trabalhando com Arrays

Insira documentos para os exemplos abaixo

```js
db.inventory.insertMany([
   { item: "journal", qty: 25, tags: ["blank", "red"], dim_cm: [ 14, 21 ] },
   { item: "notebook", qty: 50, tags: ["red", "blank"], dim_cm: [ 14, 21 ] },
   { item: "paper", qty: 100, tags: ["red", "blank", "plain"], dim_cm: [ 14, 21 ] },
   { item: "planner", qty: 75, tags: ["blank", "red"], dim_cm: [ 22.85, 30 ] },
   { item: "postcard", qty: 45, tags: ["blue"], dim_cm: [ 10, 15.25 ] }
]);
```

```js
// Encontrará os elementos na ordem exata
db.inventory.find( { tags: ["red", "blank"] } ) // journal
// Encontrará os elementos independente da ordem dos elementos
db.inventory.find({ tags: {$all: ["red", "blank"]}}) // journal, notebook e paper
// Encontrará todos os documentos que contenham o valor "red" no array "tags"
db.inventory.find( { tags: "red" } ) // journal, notebook, paper, planner
// Encontrará todos que contenham um elemento do campo dim_cm é maior que 25
db.inventory.find( { dim_cm: { $gt: 25 } } ) // planner
// Encontrará todos que contenham um elemento do campo dim_cm entre 15 e 20
// O elemento que satisfaz a primeira condição não precisa ser o mesmo que satisfaz a segunda condição
db.inventory.find( { dim_cm: { $gt: 15, $lt: 20 } } ) // journal, notebook, paper, postcard
// Encontrará todos que contenham um único elemento do campo dim_cm que seja maior que 22 e menor que 30
db.inventory.find( { dim_cm: { $elemMatch: { $gt: 22, $lt: 30 } } } ) // planner
// Encontrará todos que contenham o segundo elemento do campo dim_cm maior que 25
db.inventory.find( { "dim_cm.1": { $gt: 25 } } ) // planner
// Encontrará todos os documentos onde o array "tags" tenha exatamente 3 elementos
db.inventory.find( { "tags": { $size: 3 } } ) // paper
```

Insira documentos para os exemplos abaixo

```js
db.inventory.insertMany( [
   { item: "journal", instock: [{ warehouse: "A", qty: 5 }, { warehouse: "C", qty: 15 } ] },
   { item: "notebook", instock: [ { warehouse: "C", qty: 5 } ] },
   { item: "paper", instock: [ { warehouse: "A", qty: 60 }, { warehouse: "B", qty: 15 } ] },
   { item: "planner", instock: [ { warehouse: "A", qty: 40 }, { warehouse: "B", qty: 5 } ] },
   { item: "postcard", instock: [ { warehouse: "B", qty: 15 }, { warehouse: "C", qty: 35 } ] }
]);
```

```js
// Encontrará todos os documentos onde o array "instock" contenha um documento com os campos warehouse igual a "A" e qty igual a 5
db.inventory.find( { "instock": { warehouse: "A", qty: 5 } } ) // journal
// Encontrará todos os documentos onde o array "instock" contenha um documento com o campo warehouse igual a "A"
db.inventory.find( { 'instock.warehouse': "A" } ) // journal, paper, planner
// Encontrará todos os documentos onde o array "instock" contenha um documento com o campo qty menor ou igual a 20
db.inventory.find( { 'instock.qty': { $lte: 20 } } ) // journal, notebook, paper, planner, postcard
// Encontrará todos os documentos onde o primeiro elemento do array "instock" contenha o campo qty menor ou igual a 20
db.inventory.find( { 'instock.0.qty': { $lte: 20 } } ) // journal, notebook
// Busca documentos que no campo instock possui no elemento interno qty valor maior que 10 e menor igual a 20 ( o valor de qty deve satisfazer ambas condições)
db.inventory.find( { "instock": { $elemMatch: { qty: { $gt: 10, $lte: 20 } } } } ) // paper, postcard
// Encontrará todos os documentos onde o array "instock" contenha um documento com o campo qty maior que 10 e menor ou igual a 20
db.inventory.find( { "instock.qty": { $gt: 10,  $lte: 20 } } ) // journal, paper, postcard
```

## Aggregation

Principais estágios:

- $match: Filtra documentos com base em critérios (similar ao find).
- $group: Agrupa documentos e aplica operações de agregação (como sum, avg, etc.).
- $project: Define os campos que devem ser incluídos ou transformados.
- $sort: Ordena os documentos.
- $limit e $skip: Limita ou pula um número de documentos.
- $unwind: Desfaz arrays para que cada elemento se torne um documento individual.

Insira os documentos para o exemplo

```js
db.sales.insertMany([
  { "item": "A", "quantity": 5, "price": 10 },
  { "item": "B", "quantity": 3, "price": 20 },
  { "item": "A", "quantity": 2, "price": 10 },
  { "item": "B", "quantity": 1, "price": 20 },
  { "item": "C", "quantity": 1, "price": 30 }
]);
```

```js
 
db.sales.aggregate([
  {
    $group: {
      _id: "$item", // Agrupa por campo "item"
      totalRevenue: { $sum: { $multiply: ["$quantity", "$price"] } } // Calcula a receita total por campo
    }
  }
]);

db.sales.aggregate([
  {
	  $set: {"total": {$multiply: ["$quantity", "$price"]}} // Adiciona um campo "total" que é o produto de quantity e price
  },
  {$project: {quantity: 0, item: 0}} // Remove os campos quantity e item
]);

// Contar número de restaurantes por bairro
// Ele fez isso embaixo para todos os restaurantes e os agrupa por borough
// db.restaurants.find({borough: "Bronx"}).count() -> 2338
db.restaurants.aggregate(
	[
		{$group: {"_id": "$borough", "count": {$sum: 1}}}
	]
)
```

```js
[
  { _id: 'A', totalRevenue: 70 },
  { _id: 'B', totalRevenue: 80 },
  { _id: 'C', totalRevenue: 30 }
]
```

```js
[
  { item: 'A', totalRevenue: 50 },
  { item: 'B', totalRevenue: 60 },
  { item: 'A', totalRevenue: 20 },
  { item: 'B', totalRevenue: 20 },
  { item: 'C', totalRevenue: 30 }
]
```

```js
[
  { _id: 'Brooklyn', count: 6086 },
  { _id: 'Missing', count: 51 },
  { _id: 'Staten Island', count: 969 },
  { _id: 'Queens', count: 5656 },
  { _id: 'Manhattan', count: 10259 },
  { _id: 'Bronx', count: 2338 }
]
```

Localize a quantidade de restaurantes por zipcode de cozinha brasileira no bairro do Queens.

```js
db.restaurants.aggregate(
    [
        {$match: {borough: "Queens", cuisine: "Brazilian"}}, // Filtra por borough e cuisine
        {$group: {_id: "$address.zipcode", count: {$sum: 1}}} // Agrupa por zipcode e conta
        {$sort: {count: 1}} // Ordena pela contagem em ordem crescente - opcional
    ]
)

```

Aqui estão os operadores de agrupamento: [Operadores](mongo/Operadores.md)
Aqui estão exemplos de operadores de agrupamento: [Operadores_de_agrupamento](mongo/Operadores_de_agrupamento.md)

### project

permite escolher campos de um documento a serem retornados

```js
db.sales.aggregate([{$project: {item: 1, total: 1}}])

[
  { item: 'A', total: 50 },
  { item: 'B', total: 60 },
  { item: 'A', total: 20 },
  { item: 'B', total: 20 },
  { item: 'C', total: 30 }
]
```

### set

permite adicionar novos campos ou modificar campos existentes em documentos

```js
db.scores.insertMany([
	{student: "Maya", homework: [10,5,10],quiz: [10,8], extraCredit: 0 },
	{student: "Ryan", homework: [5,6,5],quiz: [8,8], extraCredit: 8 }
]);
```

```js
db.scores.aggregate([
	{ 
		$set: {
			"totalHomework": {$sum: "$homework" },
			"totalQuiz": { $sum: "$quiz" }
		} 
	},
	{ 
		$set: {
			"totalScore": { $add: ["$totalHomework", "$totalQuiz", "$extraCredit"] }
		} 
	}
])
// resultado
{
  [
    {
      "_id": 1,
      "student": "Maya",
      "homework": [10, 5, 10],
      "quiz": [10, 8],
      "extraCredit": 0,
      "totalHomework": 25,
      "totalQuiz": 18,
      "totalScore": 43
    },
    {
      "_id": 2,
      "student": "Ryan",
      "homework": [5, 6, 5],
      "quiz": [8, 8],
      "extraCredit": 8,
      "totalHomework": 16,
      "totalQuiz": 16,
      "totalScore": 40
    }
  ]
}

```

### unwind

Desfaz um array, criando um documento para cada elemento do array

```js
db.inventory.insertOne({ "item": "ABC1", sizes: ["S", "M", "L"] } )
```

```js
db.inventory.aggregate( [ { $unwind : "$sizes" } ] )
// resultado
{ "id" : 1, "item" : "ABC", "sizes" : "S" }
{ "id" : 1, "item" : "ABC", "sizes" : "M" }
{ "id" : 1, "item" : "ABC", "sizes" : "L" }
```

```js
db.inventory2.insertMany([
  { "_id": 1, "item": "ABC", "price": NumberDecimal("80"), "sizes": ["S", "M", "L"] },
  { "_id": 2, "item": "EFG", "price": NumberDecimal("120"), "sizes": [] },
  { "_id": 3, "item": "IJK", "price": NumberDecimal("160"), "sizes": "M" },
  { "_id": 4, "item": "LMN", "price": NumberDecimal("10") },
  { "_id": 5, "item": "XYZ", "price": NumberDecimal("5.75"), "sizes": null }
])
```

```js
db.inventory2.aggregate( [ { $unwind: "$sizes" } ] )
db.inventory2.aggregate( [ { $unwind: { path: "$sizes" } } ] )
//resultado
{ "item" : "ABC", "price" : NumberDecimal("80"), "sizes" : "S" }
{ "item" : "ABC", "price" : NumberDecimal("80"), "sizes" : "M" }
{ "item" : "ABC", "price" : NumberDecimal("80"), "sizes" : "L" }
{ "item" : "IJK", "price" : NumberDecimal("160"), "sizes" : "M" }
```

```js
db.inventory2.aggregate([
   { $unwind: { path: "$sizes", preserveNullAndEmptyArrays: true } }
])
// resultado
{ "item" : "ABC", "price" : NumberDecimal("80"), "sizes" : "S" }
{ "item" : "ABC", "price" : NumberDecimal("80"), "sizes" : "M" }
{ "item" : "ABC", "price" : NumberDecimal("80"), "sizes" : "L" }
{ "item" : "EFG", "price" : NumberDecimal("120") }
{ "item" : "IJK", "price" : NumberDecimal("160"), "sizes" : "M" }
{ "item" : "LMN", "price" : NumberDecimal("10") }
{ "item" : "XYZ", "price" : NumberDecimal("5.75"), "sizes" : null }
```

### skip e limit

```js
db.restaurants.aggregate([
  { $match: { borough: "Manhattan" } },
  { $skip: 5 }, // Pula os primeiros 5 documentos
  { $limit: 10 } // Limita o resultado a 10 documentos
])
```

### Exemplos de operadores de agrupamento

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

## Operadores

### Operadores de Comparação

| Operador | Descrição | Exemplo de Uso |
|---------------|-------------------------------------------|-------------------------------------|
| $eq | Igual a | { idade: { $eq: 18 } } |
| $ne | Diferente de | { idade: { $ne: 18 } } |
| $gt | Maior que | { idade: { $gt: 18 } } |
| $gte | Maior ou igual a | { idade: { $gte: 18 } } |
| $lt | Menor que | { idade: { $lt: 18 } } |
| $lte | Menor ou igual a | { idade: { $lte: 18 } } |
| $in | Em uma lista | { cor: { $in: ["azul", "verde"] } } |
| $nin | Não está em uma lista | { cor: { $nin: ["azul", "verde"] } |

### Operadores de Elemento e Tipo

| Operador | Descrição | Exemplo de Uso |
|---------------|-------------------------------------------|-------------------------------------|
| $exists | Campo existe ou não | { nome: { $exists: true } } |
| $type | Tipo do campo | { idade: { $type: "int" } } |

### Operadores Lógicos

| Operador | Descrição | Exemplo de Uso |
|---------------|-------------------------------------------|-------------------------------------|
| $and | E lógico | { $and: [ {a: 1}, {b: 2} ] } |
| $or | Ou lógico | { $or: [ {a: 1}, {b: 2} ] } |
| $not | Negação | { idade: { $not: { $gt: 18 } } } |
| $nor | Nem um nem outro | { $nor: [ {a: 1}, {b: 2} ] } |

### Operadores de Array

| Operador | Descrição | Exemplo de Uso |
|---------------|-------------------------------------------|-------------------------------------|
| $size | Tamanho de array | { tags: { $size: 3 } } |
| $all | Todos os valores em array | { tags: { $all: ["red", "blue"] } } |
| $elemMatch | Elemento que corresponde à condição | { notas: { $elemMatch: { $gt: 7 } } }|

### Operadores de Atualização

| Operador | Descrição | Exemplo de Uso |
|---------------|-------------------------------------------|-------------------------------------|
| $set | Atualiza valor de campo | { $set: { nome: "João" } } |
| $unset | Remove campo | { $unset: { nome: "" } } |
| $inc | Incrementa valor | { $inc: { idade: 1 } } |
| $push | Adiciona valor em array | { $push: { tags: "novo" } } |
| $pull | Remove valor de array | { $pull: { tags: "velho" } } |

### Outros Operadores

| Operador | Descrição | Exemplo de Uso |
|---------------|-------------------------------------------|-------------------------------------|
| $regex | Expressão regular | { nome: { $regex: "^A" } } |

### Operadores de Agregação

| Operador | Descrição |
|------------------|------------------------------------------------------------------------------------------------------------------------------------------------|
| $bucket | Categoriza documentos em grupos (buckets) baseados em uma expressão específica e limites definidos. |
| $bucketauto | Categoriza documentos em um número específico de grupos, onde os limites são determinados automaticamente para distribuir os dados uniformemente.|
| $collstats | Retorna estatísticas sobre uma coleção ou visualização (view). |
| $count | Retorna a contagem total de documentos que chegam a este estágio do pipeline. |
| $facet | Processa múltiplos pipelines de agregação simultâneos no mesmo conjunto de documentos de entrada, permitindo análises multidimensionais. |
| $geonear | Retorna documentos ordenados por proximidade a um ponto geoespacial. Combina funcionalidades de $match, $sort e $limit. |
| $graphlookup | Realiza uma busca recursiva em uma coleção, adicionando um array com os resultados da travessia ao documento de saída. |
| $group | Agrupa documentos por uma expressão identificadora e aplica acumuladores (soma, média, etc.). Retorna um documento por grupo. |
| $indexstats | Retorna estatísticas detalhadas sobre o uso de cada índice da coleção. |
| $limit | Passa apenas os primeiros n documentos para o próximo estágio do pipeline, descartando o restante. |
| $listsessions | Lista as sessões ativas que foram propagadas para a coleção system.sessions. |

#### Operadores de Expressão e Transformação

| Operador | Descrição |
|------------|--------------------------------------------------------------------------------------------------|
| $map | Aplica uma expressão a cada elemento de um array e retorna um novo array com os resultados. |
| $filter | Retorna um novo array contendo apenas os elementos que satisfazem uma condição. |
| $reduce | Reduz um array a um único valor aplicando uma expressão acumuladora. |
| $concat | Concatena strings em uma única string. |
| $toUpper | Converte uma string para letras maiúsculas. |
| $toLower | Converte uma string para letras minúsculas. |

##### Exemplos

- **$map**\
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

- **$filter**\
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

- **$reduce**\
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

| Operador | Descrição |
|------------------|---------------------------------------------------------------------------------------------------|
| $sum | Calcula a soma dos valores de um campo específico. |
| $avg | Calcula a média dos valores de um campo específico. |
| $min | Retorna o valor mínimo de um campo específico. |
| $max | Retorna o valor máximo de um campo específico. |
| $first | Retorna o primeiro valor de um campo específico em cada grupo. |
| $last | Retorna o último valor de um campo específico em cada grupo. |
| $push | Adiciona valores de um campo específico a um array em cada grupo. |
| $addToSet | Adiciona valores únicos de um campo específico a um array em cada grupo. |
| $stdDevPop | Calcula o desvio padrão populacional dos valores de um campo específico. |
| $stdDevSamp | Calcula o desvio padrão amostral dos valores de um campo específico. |
| $mergeObjects | Combina múltiplos documentos em um único documento. |
| $count | Conta o número de documentos em cada grupo. |

### exemplos

Exemplo de coleção de vendas com datas:

```js
{ "_id" : 1, "item" : "abc", "price" : 10, "quantity" : 2, "date" : ISODate("2014-01-01T08:00:00Z") }
{ "_id" : 2, "item" : "jkl", "price" : 20, "quantity" : 1, "date" : ISODate("2014-02-03T09:00:00Z") }
{ "_id" : 3, "item" : "xyz", "price" : 5, "quantity" : 5, "date" : ISODate("2014-02-03T09:05:00Z") }
{ "_id" : 4, "item" : "abc", "price" : 10, "quantity" : 10, "date" : ISODate("2014-02-15T08:00:00Z") }
{ "_id" : 5, "item" : "xyz", "price" : 5, "quantity" : 10, "date" : ISODate("2014-02-15T09:12:00Z") }
```

##### avg

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

##### max

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

##### min

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

##### first

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

##### last

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

##### addToSet

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

##### push

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
