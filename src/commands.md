## commands

--- 

- **Date:** 2026-01-31
- **Tags:** [Bash](tags/Bash.md)
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
### pm2

```bash
pm2 delete 0
pm2 start 0
pm2 restart 0
```

### vim/neovim

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




