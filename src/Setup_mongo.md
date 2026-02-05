## Gustavo Leitão - MongoDB - Operações Básicas

--- 

- **Date:** 2026-01-27
- **Tags:** [MongoDB](tags/MongoDB.md), [Docker](tags/Docker.md), [Bash](tags/Bash.md), [BancoDeDados](tags/BancoDeDados.md), [Mongo](tags/mongo.md)
- **URL:**  ""

---

### Summary

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



