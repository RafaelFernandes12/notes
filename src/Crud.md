## Crud

--- 

- **Date:** 2026-01-27
- **Tags:** Mongo
- **URL:**  https://gustavoleitao.notion.site/MongoDb-Introdu-o-14c9da677dce80d798c3c2b37a89239e

---

### Summary

Veja como faz o [[Setup]]

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

