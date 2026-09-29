package main

import (
	"fmt"
)

func somar(n1 int8, n2 int8) int8 {
	return n1 + n2
}

func calculosMatematicos(n1, n2 int8) (int8, int8) {
	soma := n1 + n2
	subtracao := n1 - n2
	return soma, subtracao
}

func main() {
	soma := somar(10, 10)
	fmt.Println(soma)
	f := func(txt string) string {
		fmt.Println(txt)
		return txt + "resultado"
	}

	resultado := f("txt")
	fmt.Println(resultado)
	resultadosSoma, resultadoSubtracao := calculosMatematicos(10, 10)
	_, resultadosSoma2 := calculosMatematicos(50, 10)
	println(resultadosSoma, resultadoSubtracao)
	println(resultadosSoma2)
}
