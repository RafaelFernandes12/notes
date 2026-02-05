- ## Aggregation

--- 

- **Date:** 2026-01-29
- **Tags:** [Mongo](tags/mongo.md), [Nosql](tags/Nosql.md), [BancoDeDados](tags/BancoDeDados.md)
- **URL:**  https://gustavoleitao.notion.site/MongoDb-Agrega-o-1589da677dce805bb96deb237f38a7f4
- **Relacionados**: [Operadores](./Operadores.md), [Operadores_de_agrupamento_exemplo](./Operadores_de_agrupamento_exemplo)

---

### Summary

Principais estágios:

* $match: Filtra documentos com base em critérios (similar ao find).
* $group: Agrupa documentos e aplica operações de agregação (como sum, avg, etc.).
* $project: Define os campos que devem ser incluídos ou transformados.
* $sort: Ordena os documentos.
* $limit e $skip: Limita ou pula um número de documentos.
* $unwind: Desfaz arrays para que cada elemento se torne um documento individual.

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

#### project

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

#### set

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

#### unwind

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


