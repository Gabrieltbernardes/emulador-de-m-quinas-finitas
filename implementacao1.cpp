#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Declaração das funções utilizadas para conversão, complementos e transformação do valor decimal para binário normalizado
void decimal_para_binario(double valor, int n, int l, int u, char *mantissa, char *mantissaAux, int *expoente, int *overflow, int *underflow, int man);
void complemento_1(char *binario, char *resultado);
void complemento_2(char *binario, char *resultado);
void transforma(int n, char *mantissa, double valor, int expoente, int man);

int main() {
    int op = 2, n, l, u, man;
    do {
        system("cls"); // Limpa a tela para uma interface de usuário mais limpa
        if (op == 2) {
            // Define precisão da mantissa e limites do expoente
            printf("Digite a precisao da mantissa (n): ");
            scanf("%d", &n);
            do {
                printf("Digite o menor valor do expoente (l): ");
                scanf("%d", &l);
                printf("Digite o maior valor do expoente (u): ");
                scanf("%d", &u);
                man = n + 10; // Define o tamanho de 'man' como n + 10 para armazenar a mantissa
                if (l >= u)
                    printf("Valor invalido para (l) e (u), digite novos valores!\n ");
                system("pause");
                system("cls");
            } while (l >= u);
        } else
            system("cls");

        double valor;
        printf("\nDigite um numero decimal: ");
        if (scanf("%lf", &valor) != 1) break; // Recebe o número decimal, quebra o loop se houver erro

        int sinal = (valor < 0) ? 1 : 0; // Define o sinal: 1 para negativo e 0 para positivo
        if (sinal) valor = -valor; // Transforma o valor em positivo para o cálculo

        // Declaração dos vetores para armazenar a mantissa e valores auxiliares
        char mantissa[man], mantissaAux[man];
        int expoente = 0, overflow = 0, underflow = 0;

        // Chama a função para converter o valor decimal para binário e verificar overflow e underflow
        decimal_para_binario(valor, n, l, u, mantissa, mantissaAux, &expoente, &overflow, &underflow, man);

        // Verifica se ocorreu overflow ou underflow
        if (overflow || underflow) {
            printf("Erro de %s\n", overflow ? "Overflow" : "Underflow\n\n");
            system("pause");
        }

        // Exibe o número normalizado em binário
        if (valor < 1)
            printf("Numero normalizado armazenado em binario: 0.%s * 2^%d\n", mantissaAux, expoente);
        else 
            printf("Numero normalizado armazenado em binario: 0.%s * 2^%d\n", mantissaAux, expoente + 1);

        // Chama a função para transformar e exibir o valor armazenado
        transforma(n, mantissa, valor, expoente, man);
		
        // Caso o número seja negativo, calcula representações por sinal e complemento
        if (sinal) {
            char sinal_amplitude[man];
            strcpy(sinal_amplitude, mantissa);
            printf("Representacao por Sinal e Amplitude: %d %s\n", sinal, sinal_amplitude);

            char comp1[man], comp2[man];
            complemento_1(mantissa, comp1); // Calcula complemento de 1
            complemento_2(mantissa, comp2); // Calcula complemento de 2

            printf("Complemento de 1: %d %s\n", sinal, comp1);
            printf("Complemento de 2: %d %s\n", sinal, comp2);
        }

        // Pergunta ao usuário se deseja continuar com as configurações atuais
        do {
            printf("\n\n\nPretende continuar com as mesmas configuracoes?\n1:Sim\t2:Nao\t0:Sair\n");
            scanf("%d", &op);
            if (op > 2 || op < 0) {
                printf("\nopcao invalida digite novamente");
                system("pause");
                system("cls");
            }
        } while (op > 2 || op < 0);

        system("cls"); // Limpa a tela para reiniciar o processo
    } while (op != 0);

    return 0;
}

// Função para converter o número decimal em binário e normalizar
void decimal_para_binario(double valor, int n, int l, int u, char *mantissa, char *mantissaAux, int *expoente, int *overflow, int *underflow, int man) {
    // Tratamento para zero
    if (valor == 0) {
        *expoente = 0;
        strcpy(mantissa, "0");
        return;
    }

    int parte_inteira = (int)valor;
    double parte_fracionaria = valor - parte_inteira;
    char bin_inteiro[man] = "", bin_fracionario[man] = "";
    int i = 0, j;

    // Convertendo a parte inteira para binário
    while (parte_inteira > 0 && i < n) {
        bin_inteiro[i++] = (parte_inteira % 2) + '0';
        parte_inteira /= 2;
    }
    bin_inteiro[i] = '\0';
    strrev(bin_inteiro); // Reverte a string para corrigir a ordem dos bits
    j = i; // Tamanho da parte inteira
    i = 0;

    // Convertendo a parte fracionária para binário
    while (parte_fracionaria > 0 && i < n) {
        parte_fracionaria *= 2;
        bin_fracionario[i++] = ((int)parte_fracionaria) + '0';
        parte_fracionaria -= (int)parte_fracionaria;
    }

    // Completa a parte fracionária caso haja espaços
    while (i < n) {
        bin_fracionario[i++] = '0';
    }
    bin_fracionario[i] = '\0';

    // Normalização da mantissa com ajuste do expoente
    if (j > 0) {
        *expoente = j - 1;
        snprintf(mantissa, n + 1, "%s%s", bin_inteiro, bin_fracionario);
        mantissa[n] = '\0';
    } else {
        // Caso a parte inteira seja zero
        int shift = 0;
        while (bin_fracionario[shift] != '1' && shift < n) {
            (*expoente)--; // Ajuste do expoente para cada '0'
            shift++;
        }
        if (shift < n) {
            strncpy(mantissa, &bin_fracionario[shift], n);
            mantissa[n] = '\0';
        } else {
            strcpy(mantissa, "0");
            *expoente = 0;
        }
    }

    // Ajuste da mantissa para exibir apenas bits significativos
    strcpy(mantissaAux, mantissa);
    int h = n - 1;
    while (mantissaAux[h] != '1') {
        h--;
    }
    mantissaAux[h + 1] = '\0';
    
    // Verificação de overflow e underflow do expoente
    if (*expoente > u) *overflow = 1;
    else if (*expoente < l) *underflow = 1;
}

// Função para calcular o complemento de 1
void complemento_1(char *binario, char *resultado) {
    for (int i = 0; binario[i] != '\0'; i++) {
        resultado[i] = (binario[i] == '0') ? '1' : '0';
    }
    resultado[strlen(binario)] = '\0';
}

// Função para calcular o complemento de 2
void complemento_2(char *binario, char *resultado) {
    complemento_1(binario, resultado); // Inverte bits para obter complemento de 1
    int carry = 1;
    for (int i = strlen(resultado) - 1; i >= 0; i--) {
        if (resultado[i] == '1' && carry == 1) {
            resultado[i] = '0';
        } else if (resultado[i] == '0' && carry == 1) {
            resultado[i] = '1';
            carry = 0;
        }
    }
}

// Função que transforma e exibe o número como seria armazenado
void transforma(int n, char *mantissa, double valor, int expoente, int man) {
    int i, j;
    char mantissaAuxiliar[man];
    int parte_inteira = (int)valor;
    double parte_fracionaria = valor - parte_inteira;
    double inteiro = 0.0;
    
    // Calcula a contribuição da parte inteira
    for (i = 0; i < strlen(mantissa) && i < expoente; i++) {
        if (mantissa[i] == '1') {
            inteiro += (pow(2, expoente - i));
        }
    }   
    if (parte_inteira % 2 != 0)
        inteiro++;

    // Calcula a parte fracionária da mantissa
    j = expoente + 1;
    if (inteiro >= 1) {
        printf("o numero binario sera %s\n", mantissa);
        for (i = expoente + 1; i < strlen(mantissa); i++) {
            if (mantissa[i] == '1') {
                inteiro += (pow(2, expoente - j));
            }
            j++;
        } 
    } else {
        j = -expoente;
        for (i = 0; i < j; i++) {
            mantissaAuxiliar[i] = '0';
        }
        mantissaAuxiliar[j] = '\0';
        strcat(mantissaAuxiliar, mantissa);
        printf("o numero em binario sera: %s\n", mantissaAuxiliar);

        for (i = 0; i < strlen(mantissaAuxiliar); i++) {
            if (mantissaAuxiliar[i] == '1') {
                inteiro += (pow(2, -(i + 1)));
            }
        }    	
    }
    
    // Exibe o número recebido pelo computador com precisão definida
    printf("o numero recebido pelo computador sera: %.15lf \n\n\n", inteiro);
    if (inteiro != valor)
        printf("\nErro de aproximacao, o numero nao pode ser exibido corretamente.\n\n\n");
}

