package main

import (
	"errors"
	"fmt"
)

func main() {
	var numero int = -100000000000
	var numero2 uint16 = 10000
	// INT32 = RUNE
	var numero3 rune = 1000000000
	// BYTE = UINT8
	var numero4 byte = 123

	var numeroReal1 float32 = 123.45
	var numeroReal2 float64 = 123000000000000.45

	var str string = "Texto"
	str2 := "Texto"
	char := 'B'

	var texto int16
	var bool bool
	var erro error = errors.New("Error interno")

	println(numero)
	println(numero2)
	println(numero3)
	println(numero4)
	println(numeroReal1)
	println(numeroReal2)
	println(str, str2, char)
	println(texto)
	println(bool)
	fmt.Println(erro)
}
