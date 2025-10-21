#include <stdio.h>
#include <stdlib.h> // Para malloc, calloc, free, rand, srand
#include <string.h> // Para strcpy, strcmp, strcspn
#include <time.h>   // Para time()

// Estrutura para armazenar os dados de um território
struct Territorio {
    char nome[30];
    char cor[10]; // Campo atualizado conforme a nova struct
    int tropas;
};

// funções
void limparBuffer();
void cadastrarTerritorio(struct Territorio* territorio, int indice);
void listarTerritorios(struct Territorio* mapa, int totalTerritorios);
void atacar(struct Territorio* atacante, struct Territorio* defensor);
void liberarMemoria(struct Territorio* mapa);

// Função principal que gerencia o fluxo do programa
int main() {
    struct Territorio *mapa = NULL; // Ponteiro para o vetor de territórios
    int totalTerritorios;
    int territoriosCadastrados = 0;
    int opcao;

    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Alocação Dinâmica
    // Pede ao usuário o número total de territórios para alocar
    printf("==========================================\n");
    printf("      CONFIGURACAO INICIAL DO JOGO\n");
    printf("==========================================\n");
    printf("Digite o numero total de territorios no mapa: ");
    scanf("%d", &totalTerritorios);
    limparBuffer();

    // Aloca memória para o vetor de territórios usando calloc
    // calloc inicializa a memória com zeros, o que é uma boa prática
    mapa = (struct Territorio*) calloc(totalTerritorios, sizeof(struct Territorio));

    // Verifica se a alocação de memória foi bem-sucedida
    if (mapa == NULL) {
        printf("Erro ao alocar memoria! O programa sera encerrado.\n");
        return 1; // Retorna um código de erro
    }

    // Laço principal do Menu
    do {
        // Exibe o menu de opções
        printf("\n==========================================\n");
        printf("                   JOGO WAR\n");
        printf("==========================================\n");
        printf("1. Cadastrar um novo territorio\n");
        printf("2. Listar todos os territorios\n");
        printf("3. Atacar um territorio\n");
        printf("0. Sair\n");
        printf("------------------------------------------\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
        limparBuffer(); // Limpa o buffer de entrada

        // Processa a opção escolhida
        switch (opcao) {
            case 1: // Cadastrar
                if (territoriosCadastrados < totalTerritorios) {
                    // Passa o ponteiro para a posição correta do vetor
                    cadastrarTerritorio(mapa + territoriosCadastrados, territoriosCadastrados);
                    territoriosCadastrados++;
                } else {
                    printf("\nLimite de territorios atingido!\n");
                }
                printf("\nPressione ENTER para continuar...");
                getchar();
                break;

            case 2: // Listar
                listarTerritorios(mapa, territoriosCadastrados);
                printf("\nPressione ENTER para continuar...");
                getchar();
                break;
            
            case 3: // Atacar
                if (territoriosCadastrados < 2) {
                    printf("\nE necessario ter pelo menos 2 territorios cadastrados para atacar.\n");
                } else {
                    int idAtacante, idDefensor;
                    printf("\n--- Simulação de Ataque ---\n");
                    listarTerritorios(mapa, territoriosCadastrados);

                    printf("\nEscolha o territorio atacante (pelo numero): ");
                    scanf("%d", &idAtacante);
                    limparBuffer();

                    printf("Escolha o territorio defensor (pelo numero): ");
                    scanf("%d", &idDefensor);
                    limparBuffer();

                    // Converte ID para índice do vetor (ID 1 -> índice 0)
                    idAtacante--; 
                    idDefensor--;

                    // Validações
                    if (idAtacante < 0 || idAtacante >= territoriosCadastrados ||
                        idDefensor < 0 || idDefensor >= territoriosCadastrados) {
                        printf("\nNumero de territorio invalido!\n");
                    } else if (idAtacante == idDefensor) {
                        printf("\nUm territorio nao pode atacar a si mesmo!\n");
                    } else if (strcmp((mapa + idAtacante)->cor, (mapa + idDefensor)->cor) == 0) {
                        printf("\nVoce nao pode atacar um territorio da sua propria cor!\n");
                    } else {
                        // Chama a função de ataque passando os ponteiros
                        atacar(mapa + idAtacante, mapa + idDefensor);
                        printf("\n--- Territorios Atualizados ---\n");
                        listarTerritorios(mapa, territoriosCadastrados);
                    }
                }
                printf("\nPressione ENTER para continuar...");
                getchar();
                break;

            case 0: // Sair
                printf("\nSaindo do programa...\n");
                break;

            default: // Opção inválida
                printf("\nOpcao invalida! Tente novamente.\n");
                printf("\nPressione ENTER para continuar...");
                getchar();
                break;
        }

    } while (opcao != 0);

    
    // Libera a memória alocada antes de encerrar o programa
    liberarMemoria(mapa);

    return 0;
}


// Limpa o buffer de entrada do teclado.

 
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/*
  Cadastra os dados de um novo território.
  territorio Ponteiro para a struct Territorio a ser preenchida.
  indice O índice atual para exibição ao usuário.
 */
void cadastrarTerritorio(struct Territorio* territorio, int indice) {
    printf("\n--- Cadastro do Territorio %d ---\n", indice + 1);
    
    printf("Digite o nome do territorio: ");
    fgets(territorio->nome, 50, stdin);
    territorio->nome[strcspn(territorio->nome, "\n")] = '\0'; // Remove o '\n'

    printf("Digite a cor do exercito: ");
    fgets(territorio->cor, 50, stdin);
    territorio->cor[strcspn(territorio->cor, "\n")] = '\0'; // Remove o '\n'

    printf("Digite o numero de tropas: ");
    scanf("%d", &territorio->tropas);
    limparBuffer();
    
    printf("\nTerritorio cadastrado com sucesso!\n");
}

/*
  Exibe a lista de todos os territórios cadastrados.
  mapa Ponteiro para o início do vetor de territórios.
  totalTerritorios O número de territórios atualmente cadastrados.
 */
void listarTerritorios(struct Territorio* mapa, int totalTerritorios) {
    printf("\n--- Lista de Territorios Cadastrados ---\n");
    if (totalTerritorios == 0) {
        printf("Nenhum territorio cadastrado ate o momento.\n");
    } else {
        for (int i = 0; i < totalTerritorios; i++) {
            // Acessa os dados usando o ponteiro e o operador seta (->)
            struct Territorio* atual = mapa + i;
            printf("------------------------------------------\n");
            printf("Territorio %d:\n", i + 1);
            printf("Nome: %s\n", atual->nome);
            printf("Cor do exercito: %s\n", atual->cor);
            printf("Numero de tropas: %d\n", atual->tropas);
        }
        printf("------------------------------------------\n");
    }
}

/*
   Simula um ataque entre dois territórios.
   atacante Ponteiro para o território que está atacando.
   defensor Ponteiro para o território que está defendendo.
 */
void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    // Simula a rolagem de dados de 1 a 6
    int dadoAtaque = (rand() % 6) + 1;
    int dadoDefesa = (rand() % 6) + 1;

    printf("\nRolagem de Dados:\n");
    printf(" -> Atacante (%s): %d\n", atacante->nome, dadoAtaque);
    printf(" -> Defensor (%s): %d\n", defensor->nome, dadoDefesa);

    // Compara os resultados dos dados para determinar o vencedor
    if (dadoAtaque > dadoDefesa) {
        printf("\nRESULTADO: VITORIA DO ATACANTE!\n");
        printf("O territorio %s foi conquistado por %s!\n", defensor->nome, atacante->nome);
        
        // Atualiza os dados do território defensor
        // Transfere a cor do atacante para o defensor
        strcpy(defensor->cor, atacante->cor);
        
        // Transfere metade das tropas do atacante para o novo território
        defensor->tropas = atacante->tropas / 2;

    } else { // Empate ou vitória do defensor
        printf("\nRESULTADO: VITORIA DO DEFENSOR!\n");
        printf("O ataque de %s a %s falhou!\n", atacante->nome, defensor->nome);
        
        // O atacante perde uma tropa como penalidade pela derrota
        atacante->tropas--;
        if (atacante->tropas < 0) {
            atacante->tropas = 0; // Garante que as tropas não fiquem negativas
        }
    }
}

/**
 * @brief Libera a memória alocada dinamicamente para o mapa.
 * @param mapa Ponteiro para a memória que deve ser liberada.
 */
void liberarMemoria(struct Territorio* mapa) {
    printf("Liberando memoria alocada...\n");
    free(mapa); // Libera o bloco de memória
    mapa = NULL; // Boa prática: aponta o ponteiro para NULL após liberar
}
