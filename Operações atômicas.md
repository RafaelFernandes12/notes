Operações atômicas são executadas sem interrupção.
São instruções indivisíveis capazes de executar leitura e escrita da memória em uma única etapa indivísivel e ininterrupta.
Essas soluções normalmente são suportadas a nível de hardware, exemplo é o [[TSL (Test-And-Set Lock)]].
* Solução a nível de hardware
* Impede o acesso ao barramento de memória para proibir outras CPUs de ter acesso
* Garantir a atomicidade do mecanismo de trava