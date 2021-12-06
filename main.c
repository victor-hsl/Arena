#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct arena{
    char nome[50];
    int orcamento;
    int saidas;
    int x;
    int y;
    char **mat;
};
typedef struct arena arena;

struct robo{
    int x;
    int y;
    int bateria;
};
typedef struct robo rbot;

struct lstout{
    int num;
    char comando[20];
    int tempo;
    struct lstout *prox;
};
typedef struct lstout lstout;

void carregaConfig(char *nome, arena **arn){
    //INICIALIZA AS VARIAVEIS
    char linha[100];
    char *pt;
    char split[2][50];
    int i=0;
    int j=0;

    //CRIA UMA INSTANCIA COM OS DADOS DA ARENA E ALOCA DINAMICAMENTE
    arena *instancia;
    instancia = (arena*)malloc(sizeof(arena));

    //CARREGA O ARQUIVO COM O NOME INFORMADO NO ARGV NA CHAMADA DA MAIN
    FILE *config;
    config = fopen(nome, "r");

    //LE A PRIMEIRA LINHA E SETA O NOME DA ARENA
    fgets(linha, 100, config);
    pt = strtok(linha, " ");
    while(pt!= NULL){
        strcpy(split[i],pt);
        pt = strtok(NULL, "\n");
        i++;
    } i=0;
    strcpy(instancia->nome, split[1]);

    //LE A SEGUNDA LINHA E SETA O ORCAMENTO DO ROBO
    fgets(linha, 100, config);
    pt = strtok(linha, " ");
    while(pt!= NULL){
        strcpy(split[i],pt);
        pt = strtok(NULL, " ");
        i++;
    } i=0;
    instancia->orcamento = atoi(split[1]);

    //LE A TERCEIRA LINHA E SETA AS SAIDAS
    fgets(linha, 100, config);
    pt = strtok(linha, " ");
    while(pt!= NULL){
        strcpy(split[i],pt);
        pt = strtok(NULL, " ");
        i++;
    } i=0;
    instancia->saidas = atoi(split[1]);

    //LE A QUARTA LINHA E SETA AS DIMENSOES DA ARENA
    fgets(linha, 100, config);
    pt = strtok(linha, " ");
    while(pt!= NULL){
        strcpy(split[i],pt);
        pt = strtok(NULL, " ");
        i++;
    } i=0;
    char *tam;
    tam = strtok(split[1],"x");
    instancia->x = atoi(tam);
    tam = strtok(NULL,"x");
    instancia->y = atoi(tam);
    tam = strtok(NULL,"x");

    //ALOCA ESPAÇO PARA UMA MATRIZ DE CHAR X POR Y
    instancia->mat = malloc (instancia->x * sizeof (char*)) ;
    for(i=0;i<instancia->x;i++){
        instancia->mat[i] = malloc (instancia->y * sizeof(char*));
    }

    //SETA OS CARACTERES DE ACORDO COM A REPRESENTACAO DA MATRIZ
    for(i=0;i<instancia->x;i++){
        fgets(linha, 100, config);
        for(j=0;j<instancia->y;j++){
            instancia->mat[i][j] = linha[j];
        }
    }
    *arn = instancia;
}

int** encontraSaidas(arena *arn){

    int i = 0;
    int j = 0;
    int k = 0;

    arena *pt;
    pt = arn;

    int **saidas;
    saidas = malloc (pt->saidas * sizeof(int*));
    for(i=0;i < pt->saidas; i++){
        saidas[i] = malloc (3 * sizeof(int*));
    }
    i = 0;
    for(i=0;i<pt->x;i++){
        for(j=0;j<pt->y;j++){
            if(pt->mat[i][j] != ' '&&pt->mat[i][j]!='#'&&pt->mat[i][j]!='*'){
                saidas[k][0] = pt->mat[i][j] - '0';
                saidas[k][1] = i;
                saidas[k][2] = j;
                k++;
            }
        }
    }

    return saidas;
}

void posicaoInicial(arena *arn, rbot **rbt){
    rbot *narena;
    int i, j;
    arena *pt;
    pt = arn;
    narena = (rbot*)malloc(sizeof(rbot));
    for(i=0;i<pt->x;i++){
        for(j=0;j<pt->y;j++){
            if(pt->mat[i][j]=='*'){
                narena->x = i;
                narena->y = j;
                narena->bateria = pt->orcamento;
            }
        }
    }
    *rbt = narena;
}

lstout calculaCaminhos(arena *arn, rbot *rbt, int posx, int posy, int num){
    //CRIA UM PONTEIRO PARA A ARENA ORIGINAL
    arena *pt;
    pt = arn;
    //CRIA UM PONTEIRO PARA O ROBO
    rbot *ptRobo;
    ptRobo = rbt;
    char comando[pt->orcamento];

    lstout *saida;
    saida = (lstout*)malloc(sizeof(lstout));
    //I E J PARA MANUSEAR OS EIXOS X E Y
    int i = ptRobo->x;
    int j = ptRobo->y;
    int l = 0;
    //CONTINUA O LACO ATE QUE O OS DOIS EIXOS SEJAM IGUAIS AOS EIXOS DA SAIDA RECEBIDA OU ATE ACABAR A BATERIA
    while(((ptRobo->x != posx)||(ptRobo->y != posy))&&(ptRobo->bateria > 0)){
        //VERIFICA SE A POSICAO ATUAL IGUALOU A X
        if(ptRobo->x == posx){

            //VERIFICA SE O CAMINHO INTEIRO A FRENTE É LIVRE
                int livre = 1;
                int n;
                //NA DIRECAO ESQUERDA ATE x == POSx
                if(ptRobo->x > posx){
                    for(n = ptRobo->x; n > posx;n--){
                        if(pt->mat[n][posy]=='#'){
                            livre = 0;
                        }
                    }
                }
                //NA DIRECAO DA DIREITA ATE X == POSX
                else if (ptRobo->x < posx){
                    for(n = ptRobo->x; n < posx;n++){
                        if(pt->mat[n][posy]=='#'){
                            livre = 0;
                        }
                    }
                }
                //SE ESTIVER LIVRE PERCORRE ATE IGUALAR
                if(livre == 1){
                    //ANDA PRA DIREITA
                    if(ptRobo->y < posy){
                        if(pt->mat[i][j+1]!='#'){
                            j++;
                            ptRobo->y++;
                            ptRobo->bateria--;
                            comando[l]='d';
                            l++;
                        }
                    }
                    //ANDA PRA ESQUERDA
                    else if(ptRobo->y > posy){
                        if(pt->mat[i][j-1]!='#'){
                            j--;
                            ptRobo->y--;
                            ptRobo->bateria--;
                            comando[l]='e';
                            l++;
                        }
                    }
                }
                else{
                    //ANDA PRA BAIXO
                    if(ptRobo->x < posx){
                        if(pt->mat[i+1][j]!='#'){
                            i++;
                            ptRobo->x++;
                            ptRobo->bateria--;
                            comando[l]='b';
                            l++;
                        }
                    }
                    //ANDA PRA CIMA
                    else if(ptRobo->x > posx){
                        if(pt->mat[i-1][j]!='#'){
                            i--;
                            ptRobo->x--;
                            ptRobo->bateria--;
                            comando[l]='c';
                            l++;
                        }
                    }
                }
        }
        //VERIFICA SE IGUALOU A Y
        else if(ptRobo->y == posy){
            //VERIFICA SE O CAMINHO INTEIRO A FRENTE É LIVRE
                int livre = 1;
                int n;
                //NA DIRECAO CIMA ATE Y == POSY
                if(ptRobo->y > posy){
                    for(n = ptRobo->y; n > posy;n--){
                        if(pt->mat[posx][n]=='#'){
                            livre = 0;
                        }
                    }
                }
                //NA DIRECAO BAIXO ATE Y == POSY
                else if (ptRobo->y < posy){
                    for(n = ptRobo->y; n < posy;n++){
                        if(pt->mat[posx][n]=='#'){
                            livre = 0;
                        }
                    }
                }
                //SE ESTIVER LIVRE PERCORRE ATE IGUALAR
                if(livre == 1){
                    //ANDA PRA BAIXO
                    if(ptRobo->x < posx){
                        if(pt->mat[i+1][j]!='#'){
                            i++;
                            ptRobo->x++;
                            ptRobo->bateria--;
                            comando[l]='b';
                            l++;
                        }
                    }
                    //ANDA PRA CIMA
                    else if(ptRobo->x > posx){
                        if(pt->mat[i-1][j]!='#'){
                            i--;
                            ptRobo->x--;
                            ptRobo->bateria--;
                            comando[l]='c';
                            l++;
                        }
                    }
                }
                //SE NAO ESTIVER LIVRE ANDA NO OUTRO EIXO
                else{
                    //ANDA PRA DIREITA
                    if(ptRobo->y < posy){
                        if(pt->mat[i][j+1]!='#'){
                            j++;
                            ptRobo->y++;
                            ptRobo->bateria--;
                            comando[l]='d';
                            l++;
                        }
                    }
                    //ANDA PRA ESQUERDA
                    else if(ptRobo->y > posy){
                        if(pt->mat[i][j-1]!='#'){
                            j--;
                            ptRobo->y--;
                            ptRobo->bateria--;
                            comando[l]='e';
                            l++;
                        }
                    }
                }
        }
        else {
            //COMPARA A DIFERENCA DA POSICAO ATUAL DO ROBO COM O EIXO X E Y DA SAIDA
            int difx, dify;
            difx = ptRobo->x - posx;
            if(difx < 0){
                difx = difx * -1;
            }
            dify = ptRobo->y - posy;
            if(dify < 0){
                dify = dify * -1;
            }

            //ANDA NA DIRECAO DO MAIS PERTO PRIMEIRO,
            //MAIS PERTO DO X
            if(difx < dify){
                //VERIFICA SE O CAMINHO INTEIRO A FRENTE É LIVRE
                int livre = 1;
                int n;
                //NA DIRECAO CIMA ATE Y == POSY
                if(ptRobo->y > posy){
                    for(n = ptRobo->y; n > posy;n--){
                        if(pt->mat[posx][n]=='#'){
                            livre = 0;
                        }
                    }
                }
                //NA DIRECAO BAIXO ATE Y == POSY
                else if (ptRobo->y < posy){
                    for(n = ptRobo->y; n < posy;n++){
                        if(pt->mat[posx][n]=='#'){
                            livre = 0;
                        }
                    }
                }
                //SE ESTIVER LIVRE PERCORRE ATE IGUALAR
                if(livre == 1){
                    //ANDA PRA BAIXO
                    if(ptRobo->x < posx){
                        if(pt->mat[i+1][j]!='#'){
                            i++;
                            ptRobo->x++;
                            ptRobo->bateria--;
                            comando[l]='b';
                            l++;
                        }
                    }
                    //ANDA PRA CIMA
                    else if(ptRobo->x > posx){
                        if(pt->mat[i-1][j]!='#'){
                            i--;
                            ptRobo->x--;
                            ptRobo->bateria--;
                            comando[l]='c';
                            l++;
                        }
                    }
                }
                //SE NAO ESTIVER LIVRE ANDA NO OUTRO EIXO
                else{
                    //ANDA PRA DIREITA
                    if(ptRobo->y < posy){
                        if(pt->mat[i][j+1]!='#'){
                            j++;
                            ptRobo->y++;
                            ptRobo->bateria--;
                            comando[l]='d';
                            l++;
                        }
                    }
                    //ANDA PRA ESQUERDA
                    else if(ptRobo->y > posy){
                        if(pt->mat[i][j-1]!='#'){
                            j--;
                            ptRobo->y--;
                            ptRobo->bateria--;
                            comando[l]='e';
                            l++;
                        }
                    }
                }
            }
            //MAIS PERTO DO Y
            else if(difx > dify){
                //VERIFICA SE O CAMINHO INTEIRO A FRENTE É LIVRE
                int livre = 1;
                int n;
                //NA DIRECAO ESQUERDA ATE x == POSx
                if(ptRobo->x > posx){
                    for(n = ptRobo->x; n > posx;n--){
                        if(pt->mat[n][posy]=='#'){
                            livre = 0;
                        }
                    }
                }
                //NA DIRECAO DA DIREITA ATE X == POSX
                else if (ptRobo->x < posx){
                    for(n = ptRobo->x; n < posx;n++){
                        if(pt->mat[n][posy]=='#'){
                            livre = 0;
                        }
                    }
                }
                //SE ESTIVER LIVRE PERCORRE ATE IGUALAR
                if(livre == 1){
                    //ANDA PRA DIREITA
                    if(ptRobo->y < posy){
                        if(pt->mat[i][j+1]!='#'){
                            j++;
                            ptRobo->y++;
                            ptRobo->bateria--;
                            comando[l]='d';
                            l++;
                        }
                    }
                    //ANDA PRA ESQUERDA
                    else if(ptRobo->y > posy){
                        if(pt->mat[i][j-1]!='#'){
                            j--;
                            ptRobo->y--;
                            ptRobo->bateria--;
                            comando[l]='e';
                            l++;
                        }
                    }
                }
                else{
                    //ANDA PRA BAIXO
                    if(ptRobo->x < posx){
                        if(pt->mat[i+1][j]!='#'){
                            i++;
                            ptRobo->x++;
                            ptRobo->bateria--;
                            comando[l]='b';
                            l++;
                        }
                    }
                    //ANDA PRA CIMA
                    else if(ptRobo->x > posx){
                        if(pt->mat[i-1][j]!='#'){
                            i--;
                            ptRobo->x--;
                            ptRobo->bateria--;
                            comando[l]='c';
                            l++;
                        }
                    }
                }
            }
            else {
                //ANDA PRA DIREITA
                    if(ptRobo->y < posy){
                        if(pt->mat[i][j+1]!='#'){
                            j++;
                            ptRobo->y++;
                            ptRobo->bateria--;
                            comando[l]='d';
                            l++;
                        }
                    }
                    //ANDA PRA ESQUERDA
                    if(ptRobo->y > posy){
                        if(pt->mat[i][j-1]!='#'){
                            j--;
                            ptRobo->y--;
                            ptRobo->bateria--;
                            comando[l]='e';
                            l++;
                        }
                    }
                    //ANDA PRA BAIXO
                    if(ptRobo->x < posx){
                        if(pt->mat[i+1][j]!='#'){
                            i++;
                            ptRobo->x++;
                            ptRobo->bateria--;
                            comando[l]='b';
                            l++;
                        }
                    }
                    //ANDA PRA CIMA
                    if(ptRobo->x > posx){
                        if(pt->mat[i-1][j]!='#'){
                            i--;
                            ptRobo->x--;
                            ptRobo->bateria--;
                            comando[l]='c';
                            l++;
                        }
                    }
            }
        }
    }
    if(ptRobo->x == posx && ptRobo->y == posy){
        saida->num = num;
        strcpy(saida->comando,comando);
        saida->tempo = l;
        saida->prox = NULL;
        return *saida;
    } else {
        saida->num = num;
        strcpy(saida->comando,"Nao alcancada!");
        saida->tempo = l;
        saida->prox = NULL;
        return *saida;
    }

}

void salvaArquivo(arena *arn, rbot *robo, lstout *sda){
    int i = 0;
    FILE *ptArq;
    arena *pt = arn;
    rbot *rb = robo;
    char sout[50];
    strcat(strcpy(sout, pt->nome), ".out");
    ptArq = fopen(sout, "w+");
    if(ptArq == NULL){
        printf("OCORREU UM PROBLEMA AO SALVAR O ARQUVO.\n");
    }else{
        fprintf(ptArq,"Posicao inicial [x,y]: [%d,%d]\n", rb->x, rb->y);
        fprintf(ptArq,"Orcamento Disponivel: %d segundos\n\n", pt->orcamento);
        for(i=0;i<pt->saidas;i++){
                fprintf(ptArq,"Saida %d: %d segundos -> %s\n", sda->num, sda->tempo, sda->comando);
        }
        printf("Arquivo %s gerado com sucesso!", sout);
    }
    fclose(ptArq);
}

void saidaTerminal(arena *arn, int alcancadas, lstout *saida){
    arena *pt;
    pt = arn;
    printf("Instancia %s\n", pt->nome);
    printf("Orcamento disponivel: %d segundos\n", pt->orcamento);
    printf("Quantidade de saidas alcancadas: %d\n", alcancadas);
    printf("Saidas mais proximas: %d segundos\n\n", saida->tempo);
    printf("Saida %d: %s\n\n", saida->num, saida->comando);

}

int main(int argc, char *argv[]){
    arena *pt;
    rbot *robo, *rbaux;
    rbaux = (rbot*)malloc(sizeof(rbot));
    lstout *saida, *mais_rapida;
    mais_rapida = (lstout*)malloc(sizeof(lstout));
    saida = (lstout*)malloc(sizeof(lstout));
    mais_rapida->tempo = 0;

    carregaConfig(argv[1], &pt);
    int i = 0;
    int cont = 0;
    int **k = encontraSaidas(pt);

    for(i=0;i < pt->saidas; i++){
        posicaoInicial(pt, &robo);
        *rbaux = *robo;

        *saida = calculaCaminhos(pt, rbaux, k[i][1], k[i][2], k[i][0]);
        if(strcmp(saida->comando,"Nao alcancada!") != 0){
            cont++;
            if(saida->tempo < mais_rapida->tempo || mais_rapida->tempo == 0){
                *mais_rapida = *saida;
            }
        }
    }
    saidaTerminal(pt, cont, mais_rapida);
    salvaArquivo(pt, robo, saida);

    return 0;
}
