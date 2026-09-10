# Mensagens de Utilidade e Autenticação (Meta WhatsApp Business API)

______________________________________________________________________

**Date:** 2026-07-28
**Tags:**
- [WhatsApp](../tags/WhatsApp.md)
- [Meta](../tags/Meta.md)
- [Templates](../tags/Templates.md)
**URL:**
- https://developers.facebook.com/docs/whatsapp/pricing
- https://developers.facebook.com/docs/whatsapp/message-templates/guidelines

______________________________________________________________________

## Contexto

Na API do WhatsApp Business (Meta/Cloud API), toda conversa iniciada por uma
empresa usando um **template de mensagem** é classificada em uma **categoria**.
Essa categoria define a cobrança e as regras de conteúdo permitido. As
categorias oficiais são:

- **Marketing**
- **Utility** (Utilidade)
- **Authentication** (Autenticação)

## Mensagens de Utilidade (Utility)

- Servem para dar continuidade a uma transação ou fornecer informação
  solicitada/esperada pelo usuário, geralmente relacionada a uma compra ou
  conta já existente.
- Exemplos:
  - Confirmação de pedido/compra
  - Atualização de status de entrega
  - Aviso de vencimento de fatura/pagamento
  - Alterações em uma conta ou assinatura (ex: mudança de senha, dados)
- Não podem conter conteúdo promocional (ofertas, cupons, upsell).
- Tendem a ter custo menor que templates de Marketing.

## Mensagens de Autenticação (Authentication)

- Usadas exclusivamente para **verificar a identidade do usuário** dentro de
  um fluxo de login/transação.
- Exemplo clássico: envio de **código OTP (one-time password)** para
  login, recuperação de senha ou confirmação de uma ação sensível.
- A Meta tem um formato de template restrito para essa categoria:
  - Corpo de texto fixo/padronizado (pouca liberdade de customização)
  - Pode incluir botão de "copiar código" ou preenchimento automático
  - Não pode misturar conteúdo de marketing ou utilidade no mesmo template
- Geralmente é a categoria mais barata, incentivada pela Meta por ter menor
  risco de ser vista como spam.

## Diferença resumida

| Categoria      | Objetivo                                   | Contém promoção? |
|----------------|---------------------------------------------|-------------------|
| Marketing      | Divulgar produtos, ofertas, engajamento     | Sim               |
| Utility        | Dar suporte a uma transação/conta existente | Não               |
| Authentication | Verificar identidade (ex: código OTP)       | Não               |

## Por que a categoria importa

- A Meta cobra por conversa iniciada, e o **preço varia por categoria e por
  país**.
- O template enviado é analisado e categorizado automaticamente pela Meta
  (ou manualmente durante a revisão); usar o conteúdo errado para a
  categoria pode causar rejeição do template ou reclassificação.
