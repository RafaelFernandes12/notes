# Aquisição de Dados e Imagens Forenses

______________________________________________________________________

**Date:** 2026-04-22
**Tags:** [Forense](../tags/Forense.md), [Uf](../tags/Uf.md), [AquisicaoDeDados](../tags/AquisicaoDeDados.md), [Linux](../tags/Linux.md)
**URL:**

- https://www.gnu.org/software/coreutils/manual/html_node/dd-invocation.html
- https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-86.pdf
- https://dcfldd.sourceforge.net/

______________________________________________________________________

## O que é aquisição de dados?

Aquisição é o processo de criar uma **cópia forense** da evidência digital para análise, preservando ao máximo a integridade da mídia original.

Em prova, a ideia central é:

> aquisição é a fase em que a evidência é copiada de forma controlada, documentada e validada, para que a análise ocorra sobre a cópia e não sobre a mídia original.

## Aquisição estática x aquisição ao vivo

Essa comparação é muito importante.

| Tipo | O que é | Vantagem principal | Limitação principal |
|---|---|---|---|
| **Estática** | cópia de disco de sistema desligado | não altera os dados e é repetível | pode perder dados voláteis e dificultar casos com disco criptografado |
| **Ao vivo** | cópia de dados com sistema em execução | permite capturar RAM e estado descriptografado | altera o sistema e não é repetível exatamente |

### O que o PDF destaca?

- aquisição estática foi o padrão por muito tempo
- aquisição ao vivo passou a ser mais importante por causa da **criptografia de disco**
- coleta de RAM ganhou relevância

### Pegadinha de prova

A RAM normalmente **não tem timestamps nativos como um sistema de arquivos**, o que dificulta contextualização temporal.

## Termos equivalentes para a imagem forense

O PDF lembra que vários nomes significam praticamente a mesma ideia:

- cópia bit a bit
- imagem bit a bit
- imagem
- espelho
- cópia de setor

### O que todos querem dizer?

Uma representação fiel do conteúdo da mídia, setor por setor, e não só dos arquivos “visíveis”.

## Formatos de armazenamento da evidência

O material separa três grandes formatos.

## 1. Formato raw

É o formato clássico produzido por ferramentas como `dd`.

### Características

- cópia bit a bit simples
- sem metadados embutidos sofisticados
- grande compatibilidade com ferramentas forenses

### Vantagens

- transferência rápida
- ampla compatibilidade
- simplicidade

### Desvantagens

- ocupa espaço semelhante ao disco original
- pode ter limitação na coleta de setores problemáticos
- validação costuma ficar em arquivo separado

### Hashes citados no PDF

- MD5
- SHA-1 ou mais recente
- CRC-32

### Exemplo conceitual

Um disco de 500 GB tende a gerar uma imagem raw próxima de 500 GB, mesmo que poucos arquivos estejam “em uso”.

## 2. Formatos proprietários

São formatos mantidos por fabricantes ou suites forenses específicas.

### Recursos comuns

- compressão opcional
- segmentação em múltiplos arquivos
- checagem de integridade por segmento
- inclusão de metadados no próprio arquivo

### Metadados que podem ser embutidos

- hashes
- data/hora da aquisição
- nome do investigador
- nome do caso
- comentários

### Desvantagem principal

Menor interoperabilidade entre ferramentas.

### Exemplo importante

O PDF destaca o formato **Expert Witness** como padrão não oficial, com arquivos como:

- `.E01`
- `.E02`
- `.E03`

## 3. AFF - Advanced Forensic Format

Formato aberto voltado a imagens forenses.

### Objetivos de design citados

- suportar imagens compactadas ou não
- não impor restrição forte ao tamanho da imagem
- reservar espaço para metadados
- manter design simples e extensível
- ser aberto e multiplataforma

### Ponto importante

O AFF tenta combinar vantagens de formatos ricos em metadados sem prender completamente o investigador a um ecossistema fechado.

## Comparação rápida entre formatos

| Formato | Compatibilidade | Metadados | Compressão | Observação |
|---|---|---|---|---|
| Raw | muito alta | baixa | não nativa | simples e amplamente aceito |
| Proprietário | depende da ferramenta | alta | geralmente sim | mais recursos, menos portabilidade |
| AFF | boa | alta | sim | aberto e extensível |

## Métodos de aquisição

O PDF destaca quatro métodos principais.

## 1. Bit-stream disk-to-image file

É o método mais comum.

### O que faz?

Lê o disco de origem e grava uma imagem em arquivo.

### Vantagens

- permite criar mais de uma cópia
- facilita transporte e armazenamento
- preserva estrutura bit a bit

### Exemplo de uso

Investigar um notebook suspeito e gerar `caso01.E01` ou `caso01.raw` em um disco de evidência.

## 2. Bit-stream disk-to-disk

É usado quando a cópia para arquivo não é viável por erro, limitação de hardware/software ou compatibilidade com discos antigos.

### Ideia central

O disco alvo precisa espelhar a geometria necessária para receber a cópia da mídia suspeita.

### Quando lembrar disso?

Em prova, esse método aparece como alternativa para casos com mídias antigas ou situações em que a imagem em arquivo não é possível.

## 3. Aquisição lógica

Captura apenas arquivos específicos de interesse.

### Exemplo do PDF

- arquivos `.pst` ou `.ost` do Outlook

### Quando faz sentido?

- tempo limitado
- disco muito grande
- foco claro em determinados artefatos

### Limitação

Não substitui uma imagem completa quando a preservação integral da mídia é necessária.

## 4. Aquisição esparsa (sparse)

Coleta apenas partes relevantes da mídia, incluindo também determinados arquivos excluídos.

### Quando usar?

Quando o volume de dados é enorme e o caso exige foco em porções específicas.

## Como escolher o melhor método?

Uma boa resposta de prova depende de contexto.

### Exemplos

#### Caso 1: notebook corporativo comum, sem urgência

Melhor escolha: **bit-stream disk-to-image**.

#### Caso 2: servidor ligado com disco criptografado

Melhor escolha: **aquisição ao vivo**, possivelmente combinada com RAM.

#### Caso 3: disco gigante e prazo muito curto, foco em e-mails

Melhor escolha: **aquisição lógica** dos arquivos de correio.

#### Caso 4: RAID enorme e foco apenas em dados específicos

Melhor escolha: **aquisição esparsa ou lógica**, se juridicamente e tecnicamente adequada.

## Compactação de imagens

O PDF destaca a **compressão sem perdas**, que pode reduzir o tamanho da imagem em 50% ou mais, dependendo do conteúdo.

### Mas cuidado

- arquivos já compactados comprimem pouco
- JPEG já usa compactação com perdas, então não “ganha muito” em nova compressão

### Regra importante

Compressão nunca substitui a validação por hash.

## Backup em fita

É apresentado como alternativa para aquisições muito grandes.

### Vantagem

- praticamente sem limite prático de tamanho, desde que haja várias fitas

### Desvantagem

- é lento

## Devolução da unidade original

Em litígios civis ou contextos específicos, pode haver obrigação de devolver o disco original após a imagem.

### Consequência prática

Se você só terá uma chance, precisa escolher corretamente entre:

- imagem bitstream completa
- cópia lógica
- outro método mais apropriado

## Planejamento de contingência

O PDF recomenda pensar em falhas antes que elas ocorram.

### Boas práticas

- fazer cópia duplicada do arquivo de imagem
- produzir pelo menos duas imagens quando possível
- usar ferramentas ou técnicas diferentes
- copiar também área protegida do host, quando relevante
- estar preparado para discos criptografados

### Ideia importante

Contingência não é luxo; é proteção contra corrupção da imagem, falha de ferramenta ou questionamento futuro.

## Discos rígidos criptografados

O material cita BitLocker e TrueCrypt.

### Regra principal

Se a máquina estiver ligada e o volume estiver montado, a aquisição ao vivo pode capturar o disco em estado descriptografado.

### Se a máquina estiver desligada

Você normalmente precisará de:

- chave
- senha
- ou técnica especializada adicional

### O que isso ensina para prova?

Criptografia muda a estratégia de aquisição. Nem sempre desligar a máquina é a melhor primeira decisão.

## Proteção contra gravação

O objetivo do write blocking é impedir que o sistema do examinador grave algo na mídia suspeita.

### Risco evitado

- atualização de metadados
- alteração de journal
- criação de arquivos automáticos
- modificação de timestamps

## Aquisição com Linux Live CD

O PDF destaca que sistemas modernos tendem a montar discos automaticamente, o que pode alterar a evidência.

### Problemas citados

- Windows pode escrever em áreas do sistema logo ao iniciar
- Linux moderno também pode registrar metadados ao montar partições

### Por que Live CDs forenses ajudam?

Porque são preparados para:

- não montar a mídia automaticamente
- ou montá-la como somente leitura

### Exemplos citados

- Helix
- Penguin Sleuth
- FCCU

## `dd` no Linux

O comando `dd` é o exemplo clássico de aquisição raw.

### Exemplo didático

```bash
dd if=/dev/sdX of=/evidence/caso01.raw bs=4M conv=noerror,sync status=progress
```

### Como ler esse comando

- `if=`: disco de origem
- `of=`: arquivo de saída
- `bs=4M`: bloco de leitura/escrita
- `conv=noerror,sync`: tenta seguir mesmo com erro e manter alinhamento

### Limitações do `dd`

- exige mais conhecimento técnico
- não comprime
- não foi criado originalmente como ferramenta forense especializada

## `dcfldd` no Linux

O `dcfldd` estende a ideia do `dd` com recursos mais úteis à forense.

### Funções citadas no PDF

- logging de erros
- múltiplos hashes
- exibição de progresso
- segmentação em volumes menores
- verificação com a mídia original

### Exemplo didático

```bash
dcfldd if=/dev/sdX of=/evidence/caso01.img hash=sha256 hashlog=/evidence/caso01.sha256
```

### Vantagem sobre o `dd`

Menos trabalho manual para validar e documentar a aquisição.

## Validação da aquisição

Essa é uma das partes mais críticas do processo.

### Objetivo

Demonstrar que a imagem adquirida corresponde à evidência coletada.

### Técnicas citadas

- CRC-32
- MD5
- SHA-1 até SHA-512

### Observação importante

- MD5 tem colisões conhecidas
- SHA-1 também tem fragilidades
- mesmo assim, ambos ainda aparecem em materiais e ferramentas forenses por compatibilidade histórica

### Resposta de prova segura

Use o hash para verificar integridade, preferindo algoritmos mais fortes quando possível.

## Validação no Linux

### Com `dd`

```bash
sha256sum /evidence/caso01.raw > /evidence/caso01.raw.sha256
```

ou, historicamente:

```bash
md5sum /evidence/caso01.raw > /evidence/caso01.raw.md5
```

### Com `dcfldd`

O próprio processo pode gerar hashlog e comparar imagem com a origem, reduzindo etapas manuais.

## Validação no Windows

O PDF enfatiza que o Windows, historicamente, não trazia ferramentas forenses nativas de hashing equivalentes às do ambiente Linux. Por isso, costumam ser usados:

- utilitários de terceiros
- recursos internos das suites forenses comerciais

### Ponto importante

Em imagem raw, a validação manual separada é especialmente recomendada.

## Aquisição de dados RAID

RAID aparece muito em prova porque mistura armazenamento grande com dificuldade de reconstrução.

## Conceito de RAID

RAID = arranjo de múltiplos discos para ganho de desempenho, redundância ou ambos.

### RAID 0

- dados distribuídos entre discos
- maior desempenho e capacidade combinada
- **não tem redundância**

### RAID 1

- espelhamento
- foco em redundância/recuperação
- custo maior em armazenamento

### RAID 5

- distribuição de dados com paridade
- combina desempenho com tolerância a falha

## Preocupações ao adquirir RAID

- quanto armazenamento será necessário?
- qual tipo de RAID está em uso?
- a ferramenta consegue reconstruir a imagem?
- a ferramenta entende os discos separados?
- trata-se de RAID antigo de hardware/firmware?

### Regra prática

Se o RAID for grande demais, pode ser mais realista coletar só os dados relevantes com aquisição lógica ou esparsa, desde que isso atenda o caso.

## Aquisição remota pela rede

É a coleta feita acessando um computador suspeito remotamente.

### Vantagem

- pode permitir resposta rápida sem deslocamento físico imediato

### Desvantagens citadas

- velocidade da rede
- conflitos de roteamento
- necessidade de permissões em sub-redes seguras
- tráfego intenso causando erro ou atraso
- antivírus bloqueando o agente remoto

## Ferramentas remotas citadas

### ProDiscover Investigator

Permite:

- ver remotamente a unidade em uso
- fazer aquisição ao vivo
- criptografar a conexão
- copiar RAM
- usar modo furtivo

### ProDiscover Incident Response

Além do anterior, pode:

- capturar estado volátil
- analisar processos em execução
- localizar arquivos/processos ocultos
- observar portas IP
- comparar hashes para localizar trojans/rootkits

### EnCase Enterprise

Permite:

- aquisição remota de mídia e RAM
- integração com IDS
- imagem de um ou mais sistemas
- suporte a vários sistemas de arquivos
- suporte a RAID

## Resumo para revisão rápida

- aquisição estática é repetível; aquisição ao vivo é mais útil em cenários com criptografia e RAM
- raw é simples e compatível, mas pobre em metadados
- formatos proprietários e AFF trazem mais recursos
- métodos principais: disk-to-image, disk-to-disk, lógica e esparsa
- toda aquisição deve ser validada por hash
- Live CDs forenses evitam automount e alteração da evidência
- RAID exige conhecer a arquitetura antes da coleta
- aquisição remota é útil, mas traz riscos de rede e alteração do sistema

## Perguntas que podem cair em prova

1. Diferencie aquisição estática e aquisição ao vivo.
2. Quais são as vantagens e desvantagens do formato raw?
3. Quando usar aquisição lógica e quando usar aquisição esparsa?
4. Por que a validação por hash é indispensável?
5. Quais recursos tornam o `dcfldd` mais interessante que o `dd` para fins forenses?
6. O que muda na estratégia quando o disco está criptografado?
7. Quais são os desafios de adquirir um RAID?
8. Quais são os riscos da aquisição remota pela rede?
