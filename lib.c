
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct arena{
    char nome[50];
    int orcamento;
    int saidas;
    int x;
    int y;
};
typedef struct arena arena;

void carregaConfig(char *nome){
    FILE *config;
    config = fopen(nome, "r");
    char linha[100];
    while(fgets(linha, 100, config) != NULL){
        char *token;
        char split[2][50];
        linha[strcspn(linha, "\n")] = 0;
        token = strtok(linha, " ");
        int i=0;
    }

}

int main(int argc, char *argv[]){
    carregaConfig(argv[1]);


    return 0;
}
