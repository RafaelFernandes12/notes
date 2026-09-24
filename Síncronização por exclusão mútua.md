Por exemplo, para incrementar o valor de uma variável global "a" é preciso de 3 instruções de nível mais baixo.
* ler o valor atual da variável
* incrementar esse valor
* escrever o novo valor na variável global
Com o uso desse método, apenas uma thread tem acesso exclusivo à variável compartilhada, e ela faz a operação sem interferência de outra thread.
Os mecanismos reais usados para fornecer essa sincronização são
* [[Operações atômicas]]
* [[Spin Locks]]
* [[Semáforos]]