# Find

--- 

- **Date:** 2026-01-27
- **Tags:** MongoDB, NoSQL, Banco de Dados
- **URL:** https://gustavoleitao.notion.site/MongoDb-Trabalhando-com-um-exemplo-14c9da677dce809fa85dd2fb33ef4636
- **Relacionados**: [Operadores do MongoDB](./Operadores.md)
---

## Summary

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

Inclui os campos restaurant_id, name e borough, exclui o campo _id

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
