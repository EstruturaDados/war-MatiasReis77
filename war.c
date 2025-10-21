#include <stdio.h>
#include <stdlib.h> // Para malloc, calloc, free, rand, srand
#include <string.h> // Para strcpy, strcmp, strcspn
#include <time.h>   // Para time()

// Estrutura para armazenar os dados de um território

struct Territorio {
    char nome[50];
    char cor[50];
    int tropas;
};

// Funções de missão
void atribuirMissao(char** destino, char* missoes[], int totalMissoes);
void exibirMissao(const char* missao, const char* nomeJogador);
int verificarMissao(const char* missao, const struct Territorio* mapa, int totalTerritorios, const char* corJogador, const char* corOponente);

// Funções existentes
void limparBuffer();
void cadastrarTerritorio(struct Territorio* territorio, int indice);
void listarTerritorios(struct Territorio* mapa, int totalTerritorios);
void atacar(struct Territorio* atacante, struct Territorio* defensor);
void liberarMemoria(struct Territorio* mapa, char* missao1, char* missao2);


// Função principal 
int main() {
    struct Territorio *mapa = NULL; // Ponteiro para o vetor de territórios
    int totalTerritorios;
    int territoriosCadastrados = 0;
    int opcao;
    int jogoTerminou = 0;

    // Lógica dos Jogadores e Missões 
    char corJogador1[10] = "";
    char corJogador2[10] = "";
    char* missaoJogador1 = NULL;
    char* missaoJogador2 = NULL;
    int jogoConfigurado = 0; // Flag para controlar se os jogadores já foram definidos

    // Vetor de missões 
    char* missoes[] = {
        "Controlar 4 territorios no mapa.",
        "Eliminar completamente o jogador adversario.",
        "Conquistar o territorio chamado 'Brasil'.",
        "Possuir um total de 25 tropas no mapa.",
        "Manter 15 tropas em um unico territorio."
    };
    int totalMissoes = sizeof(missoes) / sizeof(missoes[0]);

    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    //  Alocação Dinâmica 
    printf("==========================================\n");
    printf("      CONFIGURACAO INICIAL DO JOGO\n");
    printf("==========================================\n");
    printf("Digite o numero total de territorios no mapa: ");
    scanf("%d", &totalTerritorios);
    limparBuffer();

    mapa = (struct Territorio*) calloc(totalTerritorios, sizeof(struct Territorio));

    if (mapa == NULL) {
        printf("Erro ao alocar memoria! O programa sera encerrado.\n");
        return 1;
    }

    // Laço principal do Menu
    do {
        printf("\n==========================================\n");
        printf("               JOGO TERRITORIOS\n");
        printf("==========================================\n");
        printf("1. Cadastrar um novo territorio\n");
        printf("2. Listar todos os territorios\n");
        printf("3. Atacar um territorio\n");
        printf("0. Sair\n");
        printf("------------------------------------------\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1: // Cadastrar
                if (territoriosCadastrados < totalTerritorios) {
                    cadastrarTerritorio(mapa + territoriosCadastrados, territoriosCadastrados);
                    
                    // Lógica para configurar os jogadores com base nos 2 primeiros territórios
                    if (territoriosCadastrados == 0) { // Primeiro território cadastrado
                        strcpy(corJogador1, (mapa + 0)->cor);
                    } else if (territoriosCadastrados == 1) { // Segundo território cadastrado
                        strcpy(corJogador2, (mapa + 1)->cor);

                        // Validação para garantir que os jogadores tenham cores diferentes
                        if (strcmp(corJogador1, corJogador2) == 0) {
                            printf("\nAVISO: As cores dos dois primeiros territorios sao iguais.\n");
                            printf("Para um jogo de 2 jogadores, por favor, use cores diferentes.\n");
                          
                        } else {
                            printf("\n--- JOGO CONFIGURADO PARA 2 JOGADORES ---\n");
                            printf("Jogador 1: Cor '%s'\n", corJogador1);
                            printf("Jogador 2: Cor '%s'\n", corJogador2);

                            // Atribui e exibe as missões para cada jogador
                            atribuirMissao(&missaoJogador1, missoes, totalMissoes);
                            atribuirMissao(&missaoJogador2, missoes, totalMissoes);
                            
                            printf("\n");
                            exibirMissao(missaoJogador1, "Jogador 1");
                            exibirMissao(missaoJogador2, "Jogador 2");
                            printf("------------------------------------------\n");

                            jogoConfigurado = 1;
                        }
                    }
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
                if (!jogoConfigurado) {
                    printf("\nCadastre pelo menos 2 territorios com cores diferentes para atacar.\n");
                } else {
                    int idAtacante, idDefensor;
                    printf("\n--- Simulacao de Ataque ---\n");
                    listarTerritorios(mapa, territoriosCadastrados);

                    printf("\nEscolha o territorio atacante (pelo numero): ");
                    scanf("%d", &idAtacante);
                    limparBuffer();

                    printf("Escolha o territorio defensor (pelo numero): ");
                    scanf("%d", &idDefensor);
                    limparBuffer();

                    idAtacante--; 
                    idDefensor--;

                    if (idAtacante < 0 || idAtacante >= territoriosCadastrados ||
                        idDefensor < 0 || idDefensor >= territoriosCadastrados) {
                        printf("\nNumero de territorio invalido!\n");
                    } else if (idAtacante == idDefensor) {
                        printf("\nUm territorio nao pode atacar a si mesmo!\n");
                    } else if (strcmp((mapa + idAtacante)->cor, (mapa + idDefensor)->cor) == 0) {
                        printf("\nVoce nao pode atacar um territorio da sua propria cor!\n");
                    } else {
                        atacar(mapa + idAtacante, mapa + idDefensor);
                        printf("\n--- Territorios Atualizados ---\n");
                        listarTerritorios(mapa, territoriosCadastrados);

                        //  Verificação de Missão ao final do turno
                        if (verificarMissao(missaoJogador1, mapa, territoriosCadastrados, corJogador1, corJogador2)) {
                            printf("\n==========================================\n");
                            printf("  JOGADOR 1 (Cor %s) VENCEU O JOGO!\n", corJogador1);
                            printf("  Missao Cumprida: %s\n", missaoJogador1);
                            printf("==========================================\n");
                            jogoTerminou = 1;
                        }
                        if (!jogoTerminou && verificarMissao(missaoJogador2, mapa, territoriosCadastrados, corJogador2, corJogador1)) {
                            printf("\n==========================================\n");
                            printf("  JOGADOR 2 (Cor %s) VENCEU O JOGO!\n", corJogador2);
                            printf("  Missao Cumprida: %s\n", missaoJogador2);
                            printf("==========================================\n");
                            jogoTerminou = 1;
                        }
                    }
                }
                if (!jogoTerminou) {
                    printf("\nPressione ENTER para continuar...");
                    getchar();
                }
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

    } while (opcao != 0 && !jogoTerminou);

    // Libera toda a memória alocada dinamicamente
    liberarMemoria(mapa, missaoJogador1, missaoJogador2);

    return 0;
}


// Limpa o buffer de entrada do teclado 
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Cadastra os dados de um novo território.
 
void cadastrarTerritorio(struct Territorio* territorio, int indice) {
    printf("\n--- Cadastro do Territorio %d ---\n", indice + 1);
    
    printf("Digite o nome do territorio: ");
    fgets(territorio->nome, 50, stdin);
    territorio->nome[strcspn(territorio->nome, "\n")] = '\0';

    printf("Digite a cor do exercito: ");
    fgets(territorio->cor, 50, stdin);
    territorio->cor[strcspn(territorio->cor, "\n")] = '\0';

    printf("Digite o numero de tropas: ");
    scanf("%d", &territorio->tropas);
    limparBuffer();
    
    printf("\nTerritorio cadastrado com sucesso!\n");
}


// Exibe a lista de todos os territórios cadastrados.

void listarTerritorios(struct Territorio* mapa, int totalTerritorios) {
    printf("\n--- Lista de Territorios Cadastrados ---\n");
    if (totalTerritorios == 0) {
        printf("Nenhum territorio cadastrado ate o momento.\n");
    } else {
        for (int i = 0; i < totalTerritorios; i++) {
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


// Simula um ataque entre dois territórios.

void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    if (atacante->tropas < 2) {
        printf("\nO territorio atacante precisa de pelo menos 2 tropas para atacar!\n");
        return;
    }
    
    int dadoAtaque = (rand() % 6) + 1;
    int dadoDefesa = (rand() % 6) + 1;

    printf("\nDados:\n");
    printf(" -> Atacante (%s): %d\n", atacante->nome, dadoAtaque);
    printf(" -> Defensor (%s): %d\n", defensor->nome, dadoDefesa);

    if (dadoAtaque > dadoDefesa) {
        printf("\nRESULTADO: VITORIA DO ATACANTE!\n");
        printf("O territorio %s foi conquistado por %s!\n", defensor->nome, atacante->nome);
        
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = 1; // O território conquistado fica com 1 tropa
        atacante->tropas--;   // O atacante perde a tropa que se moveu
    } else {
        printf("\nRESULTADO: VITORIA DO DEFENSOR!\n");
        printf("O ataque de %s a %s falhou!\n", atacante->nome, defensor->nome);
        
        atacante->tropas--;
        if (atacante->tropas < 1) {
            atacante->tropas = 1; // Garante que o território não fique sem tropas
        }
    }
}

// Sorteia uma missão do vetor de missões, aloca memória e a copia para o jogador.

void atribuirMissao(char** destino, char* missoes[], int totalMissoes) {
    int indiceSorteado = rand() % totalMissoes;
    
    // Aloca memória para a string da missão (+1 para o caractere nulo '\0')
    *destino = (char*) malloc(strlen(missoes[indiceSorteado]) + 1);
    if (*destino == NULL) {
        printf("Erro ao alocar memoria para a missao!\n");
        exit(1); // Encerra o programa em caso de falha de alocação crítica
    }

    strcpy(*destino, missoes[indiceSorteado]);
}

// Exibe a missão atribuída a um jogador.

 
void exibirMissao(const char* missao, const char* nomeJogador) {
    if (missao != NULL) {
        printf("Missao do %s: %s\n", nomeJogador, missao);
    }
}

/*
  Avalia se a missão de um jogador foi cumprida.
  missao A string da missão a ser verificada.
  mapa Ponteiro para o vetor de territórios.
  totalTerritorios Número total de territórios cadastrados.
  corJogador Cor do jogador cuja missão está sendo verificada.
  corOponente Cor do jogador adversário.
  Retorna 1 se a missão foi cumprida, 0 caso contrário.
 */
int verificarMissao(const char* missao, const struct Territorio* mapa, int totalTerritorios, const char* corJogador, const char* corOponente) {
    if (missao == NULL) return 0;

    // Lógica para "Controlar 4 territorios no mapa."
    if (strcmp(missao, "Controlar 4 territorios no mapa.") == 0) {
        int contador = 0;
        for (int i = 0; i < totalTerritorios; i++) {
            if (strcmp((mapa + i)->cor, corJogador) == 0) {
                contador++;
            }
        }
        return contador >= 4;
    }

    // Lógica para "Eliminar completamente o jogador adversario."
    if (strcmp(missao, "Eliminar completamente o jogador adversario.") == 0) {
        for (int i = 0; i < totalTerritorios; i++) {
            if (strcmp((mapa + i)->cor, corOponente) == 0) {
                return 0; // Se encontrar um território do oponente, a missão não foi cumprida
            }
        }
        return 1; // Se o loop terminar, o oponente não tem mais territórios
    }

    // Lógica para "Conquistar o territorio chamado 'Brasil'."
    if (strcmp(missao, "Conquistar o territorio chamado 'Brasil'.") == 0) {
        for (int i = 0; i < totalTerritorios; i++) {
            if (strcmp((mapa + i)->nome, "Brasil") == 0 && strcmp((mapa + i)->cor, corJogador) == 0) {
                return 1;
            }
        }
    }

    // Lógica para "Possuir um total de 25 tropas no mapa."
    if (strcmp(missao, "Possuir um total de 25 tropas no mapa.") == 0) {
        int somaTropas = 0;
        for (int i = 0; i < totalTerritorios; i++) {
            if (strcmp((mapa + i)->cor, corJogador) == 0) {
                somaTropas += (mapa + i)->tropas;
            }
        }
        return somaTropas >= 25;
    }
    
    // Lógica para "Manter 15 tropas em um unico territorio."
    if (strcmp(missao, "Manter 15 tropas em um unico territorio.") == 0) {
        for (int i = 0; i < totalTerritorios; i++) {
            if (strcmp((mapa + i)->cor, corJogador) == 0 && (mapa + i)->tropas >= 15) {
                return 1;
            }
        }
    }

    return 0; // Nenhuma missão correspondente ou condição não atingida
}

/**
 * @brief Libera toda a memória alocada dinamicamente para o mapa e as missões.
 * @param mapa Ponteiro para o mapa a ser liberado.
 * @param missao1 Ponteiro para a missão do jogador 1.
 * @param missao2 Ponteiro para a missão do jogador 2.
 */
void liberarMemoria(struct Territorio* mapa, char* missao1, char* missao2) {
    printf("Liberando memoria alocada...\n");
    
    // Libera o bloco de memória do mapa
    free(mapa);
    mapa = NULL;
    
    // Libera a memória de cada missão se tiver sido alocada
    if (missao1 != NULL) {
        free(missao1);
        missao1 = NULL;
    }
    if (missao2 != NULL) {
        free(missao2);
        missao2 = NULL;
    }
    printf("Memoria liberada com sucesso.\n");
}
