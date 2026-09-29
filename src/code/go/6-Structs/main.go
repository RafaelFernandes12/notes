package main

import "fmt"

type usuario struct {
	nome     string
	idade    uint8
	endereco endereco
}
type endereco struct {
	logradouro string
	numero     uint8
}

func main() {
	fmt.Println("arquivos structs")
	var u usuario
	fmt.Println(u)
	u.idade = 10
	u.nome = "rafinha"
	e := endereco{"rua do bobos", 0}

	u2 := usuario{"rafola", 50, e}
	u3 := usuario{nome: "rafola"}
	fmt.Println(u.nome)
	fmt.Println(u.endereco, u2, u3)
}
