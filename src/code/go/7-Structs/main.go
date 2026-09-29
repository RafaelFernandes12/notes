package main

import "fmt"

type estudante struct {
	pessoa
	curso     string
	faculdade string
}
type pessoa struct {
	nome      string
	sobrenome string
	idade     uint8
	altura    uint8
}

func main() {
	fmt.Println("arquivos structs")
	p1 := pessoa{"joao", "pedro", 20, 170}
	e1 := estudante{p1, "matematica", "ufrn"}
	fmt.Println(p1)
	fmt.Println(e1)
}
