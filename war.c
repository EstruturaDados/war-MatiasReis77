#include <stdio.h>
#include <string.h>

// Definindo estrutura
struct Territorio{
    char nome[30];
    char cor[10];
    int tropas;
};

int main(){
    // Definindo o número de cadastros
    int num_territorio = 5;

    // Declarando um array de struct
    struct Territorio territorios[num_territorio];

    printf("=================================== \n");
     printf("\n");
      printf("Vamos cadastrar os 5 territórios iniciais do nosso mundo.\n");
       printf(" \n");
    
    // Laço for para ler os dados
    for(int i=0; i<num_territorio;i++){
        printf("---Cadastrando território %d---\n",i+1);

        // Leitura do nome
        printf("Nome do Território:");
        fgets(territorios[i].nome, sizeof(territorios[i].nome),stdin);

        territorios[i].nome[strcspn(territorios[i].nome,"\n")] =0;

        // Leitura de cor
        printf("Cor do Exército:");
        fgets(territorios[i].cor, sizeof(territorios[i].cor),stdin);

        territorios[i].cor[strcspn(territorios[i].cor,"\n")] =0;

        //Leitura de tropas 
        printf("Número de tropas:");
        scanf("%d",&territorios[i].tropas);

         printf(" \n");
        //Limpa o buffer
        while(getchar()!='\n');
    }
     printf("Cadastro inicial concluido com sucesso!\n");
      printf(" \n");

    // Exibir os dados
     printf("===================================\n");
      printf("   MAPA DO MUNDO - ESTADO ATUAL\n");
       printf("=================================\n");
        printf(" \n");
    for(int i=0; i<num_territorio;i++){
        printf("TERRITÓRIO %d\n",i+1);
        printf("- Nome:%s\n", territorios[i].nome);
        printf("- Dominado por: Exercito %s\n",territorios[i].cor);
        printf("- Tropas:%d\n",territorios[i].tropas);
         printf(" \n");

    }
return 0;
}
