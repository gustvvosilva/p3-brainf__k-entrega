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

// Remove todos espaços em branco.
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

    char *nl = strchr(line, '\n');
    if (nl) *nl = '\0';

    // separa nome da variável e expressão.
    char *eq = strchr(line, '=');
    if (!eq) return 1;
    int varlen = eq - line;
    char varname[MAX_LINE];

    strncpy(varname, line, varlen);
    varname[varlen] = '\0';

    // remove espaços e recalcula comprimento em bytes.
    trim(varname);
    varlen = (int)strlen(varname);

    char expr[MAX_LINE];
    strcpy(expr, eq+1);
    trim(expr);

    Term terms[MAX_TERMS];
    int term_count = 0;
    char *p = expr;
    int curr_sign = +1;
    while (*p && term_count < MAX_TERMS) {
        if (*p == '+') { curr_sign = +1; p++; continue; }
        if (*p == '-') { curr_sign = -1; p++; continue; }
        char piece[MAX_LINE];
        char *q = p;
        while (*q && *q!='+' && *q!='-') q++;
        int len = q - p;
        strncpy(piece, p, len);
        piece[len] = '\0';
        trim(piece);

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

    // Imprime "varname=" suportando UTF-8.
    for (int i = 0; i < varlen; i++) {
        unsigned char c = (unsigned char)varname[i];
        printf(">");
        for (int j = 0; j < c; j++)
            printf("+");
        printf(".");
    }

    // imprime '='.
    printf(">");
    for (int i = 0; i < 61; i++)
        printf("+");
    printf(".");

    // Volta a celula 0.
    for (int i = 0; i < varlen + 1; i++)
        printf("<");
    printf("[-]");

    // GERA BF PARA CADA TERMO
    for (int i = 0; i < term_count; i++) {
        Term *t = &terms[i];
        if (t->is_mul) {
            int A = t->a, B = t->b;

            printf(">");
            printf("[-]");

            for (int j = 0; j < B; j++)
                printf("+");

            if (t->sign > 0) {
                printf("[<");
                for (int j = 0; j < A; j++)
                    printf("+");
                printf(">-]");
            } else {
                printf("[<");
                for (int j = 0; j < A; j++)
                    printf("-");
                printf(">-]");
            }
            printf("<");
        } else {
            int V = abs(t->val);
            if (t->sign > 0) {
                for (int j = 0; j < V; j++)
                    printf("+");
            } else {
                for (int j = 0; j < V; j++)
                    printf("-");
            }
        }
    }
    return 0;
}
