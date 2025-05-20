// compilador.c
// gcc -o compilador compilador.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1024
#define MAX_TERMS 100

typedef struct {
    int sign;     // +1 ou -1
    int is_mul;   // 1 = termo A*B, 0 = termo único
    int a, b;     // caso is_mul==1
    int val;      // caso is_mul==0
} Term;

void trim(char *s) {
    char *p = s, *w = s;
    while (*p) {
        if (!isspace((unsigned char)*p)) *w++ = *p;
        p++;
    }
    *w = '\0';
}

int main() {
    char line[MAX_LINE];
    if (!fgets(line, sizeof(line), stdin))
        return 0;
    // retira newline
    char *nl = strchr(line, '\n');
    if (nl) *nl = '\0';

    // separa nome da variável e expressão
    char *eq = strchr(line, '=');
    if (!eq) return 1;
    int varlen = eq - line;
    char varname[varlen+1];
    strncpy(varname, line, varlen);
    varname[varlen] = '\0';
    trim(varname);

    char expr[MAX_LINE];
    strcpy(expr, eq+1);
    trim(expr);

    // parse de termos com sinais + e -
    Term terms[MAX_TERMS];
    int term_count = 0;
    char *p = expr;
    int curr_sign = +1;
    while (*p && term_count < MAX_TERMS) {
        if (*p == '+') {
            curr_sign = +1; p++;
            continue;
        }
        if (*p == '-') {
            curr_sign = -1; p++;
            continue;
        }
        // extrai até próximo + ou -
        char piece[MAX_LINE];
        char *q = p;
        while (*q && *q!='+' && *q!='-') q++;
        int len = q - p;
        strncpy(piece, p, len);
        piece[len] = '\0';
        trim(piece);

        // detecta multiplicação A*B
        char *mul = strchr(piece, '*');
        if (mul) {
            *mul = '\0';
            char left[MAX_LINE], right[MAX_LINE];
            strcpy(left, piece);
            strcpy(right, mul+1);
            trim(left); trim(right);
            terms[term_count].sign = curr_sign;
            terms[term_count].is_mul = 1;
            terms[term_count].a = atoi(left);
            terms[term_count].b = atoi(right);
        } else {
            terms[term_count].sign = curr_sign;
            terms[term_count].is_mul = 0;
            terms[term_count].val = atoi(piece);
        }
        term_count++;
        p = q;
    }

    // prefixo: imprime "varname="
    for(int i = 0; i < varlen; i++) {
        printf(">");
        for(int j = 0; j < varname[i]; j++) printf("+");
        printf(".");
    }
    printf(">");
    for(int i = 0; i < 61; i++) printf("+");
    printf(".");
    for(int i = 0; i < varlen + 1; i++) printf("<");

    // zera cell 0
    printf("[-]");

    // gera BF para cada termo
    for (int i = 0; i < term_count; i++) {
        Term *t = &terms[i];
        if (t->is_mul) {
            int A = t->a, B = t->b;
            // vai para cell1 e zera
            printf(">");
            printf("[-]");
            // coloca B em cell1
            for (int j = 0; j < B; j++) printf("+");
            // loop de B vezes: soma ou subtrai A em cell0
            if (t->sign > 0) {
                // soma A:
                printf("[<");
                for (int j = 0; j < A; j++) printf("+");
                printf(">-]");
            } else {
                // subtrai A:
                printf("[<");
                for (int j = 0; j < A; j++) printf("-");
                printf(">-]");
            }
            // volta para cell0
            printf("<");
        } else {
            int V = abs(t->val);
            if (t->sign > 0) {
                for (int j = 0; j < V; j++) printf("+");
            } else {
                for (int j = 0; j < V; j++) printf("-");
            }
        }
    }

    // não imprime '\n'
    return 0;
}
