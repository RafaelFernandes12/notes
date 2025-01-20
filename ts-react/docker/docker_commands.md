Copy a official image:  `docker pull <IMAGE>`

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

Follow the logs from a container: `docker logs -f <IMAGE ID>` 

Check the networks: `docker network ls`
