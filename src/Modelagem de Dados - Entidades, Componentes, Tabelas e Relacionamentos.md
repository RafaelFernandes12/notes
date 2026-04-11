# Modelagem de Dados - Entidades, Componentes, Tabelas e Relacionamentos

______________________________________________________________________

**Date:** 2026-03-15
**Tags:**
- [BancoDeDados](../tags/BancoDeDados.md)
- [SQL](../tags/SQL.md)
- [JPA](../tags/JPA.md)
- [Hibernate](../tags/Hibernate.md)
- [Java](../tags/Java.md)
- [Annotations](../tags/Annotations.md)
- [ModelagemDeDados](../tags/ModelagemDeDados.md)
**URL:**
- https://jakarta.ee/specifications/persistence/3.1/
- https://jakarta.ee/specifications/persistence/3.1/apidocs/jakarta.persistence/jakarta/persistence/entity
- https://jakarta.ee/specifications/persistence/3.1/apidocs/jakarta.persistence/jakarta/persistence/embeddable
- https://jakarta.ee/specifications/persistence/3.1/apidocs/jakarta.persistence/jakarta/persistence/embedded
- https://jakarta.ee/specifications/persistence/3.1/apidocs/jakarta.persistence/jakarta/persistence/column
- https://docs.hibernate.org/hibernate/orm/current/userguide/html_single/
- https://www.postgresql.org/docs/current/ddl-constraints.html

______________________________________________________________________

## Por que esses nomes confundem?

Os termos **entidade**, **atributo**, **relacionamento**, **tabela**, **coluna** e **componente** aparecem em **camadas diferentes**:

- **Modelo conceitual (ER / “mundo real”)**: entidades, atributos, relacionamentos.
- **Modelo lógico relacional (banco)**: tabelas, colunas, linhas, chaves, constraints.
- **Modelo de objetos (código / DDD / ORM)**: classes/records, fields, identidade, value objects; anotações (`@Entity`, `@Embeddable`, etc.).

O mais importante é entender como esses conceitos **se correspondem** e onde eles **não** são equivalentes.

> Observação: “componente” também é palavra comum em UI (React/Vue/etc.). Aqui “componente” vai significar **componente de modelo** (ex: objeto embutido/`@Embeddable`), porque você pediu junto com entidade/tabela/coluna.

---

## Mapa mental (conceitual ↔ relacional ↔ ORM)

| Ideia | Conceitual (ER) | Relacional (SQL) | ORM/JPA (Java) |
|---|---|---|---|
| “Coisa com identidade própria” | Entidade | Linha identificável por PK | `@Entity` + `@Id` |
| “Características da coisa” | Atributos | Colunas | fields + `@Column` |
| “Ligação entre coisas” | Relacionamento | FK / tabela de junção | `@ManyToOne`, `@OneToMany`, `@ManyToMany`, ... |
| “Sub-objeto sem identidade” | (atributo composto / VO) | Colunas no mesmo row (ou tabela dependente) | `@Embeddable` + `@Embedded` (ou `@ElementCollection`) |

---

## 1) Entidade (Entity)

### 1.1 Entidade no modelo conceitual (ER)

Uma **entidade** é um “tipo de coisa” do domínio que você quer representar e sobre a qual quer guardar informação.

- Ex.: `Usuário`, `Agendamento`, `Pedido`, `Produto`, `Conta`.

**O que define uma entidade** (no sentido clássico/DDD):

- ela tem **identidade** (um “quem é”): `id`, `cpf`, `matricula`, `uuid`, etc.
- ela pode mudar no tempo (atributos variam), mas continua sendo “a mesma” entidade.

### 1.2 Entidade no modelo de objetos (DDD/OO)

Em DDD:

- **Entity**: tem identidade e ciclo de vida (ex.: `Order`, `User`).
- **Value Object**: não tem identidade própria; o que importa é o valor (ex.: `Money`, `Email`, `Address`).

Isso importa porque influencia:

- comparação: Entity geralmente compara por `id`; Value Object compara por valor.
- persistência: Entity vira tabela própria; Value Object pode virar colunas embutidas.

### 1.3 Entidade no ORM (JPA)

No JPA/Hibernate, uma entidade é uma classe/record anotada com `@Entity` (e normalmente `@Table`) que será persistida.

Exemplo mínimo:

```java
import jakarta.persistence.*;

@Entity
@Table(name = "users")
public class User {
  @Id
  @GeneratedValue(strategy = GenerationType.IDENTITY)
  private Long id;

  @Column(nullable = false, length = 120)
  private String name;
}
```

**Atributos típicos de uma entidade (no sentido de persistência):**

- **PK** (`@Id`) e como ela é gerada (`@GeneratedValue`).
- **nome da tabela** (`@Table(name=...)`) e índices/constraints associados.
- **colunas** (`@Column`) e tipos.
- **relacionamentos** (associações) e as colunas de FK.
- regras de integridade: nullability, uniqueness, checks, etc.

---

## 2) Atributos (Attributes)

“Atributo” é um termo que aparece tanto no ER quanto no relacional/ORM.

### 2.1 Atributos no ER

Tipos clássicos:

- **simples**: `nome`, `idade`.
- **composto**: `endereço` = {rua, número, cidade, CEP}.
- **multivalorado**: `telefones` (mais de um).
- **derivado**: `idade` derivada de `data_nascimento`.

### 2.2 Atributos no relacional

Viram (em geral) **colunas** e restrições:

- tipo: `varchar`, `int`, `timestamp`, `numeric(10,2)`, ...
- `NOT NULL`
- `DEFAULT`
- `UNIQUE`
- `CHECK`

### 2.3 Atributos no ORM (ex.: JPA)

Viram **campos** mapeados com anotações.

`@Column` é o “ponto de controle” mais comum:

```java
@Column(
  name = "email",
  nullable = false,
  unique = true,
  length = 255
)
private String email;
```

Coisas que são atributos no modelo, mas não são “colunas” do jeito que você imagina:

- **atributos derivados** (ex.: `getAge()`): normalmente não são persistidos.
- **atributos calculados**: podem ser `@Transient`.
- **atributos complexos**: podem ser `@Embedded` (componente) ou entidades próprias.

---

## 3) Tabelas (Tables), colunas (Columns) e linhas (Rows)

### 3.1 Tabela

No modelo relacional, uma **tabela** representa um conjunto de linhas com o mesmo “formato”.

- Ela tem um **esquema** (lista de colunas com tipos e constraints).
- Ela deve ter uma forma de **identificar unicamente** uma linha (normalmente via PK).

### 3.2 Coluna

Uma **coluna** tem:

- **nome**
- **tipo/domínio** (ex.: `TEXT`, `INTEGER`, `TIMESTAMP`)
- constraints: `NOT NULL`, `UNIQUE`, `CHECK`, `DEFAULT`

### 3.3 Linha (tupla/registro)

Uma **linha** é uma instância concreta: valores para cada coluna.

### 3.4 “Tabela = Entidade?”

Quase sempre uma `@Entity` mapeia para uma tabela.

Mas não é 1:1 em todos os casos:

- herança (uma hierarquia de classes pode virar 1 ou várias tabelas)
- `@Embeddable` (componente) vira colunas na mesma tabela
- `@ElementCollection` pode virar tabela “dependente” sem virar entidade completa

---

## 4) Componentes (Components) / Value Objects / Embeddables

### 4.1 O que é um componente no sentido de persistência?

Um **componente** (em Hibernate) ou **Embeddable** (em JPA) é um tipo que:

- **não tem identidade própria** (não tem `@Id`)
- existe “dentro” de uma entidade
- normalmente vira **colunas na mesma tabela** da entidade dona

Pense em `Address`, `Money`, `Email`, `Period`.

### 4.2 Exemplo 1 — `Address` como componente (`@Embeddable`)

```java
import jakarta.persistence.*;

@Embeddable
public class Address {
  @Column(name = "street", nullable = false)
  private String street;

  @Column(name = "city", nullable = false)
  private String city;

  @Column(name = "zip_code", nullable = false, length = 12)
  private String zipCode;
}

@Entity
@Table(name = "customers")
public class Customer {
  @Id @GeneratedValue
  private Long id;

  @Column(nullable = false)
  private String name;

  @Embedded
  private Address address;
}
```

Como isso tende a aparecer no SQL:

```sql
CREATE TABLE customers (
  id BIGSERIAL PRIMARY KEY,
  name TEXT NOT NULL,
  street TEXT NOT NULL,
  city TEXT NOT NULL,
  zip_code VARCHAR(12) NOT NULL
);
```

> Note: `Address` não vira tabela “address” (nesse mapeamento). Ele “explode” em colunas.

### 4.3 Exemplo 2 — `Money` como componente (e por que é útil)

Em vez de guardar `price` como `double`, você modela:

- `amount`
- `currency`

```java
@Embeddable
public class Money {
  @Column(name = "amount", nullable = false, precision = 12, scale = 2)
  private java.math.BigDecimal amount;

  @Column(name = "currency", nullable = false, length = 3)
  private String currency; // ISO-4217
}

@Entity
@Table(name = "products")
public class Product {
  @Id @GeneratedValue
  private Long id;

  @Column(nullable = false)
  private String name;

  @Embedded
  private Money price;
}
```

Vantagem: você “força” consistência (não existe amount sem currency) e evita erros de ponto flutuante.

### 4.4 Quando um “componente” deveria ser uma entidade?

Sinais de que vale virar Entity (tabela própria):

- precisa ter identidade e ser referenciado por múltiplas entidades
- tem ciclo de vida independente
- você quer relacionamentos “de verdade” com outras entidades

Ex.: `Address` pode ser VO (cada cliente tem o seu), mas pode virar Entity em um sistema de logística onde endereços são compartilhados, versionados, auditados.

---

## 5) Relacionamentos (Relationships) e como viram chaves estrangeiras

Relacionamento é a parte que mais gera confusão, porque:

- no **conceitual**, você fala em cardinalidade/participação (1:1, 1:N, N:N)
- no **relacional**, você implementa isso com PK/FK (e às vezes tabela de junção)
- no **ORM**, você precisa saber quem é o “dono” do relacionamento (owning side)

### 5.1 Vocabulário essencial

- **Cardinalidade**: 1:1, 1:N, N:N.
- **Opcionalidade (participação)**:
  - obrigatório: precisa existir (FK `NOT NULL`)
  - opcional: pode não existir (FK `NULL`)
- **Chave estrangeira (FK)**: coluna(s) que apontam para a PK (ou unique) de outra tabela.

---

## 6) Exemplos completos (SQL + mapeamento mental)

### Exemplo A — 1:N (Um usuário tem vários agendamentos)

#### Conceitual

- `User` 1 —— N `Appointment`

#### Relacional (SQL)

```sql
CREATE TABLE users (
  id BIGSERIAL PRIMARY KEY,
  name TEXT NOT NULL,
  email TEXT NOT NULL UNIQUE,
  password_hash TEXT NOT NULL,
  type TEXT NOT NULL
);

CREATE TABLE appointments (
  id BIGSERIAL PRIMARY KEY,
  appointment_time TIMESTAMP NOT NULL,
  user_id BIGINT NOT NULL,
  CONSTRAINT fk_appointments_user
    FOREIGN KEY (user_id) REFERENCES users(id)
);

CREATE INDEX idx_appointments_user_id ON appointments(user_id);
CREATE INDEX idx_appointments_time ON appointments(appointment_time);
```

Onde estão os “atributos” aqui?

- `users.name`, `users.email`, ... são atributos do `User`
- `appointments.appointment_time` é atributo do `Appointment`
- `appointments.user_id` é o atributo relacional que implementa o relacionamento

#### ORM (JPA) — como se lê

O lado “muitos” normalmente tem a FK, então costuma ser `@ManyToOne`.

```java
@Entity
@Table(name = "appointments")
public class Appointment {
  @Id @GeneratedValue
  private Long id;

  @Column(name = "appointment_time", nullable = false)
  private java.time.LocalDateTime appointmentTime;

  @ManyToOne(optional = false)
  @JoinColumn(name = "user_id", nullable = false)
  private User user;
}

@Entity
@Table(name = "users")
public class User {
  @Id @GeneratedValue
  private Long id;

  @OneToMany(mappedBy = "user")
  private java.util.List<Appointment> appointments = new java.util.ArrayList<>();
}
```

Pontos que quase sempre importam:

- `mappedBy = "user"` diz que **a FK está em Appointment** (lado dono).
- `optional=false` + `nullable=false` é a parte “obrigatória” do relacionamento.
- índice em `user_id` ajuda muito consultas por usuário.

---

### Exemplo B — N:N (Alunos e disciplinas) com tabela de junção

#### Conceitual

- `Student` N —— N `Course`

#### Relacional (SQL)

```sql
CREATE TABLE students (
  id BIGSERIAL PRIMARY KEY,
  name TEXT NOT NULL
);

CREATE TABLE courses (
  id BIGSERIAL PRIMARY KEY,
  title TEXT NOT NULL
);

CREATE TABLE student_courses (
  student_id BIGINT NOT NULL,
  course_id BIGINT NOT NULL,
  PRIMARY KEY (student_id, course_id),
  FOREIGN KEY (student_id) REFERENCES students(id),
  FOREIGN KEY (course_id) REFERENCES courses(id)
);
```

Regra prática: relação N:N vira **tabela de junção**.

#### ORM (JPA)

```java
@Entity
@Table(name = "students")
public class Student {
  @Id @GeneratedValue
  private Long id;

  @ManyToMany
  @JoinTable(
    name = "student_courses",
    joinColumns = @JoinColumn(name = "student_id"),
    inverseJoinColumns = @JoinColumn(name = "course_id")
  )
  private java.util.Set<Course> courses = new java.util.HashSet<>();
}

@Entity
@Table(name = "courses")
public class Course {
  @Id @GeneratedValue
  private Long id;

  @ManyToMany(mappedBy = "courses")
  private java.util.Set<Student> students = new java.util.HashSet<>();
}
```

Quando N:N começa a ter atributos próprios (ex.: `enrolled_at`, `grade`), a tabela de junção geralmente vira uma **entidade associativa** (`Enrollment`) com 2 FKs.

---

### Exemplo C — 1:N com “entidade dependente” (Pedido e Itens)

Aqui o relacionamento costuma ter regras fortes de ciclo de vida (itens não fazem sentido sem o pedido).

#### Relacional (SQL)

```sql
CREATE TABLE orders (
  id BIGSERIAL PRIMARY KEY,
  created_at TIMESTAMP NOT NULL DEFAULT now(),
  status TEXT NOT NULL
);

CREATE TABLE order_items (
  id BIGSERIAL PRIMARY KEY,
  order_id BIGINT NOT NULL,
  product_sku TEXT NOT NULL,
  quantity INT NOT NULL CHECK (quantity > 0),
  unit_price NUMERIC(12,2) NOT NULL CHECK (unit_price >= 0),
  FOREIGN KEY (order_id) REFERENCES orders(id)
);

CREATE INDEX idx_order_items_order_id ON order_items(order_id);
```

#### ORM (JPA)

```java
@Entity
@Table(name = "orders")
public class Order {
  @Id @GeneratedValue
  private Long id;

  @OneToMany(mappedBy = "order", cascade = CascadeType.ALL, orphanRemoval = true)
  private java.util.List<OrderItem> items = new java.util.ArrayList<>();
}

@Entity
@Table(name = "order_items")
public class OrderItem {
  @Id @GeneratedValue
  private Long id;

  @ManyToOne(optional = false)
  @JoinColumn(name = "order_id", nullable = false)
  private Order order;
}
```

Notas importantes:

- `cascade = ALL` + `orphanRemoval = true` modela “item nasce/morre com o pedido”.
- isso é uma regra de domínio/consistência, não apenas “conveniência”.

---

## 7) Exemplo do seu código (barbearia) e como interpretar

No seu repositório `barbearia_do_matheus`, há estes records:

`User`:

```java
@Table()
@Entity()
public record User(
    @Id int id,
    @NotBlank String name,
    @NotBlank @Email String email,
    @NotBlank String password,
    Type type) {
}
```

`Appointment`:

```java
@Table()
@Entity()
public record Appointment(
    @NotBlank LocalDateTime AppointmentTime,

    @OneToMany() @NotBlank User user) {
}
```

Como ler isso em termos de modelagem:

- `User` é uma **entidade** com atributos (`id`, `name`, `email`, `password`, `type`).
- `Appointment` é outra **entidade** com atributo `AppointmentTime`.
- o campo `user` quer dizer “um agendamento está ligado a um usuário”.

O detalhe: `@OneToMany` em cima de `User user` está estranho porque:

- `@OneToMany` significa “um *Appointment* tem **muitos** Users”, mas o tipo do campo é um único `User`.
- na maioria dos domínios de agenda, o natural é: **um usuário tem muitos agendamentos** e **um agendamento pertence a um usuário**.

Isso normalmente vira:

- em `Appointment`: `@ManyToOne User user`
- (opcional) em `User`: `@OneToMany List<Appointment> appointments`

Use isso como checklist mental: *onde está a FK?* Geralmente no lado “muitos”.

---

## 8) “Atributos” de relacionamentos (o que você escolhe ao modelar)

Quando você cria um relacionamento, você quase sempre precisa decidir:

1. **Cardinalidade**: 1:1, 1:N, N:N.
2. **Opcionalidade**: pode existir sem o outro? (FK `NULL` vs `NOT NULL`).
3. **Ciclo de vida**: o filho depende do pai? (cascade, orphanRemoval).
4. **Direção de navegação**: precisa navegar nos dois sentidos no código?
5. **Performance**:
   - índices nas FKs
   - `JOIN` vs consultas separadas
   - carregamento lazy/eager (no ORM)

---

## 9) Checklist rápido: entidade vs componente vs tabela

### Se a coisa tem identidade e é referenciada por outras coisas

- provavelmente é **Entidade**
- provavelmente vira **Tabela**

### Se a coisa é “parte de” uma entidade e não faz sentido sozinha

- provavelmente é **Componente / Value Object**
- provavelmente vira **colunas na mesma tabela** (ou uma tabela dependente via `@ElementCollection`)

### Se você precisa de N:N

- no relacional você vai criar **tabela de junção**
- no ORM você vai usar `@ManyToMany` ou modelar uma entidade associativa

---

## 10) Próximos tópicos úteis (para continuar)

- normalização (1FN/2FN/3FN) vs denormalização
- constraints e integridade referencial (FK, ON DELETE/UPDATE)
- índices e planos de execução
- herança no JPA (SINGLE_TABLE / JOINED / TABLE_PER_CLASS)
- `@ElementCollection` vs entidade de verdade
