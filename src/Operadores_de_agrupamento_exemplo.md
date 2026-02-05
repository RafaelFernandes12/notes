## Operadores_de_agrupamento

--- 

- **Date:** 2026-01-31
- **Tags:** [Mongo](tags/mongo.md), [Nosql](tags/Nosql.md), [BancoDeDados](tags/BancoDeDados.md) 
- **URL:**  https://gustavoleitao.notion.site/MongoDb-Agrega-o-1589da677dce805bb96deb237f38a7f4

---

### Summary

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
