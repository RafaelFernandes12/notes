# Redis

______________________________________________________________________

- **Date:** 2026-02-10
- **Tags:** [Redis](tags/Redis.md), [BancoDeDados](tags/BancoDeDados.md), [Nosql](tags/Nosql.md)
- **URL:**

______________________________________________________________________

## TLS (Transport Layer Security) no Redis

TLS e a camada de seguranca que criptografa o trafego entre cliente e servidor Redis, protegendo contra espionagem e alteracao de dados em transito. Alem de confidencialidade e integridade, TLS tambem pode autenticar o servidor (validando o certificado) e, opcionalmente, exigir certificado do cliente (mTLS).

Pontos importantes no contexto Redis:

- TLS protege a conexao; nao substitui ACL/senha. Continue usando `requirepass`/ACL e controles de rede.
- Redis suporta TLS nativamente (Redis 6+), configurado via `redis.conf`.
- Boa pratica: desabilitar a porta sem TLS quando possivel.

### Exemplo de configuracao (redis.conf)

```conf
# Desabilita porta TCP sem TLS
port 0

# Habilita TLS
tls-port 6379
tls-cert-file /etc/redis/server.crt
tls-key-file  /etc/redis/server.key
tls-ca-cert-file /etc/redis/ca.crt

# Exigir certificado de cliente (mTLS) (opcional)
tls-auth-clients yes
```

### Conectando com redis-cli

```bash
redis-cli -h <host> -p 6379 --tls \
  --cacert /path/ca.crt \
  --cert /path/client.crt \
  --key /path/client.key
```

Se voce nao validar o certificado do servidor (ex.: `--insecure`), perde a garantia de estar falando com o servidor correto (nao recomendado).
