# Conversão de Números Decimais para Binário Normalizado

Este projeto em C realiza a conversão de números decimais (positivos ou negativos) para representação binária normalizada, com suporte a:

- Representação com precisão configurável da mantissa.
- Limites de expoente definidos pelo usuário.
- Representações alternativas (Sinal e Magnitude, Complemento de 1 e Complemento de 2).
- Verificação de **Overflow** e **Underflow** com base nos limites do expoente.

## Funcionalidades

- Conversão de um número decimal em binário normalizado.
- Controle de precisão por meio da definição do número de bits da mantissa.
- Representação do número usando:
  - Sinal e magnitude
  - Complemento de 1
  - Complemento de 2
- Verificação de erros de aproximação devido à limitação da mantissa.
- Sistema de menu interativo com possibilidade de reutilização de configurações.

## Como Compilar

Você pode compilar o código usando o `gcc` (ou qualquer compilador C):

```bash
gcc -o conversor conversor.c -lm
