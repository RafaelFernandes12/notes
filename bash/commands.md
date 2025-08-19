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

