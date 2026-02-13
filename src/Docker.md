# Docker

______________________________________________________________________

**Date:** 2026-02-06
**Tags:** [Bash](tags/Bash.md) [Docker](tags/Docker.md)
**URL:**

______________________________________________________________________

## build

- --no-cache: Do not use cache when building the image. This forces Docker to execute all steps in the Dockerfile, ensuring a fresh build.

- --pull: Always attempt to pull a newer version of the base image. This ensures that you are using the latest version of the base image, which may include important security updates or bug fixes.

* -t pg-trigger: Tags the built image with the name pg-trigger, making it easier to reference in future commands (e.g., docker run pg-trigger).

## run

Command

```bash
docker run --rm -p 4570:4570 --add-host=host.docker.internal:host-gateway -e DATABASE_URL="postgresql://icone:postgres@host.docker.internal:5433/pg_trigger?schema=public" -d --name pg_trigger pg-trigger
```

- -p 4570:4570: Maps port 4570 of the container to port 4570 on the host machine, allowing you to access the application running inside the container via localhost:4570.

- --add-host=host.docker.internal:host-gateway: Adds a custom host entry to the container’s /etc/hosts file, allowing the container to resolve host.docker.internal to the host machine’s gateway IP address. This is useful for connecting to services running on the host from within the container.

- -e sets an environment variable inside the container. In this case, it sets DATABASE_URL to a PostgreSQL connection string that points to a database running on the host machine (host.docker.internal) on port 5433, with the username icone, password postgres, and database name pg_trigger.

- --rm: Automatically removes the container when it exits, keeping your system clean from stopped containers.

- -d: Runs the container in detached mode, allowing it to run in the background.

- --name pg_trigger: Assigns the name pg_trigger to the container, making it easier to reference in future commands (e.g., docker stop pg_trigger).

- --restart unless-stopped: Configures the container to automatically restart if it stops unexpectedly, unless it is explicitly stopped by the user. This helps ensure that your application remains available even if it encounters issues.

- --restart always: Configures the container to always restart regardless of the exit status. This is useful for critical applications that need to be running at all times, but it may lead to issues if the application has a persistent failure.

## commands

create postgres database

```bash
docker exec -it pg psql -U postgres -d postgres -c "create database pg_trigger;"
```

Copy a official image: `docker pull <IMAGE>`

See all images: `docker images`

Delete image: `docker rmi <IMAGE ID>`

Create your own image from the Dockerfile: `docker build -t <NAME:VERSION> .`

Run container of a image: `docker run <IMAGE ID/REPOSITORY/TAG>`

Run a container in detached mode: `docker run -d <IMAGE ID/REPOSITORY/TAG>`

Expose the port of the container: `docker run -p <HOST_PORT:CONTAINER_PORT> <IMAGE ID>`

Give a name to the container: `docker run —name <NAME> <IMAGE ID>`

Run an existing container: `docker start <NAME>`

See all container running: `docker ps`

See ALL container: `docker ps -a`

Stop a container running in detached mode: `docker stop <IMAGE ID>` or `<NAME>`

Delete a container: `docker rm <IMAGE ID>`or `<NAME>`

Delete all non running containers: `docker rm $(docker ps -aq)`

Delete ALL containers: `docker rm -f $(docker ps -aq)`

Run a container with a volume: `docker run -v <PATH_HOST:PATH_CONTAINER> <IMAGE ID>`

Share volume between containers: `docker run --volumes-from <NAME_CONTAINER> <IMAGE ID>`

Access the container files: `docker exec -it <NAME/IMAGE ID> sh`

Copy existing image and name it:`docker tag <OLD_NAME:OLD_VERSION> <NEW_NAME:NEW_VERSON>`

Push images to docker registry: `docker push <USERNAME/IMAGE_NAME:IMAGE_VERSION>`

Get more informations from the container: `docker inspect <NAME>`

See the logs from a container: `docker logs <IMAGE ID>`

See the logs with timestamp from a container: `docker logs -t <IMAGE ID>`

Follow the logs from a container: `docker logs -f <CONTAINER ID>`

Check the networks: `docker network ls`
