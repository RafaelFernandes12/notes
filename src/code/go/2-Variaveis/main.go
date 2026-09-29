package main

func main() {
	var variavel1 string = "Variavel 1"
	variavel2 := "variavel 2"
	var (
		variavel3 string = "lalala"
		variavel4 string = "lalala"
	)
	variavel5, variavel6 := "variavel5", "variavel6"
	println(variavel1)
	println(variavel2)
	println(variavel3, variavel4)
	println(variavel5, variavel6)
	const constante1 string = "contante 1"
	println(constante1)
	variavel5, variavel6 = variavel6, variavel5
}
