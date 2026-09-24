A ideia do spin lock é executar um laço ou ficar em espera ocupada (busy waiting) até que a trava (lock) seja liberada
Ela deve ser evitada em geral, pois gasta tempo de cpu
Pode ser mantido por no máximo uma thread e é usado para travas por períodos curtos