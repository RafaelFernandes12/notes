# Fundamentos de Java e POO

______________________________________________________________________

**Date:** 2026-03-07
**Tags:** [Java](../tags/Java.md) [POO](../tags/POO.md)
**Subject:**[LP2.md](tags/LP2.md)
**Assets:** https://drive.google.com/file/d/1cH3bQ5xL1iHyCM_2hujhhZhyV_seuteC/view
**URL:** https://docs.oracle.com/en/java/ | https://docs.oracle.com/javase/specs/ | https://openjdk.org/

______________________________________________________________________

## Ideia geral: por que Java “roda em qualquer lugar”?

Em Java, normalmente você escreve **código-fonte** (`.java`), compila para **bytecode** (`.class`) e executa na **JVM** (Java Virtual Machine).

```mermaid
flowchart LR
  A[MeuPrograma.java] -->|javac| B[MeuPrograma.class (bytecode)]
  B -->|java + JVM| C[Execução na plataforma]
```

- **Compilação**: transforma código-fonte em outro formato (em Java, bytecode).
- **Interpretação/JIT**: a JVM executa o bytecode (interpretando e/ou compilando “em runtime” partes para código nativo).
- **Transpilação**: converte código-fonte de uma linguagem para outra linguagem de alto nível (ex.: TypeScript → JavaScript).

## Ferramentas mínimas

- **JDK**: kit para desenvolver (inclui `javac`, `java`, etc.)
- Testes rápidos:

```bash
java -version
javac -version
```

> OpenJDK é a implementação open-source mais comum em Linux. Oracle JDK tem política de licença diferente.

## Tudo começa em uma classe (e no `main`)

Em programas simples, o ponto de entrada é:

```java
public class Main {
  public static void main(String[] args) {
    System.out.println("Hello World");
  }
}
```

`String[] args` são argumentos passados pela linha de comando.

## Tipos e variáveis (primitivos vs referência)

Java é **estaticamente tipada**: você declara o tipo.

```java
int idade = 25;
double altura = 1.75;
boolean ativo = true;
String nome = "Maria"; // String é tipo de referência
```

### `var` (inferência de tipo local)

`var` infere o tipo **na compilação** e exige inicialização:

```java
var total = 10;        // int
var saudacao = "oi";   // String
// var x;              // não compila: precisa inicializar
```

### `null`

`null` significa “ausência de objeto” (tipos de referência):

```java
String s = null;
// s.length(); // NullPointerException
```

## Arrays e matrizes

```java
int[] nums = {10, 20, 30};
int[] dez = new int[10];

int[][] matriz = {
  {1, 2, 3},
  {4, 5, 6}
};
```

Percorrendo:

```java
for (int i = 0; i < nums.length; i++) {
  System.out.println(nums[i]);
}

for (int n : nums) {
  System.out.println(n);
}
```

## Controle de fluxo (decisão e repetição)

### `if / else`

```java
int idade = 18;
if (idade >= 18) {
  System.out.println("maior");
} else {
  System.out.println("menor");
}
```

> Em Java, a condição precisa ser `boolean` (não existe `if (numero)` como em C).

### `switch` clássico vs “moderno” (expression)

Clássico:

```java
int dia = 2;
String nome;
switch (dia) {
  case 1: nome = "Dom"; break;
  case 2: nome = "Seg"; break;
  default: nome = "?";
}
```

Expression:

```java
int opcao = 2;
String texto = switch (opcao) {
  case 1 -> "Um";
  case 2 -> {
    System.out.print("log...");
    yield "Dois";
  }
  default -> "Outro";
};
```

### `while`, `do...while`, `for` e `continue`

```java
int i = 0;
while (i < 3) {
  i++;
}

int j = 0;
do {
  j++;
} while (j < 3);

for (int k = 0; k < 10; k++) {
  if (k % 2 == 0) continue;
  System.out.println(k); // só ímpares
}
```

## Métodos (o “como” do comportamento)

Assinatura típica: `[modificadores] retorno nome(parâmetros)`.

### Exemplo 1: `void` (não retorna)

```java
public static void cumprimentar(String nome) {
  System.out.println("Olá, " + nome);
}
```

### Exemplo 2: retorna valor

```java
public static int somar(int a, int b) {
  return a + b;
}
```

### Exemplo 3: busca em array (exercício típico)

```java
public static int indexOf(int[] v, int alvo) {
  for (int i = 0; i < v.length; i++) {
    if (v[i] == alvo) return i;
  }
  return -1;
}
```

## Entrada e saída (console)

### Imprimir

```java
System.out.print("Nome: " + nome);
System.out.println(" (fim)");
```

### Ler com `Scanner` (e converter)

Um problema comum é “sobrar `\n` no buffer”. Uma estratégia simples: ler tudo como `String` e converter.

```java
import java.util.Scanner;

Scanner sc = new Scanner(System.in);

System.out.print("Idade: ");
int idade = Integer.parseInt(sc.nextLine());

System.out.print("Altura: ");
double altura = Double.parseDouble(sc.nextLine());

System.out.print("Ativo (true/false): ");
boolean ativo = Boolean.parseBoolean(sc.nextLine());
```

## POO: modelar com classes e objetos

### O que é?

- **Classe**: o molde (atributos + métodos).
- **Objeto**: a instância concreta daquele molde.

### Exemplo 1: modelagem simples (Livro / Usuário / Empréstimo)

```java
class Livro {
  String codigo;
  String titulo;
  String autor;
  int qtdDisponivel;

  boolean disponivel() {
    return qtdDisponivel > 0;
  }
}

class Usuario {
  String id;
  String nome;

  void exibir() {
    System.out.println(id + ": " + nome);
  }
}

class Emprestimo {
  Livro livro;
  Usuario usuario;

  void emprestar() {
    if (!livro.disponivel()) throw new IllegalStateException("Sem estoque");
    livro.qtdDisponivel--;
  }

  void devolver() {
    livro.qtdDisponivel++;
  }
}
```

Ponto de aprendizagem: o **estado** está nos atributos (`qtdDisponivel`), e o **comportamento** está nos métodos (`emprestar`, `devolver`).

### Exemplo 2: separando responsabilidades (Conta vs Pessoa)

Pergunta clássica: `atualizarSaldo(valor)` pertence a quem? **ContaBancaria**.

```java
class ContaBancaria {
  double saldo;

  void depositar(double valor) {
    if (valor <= 0) throw new IllegalArgumentException("valor inválido");
    saldo += valor;
  }
}

class Pessoa {
  String nome;
  ContaBancaria conta;
}
```

## Qualidade de design: coerência, coesão e responsabilidade

- **Coerência**: “as coisas nessa classe fazem sentido juntas?”
- **Coesão**: “a classe tem um propósito único bem definido?”
- **Responsabilidade**: “quantos motivos diferentes eu tenho para mudar essa classe?”

Exemplo de “cheiro ruim”: uma classe `Pedido` que calcula total, gera nota fiscal, persiste em banco, envia e-mail etc. Mesmo que tudo “tenha a ver” com pedido, isso mistura motivos de mudança.

## Construtores (inicialização correta)

Construtor:

- tem o **mesmo nome** da classe
- **não tem** tipo de retorno

```java
class Pessoa {
  String nome;
  int idade;

  Pessoa(String nome, int idade) {
    if (idade < 0) throw new IllegalArgumentException("idade inválida");
    this.nome = nome;
    this.idade = idade;
  }
}
```

### Construtor padrão

Se você **não** define nenhum construtor, o Java cria um construtor sem parâmetros.
Se você define **qualquer** construtor, o padrão **não é criado automaticamente**.

### Overload e chaining com `this(...)`

```java
class Pessoa {
  String nome;
  int idade;

  Pessoa(String nome) {
    this(nome, 0); // this(...) precisa ser a 1ª linha
  }

  Pessoa(String nome, int idade) {
    this.nome = nome;
    this.idade = idade;
  }
}
```

### Muitos parâmetros? Considere Builder

```java
class Usuario {
  private final String nome;
  private final String email;
  private final int idade;

  private Usuario(Builder b) {
    this.nome = b.nome;
    this.email = b.email;
    this.idade = b.idade;
  }

  static class Builder {
    String nome;
    String email;
    int idade;

    Builder nome(String nome) { this.nome = nome; return this; }
    Builder email(String email) { this.email = email; return this; }
    Builder idade(int idade) { this.idade = idade; return this; }
    Usuario build() { return new Usuario(this); }
  }
}

// uso:
// var u = new Usuario.Builder().nome("Ana").email("ana@mail").idade(25).build();
```

## Referências de objetos (não é “passar objeto por valor”)

Em Java, quando você “passa um objeto”, você passa a **referência**.

```java
class ContaBancaria {
  double saldo;
}

class Pessoa {
  ContaBancaria conta;
  Pessoa(ContaBancaria conta) {
    this.conta = conta; // mesma referência
  }
}

var conta = new ContaBancaria();
conta.saldo = 100;
var p = new Pessoa(conta);

conta.saldo = 150;
// p.conta.saldo também é 150
```

Se você precisa de “cópia”, você cria outra instância e copia os dados (ex.: **copy constructor**).

## Relações entre objetos: associação, agregação, composição

### Associação (usa/conhece)

```java
class Pessoa {
  void dirigir(Carro c) {
    System.out.println("dirigindo " + c.modelo);
  }
}

class Carro { String modelo; }
```

### Agregação (todo-parte, parte vive sem o todo)

```java
class Professor { String nome; }

class Departamento {
  java.util.List<Professor> professores = new java.util.ArrayList<>();
}
```

### Composição (todo-parte, parte não vive sem o todo)

```java
class Pedido {
  final java.util.List<ItemPedido> itens = new java.util.ArrayList<>();
}

class ItemPedido {
  final String sku;
  ItemPedido(String sku) { this.sku = sku; }
}
```

## Debugging (depuração)

Usos comuns:

- **breakpoint** em linha/método/condição
- **step over / into / out**
- observar variáveis (watch)

Nem todo sistema depura bem (ex.: real-time, multi-thread complexo, distribuído), mas para exercícios e programas pequenos a depuração costuma acelerar muito o diagnóstico.

## Referências (para aprofundar)

- Java Documentation (Oracle): https://docs.oracle.com/en/java/
- Java Language Specification (JLS): https://docs.oracle.com/javase/specs/
- OpenJDK: https://openjdk.org/
- Switch expressions: https://docs.oracle.com/en/java/javase/21/language/switch-expressions.html
- Scanner: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/Scanner.html
