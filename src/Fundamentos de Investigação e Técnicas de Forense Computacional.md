# Fundamentos de Investigação e Técnicas de Forense Computacional

______________________________________________________________________

**Date:** 2026-04-22
**Tags:** [Forense](../tags/Forense.md), [Uf](../tags/Uf.md), [Security](../tags/Security.md), [InvestigacaoForense](../tags/InvestigacaoForense.md)
**URL:**

- https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-86.pdf
- https://www.rfc-editor.org/rfc/rfc5322
- https://hackmd.io/@ramonfontes/mactimes
- https://hackmd.io/@ramonfontes/data_carving

______________________________________________________________________

## O que é computação forense?

Computação forense é a área que **coleta, preserva, analisa e apresenta evidências digitais** de forma tecnicamente correta e juridicamente defensável.

O objetivo não é só “achar um arquivo suspeito”, mas responder perguntas como:

- **o que aconteceu?**
- **quando aconteceu?**
- **quem fez?**
- **como fez?**
- **quais evidências sustentam essa conclusão?**

Em prova, uma definição segura é:

> Computação forense é o processo de obtenção e análise de evidências digitais com preservação de integridade, cadeia de custódia e capacidade de apresentação em contexto judicial ou corporativo.

## Papel do profissional de forense computacional

O perito ou analista forense precisa:

- reunir evidências que comprovem um crime ou violação de política
- investigar o equipamento ou ambiente suspeito
- **preservar as evidências em outro sistema**, evitando alterações na mídia original
- documentar tudo o que foi feito
- transformar achados técnicos em um **laudo compreensível**

### Exemplo prático

Se um funcionário é suspeito de vazar documentos:

1. o computador dele não deve ser “explorado livremente” no uso normal
2. a mídia deve ser preservada e copiada de forma forense
3. a análise deve ocorrer sobre a cópia
4. o resultado precisa indicar **quais arquivos**, **em que datas**, **por qual conta** e **com qual trilha de evidências**

## Cadeia de custódia

**Cadeia de custódia** é o histórico completo da evidência desde sua coleta até o encerramento do caso.

Ela responde:

- quem encontrou a evidência?
- quando ela foi coletada?
- onde foi armazenada?
- quem teve acesso?
- quando foi aberta, movida, copiada ou devolvida?

### Por que isso é tão importante?

Porque evidência digital é muito fácil de alterar. Sem cadeia de custódia, a defesa pode argumentar que:

- a evidência foi adulterada
- não há prova de origem
- várias pessoas mexeram nela sem controle

### Exemplo simples

Um notebook é apreendido às 14:10.

- 14:12: equipamento fotografado e identificado
- 14:20: colocado em embalagem apropriada e lacrado
- 15:05: entregue ao laboratório
- 16:00: imagem forense gerada
- 16:45: evidência original volta ao cofre

Esse registro é exatamente o tipo de documentação que protege o caso.

## Abordagem sistemática da investigação

O PDF enfatiza que a investigação deve seguir uma sequência organizada.

### Etapas principais

1. fazer uma avaliação inicial do caso
2. definir uma abordagem preliminar
3. criar uma checklist detalhada
4. determinar os recursos necessários
5. obter e copiar as mídias com evidências
6. identificar riscos
7. mitigar os riscos
8. testar o plano
9. analisar e recuperar evidências digitais
10. investigar os dados recuperados
11. concluir o relato do caso
12. criticar o caso e aprender com ele

### O que costuma cair em prova

- forense **não é improviso**
- a coleta ocorre antes da análise detalhada
- a mitigação de risco faz parte do processo
- o caso deve ser **encerrado com relatório** e revisão crítica

## Avaliação do caso

Antes de começar, o investigador precisa descrever o caso de forma sistemática.

### Informações mínimas

- situação
- natureza do caso
- especificidades do caso
- tipo de evidência
- sistema operacional
- formato de disco conhecido
- localização das evidências

### O que isso permite decidir?

- quais ferramentas usar
- se será aquisição estática ou ao vivo
- se há criptografia envolvida
- se será preciso pessoal especializado
- se o ambiente exige tratamento legal especial

### Exemplo

Caso A: acesso não autorizado em servidor Linux.

- evidência principal: logs, memória, conexões, arquivos de configuração
- prioridade: dados voláteis
- ferramentas: captura de RAM, coleta de logs, timeline, hash

Caso B: assédio por e-mail.

- evidência principal: cabeçalhos, corpo, anexos, logs do servidor de e-mail, cliente de correio
- prioridade: preservar mensagens originais e cadeia de envio

## Planejamento da investigação

Um plano básico precisa incluir:

- aquisição das evidências
- preenchimento do formulário de evidências
- estabelecimento da cadeia de custódia
- transporte ao laboratório
- armazenamento em contêiner seguro
- preparação da estação forense
- criação da cópia forense
- devolução da mídia original ao armazenamento seguro
- processamento da cópia com ferramentas forenses

### Ideia central

**evidência original deve ser preservada; análise deve ocorrer sobre a cópia**.

## Proteção das evidências

Boas práticas destacadas no PDF:

- usar sacos de evidência
- usar materiais antiestáticos
- usar recipientes acolchoados
- selar aberturas com fita
- catalogar tudo adequadamente

### Erro clássico de prova

Ligar a máquina suspeita “só para dar uma olhada” pode:

- alterar timestamps
- disparar sincronizações
- mudar logs
- acionar scripts automáticos
- invalidar parte da confiabilidade da evidência

## Técnicas de investigação

O objetivo mais comum da análise forense é **entender um evento de interesse** e encontrar os fatos relacionados a ele.

O tipo de técnica depende do caso.

### Exemplos por tipo de crime

- **acesso não autorizado** → evidências de conexão, autenticação, IP, portas, processos
- **fraude por e-mail** → cabeçalhos, anexos, logs SMTP/IMAP/Exchange, conteúdo
- **pornografia infantil** → imagens, vídeos, histórico, compartilhamento, metadados
- **uso indevido de recurso corporativo** → logs de acesso, navegação, envios, arquivos copiados

### Áreas da forense digital

- computação forense tradicional
- forense em dispositivos móveis
- forense em redes
- forense em internet das coisas
- forense em banco de dados

## Análise de vulnerabilidade x teste de penetração

Essa diferença é clássica e pode aparecer em prova.

| Tema | Análise de vulnerabilidade | Teste de penetração |
|---|---|---|
| Objetivo | identificar fragilidades | tentar explorar fragilidades |
| Profundidade | geralmente mais ampla e menos invasiva | mais profunda e manual |
| Saída | lista e grau de risco | prova prática de exploração |
| Quando usar | para mapear exposição | para verificar se a defesa realmente resiste |

### Resposta curta para prova

- **análise de vulnerabilidade** mede fragilidade
- **pentest** tenta penetrar no sistema explorando vulnerabilidades

### Metodologias citadas

- OWASP
- ISSAF
- NIST
- OSSTMM
- PTES

## MACTimes

MACTimes resumem timestamps importantes do sistema de arquivos.

### MTime - Modification time

Momento da última modificação no **conteúdo** do arquivo.

Exemplo:

```bash
echo "novo texto" >> arquivo.txt
```

### ATime - Access time

Momento do último acesso ao arquivo.

Exemplo:

```bash
cat arquivo.txt
```

### CTime - Change time

Momento da última alteração no conteúdo **ou nos metadados** do arquivo.

Exemplo:

```bash
chown usuario arquivo.txt
```

### Pegadinha comum

No Unix/Linux, **ctime não é creation time**. É **change time**.

### Exemplo de interpretação

Se um arquivo tem:

- mtime: 10:00
- atime: 10:30
- ctime: 11:00

Uma leitura possível é:

- conteúdo foi alterado às 10:00
- arquivo foi acessado às 10:30
- algum metadado mudou às 11:00, ou houve nova alteração que mexeu no inode

## Ordem de volatilidade das evidências

Quanto mais volátil o dado, mais rápido ele desaparece.

### Ordem apresentada no PDF

1. registradores, memória periférica e caches
2. memória principal
3. estado da rede
4. processos em execução
5. disco
6. disquetes e mídia de backup
7. CD-ROMs e impressões

### Regra prática

Se o sistema está ligado e a situação permite, dados voláteis podem ser mais valiosos do que o conteúdo do disco.

### Exemplo

Em um ransomware em execução:

- chaves podem estar na RAM
- conexões ativas podem indicar comando e controle
- processos podem revelar o executável real

Se a máquina for desligada antes da coleta, tudo isso pode sumir.

## Data Carving

**Data carving** é a recuperação de arquivos a partir da assinatura do tipo de arquivo, mesmo quando o sistema de arquivos já não mantém mais a referência normal para ele.

### Quando ele é útil?

Quando:

- o arquivo foi apagado
- o sistema de arquivos está corrompido
- a tabela de alocação não aponta mais para o conteúdo

### Pressupostos citados no PDF

1. o início do arquivo ainda existe
2. a assinatura procurada não é comum demais, evitando muitos falsos positivos
3. o arquivo está gravado de forma contínua e não fragmentada

### Exemplo

Um JPEG costuma começar com uma assinatura como:

```text
FF D8 FF
```

Se essa assinatura for encontrada em setores sem referência ativa do sistema de arquivos, a ferramenta pode tentar reconstruir a imagem.

### Limitações

- falsos positivos
- arquivos fragmentados
- recuperação parcial
- dificuldade de reconstruir nomes e caminhos originais

## Forense de e-mail

O PDF destaca um ponto muito importante: **analisar e-mail não é só ler o texto da mensagem**.

### Estrutura básica

Segundo o padrão de e-mail:

- **header**: remetente, destinatário, caminho da mensagem, IPs, autenticação, assunto etc.
- **body**: conteúdo e anexos

### Por que o cabeçalho é tão importante?

Porque ele ajuda a responder:

- quem enviou?
- de qual IP?
- por quais servidores a mensagem passou?
- houve SPF, DKIM ou outros sinais de autenticação?
- a mensagem foi realmente originada daquele domínio?

### Problema clássico

Encaminhar o e-mail normalmente pode **perder ou sobrescrever cabeçalhos**.

Exportar pelo Outlook também pode manter apenas parte do cabeçalho, porque o cliente pode salvar a mensagem como um objeto próprio do Outlook e não como a mensagem crua completa.

### Melhor prática

Coletar:

- cabeçalho completo
- mensagem original/raw source
- logs do servidor de e-mail
- artefatos do cliente de e-mail

### Exemplo de campos úteis

- `Received`
- `Return-Path`
- `Message-ID`
- `From`
- `Reply-To`
- resultados de SPF/DKIM/DMARC

## Elaboração de laudo

O laudo deve apresentar os resultados do processo de exame, coleta e análise.

Ele precisa:

- descrever metodologia
- explicar evidências relevantes
- traduzir linguagem técnica
- registrar limitações
- apresentar conclusão fundamentada

### Estrutura útil

1. objetivo
2. escopo
3. evidências recebidas
4. cadeia de custódia
5. ferramentas utilizadas
6. procedimentos executados
7. achados
8. timeline
9. conclusão
10. anexos e hashes

## Agente infiltrado virtual

O PDF relaciona essa técnica às dificuldades do meio cibernético:

- anonimato
- dispersão geográfica
- baixo custo para o criminoso e grande alcance

### Ideia central

O agente infiltrado virtual é uma técnica investigativa em que o agente atua no ambiente digital para obter prova e identificar autoria/materialidade.

### Contexto brasileiro destacado

- ampliação pela Lei 12.850/2013
- tratamento específico de infiltração virtual de agentes policiais na Lei 13.441/2017

### Requisitos de uso mencionados no PDF

- indícios de crime
- inexistência de outro meio possível para obtenção da prova necessária

### O que pode cair em prova

- surgiu para responder às dificuldades do ciberespaço
- não é ferramenta livre ou irrestrita
- depende de base legal e necessidade investigativa

## Técnicas anti-forenses

**Anti-forense** é qualquer técnica usada para dificultar, atrasar, confundir ou impedir a investigação forense.

### Formas citadas no PDF

- criptografia
- esteganografia
- sanitização de discos
- evolução tecnológica
- rootkits
- desmagnetizador
- empacotamento
- anti-debugging
- anti-disassembly
- anti-VM

### 1. Criptografia

Elementos básicos:

- **mensagem**: conteúdo original
- **cifra**: mecanismo/chave
- **código**: resultado cifrado

#### Relevância forense

Se o disco estiver criptografado, a aquisição pode capturar apenas dados indecifráveis, a menos que:

- a máquina esteja ligada e descriptografada
- a chave seja obtida
- existam artefatos de senha/chave

#### Criptoanálise

Busca recuperar a informação sem a chave original, usando técnicas como análise matemática, força bruta ou tabelas pré-computadas.

### 2. Esteganografia

É a ocultação de uma mensagem dentro de outro arquivo ou mídia.

Exemplo clássico:

- texto secreto escondido dentro de uma imagem aparentemente normal

Pistas sugeridas no PDF:

- palavras-chave
- tamanho de arquivo estranho
- presença de software específico

### 3. Sanitização e wipe

Sanitização = apagar informações de forma a impedir recuperação.

**Wipe** = sobrescrever a área do arquivo bit a bit.

### 4. Evolução tecnológica

Traz desafios forenses como:

- virtualização
- dispositivos móveis
- grandes volumes de dados
- cloud computing
- BYOD
- SSDs

### 5. Rootkits

Rootkits escondem processos, arquivos ou acessos, dificultando a detecção normal. Eles funcionam como forte mecanismo de ocultação.

### 6. Desmagnetizador

Aplica campo magnético à mídia, eliminando dados. É eficaz, mas caro e menos comum para o usuário médio.

### 7. Empacotamento e ofuscação

Malwares podem ser compactados/ofuscados para dificultar engenharia reversa.

Sinal prático citado no PDF:

- programas legítimos costumam ter muitas strings
- um executável com pouquíssimas strings pode estar empacotado/ofuscado

### 8. Anti-debugging

Técnicas para detectar se o malware está sendo depurado.

### 9. Anti-disassembly

Técnicas para gerar desmontagem incorreta ou confusa, atrasando a engenharia reversa.

Exemplos mencionados:

- opcode obfuscation
- API obfuscation
- junk/spaghetti code
- control flow flattening

### 10. Anti-VM

O malware tenta descobrir se está rodando em máquina virtual, porque analistas costumam usar VMs.

Exemplos citados:

- checagem de MAC address
- CPUID
- Red Pill
- checagem de processos, registro, arquivos ou serviços típicos da VM

## Resumo para revisão rápida

- cadeia de custódia = histórico da evidência
- evidência original deve ser preservada; análise ocorre na cópia
- MACTimes = mtime, atime, ctime
- ctime **não** é creation time
- ordem de volatilidade: RAM/processos/rede vêm antes do disco
- data carving recupera por assinatura
- cabeçalho de e-mail é essencial
- laudo precisa ser técnico e compreensível
- anti-forense tenta dificultar a investigação

## Perguntas que podem cair em prova

1. O que é cadeia de custódia e por que ela é importante?
2. Diferencie mtime, atime e ctime.
3. O que é ordem de volatilidade e como ela afeta a coleta?
4. Quando usar data carving? Quais são seus pressupostos?
5. Qual a diferença entre análise de vulnerabilidade e teste de penetração?
6. Por que encaminhar um e-mail pode prejudicar a análise forense?
7. O que caracteriza uma técnica anti-forense?
8. O que deve constar em um laudo pericial?
