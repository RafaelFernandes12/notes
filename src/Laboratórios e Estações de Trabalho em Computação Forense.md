# Laboratórios e Estações de Trabalho em Computação Forense

______________________________________________________________________

**Date:** 2026-04-22
**Tags:** [Forense](../tags/Forense.md), [Uf](../tags/Uf.md), [LaboratorioForense](../tags/LaboratorioForense.md), [Security](../tags/Security.md)
**URL:**

- https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-86.pdf

______________________________________________________________________

## Por que o laboratório forense é importante?

A maior parte da investigação forense é conduzida dentro de um laboratório. Por isso, o laboratório precisa ser projetado para:

- preservar a integridade das evidências
- reduzir o risco de perda, corrupção ou destruição
- controlar quem entra, quem sai e quem acessa cada item
- dar previsibilidade ao trabalho do investigador

### Ideia para prova

O laboratório forense não é só um lugar com computadores potentes. Ele é também um **ambiente controlado de segurança física e processual**.

## Requisitos físicos mínimos do laboratório

Segundo o PDF, um laboratório forense básico deve ter:

- sala pequena com paredes do chão ao teto
- porta com mecanismo de travamento
- contêiner seguro para evidências
- registro de visitantes

Além disso:

- pessoas que trabalham juntas devem ter o mesmo nível de acesso
- a equipe precisa conhecer a política de segurança do laboratório

### Exemplo de prova

Uma sala compartilhada com circulação livre de terceiros, sem porta trancada e sem registro de visitantes, **não atende** ao mínimo esperado para preservação de evidências.

## Modelos de laboratório

O material mostra três escalas de estrutura.

### 1. Laboratório pequeno ou doméstico

Características:

- poucos armários/cabinetes
- uma ou duas máquinas
- uma bancada de trabalho
- estrutura simples

**Uso típico:** casos pequenos, atendimento individual, estudo, laboratório inicial.

### 2. Laboratório de porte médio

Características:

- várias estações forenses
- área própria para PCs de Internet/intranet
- bancadas maiores
- mais capacidade de trabalho paralelo

**Uso típico:** unidade institucional ou regional com mais de um examinador.

### 3. Laboratório regional/maior

Características:

- sala de evidências separada
- múltiplas estações
- mesa de conferência
- escritórios
- biblioteca ou área de apoio técnico

**Uso típico:** organizações com volume maior de casos e divisão mais clara de funções.

## Investigações de alto risco

Casos de segurança nacional, homicídio ou situações extremamente sensíveis podem exigir proteção adicional contra escutas ou captação eletromagnética.

### TEMPEST

Instalações TEMPEST são projetadas para resistir a vazamentos por radiação eletromagnética.

### O que precisa lembrar?

- são para investigações de **alto risco**
- aumentam muito o custo da infraestrutura
- não são o padrão para qualquer laboratório comum

## Contêineres de evidência

Também chamados de armários de evidências, devem impedir acesso fácil por pessoas não autorizadas.

### Recomendações centrais

- manter em área restrita
- limitar o número de pessoas autorizadas
- registrar quem pode acessar cada contêiner
- manter trancado quando não estiver em uso

### Ideia-chave

O controle físico do contêiner faz parte da cadeia de custódia.

## Se o contêiner usa combinação

Boas práticas destacadas:

- proteger a combinação com o mesmo cuidado dado ao conteúdo
- destruir combinações antigas após mudança
- permitir alteração apenas por pessoal autorizado
- trocar a combinação a cada seis meses ou quando necessário

### Exemplo

Se um servidor antigo sai da equipe e conhecia a combinação do cofre, a combinação deve ser trocada. Manter a mesma combinação compromete a segurança do armazenamento.

## Se o contêiner usa chave

O PDF recomenda:

- nomear um guardião das chaves
- numerar chaves duplicadas
- manter registro de atribuição de cada chave
- realizar auditoria mensal
- fazer inventário das chaves
- trocar fechaduras e chaves anualmente
- não usar chave mestra para várias fechaduras

### Pegadinha de prova

“Chave mestra para facilitar” parece prático, mas **reduz o isolamento entre evidências** e amplia o impacto de um vazamento.

## Estrutura do local de armazenamento

O armazenamento ideal deve prever:

- aço com gabinete interno ou cadeado externo
- cofre de mídia, se possível
- proteção contra incêndio
- sala de evidências separada do restante do laboratório, quando houver recursos
- registro de abertura e fechamento do contêiner

### Exemplo do que deve ser registrado

- data e hora da abertura
- pessoa responsável
- motivo do acesso
- data e hora do fechamento

## Manutenção das instalações

Segurança física não é só instalar e esquecer. O laboratório precisa de manutenção contínua.

### Cuidados citados

- reparar imediatamente danos físicos
- acompanhar equipes de limpeza
- minimizar eletricidade estática
- separar lixo comum de material sensível
- usar descarte especializado quando possível

### Por que acompanhar a limpeza?

Porque mesmo pessoas sem intenção maliciosa podem:

- mover objetos de lugar
- tocar em mídias
- misturar material sensível com lixo comum
- comprometer trilhas documentais

## Segurança física do laboratório

O PDF sugere uma política formal de segurança física.

### Itens importantes

- registro de login para visitantes
- acompanhamento de visitantes o tempo todo
- uso de selo/crachá de visitante
- sistema de alarme de intrusão
- força de guarda, se necessário

### Regra simples

Quem não está designado ao laboratório deve ser tratado como visitante.

## Auditoria do laboratório

Auditar significa verificar se as políticas estão sendo realmente cumpridas.

### O que deve ser inspecionado

- teto, piso, telhado e paredes externas
- portas e fechaduras
- registros de visitantes
- logs dos contêineres de evidência
- armazenamento seguro do material ao final do expediente

### Ideia de prova

Auditoria não é um detalhe administrativo. Ela é mecanismo de verificação da conformidade do laboratório.

## Seleção da estação de trabalho forense

A escolha da workstation depende de:

- orçamento
- perfil dos casos
- volume de dados
- necessidade de mobilidade
- softwares que serão usados

### Regra geral do PDF

- usar máquinas menos potentes para tarefas rotineiras
- usar estações multifuncionais para análise de alto nível

### Exemplo

Uma máquina simples pode servir para:

- documentação
- elaboração de relatórios
- triagem básica

Já uma workstation mais robusta é mais adequada para:

- indexação massiva
- carving
- reconstrução de RAID
- análise de grandes imagens forenses

## Laboratórios policiais

O PDF comenta que laboratórios policiais lidam com necessidades muito diversas.

### Pontos destacados

- ferramentas devem cobrir desde sistemas modernos até legados
- grupos de interesse especial podem ajudar em sistemas antigos
- heurística de planejamento: um investigador para cada 250 mil pessoas na região
- estrutura mínima sugerida: uma workstation forense multifuncional e uma estação de uso geral

### Observação importante

Essa proporção é uma **regra prática**, não uma lei universal.

## Laboratórios privados e corporativos

Nesses ambientes, os requisitos costumam ser mais fáceis de definir porque é possível especializar a estrutura.

### Passos sugeridos

1. identificar o ambiente investigado
2. mapear plataforma de hardware
3. mapear sistema operacional
4. reunir ferramentas adequadas ao ambiente

### Exemplo

Se a empresa trabalha quase toda com Windows e Microsoft 365, faz sentido priorizar:

- análise de Windows
- cliente de e-mail corporativo
- logs de autenticação
- artefatos de nuvem e aplicações Microsoft

## Plano de recuperação de desastres

O laboratório não pode depender da sorte. É necessário prever falhas.

### Requisitos importantes

- backups regulares
- armazenamento seguro fora do local
- capacidade de restaurar a estação de trabalho ao estado original
- gerenciamento de configurações e atualizações de software

### Situações cobertas pelo plano

- falha física do equipamento
- infecção por malware
- erro de configuração
- desastre local

### O que costuma cair em prova

O plano de recuperação serve não só para “voltar a funcionar”, mas também para **restaurar ambiente e arquivos de investigação sem comprometer o trabalho forense**.

## Planejamento de atualização de equipamentos

O PDF sugere tratar atualização como gestão de risco.

### Passos principais

- identificar equipamentos críticos
- decidir o que deve ser substituído preventivamente
- decidir o que pode ser trocado apenas quando falhar

### Vida útil indicada

Componentes de computação costumam durar entre **18 e 36 meses** em condições normais.

### Recomendação do material

- atualizar ao menos a cada 18 meses
- preferencialmente a cada 12 meses

## Estações de trabalho forenses em laptop

O PDF mostra a possibilidade de montar uma estação leve e móvel usando notebook.

### Interfaces citadas

- FireWire
- USB 2.0
- disco PCMCIA SATA

### Vantagens

- mobilidade
- atendimento em campo
- resposta rápida a incidentes

### Limitações

- menor capacidade que workstations dedicadas
- menos expansão
- menor conforto para tarefas longas e pesadas

### Leitura atual

Mesmo que algumas interfaces citadas estejam datadas, o conceito continua válido: **notebooks são úteis para mobilidade, mas ainda não substituem completamente uma estação forense robusta**.

## Checklist de revisão rápida

- laboratório forense precisa de segurança física e controle de acesso
- contêiner de evidência deve ficar em área restrita e com log de acesso
- visitantes precisam ser registrados e acompanhados
- auditoria verifica conformidade das políticas
- estação de trabalho depende do perfil dos casos
- backups, recuperação e atualização fazem parte da operação do laboratório

## Perguntas que podem cair em prova

1. Quais são os requisitos físicos mínimos de um laboratório forense?
2. Como deve ser o controle de acesso a contêineres de evidência?
3. O que muda em investigações de alto risco?
4. Qual a diferença entre uma estação de uso geral e uma estação forense multifuncional?
5. Por que auditorias são importantes em laboratórios forenses?
6. Por que o plano de recuperação de desastres é relevante em forense?
7. Quais são as limitações de workstations baseadas em laptop?
