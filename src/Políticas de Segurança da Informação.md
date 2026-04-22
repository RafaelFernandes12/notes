# Políticas de Segurança da Informação

______________________________________________________________________

**Date:** 2026-04-22
**Tags:** [Forense](../tags/Forense.md), [Uf](../tags/Uf.md), [Security](../tags/Security.md), [PoliticasDeSeguranca](../tags/PoliticasDeSeguranca.md)
**URL:**

- https://www.iso.org/isoiec-27001-information-security.html
- https://www.sans.org/information-security-policy/
- https://www.nist.gov/cyberframework

______________________________________________________________________

## O que é uma política de segurança da informação?

É um conjunto de princípios, regras e diretrizes que orientam a gestão da segurança da informação em uma organização.

Ela deve ser observada por:

- corpo técnico
- gestão
- usuários internos
- usuários externos, quando aplicável

### Definição boa para prova

> Política de segurança da informação é o documento que estabelece as linhas mestras para proteção dos recursos computacionais e das informações de uma instituição.

## O que a política busca garantir?

O PDF destaca quatro propriedades fundamentais:

- **integridade**
- **confidencialidade**
- **disponibilidade**
- **autenticidade**

### Como entender cada uma

- **integridade**: a informação não pode ser alterada indevidamente
- **confidencialidade**: só acessa quem deve acessar
- **disponibilidade**: a informação deve estar acessível quando necessária
- **autenticidade**: deve ser possível confiar na origem/identidade relacionada à informação

### Exemplo rápido

Uma folha de pagamento:

- não pode ser alterada por qualquer pessoa → integridade
- não deve ficar pública → confidencialidade
- deve estar acessível no fechamento mensal → disponibilidade
- precisa ser emitida por fonte legítima → autenticidade

## O que a alta administração deve fazer?

Segundo o material e o alinhamento com a ISO 27001, a alta administração deve estabelecer uma política que:

- seja apropriada ao propósito da organização
- inclua objetivos de segurança ou base para defini-los
- inclua compromisso com requisitos aplicáveis
- inclua compromisso com melhoria contínua

Além disso, a política deve:

- estar documentada
- ser comunicada internamente
- estar disponível às partes interessadas relevantes

### Ideia para prova

Política sem apoio da alta administração tende a virar documento decorativo.

## ISO 27001 e objetivos de segurança

O slide também resume requisitos clássicos da ISO 27001 para objetivos de segurança.

### Eles devem ser

- consistentes com a política
- mensuráveis, quando possível
- alinhados a requisitos e avaliação de riscos
- monitorados
- comunicados
- atualizados quando necessário
- documentados

### A organização também deve definir

- o que será feito
- quais recursos serão usados
- quem será responsável
- quando será concluído
- como o resultado será avaliado

### Exemplo prático

Objetivo ruim:

> Melhorar a segurança do e-mail.

Objetivo melhor:

> Reduzir em 80% o número de contas sem MFA em até 6 meses, com acompanhamento mensal pelo time de infraestrutura.

## Por que a informação é tão importante?

Porque informação é um ativo crítico da organização.

O PDF enfatiza que, hoje, ela pode ser considerada um dos recursos patrimoniais mais relevantes.

### Exemplos de ativos informacionais

- dados financeiros
- dados acadêmicos
- contratos
- chaves criptográficas
- logs
- propriedade intelectual

## Quem deve elaborar a política?

O material destaca que o ideal é haver uma área responsável, mas a elaboração não deve ficar isolada.

### Devem participar

- áreas críticas da instituição
- jurídico
- auditoria
- gestão de TI/segurança
- alta administração, na aprovação final

### Ponto-chave

A política precisa ser aprovada pelo mais alto escalão.

## O que a política deve abordar?

O PDF lista vários tópicos. Vale muito transformar isso em checklist.

### Checklist de conteúdo

- definição de segurança da informação e sua importância
- compromisso da alta administração
- objetivos de segurança
- responsabilidades gerais
- orientação sobre análise e gestão de riscos
- princípios de conformidade dos sistemas com a política
- padrões mínimos de qualidade dos sistemas
- políticas de controle de acesso
- classificação da informação
- prevenção e detecção de malware
- princípios legais ligados à TI
- supervisão de tentativas de violação
- consequências para violações
- gestão da continuidade do negócio
- plano de treinamento em segurança

## O que uma política pode proteger?

O material dá exemplos diretos:

- aplicativos
- arquivos de dados
- sistema operacional
- senhas
- logs

### Interpretação importante

A política não protege só “documentos sensíveis”. Ela também cobre infraestrutura e mecanismos de controle.

## Etapas importantes para construir a política

## 1. Avaliação das informações

É a fase de entender o valor da informação para a organização.

### Perguntas úteis

- o que se deseja proteger?
- de quem ou do quê?
- quem deve acessar?
- quais são as políticas internas?
- qual é a missão da organização?

### Exemplo

Se uma base de alunos é essencial ao funcionamento da instituição, isso influencia:

- prioridade de backup
- controles de acesso
- criptografia
- plano de recuperação

## 2. Classificação da informação

O PDF usa a classificação:

- pública
- interna
- confidencial
- secreta

### Exemplo didático

| Classificação | Exemplo |
|---|---|
| Pública | calendário acadêmico publicado |
| Interna | comunicados de trabalho para servidores |
| Confidencial | folha de pagamento, dados disciplinares |
| Secreta | chaves privadas, material altamente restrito |

### O que costuma cair

Classificação não é enfeite. Ela orienta acesso, backup, retenção, compartilhamento e punições.

## 3. Regras de acesso e responsabilidade

A política deve deixar claro:

- quem pode acessar a informação
- quando pode acessar
- como pode acessar
- quem recupera em caso de perda
- em quanto tempo a recuperação deve ocorrer

### Exemplo

Somente RH acessa folhas individuais de pagamento; backup é recuperado pela equipe de infraestrutura mediante solicitação formal; prazo máximo de restauração: 4 horas.

## 4. Treinamento

Todos devem ser informados sobre:

- importância da gestão da informação
- uso correto de equipamentos
- atualização de sistemas
- detecção de problemas
- forma de registrar incidentes

### Ponto importante

Sem treinamento, é injusto cobrar conformidade plena do usuário.

## 5. Punições

A política deve prever consequências para descumprimento.

### O grau da punição deve considerar

- valor da informação
- prejuízo causado
- intenção ou negligência
- clareza prévia das regras

### Regra de justiça organizacional

Para punir corretamente, a organização precisa ter regras claras e conhecidas.

## Violação da política

O material lembra que a própria política deve prever procedimentos de resposta a violações.

Também cita referências legais como:

- art. 313-A do Código Penal: inserção de dados falsos em sistema
- art. 313-B do Código Penal: modificação/alteração não autorizada
- art. 153 e art. 325 em contextos relacionados a sigilo e fornecimento indevido de acesso/senha

### Ideia para prova

Política de segurança não existe isolada; ela conversa com consequências administrativas, civis e penais.

## Tipos de políticas

O PDF destaca dois níveis.

## 1. Programa de política organizacional

Diretrizes da diretoria para estabelecer o programa de segurança, seus objetivos e responsabilidades.

## 2. Políticas de sistema

Regras específicas para proteger redes, máquinas, softwares ou ambientes concretos.

### Exemplo

- política organizacional: “todo dado institucional deve ser classificado”
- política de sistema: “roteadores da rede de produção devem usar AAA/TACACS+”

## Implementação: política, padrão, diretriz e procedimento

Essa distinção costuma cair bastante.

| Termo | Ideia central | Exemplo |
|---|---|---|
| **Política** | regra macro e princípio | todo acesso remoto deve ser protegido |
| **Padrão** | forma obrigatória e uniforme de implementar | todos os notebooks usarão Windows versão X e MFA |
| **Diretriz** | recomendação quando não dá para padronizar tudo | preferir MFA por aplicativo autenticador |
| **Procedimento** | passo a passo operacional | como criar conta de e-mail e habilitar MFA |

### Exemplo mais concreto

- **política**: senhas fortes são obrigatórias
- **padrão**: mínimo de 12 caracteres, bloqueio após 5 tentativas
- **diretriz**: preferir passphrases
- **procedimento**: tutorial para trocar a senha no sistema

## Componentes de uma política

O material destaca quatro blocos essenciais.

### 1. Objetivo

Por que a política existe?

### 2. Escopo

Aplica-se à organização inteira ou a parte dela?

### 3. Responsabilidades

Quem faz o quê? Qual a estrutura formal?

### 4. Conformidade

Como fiscalizar? O que acontece se alguém descumprir?

### Exemplo simples

- objetivo: proteger dados acadêmicos e administrativos
- escopo: todos os sistemas institucionais
- responsabilidades: STI, chefias, usuários
- conformidade: auditorias, logs, sanções disciplinares

## Exemplo do SANS para roteadores

O PDF mostra um exemplo útil de política específica para roteadores.

### Ideias presentes no exemplo

- uso de AAA/TACACS+ em vez de contas locais
- criptografia da senha de enable
- desabilitação de serviços inseguros
- desabilitação de source routing
- regras de acesso definidas por necessidade

### O que isso ensina?

Uma política específica precisa ser suficientemente concreta para orientar configuração real.

## Boas práticas destacadas no material

### 1. Criptografia de dados

Dados armazenados e comunicações remotas devem ser criptografados.

### 2. Certificados digitais

Salvar certificados em dispositivos apropriados e usar autoridades confiáveis.

### 3. DLP e auditoria

Monitorar, alertar e bloquear perda de dados dentro e fora da rede.

### 4. Política rigorosa de mídia removível

Restringir:

- pendrives
- discos externos
- gravadores externos
- mídias graváveis em geral

### 5. Filtro de spam

Especialmente importante em servidores de e-mail.

### 6. Patches de segurança atualizados

Manter sistemas atualizados reduz superfície de ataque.

### 7. Educação

Treinamento contínuo faz parte da própria segurança.

## Como isso conversa com forense?

Política de segurança é importante para a forense porque:

- define o que é uso aceitável
- ajuda a classificar incidentes como violação
- determina logs e controles que poderão virar evidência
- fundamenta sanções administrativas
- melhora a rastreabilidade dos eventos

### Exemplo

Sem política clara de uso de e-mail institucional, fica muito mais difícil sustentar que certo comportamento foi realmente uma violação de regra previamente conhecida.

## Resumo para revisão rápida

- PSI é o conjunto de diretrizes mestras da segurança da informação
- protege integridade, confidencialidade, disponibilidade e autenticidade
- deve ter apoio e aprovação da alta administração
- precisa ser documentada, comunicada e revisada
- deve incluir classificação, acesso, riscos, treinamento e sanções
- política, padrão, diretriz e procedimento não são sinônimos
- boas práticas incluem criptografia, DLP, restrição de mídia removível e educação

## Perguntas que podem cair em prova

1. O que é uma política de segurança da informação?
2. Quais propriedades de segurança a política busca garantir?
3. Qual o papel da alta administração segundo a ISO 27001?
4. Quem deve participar da elaboração da política?
5. O que diferencia política, padrão, diretriz e procedimento?
6. Quais etapas são importantes na construção de uma PSI?
7. Por que treinamento e punição precisam estar previstos?
8. Como a PSI ajuda a investigação forense?
