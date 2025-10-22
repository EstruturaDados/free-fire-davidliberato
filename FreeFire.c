#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Código da Ilha – Edição Free Fire
// Nível: Mestre
// Este programa simula o gerenciamento avançado de uma mochila com componentes coletados durante a fuga de uma ilha.
// Ele introduz ordenação com critérios e busca binária para otimizar a gestão dos recursos.

#define MAX_STR_LEN 50

typedef struct{
    char *nome;
    char *tipo;
    int quantidade;
    int prioridade;
} Item;

typedef struct {
    Item *itens;
    int quantidade;
    int capacidade;
    int status_ordenacao;
} Mochila;

void inicializarMochila(Mochila*mochila , int capacidade){
    mochila->itens = (Item*) malloc(capacidade * sizeof(Item));
    if (!mochila->itens){
        printf("Erro: Falha na alocação de memoria para mochila!\n");
        exit(1);
    }
    mochila->quantidade = 0;
    mochila->capacidade = capacidade;
    mochila->status_ordenacao = 0;
}

void inserirItem(Mochila*mochila, const char* nome, const char* tipo, int qtd,  int prioridade){
    for ( int i = 0; i < mochila->quantidade; i++ ){
        if (strcmp(mochila->itens[i].nome, nome ) == 0){
            mochila->itens[i].quantidade += qtd;
            printf("Adicionando %d ao Item \"%s\". Total: %d. \n", qtd,nome, mochila->itens[i].quantidade);
            return;
        }
    }

    if (mochila->quantidade >= mochila->capacidade){
        printf("Mochila Cheia!! Não foi possivel acrescentar o novo item \"%s\".", nome);
        return;
    }
   

    mochila->itens[mochila->quantidade].nome = strdup(nome);
    mochila->itens[mochila->quantidade].tipo = strdup(tipo);

    if (!mochila->itens[mochila->quantidade].nome || !mochila->itens[mochila->quantidade].tipo){
        printf("Erro: Falha a alocação da memoria para as strings.\n");
        return;
    }
    mochila->itens[mochila->quantidade].quantidade = qtd;
    mochila->itens[mochila->quantidade].prioridade = prioridade;

    mochila->quantidade++;

    printf("\nItem \"%s\" Inserido com sucesso.\n", nome);
}


void removerItem(Mochila*mochila,const char* nome, int qtd){
    for ( int i= 0; i < mochila->quantidade; i++){
        if (strcmp(mochila->itens[i].nome, nome) == 0){
            if (qtd < mochila->itens[i].quantidade){
                // Diminuindo apenas quantidade especifica;
                mochila->itens[i].quantidade -= qtd;
                printf("\nRemovendo %d do Item \"%s\". restam: %d. \n", qtd,nome, mochila->itens[i].quantidade);
                return;
            } else {
                // Removendo todo slot.
                printf("\nRemovendo Item \"%s\"\n", nome);

                free(mochila->itens[i].nome);
                free(mochila->itens[i].tipo);
                for ( int j = i; j < mochila->quantidade -1; j++){
                    mochila->itens[j] = mochila->itens[j+1];
                }
                mochila->quantidade--;
                return;
            }
        }

    }
}


void listarItens(const Mochila*mochila){
    
    if ( mochila->quantidade == 0){
        printf("Mochila está Vazia!\n");
        return;
    }
    printf("+----+-----------------+--------------+------------+--------------+\n");
    printf("| %2s | %-15s | %-12s | %-9s | %-12s |\n",
           "ID", "Item", "Tipo", "Quantidade", "Prioridade");
    printf("+----+-----------------+--------------+------------+--------------+\n");
    
    for ( int i = 0; i < mochila->quantidade; i++){
        printf("| %2d | %-15s | %-12s | %-10d | %-12d |\n",
            i+1, 
            mochila->itens[i].nome,
            mochila->itens[i].tipo, 
            mochila->itens[i].quantidade,
            mochila->itens[i].prioridade);
        }
    printf("+----+-----------------+--------------+------------+--------------+\n");

}


void liberarMemoria(Mochila*mochila){

    for ( int i = 0; i < mochila->quantidade; i++){
        free(mochila->itens[i].nome);
        free(mochila->itens[i].tipo);
    }
    free(mochila->itens);
    mochila->itens = NULL;
    mochila->capacidade = 0;
    mochila->quantidade = 0;
}


void limparBufferEntrada(){
    int c;
    while ((c = getchar())!= '\n' && c != EOF);
}

void exibirMenu(int qtd_atual,int qtd_max, int status_ordenacao){
    
    printf("=====================================\n");
    printf("--- MOCHILA DA SOBREVIVENCIA ---\n");
    printf("=====================================\n");
    printf("Itens da Mochila: %d/%d\n", qtd_atual, qtd_max);
    printf("Status da Ordenação por NOME: %s\n\n",status_ordenacao?"ORDENADO": "NÃO ORDENADO");


    printf("1. Inserir Item.\n");
    printf("2. Remover Item.\n");
    printf("3. Listar Itens.\n");
    printf("4. Realizar Busca Sequencial por nome.\n");
    printf("5. Ordenar os itens por critério ( nome, tipo, priorirade ).\n");
    printf("6. Realizar Busca Binária Por Nome.\n");
    printf("0. Sair do programa.\n");
    printf("Escolha sua opção: ");
       
}

int buscaBinariaPorNome_aux( const Mochila *mochila, const char* item, int inicio, int fim){

    if (inicio > fim){
        return -1;
    }

    int meio = (inicio + fim) / 2;

    int compara = strcmp(mochila->itens[meio].nome, item);

    if (compara == 0){
        return meio;
        // Se o que eu busco for MAIOR, preciso DESCARTAR o FIM (novoFim = meio - 1)
        // Se o que eu busco for MENOR, preciso DESCARTAR o INICIO ( novoInicio = meio +1)
    } else if (compara > 0){
        return buscaBinariaPorNome_aux(mochila, item, inicio, meio -1);
    } else {
        return buscaBinariaPorNome_aux(mochila, item, meio +1, fim);
    }

}

int buscaBinariaPorNome( const Mochila *mochila, const char* item){

    return buscaBinariaPorNome_aux(mochila, item, 0, mochila->quantidade -1);
}

int buscaSequencialPorNome_aux(const Mochila *mochila, const char*item, int indice){
    if ( indice >= mochila->quantidade){
        return -1;
    }
    if (strcmp(mochila->itens[indice].nome, item) == 0){
        return indice;
    }
    return buscaSequencialPorNome_aux(mochila, item, indice + 1);
}

int buscaSequencialPorNome(const Mochila *mochila, const char* item){

    return buscaSequencialPorNome_aux(mochila, item, 0);
}

void menuDeOrdenacao(){

    printf("\n\n----- MENU DE ORDENAÇÃO ------\n");
    printf("Qual será o critério da ordenação ?\n");
    printf("1 - Ordenação por NOME.\n");
    printf("2 - Ordenação por TIPO.\n");
    printf("3 - Ordenação por PRIORIDADE.\n");
    printf("0 - Voltar ao menu principal.\n");
}

void ordenacaoPorNome(Mochila *mochila){

    for ( int i = 1; i < mochila->quantidade; i++){
        Item chave = mochila->itens[i];
        int j = i - 1;


        while (j >= 0 && strcmp(mochila->itens[j].nome, chave.nome) > 0){
            mochila->itens[j+1] = mochila->itens[j];
            j--;
        }
        mochila->itens[j + 1] = chave;
    }
    mochila->status_ordenacao = 1;

}

void ordenacaoPorTipo(Mochila *mochila){
    for ( int i = 1; i < mochila->quantidade; i++){
        Item chave = mochila->itens[i];
        int j = i - 1;


        while (j >= 0 && strcmp(mochila->itens[j].tipo, chave.tipo) > 0){
            mochila->itens[j+1] = mochila->itens[j];
            j--;
        }
        mochila->itens[j + 1] = chave;
    }

}

void ordenacaoPorPrioridade(Mochila *mochila){
    for ( int i = 1; i < mochila->quantidade; i++){
        Item chave = mochila->itens[i];
        int j = i - 1;


        while (j >= 0 && mochila->itens[j].prioridade < chave.prioridade){
            mochila->itens[j+1] = mochila->itens[j];
            j--;
        }
        mochila->itens[j + 1] = chave;
    }

}

int main(){
    // Menu principal com opções:
    // 1. Adicionar um item
    // 2. Remover um item
    // 3. Listar todos os itens
    // 4. Ordenar os itens por critério (nome, tipo, prioridade)
    // 5. Realizar busca binária por nome
    // 0. Sair
    
    // A estrutura switch trata cada opção chamando a função correspondente.
    // A ordenação e busca binária exigem que os dados estejam bem organizados.
    int capacidade_max_atual = 10;
    Mochila mochila;
    inicializarMochila(&mochila, capacidade_max_atual);
    int opcao, opcao_ordenacao;

    do {
        char nome[MAX_STR_LEN], tipo[MAX_STR_LEN];
        int qtd, prioridade, resultado;

        exibirMenu(mochila.quantidade, mochila.capacidade, mochila.status_ordenacao);

        if (scanf("%d", &opcao) != 1) {
            printf("\nEntrada inválida! Digite um número.\n");
            limparBufferEntrada();
            opcao = -1;
            continue;
        }
        limparBufferEntrada();


        switch(opcao){
            case 1:
                printf("Digite o nome Item que será inserido: ");
                fgets(nome, MAX_STR_LEN, stdin);
                nome[strcspn(nome, "\n")] = 0;

                printf("Digite o Tipo do item: ");
                fgets(tipo, MAX_STR_LEN, stdin);
                tipo[strcspn(tipo, "\n")] = 0;
                
                printf("Digite a quantidade de itens: ");
                scanf("%d", &qtd);
                limparBufferEntrada();

                printf("Digite o Nivel de prioridade: ");
                scanf("%d", &prioridade);
                limparBufferEntrada();

                inserirItem(&mochila, nome, tipo, qtd, prioridade);

                listarItens(&mochila);

                printf("\nPressione Enter para continuar...");
                getchar();
                break;

            case 2:
                printf("Digite o nome Item que será Removido: ");
                fgets(nome, MAX_STR_LEN, stdin);
                nome[strcspn(nome, "\n")] = 0;
                
                printf("Digite a quantidade de itens que serão removidos: ");
                scanf("%d", &qtd);
                limparBufferEntrada();

                removerItem(&mochila, nome, qtd);

                listarItens(&mochila);

                printf("\nPressione Enter para continuar...");
                getchar();
                break;

            case 3:
                listarItens(&mochila);
                printf("\nPressione Enter para continuar...");
                getchar();
                break;
                
            case 4: 
                if ( mochila.quantidade == 0){
                    printf("A mochila está vazia. Insira itens para buscar. \n");
                    printf("\nPressione Enter para continuar...");
                    getchar();
                    break;

                }
                printf(" \nPor qual item deseja buscar ?: ");
                fgets(nome, MAX_STR_LEN, stdin);
                nome[strcspn(nome, "\n")] = 0;
                
                resultado = buscaSequencialPorNome(&mochila, nome);
                
                if ( resultado != -1){
                    printf("\n-------- Item encontrado ------ \n");
                    printf("Nome: %s\n", mochila.itens[resultado].nome);
                    printf("Tipo: %s\n",mochila.itens[resultado].tipo);
                    printf("Quantidade: %d\n", mochila.itens[resultado].quantidade);
                    printf("Prioridade: %d\n", mochila.itens[resultado].prioridade);

                } else {
                    printf("\nItem NÃO ENCONTRADO na lista. \n\n");
                }
                
                printf("\nPressione Enter para continuar...");
                getchar();
                break;
            
            case 5:
                menuDeOrdenacao();
                scanf("%d", &opcao_ordenacao);
                limparBufferEntrada();
                switch(opcao_ordenacao){
                    case 1:
                        ordenacaoPorNome(&mochila);
                        listarItens(&mochila);
                        break;
                    case 2:
                        ordenacaoPorTipo(&mochila);
                        listarItens(&mochila);
                        break;
                    case 3:
                        ordenacaoPorPrioridade(&mochila);
                        listarItens(&mochila);
                        break;
                    case 0:
                        printf("\nVoltando ao menu principal... \n");
                        break;

                    default:
                        printf("\nOpção Inválida, tente novamente...\n");
                        continue;

                }
                printf("\nPressione Enter para continuar...");
                getchar();
                break;
            case 6:
                if (mochila.status_ordenacao){
                    printf(" \nPor qual item deseja buscar ?: ");
                    fgets(nome, MAX_STR_LEN, stdin);
                    nome[strcspn(nome, "\n")] = 0;
                
                    resultado = buscaBinariaPorNome(&mochila, nome);

                    if ( resultado != -1){
                    printf("\n-------- Item encontrado ------ \n");
                    printf("Nome: %s\n", mochila.itens[resultado].nome);
                    printf("Tipo: %s\n",mochila.itens[resultado].tipo);
                    printf("Quantidade: %d\n", mochila.itens[resultado].quantidade);
                    printf("Prioridade: %d\n", mochila.itens[resultado].prioridade);

                } else {
                    printf("\nItem NÃO ENCONTRADO na lista. \n\n");
                }
                } else {
                    printf("A busca binária requer que a mochila esteja ordenada por NOME.\n");
                    printf("Use a opção 5 para organizar a mochila.\n");
                }
                printf("\nPressione Enter para continuar...");
                getchar();
                break;
            case 0:
                printf("\nSaindo do programa... \n");
                break;


            default:
                printf("\nOpção Inválida, tente novamente...\n");
                printf("\nPressione Enter para continuar...");
                getchar();
                break;
        }


    } while (opcao != 0);

    liberarMemoria(&mochila);

    return 0;

}

// Struct Item:
// Representa um componente com nome, tipo, quantidade e prioridade (1 a 5).
// A prioridade indica a importância do item na montagem do plano de fuga.

// Enum CriterioOrdenacao:
// Define os critérios possíveis para a ordenação dos itens (nome, tipo ou prioridade).

// Vetor mochila:
// Armazena até 10 itens coletados.
// Variáveis de controle: numItens (quantidade atual), comparacoes (análise de desempenho), ordenadaPorNome (para controle da busca binária).

// limparTela():
// Simula a limpeza da tela imprimindo várias linhas em branco.

// exibirMenu():
// Apresenta o menu principal ao jogador, com destaque para status da ordenação.

// inserirItem():
// Adiciona um novo componente à mochila se houver espaço.
// Solicita nome, tipo, quantidade e prioridade.
// Após inserir, marca a mochila como "não ordenada por nome".

// removerItem():
// Permite remover um componente da mochila pelo nome.
// Se encontrado, reorganiza o vetor para preencher a lacuna.

// listarItens():
// Exibe uma tabela formatada com todos os componentes presentes na mochila.

// menuDeOrdenacao():
// Permite ao jogador escolher como deseja ordenar os itens.
// Utiliza a função insertionSort() com o critério selecionado.
// Exibe a quantidade de comparações feitas (análise de desempenho).

// insertionSort():
// Implementação do algoritmo de ordenação por inserção.
// Funciona com diferentes critérios de ordenação:
// - Por nome (ordem alfabética)
// - Por tipo (ordem alfabética)
// - Por prioridade (da mais alta para a mais baixa)

// buscaBinariaPorNome():
// Realiza busca binária por nome, desde que a mochila esteja ordenada por nome.
// Se encontrar, exibe os dados do item buscado.
// Caso contrário, informa que não encontrou o item.
