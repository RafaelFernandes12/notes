## commands

--- 

- **Date:** 2026-01-31
- **Tags:** [Bash](tags/Bash.md) [docker](tags/Docker.md) 
- **URL:**  

---

### Summary

Change default terminal

```bash
sudo update-alternatives --config x-terminal-emulator
```

Run C code

```bash
gcc -o main main.c
./main
```

Run C++ code

```bash
g++ -o main main.cpp
./main
```

Run maven project

```bash
mvn spring-boot:run
```

Kill process by port

```bash
// check whats running
sudo lsof -i :<port>
// kill it
sudo kill -9 <PID>
```

activate boxes

```bash
sudo modprobe kvm_amd
```


Restore mongo docker backup

```bash
docker cp ~/Downloads/assethub-aas-dev mongo:/tmp/assethub-aas-dev
docker exec -it mongo mongorestore --db=assethub-aas-dev /tmp/assethub-aas-dev
```

```bash
grep "pattern" filename
```

```bash
pm2 delete 0
pm2 start 0
pm2 restart 0
```

Replace 'foo' with 'bar' in all .js files in src/routes directory

```bash
:args src/routes/*.js
:argdo %s/foo/bar/g | update

```

Format JSON file
```bash
:%!jq .
```

create postgres database
```bash
docker exec -it pg psql -U postgres -d postgres -c "create database pg_trigger;"
```

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

reset last commit but keep changes in working directory
```bash
git reset --soft HEAD~1
```

reset last commit and discard changes in working directory
```bash
git reset --hard HEAD~1
```

set git pull to always use merge or rebase
```bash
git config pull.rebase false  # merge only in repo
git config pull.rebase true   # rebase only in repo
git config --global pull.rebase false  # merge
git config --global pull.rebase true   # rebase
```

generate ssh key and add to ssh agent
```bash
ssh-keygen -t ed25519 -C "interas@gateway"
ssh-add ~/.ssh/id_ed25519
cat ~/.ssh/id_ed25519.pub
```
GitHub -> Settings -> SSH and GPG keys -> New SSH key (ou “Deploy key” no repo, se for chave so dessa VM).

copy folder to vm
```bash
# -r is recursive, -P is port
scp -r -P 77 ./instantclient_23_26 interas@131.100.25.67:~/ 
```


know public IP address of machine
```bash
curl ifconfig.me
```


* --frozen-lockfile: refuses to update yarn.lock. If package.json and yarn.lock don’t match, it fails. Good for reproducible builds.
- --production: installs only dependencies, skips devDependencies.
- --non-interactive: prevents prompts (good for CI/Docker).

```bash
yarn install [options]
```

Check memory usage

```bash
free -h
```

