# Legislação Aplicada à Forense Computacional

______________________________________________________________________

**Date:** 2026-04-22
**Tags:** [Forense](../tags/Forense.md), [Uf](../tags/Uf.md), [Legislacao](../tags/Legislacao.md), [Security](../tags/Security.md)
**URL:**

- https://www.planalto.gov.br/ccivil_03/_ato2011-2014/2014/lei/l12965.htm
- https://www.coe.int/en/web/cybercrime/the-budapest-convention
- https://www.planalto.gov.br/ccivil_03/decreto-lei/del2848compilado.htm

______________________________________________________________________

## Por que legislação importa na forense?

Na computação forense, **não basta encontrar a evidência**. Também é preciso demonstrar que a obtenção, preservação e uso dessa evidência respeitaram a legislação aplicável.

Em prova, a ideia central é:

> Evidência tecnicamente correta, mas juridicamente irregular, pode perder valor ou gerar questionamentos sobre sua admissibilidade.

## Jurisdição brasileira

### Soberania, jurisdição e territorialidade

O slide destaca o **Princípio da Territorialidade** do Código Penal: a lei penal brasileira se aplica, em regra, aos crimes cometidos no território nacional.

### Ponto importante no contexto digital

No mundo atual, o conceito de território precisa ser lido também à luz do **espaço virtual**, principalmente quando o serviço de Internet é prestado no Brasil ou produz efeitos relevantes aqui.

### Exemplo

Se um crime cibernético é praticado por meio de um serviço oferecido ao público brasileiro e com efeitos concretos no Brasil, a discussão sobre aplicação da lei brasileira se fortalece bastante.

## Código de Processo Civil brasileiro

O PDF destaca o art. 21 do CPC: a autoridade judiciária brasileira pode processar e julgar ações quando:

- o réu estiver domiciliado no Brasil
- a obrigação tiver de ser cumprida no Brasil
- o fundamento for fato ocorrido ou ato praticado no Brasil

### Consequência prática

Mesmo que a empresa seja estrangeira, se houver agência, filial ou sucursal no Brasil, a ligação com a jurisdição brasileira fica mais forte.

### Exemplo

Uma plataforma estrangeira com filial no Brasil, atendendo usuários brasileiros e sendo usada em um caso de crime digital com efeitos locais, pode entrar na esfera de atuação da justiça brasileira.

## Código de Defesa do Consumidor na Internet

Nas relações de consumo derivadas de serviços de Internet prestados por empresas nacionais ou estrangeiras no Brasil, o CDC pode incidir para proteger o consumidor.

O PDF menciona a responsabilidade subsidiária entre controladoras e controladas em certas situações.

### Ideia para prova

Quando o tema é serviço oferecido ao mercado brasileiro, a discussão não é apenas técnica ou penal: **há também dimensão consumerista**.

## Prova transnacional e cooperação internacional

### O que é prova transnacional?

É a prova cujo meio ou fonte está em país diferente daquele da autoridade que investiga ou julga.

### Por que isso é um problema?

Porque a evidência pode estar:

- em servidor estrangeiro
- sob controle de empresa estrangeira
- espalhada em diferentes países

Nesse caso, entra a necessidade de **cooperação internacional**.

### Resposta curta para prova

Se a fonte de prova está dentro da soberania de outro Estado, normalmente será necessário recorrer à cooperação internacional para obtenção válida do dado.

## Jurisdição na Internet

A Internet não respeita fronteiras físicas da mesma forma que o mundo tradicional. Isso cria conflitos entre diferentes legislações, principalmente sobre:

- privacidade
- guarda de dados
- fornecimento de registros
- acesso a conteúdo

### Critérios citados no PDF para definir a lei aplicável

Segundo a aula, alguns critérios possíveis são:

1. lei do local onde está o usuário
2. lei do local onde estão os servidores
3. lei do local de incorporação da empresa prestadora
4. lei do local do registrador do domínio

### O que isso mostra?

Não existe resposta simples e universal. Em casos reais, diferentes critérios podem apontar para conclusões diferentes.

### Exemplo comparativo

| Situação | Critério que ganha força |
|---|---|
| usuário brasileiro usando serviço voltado ao Brasil | lei brasileira |
| dados armazenados só em servidor estrangeiro | cooperação internacional e lei do país hospedeiro |
| empresa com filial no Brasil | conexão mais forte com a jurisdição brasileira |

## Convenção de Budapeste

A Convenção de Budapeste é o tratado internacional mais conhecido sobre crimes cibernéticos.

### Objetivos centrais

- harmonizar legislações penais e processuais
- facilitar cooperação internacional
- melhorar obtenção de provas digitais

### Ponto muito importante do PDF

O tratado prevê **preservação de dados por 90 dias**, prorrogáveis por igual período, quando houver requerimento adequado.

Também trata de:

- auxílio mútuo para dados de tráfego
- cooperação para interceptação de conteúdo

### O que decorar

- Convenção de Budapeste = referência internacional em cibercrime
- preservação de dados = **90 dias + possível prorrogação**

## Decisões judiciais citadas na aula

O PDF menciona casos clássicos que ajudam a entender os conflitos de jurisdição.

### 1. Yahoo! na Bélgica

Lição principal: um provedor de aplicação pode ser enquadrado como prestador sujeito à obrigação de cooperar com investigação criminal, conforme a interpretação da lei local.

### 2. Microsoft Irlanda x EUA

Lição principal: armazenar dados fora do país gera conflito sobre até onde vai o alcance territorial de uma ordem judicial nacional.

### 3. Google Inc. na Corte Federal da Pennsylvania

Lição principal: tribunais podem determinar entrega de dados necessários à investigação criminal, reforçando a disputa entre localização da empresa e localização dos dados.

### 4. Google Brasil Internet Ltda

Esse caso teve grande repercussão no Brasil no contexto da antiga CPI da Pedofilia.

Lição principal:

- debate sobre dever de cooperação das empresas de Internet
- relevância da presença da empresa no Brasil
- base para discussões que depois dialogam com o Marco Civil da Internet

### Observação adicional

Os slides também citam casos Apple/FBI para mostrar um ponto prático importante: **mesmo quando existe interesse investigativo, a criptografia e o desenho do dispositivo podem impor limites técnicos e jurídicos**.

## Marco Civil da Internet (MCI)

### O que é

Lei 12.965/2014, sancionada em 23/04/2014.

### Temas lembrados pela aula

- guarda de registros
- privacidade dos usuários
- liberdade de expressão

## Guarda de registros no MCI

Esse é um ponto que costuma cair muito em prova.

### Registros de conexão

Devem ser mantidos pelo prazo de **1 ano**.

### Registros de acesso a aplicações

Devem ser mantidos pelo prazo de **6 meses** pelos provedores de aplicações enquadrados na lei.

### Forma de memorização

- **conexão = 1 ano**
- **aplicação = 6 meses**

## Minimização e exclusão de dados

O material também destaca que os provedores devem reter a menor quantidade possível de dados pessoais e comunicações privadas, excluindo-os:

- quando atingida a finalidade do uso
- quando encerrado o prazo legal

### Ideia importante

O MCI não autoriza retenção ilimitada de dados “por precaução”.

## Problema prático citado na aula

O decreto regulamentador mencionado no PDF prevê que, se o provedor não coletar dados cadastrais, ele deve informar esse fato à autoridade, ficando desobrigado de fornecê-los.

### Consequência apontada pelo professor

Isso pode dificultar a identificação do usuário e favorecer abusos em cenários como redes Wi‑Fi abertas.

## O MCI resolve toda disputa de lei aplicável?

Não.

O próprio PDF lembra que o Marco Civil não resolve toda a disciplina contratual de transações internacionais. Em vários casos, ainda será necessário considerar:

- LINDB
- CDC
- jurisprudência

## Cenários clássicos de aplicação da lei

### 1. Empresa estrangeira com filial no Brasil e serviço voltado ao público brasileiro

Há forte tendência de aplicação da legislação brasileira, inclusive CDC e MCI quando cabível.

### 2. Empresa sem filial no Brasil e serviço não voltado ao público brasileiro

Ganha força a aplicação da lei estrangeira para disciplinar a relação contratual.

### 3. Empresa estrangeira oferecendo serviço ao público brasileiro, com ou sem filial

Se o serviço é direcionado ao Brasil, a legislação brasileira pode incidir nas hipóteses previstas no art. 11 do MCI.

## Quando a cooperação internacional provavelmente será necessária?

O PDF destaca duas hipóteses típicas.

### Hipótese 1

É necessário transmitir ordem judicial a provedor de Internet sem presença física no Brasil.

### Hipótese 2

O provedor não oferece seus serviços ao público brasileiro, mas o acesso ocorreu a partir do território nacional.

### Tradução prática

Se a empresa está fora do país, sem filial local e sem submissão clara ao mercado brasileiro, a obtenção da prova pode depender de vias diplomáticas e mecanismos formais de cooperação.

## Sanções previstas no MCI

Para garantir efetividade da jurisdição brasileira, o MCI prevê sanções como:

- advertência
- multa de até 10% do faturamento do grupo econômico no Brasil
- suspensão temporária das atividades
- proibição do exercício de atividades relativas aos atos previstos no art. 11

Além disso, tratando-se de empresa estrangeira, a filial, sucursal, escritório ou estabelecimento no país pode responder solidariamente pela multa.

## Resumo para revisão rápida

- territorialidade é a regra básica da jurisdição penal brasileira
- na Internet, território e jurisdição ficam mais complexos
- prova transnacional depende de cooperação internacional
- Convenção de Budapeste é a principal referência internacional
- MCI: **logs de conexão = 1 ano**; **logs de acesso a aplicações = 6 meses**
- serviço voltado ao Brasil fortalece aplicação da lei brasileira
- ausência de presença física do provedor no Brasil pode exigir cooperação internacional

## Perguntas que podem cair em prova

1. O que é o princípio da territorialidade e como ele se complica na Internet?
2. O que é prova transnacional?
3. Quais critérios podem ser usados para discutir a lei aplicável a dados digitais?
4. O que a Convenção de Budapeste prevê sobre preservação de dados?
5. Qual a diferença entre registro de conexão e registro de acesso a aplicação no MCI?
6. Em que situações a cooperação internacional tende a ser necessária?
7. Quais sanções o MCI prevê para descumprimento?
