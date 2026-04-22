# Forense de E-mail e Uso Indevido de Correio Institucional

______________________________________________________________________

**Date:** 2026-04-22
**Tags:** [Forense](../tags/Forense.md), [Uf](../tags/Uf.md), [Email](../tags/Email.md), [Legislacao](../tags/Legislacao.md)
**URL:**

- https://www.rfc-editor.org/rfc/rfc5322
- https://www.planalto.gov.br/ccivil_03/_ato2011-2014/2014/lei/l12965.htm

______________________________________________________________________

## O que é forense de e-mail?

Forense de e-mail é a análise técnica de mensagens, cabeçalhos, anexos, logs e artefatos relacionados ao correio eletrônico para responder perguntas como:

- quem enviou a mensagem?
- de onde ela saiu?
- quando foi enviada?
- quais destinatários receberam?
- houve envio em massa, spam, fraude ou assédio?
- a conta foi usada pelo titular ou comprometida por terceiro?

Esse tema é muito importante porque e-mail institucional costuma ser:

- canal oficial de comunicação
- meio de prova em processos administrativos
- recurso computacional sujeito a política interna

## Estrutura básica de um e-mail

Para análise forense, o e-mail tem dois blocos principais:

- **header**: metadados da mensagem
- **body**: conteúdo, links e anexos

### Campos do cabeçalho que normalmente interessam

- `From`
- `To`
- `Cc`
- `Date`
- `Message-ID`
- `Received`
- `Return-Path`
- resultados de SPF, DKIM e DMARC

### Ponto central

O texto da mensagem sozinho raramente basta. O cabeçalho é o que ajuda a reconstruir a rota e a autenticidade do envio.

## Caso-base para estudo

O cenário fornecido envolve suspeita de uso indevido de e-mail institucional para:

- propaganda política
- spam/correntes
- assédio moral

Esse tipo de caso é ótimo para prova porque mistura:

- evidência digital
- política institucional
- privacidade e sigilo
- cadeia de custódia
- elaboração de relatório técnico

## Quais evidências digitais são relevantes?

Uma boa resposta de prova é separar as evidências por categoria.

## 1. Evidências do servidor de e-mail

- logs SMTP/IMAP/POP/Exchange/Postfix
- registros de autenticação
- data e hora de envio
- IP de origem do login ou do envio
- identificador da mensagem (`Message-ID`)
- fila de mensagens e trilhas de entrega
- quantidade de destinatários por mensagem
- tentativas de envio em massa

### O que isso ajuda a provar?

- se o e-mail realmente saiu da conta investigada
- se houve envio em lote
- se o envio partiu da rede da instituição ou de acesso remoto

## 2. Evidências da própria mensagem

- mensagem original em formato bruto
- cabeçalho completo
- corpo da mensagem
- anexos
- assunto
- lista de destinatários

### Muito importante

Se possível, a coleta deve preservar a **mensagem original**, não apenas uma cópia encaminhada.

## 3. Evidências do cliente de e-mail e da estação de trabalho

- arquivos PST/OST/MBOX
- cache do webmail
- histórico de navegação
- arquivos temporários de anexos
- artefatos do Outlook ou Thunderbird
- documentos usados para compor a mensagem
- imagem forense do computador

### Exemplo

Mesmo que o usuário apague um e-mail da caixa “Enviados”, fragmentos podem continuar em OST/PST, cache, arquivos temporários ou imagem do disco.

## 4. Evidências de rede e infraestrutura

- logs de proxy
- logs de VPN
- firewall
- DHCP
- logs de autenticação em domínio
- SIEM/correlação centralizada

### Exemplo

Se o envio ocorreu às 09:14, pode ser possível cruzar:

- login do usuário no e-mail
- IP concedido por DHCP
- máquina autenticada no domínio
- estação em uso naquele horário

## 5. Evidências contextuais e administrativas

- denúncias recebidas
- mensagens reportadas pelas vítimas
- política de uso do e-mail institucional
- resolução interna aplicável
- registros de treinamento ou ciência do usuário sobre as regras

### Por que isso importa?

Porque a análise técnica precisa ser conectada à **violação normativa**.

## Resposta curta para a pergunta “Que tipos de evidência poderiam ser coletados?”

> Logs de acesso e autenticação do servidor de e-mail, mensagens originais com cabeçalho completo, anexos, trilhas de entrega, caixa postal do suspeito, arquivos PST/OST, imagem forense do computador, artefatos do cliente de e-mail, logs de rede e documentos administrativos que provem a política violada.

## Quais informações a equipe deve buscar para o relatório?

O relatório precisa responder não só “há e-mail impróprio?”, mas principalmente **como essa conclusão foi alcançada**.

### Informações essenciais

- identificação do caso
- escopo da investigação
- quais mídias/dados foram coletados
- hashes e forma de preservação
- período analisado
- conta(s) envolvida(s)
- datas e horários relevantes
- IPs de origem
- origem do envio (máquina, rede, VPN, webmail etc.)
- quantidade de mensagens enviadas
- destinatários impactados
- conteúdo relevante das mensagens
- evidências de envio em massa ou repetição
- indícios de autoria ou de comprometimento da conta
- política institucional possivelmente violada
- limitações da análise

## O que, de fato, precisa ser comprovado?

Em um caso desses, a investigação normalmente tenta responder:

1. a conta institucional foi usada?
2. as mensagens realmente foram enviadas?
3. qual o conteúdo das mensagens?
4. quem operou a conta naquele momento?
5. houve dolo, negligência ou comprometimento da conta?
6. houve violação da norma institucional?

## Exemplo de correlação de evidências

Imagine uma mensagem com propaganda política enviada para 300 destinatários.

Uma correlação forte seria:

- cabeçalho mostra `Message-ID` e data
- log do servidor mostra o envio naquele mesmo horário
- log de autenticação mostra login da conta suspeita
- DHCP/VPN aponta IP usado
- workstation image contém rascunho ou arquivo anexado

Quando várias fontes independentes convergem, a conclusão fica muito mais robusta.

## Encaminhar ou exportar e-mail pode atrapalhar a análise

Esse é um ponto clássico de prova.

### Problemas comuns

- encaminhamento normal gera novos cabeçalhos
- exportação por alguns clientes pode manter apenas parte dos metadados
- copiar/colar o texto perde informações críticas

### Melhor prática

Coletar:

- cabeçalho completo
- mensagem raw/original
- logs do servidor
- caixa original ou exportação forense da caixa

## Ferramentas forenses que podem ser usadas

Uma resposta boa mistura ferramentas de **aquisição**, **análise de disco**, **análise de memória** e **análise de logs**.

### Para aquisição e preservação

- **FTK Imager**: criação de imagens e exportação de artefatos
- **dd / dcfldd**: aquisição bruta em Linux
- **sha256sum**: validação por hash

### Para análise do computador

- **Autopsy / The Sleuth Kit**: análise de imagem de disco, timeline, recuperação de artefatos
- **EnCase** ou **X-Ways Forensics**: suites comerciais de análise forense
- **Magnet AXIOM**: correlação de artefatos de sistema e aplicativos

### Para memória volátil, se a máquina estiver ligada

- **Volatility**: análise de RAM, processos e conexões

### Para logs de servidor e correlação

- **ELK/Elastic**, **Graylog** ou **Splunk**: busca e correlação de logs
- ferramentas nativas do servidor de e-mail, como trilhas de mensagem do Exchange ou logs do Postfix

### Para análise de cabeçalhos

- leitura manual do header
- validadores e analisadores de cabeçalhos
- checagem de SPF, DKIM, DMARC e trilhas `Received`

## A coleta de dados da UFRN exige mandado judicial?

Essa pergunta exige cuidado. Para prova, o melhor é diferenciar **investigação administrativa interna** de **produção de prova para persecução penal**.

## Resposta segura de estudo

### Em investigação administrativa interna

Quando os dados estão em **servidores institucionais da própria UFRN** e o recurso é institucional, a coleta de logs e registros administrativos tende a ser juridicamente mais viável **sem mandado judicial**, desde que haja:

- finalidade legítima
- base normativa interna
- respeito à proporcionalidade
- acesso restrito ao necessário
- documentação da cadeia de custódia

### Porém, existe uma distinção importante

- **logs e metadados** costumam ter tratamento menos sensível que o conteúdo integral das comunicações
- **conteúdo de e-mails** é mais delicado e exige fundamentação muito mais cuidadosa

### Quando o problema aumenta?

- quando a investigação sai do âmbito administrativo e entra no criminal
- quando será necessário compartilhar dados com autoridades externas
- quando os dados estão com provedor terceiro
- quando há forte expectativa de privacidade e não existe base normativa clara

### Leis relevantes para mencionar

- **Marco Civil da Internet**: guarda e proteção de registros, dados pessoais e comunicações
- **Constituição**: proteção da intimidade e do sigilo das comunicações
- **LGPD**: necessidade, finalidade e minimização de dados
- **norma interna da instituição**: delimita uso institucional e eventual fiscalização

### Formulação boa para prova

> Em ambiente institucional próprio, a coleta de logs para apuração administrativa pode não exigir mandado judicial, mas o acesso ao conteúdo das comunicações exige maior cautela, base legal adequada, proporcionalidade e, em certos contextos, controle judicial ou requisição formal específica.

## Como a legislação brasileira se aplica ao caso?

Ela se aplica em camadas.

### 1. Norma interna

A resolução institucional define o uso permitido do e-mail e as sanções administrativas.

### 2. Marco Civil da Internet

Ajuda a enquadrar guarda de registros e proteção de dados/comunicações.

### 3. Direito administrativo e disciplinar

A apuração interna precisa respeitar rito, motivação e ampla documentação.

### 4. Direito penal e processual penal

Se houver crime, a prova pode ser encaminhada a autoridades competentes, observando os requisitos legais da persecução penal.

## Impacto institucional do uso indevido de e-mail

Essa é uma pergunta conceitual importante.

### Possíveis impactos

- desgaste da reputação da instituição
- perda de confiança na comunicação oficial
- aumento de denúncias e conflito interno
- risco jurídico e disciplinar
- risco de bloqueio reputacional do domínio por spam
- percepção de fragilidade na governança de TI

### Exemplo

Se servidores e alunos deixam de confiar em e-mails institucionais, mensagens legítimas podem passar a ser ignoradas, o que afeta a operação administrativa.

## Estrutura recomendada de relatório técnico

Um relatório bem feito deve conter:

1. identificação do caso
2. objetivo da análise
3. base normativa considerada
4. fontes de evidência
5. cadeia de custódia
6. metodologia e ferramentas utilizadas
7. achados técnicos
8. timeline dos eventos
9. conclusão
10. anexos, hashes e amostras de cabeçalhos/logs

## Exemplo de conclusão objetiva

> Os artefatos analisados indicam que a conta institucional X realizou autenticação a partir do IP Y no horário Z e enviou N mensagens para múltiplos destinatários, contendo material de propaganda política e conteúdo ofensivo, em desacordo com a norma institucional. Não foram encontrados sinais técnicos suficientes de comprometimento externo da conta no intervalo analisado.

## Resumo para revisão rápida

- forense de e-mail depende muito de cabeçalhos e logs
- mensagem encaminhada pode perder metadados importantes
- o relatório deve provar autoria, tempo, origem, volume e conteúdo relevante
- ferramentas de disco, memória e logs podem ser combinadas
- em ambiente institucional, diferenciar **logs/metadados** de **conteúdo** é essencial na análise jurídica
- uso indevido de e-mail institucional afeta reputação, confiança e governança

## Perguntas que podem cair em prova

1. Quais evidências são mais importantes em um caso de uso indevido de e-mail?
2. Por que o cabeçalho é tão valioso na análise forense?
3. Que informações mínimas devem entrar no relatório técnico?
4. Qual a diferença entre analisar logs do servidor e analisar o conteúdo da mensagem?
5. Quais ferramentas podem ser usadas para logs, imagem de disco e memória?
6. Em investigação administrativa interna, o acesso a dados institucionais exige mandado judicial em toda hipótese?
7. Como o uso indevido do e-mail institucional impacta a organização?
