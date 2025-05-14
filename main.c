#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_HEAP 20

// ---------------------------
// STRUCTS DE LISTA-CADASTRAR
// ---------------------------

// struct de DATA
typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

// struct de REGISTRO
typedef struct {
    char nome[100];
    int idade;
    char rg[20];
    Data *entrada; 
} Registro;

// struct de ELISTA (elemento de lista - CÉLULA)
typedef struct ELista {
    Registro *dados;          
    struct ELista *proximo;  
} ELista;

// struct de LISTA
typedef struct {
    ELista *inicio; 
    int qtde;      
} Lista;


// ---------------------------
// STRUCTS DE FILA-ATENDIMENTO
// ---------------------------

// Struct de EFILA (elemento da FILA)
typedef struct EFila {
    struct EFila *proximo;  
    struct EFila *anterior;     
    Registro *dados;           
} EFila;

// Struct de FILA
typedef struct {
    EFila *head;               
    EFila *tail;               
    int qtde;
} Fila;


// ----------------------------------------
// STRUCTS DE HEAP-ATENDIMENTO PRIORITARIO
// ----------------------------------------

// Struct de HEAP
typedef struct {
    Registro *dados[MAX_HEAP];
    int qtde;
} Heap;

// --------------------------
// STRUCTS DE ABB-PESQUISAR
// --------------------------

// Struct de EABB (elemento da ABB)
typedef struct EABB {
    Registro *dados;
    struct EABB *filhoEsq;
    struct EABB *filhoDir;
} EABB;

// Struct de ABB
typedef struct {
    EABB *raiz;
    int qtde;
} ABB;


// ---------------------------
// STRUCTS DE PILHA-DESFAZER
// ---------------------------

typedef struct Cell {
    struct Cell *anterior;
    struct Cell *proximo;
    char operacao;       
    Registro *paciente;    
} Cell;

typedef struct {
    Cell *top;
    int qtde;
} Stack;




// ------------------------------------
// FUNÇÕES DE ITEM DE MENU: CADASTRAR
// ------------------------------------

// Inicializa a lista
Lista *inicializa_lista() {
    Lista *l = malloc(sizeof(Lista));
    l->inicio = NULL;
    l->qtde = 0;
    return l;
}

// Cria uma nova data
Data *cria_data(int dia, int mes, int ano) {
    Data *d = malloc(sizeof(Data));
    d->dia = dia;
    d->mes = mes;
    d->ano = ano;
    return d;
}

// Insere novo paciente no início da lista
void cadastrar_paciente(Lista *l, Registro paciente) {
    ELista *novo = malloc(sizeof(ELista));
    novo->dados = malloc(sizeof(Registro)); 
    *(novo->dados) = paciente;              
    novo->proximo = l->inicio;
    l->inicio = novo;
    l->qtde++;
}


// Imprime todos os pacientes da lista
void imprimir_lista(const Lista *l) {
    ELista *atual = l->inicio;
    while (atual != NULL) {
        printf("Nome: %s | Idade: %d | RG: %s | Entrada: %02d/%02d/%04d\n",
               atual->dados->nome, atual->dados->idade, atual->dados->rg,
               atual->dados->entrada->dia, atual->dados->entrada->mes, atual->dados->entrada->ano);
        atual = atual->proximo;
    }
}


// Consulta paciente específico
ELista* consultar_paciente(const Lista *l, const char *nome) {
    ELista *atual = l->inicio;
    while (atual != NULL) {
        if (strcmp(atual->dados->nome, nome) == 0) {
            return atual;
        }
    atual = atual->proximo;
    }
    return NULL;
}

// Atualiza dados do paciente
void atualizar_paciente(Lista *l) {
    char nome_busca[100];
    printf("✏️ Digite o nome do paciente que deseja atualizar: ");
    fgets(nome_busca, sizeof(nome_busca), stdin);
    nome_busca[strcspn(nome_busca, "\n")] = '\0';

    ELista *paciente = consultar_paciente(l, nome_busca);
    if (paciente == NULL) {
        printf("❌ Paciente não encontrado.\n");
        return;
    }

    int opcao;
    printf("\n📋 Qual dado deseja atualizar?\n");
    printf("1️⃣  Nome\n");
    printf("2️⃣  Idade\n");
    printf("3️⃣  RG\n");
    printf("4️⃣  Data de entrada\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);
    getchar();

    if (opcao == 1) {
        printf("📝 Novo nome: ");
        fgets(paciente->dados->nome, sizeof(paciente->dados->nome), stdin);
        paciente->dados->nome[strcspn(paciente->dados->nome, "\n")] = '\0';
    } 
    else if (opcao == 2) {
        printf("🎂 Nova idade: ");
        scanf("%d", &paciente->dados->idade);
        getchar();
    } 
    else if (opcao == 3) {
        printf("🆔 Novo RG: ");
        fgets(paciente->dados->rg, sizeof(paciente->dados->rg), stdin);
        paciente->dados->rg[strcspn(paciente->dados->rg, "\n")] = '\0';
    } 
    else if (opcao == 4) {
        int dia, mes, ano;
        printf("📅 Nova data de entrada (dd mm aaaa): ");
        scanf("%d %d %d", &dia, &mes, &ano);
        getchar();
        paciente->dados->entrada->dia = dia;
        paciente->dados->entrada->mes = mes;
        paciente->dados->entrada->ano = ano;
    } 
    else {
        printf("❌ Opção inválida.\n");
        return;
    }

    printf("✅ Dados atualizados com sucesso!\n");
}

// Remove Paciente
void remover_paciente(Lista *l, const char *nome) {
    ELista *atual = l->inicio;
    ELista *anterior = NULL;

    while (atual != NULL) {
        if (strcmp(atual->dados->nome, nome) == 0) {
            if (anterior == NULL) {
                l->inicio = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }

            // Libera memória da data e da célula
            free(atual->dados->entrada);
            free(atual);
            l->qtde--;

            printf("🗑️ Paciente \"%s\" removido com sucesso!\n", nome);
            return;
        }

        anterior = atual;
        atual = atual->proximo;
    }

    printf("❌ Paciente \"%s\" não encontrado.\n", nome);
}


// ----------------------------------
// FUNÇÕES DE ITEM DE MENU: DESFAZER
// ----------------------------------

Cell *start_cell(char valor) {
  Cell *nova = malloc(sizeof(Cell));
  nova->anterior = NULL;
  nova->proximo = NULL;
  nova->operacao = valor;
  nova->paciente = NULL;
  return nova;
}

Stack *start_stack() {
  Stack *stack = malloc(sizeof(Stack));
  stack->qtde = 0;
  stack->top = NULL;
  return stack;
}

void push(Stack *stack, char operacao, Registro *paciente) {
    Cell *nova = malloc(sizeof(Cell));
    nova->operacao = operacao;
    nova->paciente = paciente;
    nova->proximo = stack->top;
    nova->anterior = NULL;
    if (stack->top != NULL) {
        stack->top->anterior = nova;
    }
    stack->top = nova;
    stack->qtde++;
}


Cell *pop(Stack *stack) {
    if (stack->qtde == 0) return NULL;
    Cell *removida = stack->top;
    stack->top = removida->proximo;
    if (stack->top != NULL) stack->top->anterior = NULL;
    stack->qtde--;
    return removida;
}


void imprimir_stack(Stack *stack) {
    printf("\n📜 Log de operações:\n");
    Cell *atual = stack->top;
    while (atual != NULL) {
        if (atual->operacao == 'E') {
            printf("🟢 Enfileiramento de %s\n", atual->paciente ? atual->paciente->nome : "(desconhecido)");
        } else if (atual->operacao == 'D') {
            printf("🔴 Desenfileiramento de %s\n", atual->paciente ? atual->paciente->nome : "(desconhecido)");
        } else {
            printf("❓ Operação desconhecida\n");
        }
        atual = atual->proximo;
    }
    printf("\n");
}




void desfazer_ultima_operacao(Stack *stack, Fila *fila) {
    Cell *ultima = pop(stack);
    if (ultima == NULL) {
        printf("⚠️ Nenhuma operação para desfazer.\n");
        return;
    }

    if (ultima->operacao == 'E') {
        // desfaz enfileiramento
        EFila *removido = fila->tail;
        if (removido == NULL) {
            printf("⚠️ Nenhum paciente na fila para remover.\n");
            return;
        }
        fila->tail = removido->anterior;
        if (fila->tail != NULL) fila->tail->proximo = NULL;
        else fila->head = NULL;
        free(removido);
        fila->qtde--;
        printf("↩️ Desfeita a operação de enfileiramento.\n");
    } else if (ultima->operacao == 'D') {
        // desfaz desenfileiramento
        EFila *nova = malloc(sizeof(EFila));
        nova->dados = ultima->paciente;
        nova->anterior = NULL;
        nova->proximo = fila->head;
        if (fila->head != NULL) fila->head->anterior = nova;
        else fila->tail = nova;
        fila->head = nova;
        fila->qtde++;
        printf("↩️ Desfeita a operação de desenfileiramento. Paciente restaurado: %s\n", nova->dados->nome);
    } else {
        printf("❌ Operação desconhecida.\n");
    }

    free(ultima); // libera a célula da pilha
}


// -------------------------------------
// FUNÇÕES DE ITEM DE MENU: ATENDIMENTO
// -------------------------------------

// Inicializa a fila
Fila *inicializa_fila() {
    Fila *f = malloc(sizeof(Fila));
    f->head = NULL;
    f->tail = NULL;
    f->qtde = 0;
    return f;
}


// Enfileirar paciente
void enfileirar_paciente(Lista *lista, Fila *fila, Stack *stack) {
    char nome_busca[100];
    printf("🔍 Digite o nome do paciente a ser enfileirado: ");
    fgets(nome_busca, sizeof(nome_busca), stdin);
    nome_busca[strcspn(nome_busca, "\n")] = '\0';

    ELista *paciente = consultar_paciente(lista, nome_busca);
    if (paciente == NULL) {
        printf("❌ Paciente não encontrado na lista de cadastro.\n");
        return;
    }

    // Criar uma cópia do registro para enfileirar
    Registro *copia = malloc(sizeof(Registro));
    *copia = *(paciente->dados);


    EFila *nova = malloc(sizeof(EFila));
    nova->dados = copia;
    nova->proximo = NULL;
    nova->anterior = NULL;

    if (fila->qtde == 0) {
        fila->head = fila->tail = nova;
    } else {
        fila->tail->proximo = nova;
        nova->anterior = fila->tail;
        fila->tail = nova;
    }

    fila->qtde++;
    push(stack, 'E', copia); 
    printf("✅ Paciente \"%s\" enfileirado com sucesso!\n", copia->nome);
}

// Desenfileira paciente
void desenfileirar_paciente(Fila *fila, Stack *stack) {
    if (fila->qtde == 0) {
        printf("⚠️ Nenhum paciente na fila de atendimento.\n");
        return;
    }
    EFila *removido = fila->head;
    fila->head = removido->proximo;
    if (fila->head != NULL) fila->head->anterior = NULL;
    else fila->tail = NULL;

    push(stack, 'D', removido->dados); // salva o paciente
    printf("🧑‍⚕️ Paciente atendido: %s\n", removido->dados->nome);
    free(removido);
    fila->qtde--;
}



// Mostra todos os pacientes na fila de atendimento
void mostrar_fila(const Fila *fila) {
    if (fila->qtde == 0) {
        printf("📭 A fila está vazia.\n");
        return;
    }

    printf("📋 Pacientes na fila de atendimento:\n");
    EFila *atual = fila->head;
    int posicao = 1;
    while (atual != NULL) {
        printf("%d️⃣  Nome: %s | Idade: %d | RG: %s | Entrada: %02d/%02d/%04d\n",
               posicao++, atual->dados->nome, atual->dados->idade, atual->dados->rg, atual->dados->entrada->dia, atual->dados->entrada->mes, atual->dados->entrada->ano);
        atual = atual->proximo;
    }
}



// -------------------------------------------------
// FUNÇÕES DE ITEM DE MENU: ATENDIMENTO PRIORITÁRIO
// -------------------------------------------------
int filho_esquerda(int pai) {
    return (2 * pai) + 1;
}

int filho_direita(int pai) {
    return (2 * pai) + 2;
}

int pai(int filho) {
    return (filho - 1) / 2;
}

void peneirar(Heap *h, int pai) {
    int maior = pai;
    int fe = filho_esquerda(pai);
    int fd = filho_direita(pai);

    if (fe < h->qtde && h->dados[fe]->idade > h->dados[maior]->idade) {
        maior = fe;
    }

    if (fd < h->qtde && h->dados[fd]->idade > h->dados[maior]->idade) {
        maior = fd;
    }

    if (maior != pai) {
        Registro* temp = h->dados[pai];
        h->dados[pai] = h->dados[maior];
        h->dados[maior] = temp;
        peneirar(h, maior);
    }
}

void construir(Heap *h) {
    for (int i = (h->qtde / 2) - 1; i >= 0; i--) {
        peneirar(h, i);
    }
}

void inicializar_heap(Heap *h) {
    h->qtde = 0;
    for (int i = 0; i < MAX_HEAP; i++) {
        h->dados[i] = NULL;
    }
}

void inserir_heap(Heap *h, Registro *paciente) {
    if (h->qtde >= MAX_HEAP) {
        printf("❌ Heap cheia. Capacidade máxima atingida.\n");
        return;
    }
    h->dados[h->qtde] = paciente;
    h->qtde++;
    construir(h);
}

void remover_heap(Heap *h) {
    if (h->qtde == 0) {
        printf("⚠️ Heap vazia.\n");
        return;
    }

    printf("🩺 Paciente prioritário atendido: %s\n", h->dados[0]->nome);
    h->dados[0] = h->dados[h->qtde - 1];
    h->qtde--;
    construir(h);
}

void mostrar_heap(const Heap *h) {
    if (h->qtde == 0) {
        printf("📭 Nenhum paciente na fila prioritária.\n");
        return;
    }

    printf("📋 Fila de atendimento prioritário:\n");
    for (int i = 0; i < h->qtde; i++) {
        Registro *p = h->dados[i];
        printf("%d️⃣  Nome: %s | Idade: %d | RG: %s | Entrada: %02d/%02d/%04d\n",
               i + 1, p->nome, p->idade, p->rg, p->entrada->dia, p->entrada->mes, p->entrada->ano);
    }
}

// ------------------------------------
// FUNÇÕES DE ITEM DE MENU - PESQUISAR
// ------------------------------------

ABB *cria_abb() {
    ABB *arvore = malloc(sizeof(ABB));
    arvore->raiz = NULL;
    arvore->qtde = 0;
    return arvore;
}

EABB *cria_vertice(Registro paciente) {
    EABB *novo = malloc(sizeof(EABB));
    novo->dados = malloc(sizeof(Registro));
    *(novo->dados) = paciente;
    novo->filhoEsq = NULL;
    novo->filhoDir = NULL;
    return novo;
}


void inserir_abb(ABB *arvore, Registro paciente, int (*criterio)(Registro, Registro)) {
    EABB *novo = cria_vertice(paciente);

    if (arvore->raiz == NULL) {
        arvore->raiz = novo;
    } else {
        EABB *atual = arvore->raiz;
        EABB *anterior = NULL;
        while (atual != NULL) {
            anterior = atual;
            if (criterio(paciente, *(atual->dados)) <= 0) {
                atual = atual->filhoEsq;
            } else {
                atual = atual->filhoDir;
            }
        }
        if (criterio(paciente, *(anterior->dados)) <= 0) {
            anterior->filhoEsq = novo;
        } else {
            anterior->filhoDir = novo;
        }
    }
    arvore->qtde++;
}

void imprimir_in_ordem(EABB *raiz) {
    if (raiz != NULL) {
        imprimir_in_ordem(raiz->filhoEsq);
        printf("Nome: %s | Idade: %d | RG: %s | Entrada: %02d/%02d/%04d\n",
               raiz->dados->nome,raiz->dados->idade, raiz->dados->rg, raiz->dados->entrada->dia, raiz->dados->entrada->mes, raiz->dados->entrada->ano);
        imprimir_in_ordem(raiz->filhoDir);
    }
}

int comparar_por_ano(Registro a, Registro b) {
    return a.entrada->ano - b.entrada->ano;
}

int comparar_por_mes(Registro a, Registro b) {
    return a.entrada->mes - b.entrada->mes;
}

int comparar_por_dia(Registro a, Registro b) {
    return a.entrada->dia - b.entrada->dia;
}

int comparar_por_idade(Registro a, Registro b) {
    return a.idade - b.idade;
}

void reconstruir_abb(Lista *lista, ABB *arvore, int (*criterio)(Registro, Registro)) {
    ELista *atual = lista->inicio;
    while (atual != NULL) {
        inserir_abb(arvore, *(atual->dados), criterio);
        atual = atual->proximo;
    }
}


// -----------------------------------------
// FUNÇÕES DE ITEM DE MENU: CARREGAR/SALVAR
// -----------------------------------------

void salvar_lista(Lista *lista, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        printf("❌ Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    ELista *atual = lista->inicio;
    while (atual != NULL) {
        fprintf(arquivo, "Nome: %s | Idade: %d | RG: %s | Entrada: %02d/%02d/%04d\n",
        atual->dados->nome, atual->dados->idade, atual->dados->rg,
        atual->dados->entrada->dia, atual->dados->entrada->mes, atual->dados->entrada->ano);
        atual = atual->proximo; 
    }

    fclose(arquivo);
    printf("✅ Lista salva com sucesso em \"%s\"!\n", nome_arquivo);
}




void carregar_lista(Lista *lista, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("❌ Erro ao abrir o arquivo para leitura.\n");
        return;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo)) {
        Registro novo;
        int dia, mes, ano;

        // Extrai os dados do formato: Nome: X | Idade: Y | RG: Z | Entrada: DD/MM/AAAA
        sscanf(linha, "Nome: %[^|]| Idade: %d | RG: %[^|]| Entrada: %d/%d/%d",
               novo.nome, &novo.idade, novo.rg, &dia, &mes, &ano);

        // Limpa espaços à direita do nome
        int i = strlen(novo.nome) - 1;
        while (i >= 0 && (novo.nome[i] == ' ' || novo.nome[i] == '\n')) {
            novo.nome[i--] = '\0';
        }

        // Limpa espaços à direita do RG
        i = strlen(novo.rg) - 1;
        while (i >= 0 && (novo.rg[i] == ' ' || novo.rg[i] == '\n')) {
            novo.rg[i--] = '\0';
        }

        novo.entrada = cria_data(dia, mes, ano);
        cadastrar_paciente(lista, novo);
    }

    fclose(arquivo);
    printf("✅ Lista carregada com sucesso de \"%s\"!\n", nome_arquivo);
}


// -------------------------------
// FUNÇÕES DO ITEM DE MENU: SOBRE
// -------------------------------
void mostrar_sobre() {
    printf("\n📘 SOBRE O PROJETO\n");
    printf("----------------------------------------\n");
    printf("👨 Nome: Thiago Luppi Blumer\n");
    printf("#️⃣  RA: 11.122.030-7\n");
    printf("📚 Ciclo: Sétimo Semestre\n");
    printf("🎓 Curso: Engenharia de Robôs\n");
    printf("📘 Disciplina: Estrutura de Dados e Análise de Algoritmos\n");
    printf("📅 Data: 14/05/2025\n");
    printf("----------------------------------------\n");
    printf("👨 Nome: Nicolas Kuhnast\n");
    printf("#️⃣  RA: 11.122.431-7\n");
    printf("📚 Ciclo: Sétimo Semestre\n");
    printf("🎓 Curso: Engenharia de Robôs\n");
    printf("📘 Disciplina: Estrutura de Dados e Análise de Algoritmos\n");
    printf("📅 Data: 14/05/2025\n");
    printf("----------------------------------------\n\n");
}



// ------------------------
// FUNÇÃO PRINCIPAL - MAIN
// ------------------------
int main() {
    Lista* lista = inicializa_lista();
    Fila *fila = inicializa_fila();
    Heap *heap = malloc(sizeof(Heap));
    inicializar_heap(heap);
    Stack *stack = start_stack();

    int opcao = -1;

    while (opcao != 0) {
        printf("\n🩺 --- MENU PRINCIPAL --- 🩺\n");
        printf("1️⃣  Cadastrar\n");
        printf("2️⃣  Atendimento\n");
        printf("3️⃣  Atendimento Prioritário\n");
        printf("4️⃣  Pesquisa\n");
        printf("5️⃣  Desfazer\n");
        printf("6️⃣  Carregar / Salvar dados\n");
        printf("7️⃣  Sobre\n");
        printf("0️⃣  Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar();

        if (opcao == 1) {
            int cadastrar_opcao = -1;
            while (cadastrar_opcao != 0) {
                printf("\n📥 --- SUBMENU DE CADASTRO --- 📥\n");
                printf("1️⃣  Cadastrar paciente\n");
                printf("2️⃣  Consultar paciente\n");
                printf("3️⃣  Atualizar dados do paciente\n");
                printf("4️⃣  Remover paciente\n");
                printf("5️⃣  Imprimir lista de pacientes\n");
                printf("0️⃣  Voltar ao menu principal\n");
                printf("Escolha uma opção: ");
                scanf("%d", &cadastrar_opcao);
                getchar();

                if (cadastrar_opcao == 1) {
                    Registro novo;
                    int dia, mes, ano;

                    printf("📝 Nome: ");
                    fgets(novo.nome, sizeof(novo.nome), stdin);
                    novo.nome[strcspn(novo.nome, "\n")] = '\0';

                    printf("🎂 Idade: ");
                    scanf("%d", &novo.idade);
                    getchar();

                    printf("🆔 RG: ");
                    fgets(novo.rg, sizeof(novo.rg), stdin);
                    novo.rg[strcspn(novo.rg, "\n")] = '\0';

                    printf("📅 Data de entrada (dd mm aaaa): ");
                    scanf("%d %d %d", &dia, &mes, &ano);
                    getchar();

                    novo.entrada = cria_data(dia, mes, ano);
                    cadastrar_paciente(lista, novo);
                    printf("✅ Paciente cadastrado com sucesso!\n");
                }

                else if (cadastrar_opcao == 2) {
                    char nome_busca[100];
                    printf("🔍 Digite o nome do paciente: ");
                    fgets(nome_busca, sizeof(nome_busca), stdin);
                    nome_busca[strcspn(nome_busca, "\n")] = '\0';

                    ELista *resultado = consultar_paciente(lista, nome_busca);
                    if (resultado != NULL) {
                        printf("✅ Paciente encontrado:\n");
                        printf("Nome: %s | Idade: %d | RG: %s | Entrada: %02d/%02d/%04d\n",
                               resultado->dados->nome, resultado->dados->idade, resultado->dados->rg, resultado->dados->entrada->dia, resultado->dados->entrada->mes, resultado->dados->entrada->ano);
                    } else {
                        printf("❌ Paciente não encontrado.\n");
                    }
                }

                else if (cadastrar_opcao == 3) {
                    atualizar_paciente(lista);
                }

                else if (cadastrar_opcao == 4) {
                    char nome_remover[100];
                    printf("🗑️ Digite o nome do paciente a ser removido: ");
                    fgets(nome_remover, sizeof(nome_remover), stdin);
                    nome_remover[strcspn(nome_remover, "\n")] = '\0';
                    remover_paciente(lista, nome_remover);
                }

                else if (cadastrar_opcao == 5) {
                    printf("\n📋 Lista de pacientes:\n");
                    imprimir_lista(lista);
                }

                else if (cadastrar_opcao == 0) {
                    printf("🔙 Retornando ao menu principal...\n");
                }

                else {
                    printf("❌ Opção inválida. Tente novamente.\n");
                }
            }
        }

        else if (opcao == 2) {
            int atendimento_opcao = -1;
            while (atendimento_opcao != 0) {
                printf("\n🏥 --- SUBMENU DE ATENDIMENTO --- 🏥\n");
                printf("1️⃣  Enfileirar paciente\n");
                printf("2️⃣  Desenfileirar paciente (atender)\n");
                printf("3️⃣  Mostrar fila de atendimento\n");
                printf("0️⃣  Voltar ao menu principal\n");
                printf("Escolha uma opção: ");
                scanf("%d", &atendimento_opcao);
                getchar();
        
                if (atendimento_opcao == 1) {
                    enfileirar_paciente(lista, fila, stack);
                }

                else if (atendimento_opcao == 2) {
                    desenfileirar_paciente(fila, stack);
                }
                
                else if (atendimento_opcao == 3) {
                    mostrar_fila(fila);
                }

                else if (atendimento_opcao == 0) {
                    printf("🔙 Retornando ao menu principal...\n");
                }
        
                else {
                    printf("❌ Opção inválida. Tente novamente.\n");
                }
            }
        }
        
        else if (opcao == 3) {
            int prioridade_opcao = -1;
            while (prioridade_opcao != 0) {
                printf("\n🚨 --- SUBMENU DE ATENDIMENTO PRIORITÁRIO --- 🚨\n");
                printf("1️⃣  Enfileirar paciente prioritário\n");
                printf("2️⃣  Desenfileirar paciente prioritário (atender)\n");
                printf("3️⃣  Mostrar fila prioritária\n");
                printf("0️⃣  Voltar ao menu principal\n");
                printf("Escolha uma opção: ");
                scanf("%d", &prioridade_opcao);
                getchar();
        
                if (prioridade_opcao == 1) {
                    char nome_busca[100];
                    printf("🔍 Digite o nome do paciente: ");
                    fgets(nome_busca, sizeof(nome_busca), stdin);
                    nome_busca[strcspn(nome_busca, "\n")] = '\0';
        
                    ELista *paciente = consultar_paciente(lista, nome_busca);
                    if (paciente == NULL) {
                        printf("❌  Paciente não encontrado.\n");
                    } else {
                        inserir_heap(heap, paciente->dados);
                        printf("✅  Paciente \"%s\" enfileirado com prioridade!\n", paciente->dados->nome);
                    }
                }
        
                else if (prioridade_opcao == 2) {
                    remover_heap(heap);
                }
        
                else if (prioridade_opcao == 3) {
                    mostrar_heap(heap);
                }
        
                else if (prioridade_opcao == 0) {
                    printf("🔙 Retornando ao menu principal...\n");
                }
        
                else {
                    printf("❌ Opção inválida. Tente novamente.\n");
                }
            }
        }
        
        else if (opcao == 4) {
            int pesquisa_opcao = -1;
            while (pesquisa_opcao != 0) {
                printf("\n🔍 --- SUBMENU DE PESQUISA --- 🔍\n");
                printf("1️⃣  Mostrar por ano de entrada\n");
                printf("2️⃣  Mostrar por mês de entrada\n");
                printf("3️⃣  Mostrar por dia de entrada\n");
                printf("4️⃣  Mostrar por idade\n");
                printf("0️⃣  Voltar ao menu principal\n");
                printf("Escolha uma opção: ");
                scanf("%d", &pesquisa_opcao);
                getchar();
        
                ABB *arvore = cria_abb();
        
                if (pesquisa_opcao == 1) {
                    reconstruir_abb(lista, arvore, comparar_por_ano);
                    printf("📅  Pacientes ordenados por ano de entrada:\n");
                    imprimir_in_ordem(arvore->raiz);
                } else if (pesquisa_opcao == 2) {
                    reconstruir_abb(lista, arvore, comparar_por_mes);
                    printf("📅  Pacientes ordenados por mês de entrada:\n");
                    imprimir_in_ordem(arvore->raiz);
                } else if (pesquisa_opcao == 3) {
                    reconstruir_abb(lista, arvore, comparar_por_dia);
                    printf("📅  Pacientes ordenados por dia de entrada:\n");
                    imprimir_in_ordem(arvore->raiz);
                } else if (pesquisa_opcao == 4) {
                    reconstruir_abb(lista, arvore, comparar_por_idade);
                    printf("👴  Pacientes ordenados por idade:\n");
                    imprimir_in_ordem(arvore->raiz);
                } else if (pesquisa_opcao == 0) {
                    printf("🔙  Retornando ao menu principal...\n");
                } else {
                    printf("❌  Opção inválida. Tente novamente.\n");
                }
        
                free(arvore); // Libera a estrutura da árvore (você pode melhorar isso liberando os nós também)
            }
        }

        else if (opcao == 5) {
            printf("🧾 Log de operações:\n");
            imprimir_stack(stack);
            printf("❓ Deseja desfazer a última operação? (s/n): ");
            char confirmacao;
            scanf("%c", &confirmacao);
            getchar();
            if (confirmacao == 's' || confirmacao == 'S') {
                desfazer_ultima_operacao(stack, fila);
            } else {
                printf("🔁 Operação não desfeita.\n");
            }
        }

        else if (opcao == 6) {
            int arquivo_opcao = -1;
            while (arquivo_opcao != 0) {
                printf("\n💾 --- MENU DE ARQUIVOS --- 💾\n");
                printf("1️⃣  Salvar lista de pacientes\n");
                printf("2️⃣  Carregar lista de pacientes\n");
                printf("0️⃣  Voltar ao menu principal\n");
                printf("Escolha uma opção: ");
                scanf("%d", &arquivo_opcao);
                getchar();
        
                if (arquivo_opcao == 1) {
                    salvar_lista(lista, "pacientes.txt");
                } else if (arquivo_opcao == 2) {
                    carregar_lista(lista, "pacientes.txt");
                } else if (arquivo_opcao == 0) {
                    printf("🔙 Retornando ao menu principal...\n");
                } else {
                    printf("❌ Opção inválida. Tente novamente.\n");
                }
            }
        }

        else if (opcao == 7) {
            mostrar_sobre();
        }



        else if (opcao == 0) {
            printf("👋 Saindo do programa...\n");
        }

        else {
            printf("❌ Opção inválida. Tente novamente.\n");
        }
    }

    return 0;
}