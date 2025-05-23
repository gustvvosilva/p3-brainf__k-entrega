#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 30000
#define BUFFER_SIZE 30000
#define MAX_NESTED_LOOPS 100

// Interpretador de Brainfuck
int brainfuck(const char* cod) {
    unsigned char memoria[MEMORY_SIZE] = {0};
    int ptr = 0;
    int code_pos = 0;
    int loop_stack[MAX_NESTED_LOOPS];
    int loop_index = 0;
    
    while (cod[code_pos] != '\0') {
        switch (cod[code_pos]) {
            case '>':
                ptr++;
                if (ptr >= MEMORY_SIZE) {
                    printf("Erro: Ponteiro fora dos limites de memória (para frente)\n");
                    return 1;
                }
                break;
                
            case '<':
                ptr--;
                if (ptr < 0) {
                    printf("Erro: Ponteiro fora dos limites de memória (para trás)\n");
                    return 1;
                }
                break;
                
            case '+':
                memoria[ptr]++;
                break;
                
            case '-':
                memoria[ptr]--;
                break;
                
            case '.':
                putchar(memoria[ptr]);
                break;
                
            case ',':
                memoria[ptr] = getchar();
                break;
                
            case '[':
                if (memoria[ptr] == 0) {
                    // Pula para o correspondente ]
                    int depth = 1;
                    while (depth > 0) {
                        code_pos++;
                        if (cod[code_pos] == '[') {
                            depth++;
                        } else if (cod[code_pos] == ']') {
                            depth--;
                        }
                        
                        if (cod[code_pos] == '\0') {
                            printf("Erro: Loop não fechado\n");
                            return 1;
                        }
                    }
                } else {
                    if (loop_index >= MAX_NESTED_LOOPS) {
                        printf("Erro: Loops aninhados demais\n");
                        return 1;
                    }
                    loop_stack[loop_index] = code_pos;
                    loop_index++;
                }
                break;
                
            case ']':
                if (loop_index <= 0) {
                    printf("Erro: ']' sem correspondente '['\n");
                    return 1;
                }
                
                if (memoria[ptr] != 0) {
                    code_pos = loop_stack[loop_index - 1];
                } else {
                    loop_index--;
                }
                break;
        }
        
        code_pos++;
    }

    int result = memoria[0];
    printf("%d\n", result);
    
    return 0;
}

int main() {
    char cod[BUFFER_SIZE];
    char c;
    int pos = 0;
    
    // Lê o código Brainfuck da entrada padrão
    while ((c = getchar()) != EOF && pos < BUFFER_SIZE - 1) {
        // Apenas caracteres válidos de Brainfuck são armazenados
        if (c == '>' || c == '<' || c == '+' || c == '-' || 
            c == '.' || c == ',' || c == '[' || c == ']') {
            cod[pos] = c;
            pos++;
        }
    }
    cod[pos] = '\0';
    
    // Executa o código Brainfuck
    brainfuck(cod);
    
    return 0;
}