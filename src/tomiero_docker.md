# tomiero_docker

______________________________________________________________________

**Date:** 2026-03-20
**Tags:**[Docker.md](tags/Docker.md)
**URL:**

______________________________________________________________________

## Summary

```bash
sudo systemctl status docker
```

inside docker you can run differentes package manager because only the kernel is shared, so you can have a debian container running on an ubuntu host, and use apt inside the container without affecting the host system.

```bash
docker run -it ubuntu
apt update # totally possible
```

when you kill the container all data is lost.

## Simple Dockerfile example

```Dockerfile
FROM ubuntu:24.04

RUN apt update && apt install -y curl

CMD ["bash"]
```

Build it with:

```bash
docker build -t my-ubuntu-example .
docker run -it my-ubuntu-example
```

## Simple docker-compose example

```yaml
services:
  app:
    build: . # context
    container_name: my-ubuntu-example
    stdin_open: true
    tty: true
```

Run it with:

```bash
docker compose up --build
```
