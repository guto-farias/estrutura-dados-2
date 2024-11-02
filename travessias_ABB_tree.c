#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura do Livro
typedef struct {
    char titulo[100];
    char autor[100];
    char isbn[14];
} Livro;

// Estrutura do Nó da Árvore Binária de Busca
typedef struct Node {
    Livro livro;
    struct Node *esq, *dir;
} Node;

// Funções
Node* criarNo(Livro livro);
Node* inserir(Node* raiz, Livro livro);
void imprimirArvore(Node* raiz, int nivel);
Node* buscarPorISBN(Node* raiz, char* isbn);
Node* buscarPorTitulo(Node* raiz, char* titulo);
Node* encontrarMinimo(Node* raiz);
Node* remover(Node* raiz, char* isbn);
Node* removerPorTitulo(Node* raiz, char* titulo);
void emOrdem(Node* raiz);
void preOrdem(Node* raiz);
void posOrdem(Node* raiz);

// Função para criar um novo nó com um livro
Node* criarNo(Livro livro) {
    Node* novoNo = (Node*)malloc(sizeof(Node));
    if (!novoNo) {
        printf("Erro de alocação de memória!\n");
        return NULL;
    }
    novoNo->livro = livro;
    novoNo->esq = novoNo->dir = NULL;
    return novoNo;
}

// Função de Inserção
Node* inserir(Node* raiz, Livro livro) {
    if (raiz == NULL) return criarNo(livro);

    if (strcmp(livro.isbn, raiz->livro.isbn) < 0) {
        raiz->esq = inserir(raiz->esq, livro);
    } else if (strcmp(livro.isbn, raiz->livro.isbn) > 0) {
        raiz->dir = inserir(raiz->dir, livro);
    } else {
        printf("ISBN já existe!\n");
    }
    return raiz;
}

// Função para exibir a estrutura da árvore em formato hierárquico
void imprimirArvore(Node* raiz, int nivel) {
    if (raiz == NULL) {
        for (int i = 0; i < nivel; i++) printf("    ");
        printf("-> NULL\n");
        return;
    }

    for (int i = 0; i < nivel; i++) printf("    ");
    printf("-> ISBN: %s\n", raiz->livro.isbn);

    imprimirArvore(raiz->esq, nivel + 1);
    imprimirArvore(raiz->dir, nivel + 1);
}

// Função de Busca por ISBN
Node* buscarPorISBN(Node* raiz, char* isbn) {
    if (raiz == NULL || strcmp(isbn, raiz->livro.isbn) == 0)
        return raiz;

    if (strcmp(isbn, raiz->livro.isbn) < 0)
        return buscarPorISBN(raiz->esq, isbn);
    else
        return buscarPorISBN(raiz->dir, isbn);
}

// Função de Busca por Título
Node* buscarPorTitulo(Node* raiz, char* titulo) {
    if (raiz == NULL) return NULL;

    if (strcmp(titulo, raiz->livro.titulo) == 0)
        return raiz;

    Node* encontrado = buscarPorTitulo(raiz->esq, titulo);
    if (encontrado) return encontrado;

    return buscarPorTitulo(raiz->dir, titulo);
}

// Função para encontrar o menor valor à direita (substituto para remoção)
Node* encontrarMinimo(Node* raiz) {
    while (raiz->esq != NULL) raiz = raiz->esq;
    return raiz;
}

// Função de Remoção por ISBN
Node* remover(Node* raiz, char* isbn) {
    if (raiz == NULL) return raiz;

    if (strcmp(isbn, raiz->livro.isbn) < 0) {
        raiz->esq = remover(raiz->esq, isbn);
    } else if (strcmp(isbn, raiz->livro.isbn) > 0) {
        raiz->dir = remover(raiz->dir, isbn);
    } else {
        if (raiz->esq == NULL) {
            Node* temp = raiz->dir;
            free(raiz);
            return temp;
        } else if (raiz->dir == NULL) {
            Node* temp = raiz->esq;
            free(raiz);
            return temp;
        }
        Node* temp = encontrarMinimo(raiz->dir);
        raiz->livro = temp->livro;
        raiz->dir = remover(raiz->dir, temp->livro.isbn);
    }
    return raiz;
}

// Função de Remoção por Título
Node* removerPorTitulo(Node* raiz, char* titulo) {
    Node* livroEncontrado = buscarPorTitulo(raiz, titulo);
    if (livroEncontrado != NULL) {
        return remover(raiz, livroEncontrado->livro.isbn);
    } else {
        printf("Livro com título '%s' não encontrado para remoção.\n", titulo);
        return raiz;
    }
}

// Funções de Travessia
void emOrdem(Node* raiz) {
    if (raiz == NULL) return;
    emOrdem(raiz->esq);
    printf("ISBN: %s, Título: %s\n", raiz->livro.isbn, raiz->livro.titulo);
    emOrdem(raiz->dir);
}

void preOrdem(Node* raiz) {
    if (raiz == NULL) return;
    printf("ISBN: %s, Título: %s\n", raiz->livro.isbn, raiz->livro.titulo);
    preOrdem(raiz->esq);
    preOrdem(raiz->dir);
}

void posOrdem(Node* raiz) {
    if (raiz == NULL) return;
    posOrdem(raiz->esq);
    posOrdem(raiz->dir);
    printf("ISBN: %s, Título: %s\n", raiz->livro.isbn, raiz->livro.titulo);
}

// Função principal para executar o menu
int main() {
    Node* raiz = NULL;
    int opcao;
    Livro livro;

    while (1) {
        printf("\nMenu:\n");
        printf("1. Cadastrar Livro\n");
        printf("2. Consultar Livro por ISBN\n");
        printf("3. Consultar Livro por Título\n");
        printf("4. Remover Livro por ISBN\n");
        printf("5. Remover Livro por Título\n");
        printf("6. Exibir Estrutura da Árvore\n");
        printf("7. Exibir Travessias (Em-Ordem, Pré-Ordem, Pós-Ordem)\n");
        printf("8. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer

        switch (opcao) {
            case 1: // Cadastrar Livro
                printf("Digite o título do livro: ");
                fgets(livro.titulo, sizeof(livro.titulo), stdin);
                livro.titulo[strcspn(livro.titulo, "\n")] = 0;

                printf("Digite o autor do livro: ");
                fgets(livro.autor, sizeof(livro.autor), stdin);
                livro.autor[strcspn(livro.autor, "\n")] = 0;

                printf("Digite o ISBN do livro: ");
                fgets(livro.isbn, sizeof(livro.isbn), stdin);
                livro.isbn[strcspn(livro.isbn, "\n")] = 0;

                raiz = inserir(raiz, livro);
                printf("Inserindo %s:\n", livro.titulo);
                imprimirArvore(raiz, 0);
                break;

            case 2: // Consultar Livro por ISBN
                printf("Digite o ISBN do livro: ");
                fgets(livro.isbn, sizeof(livro.isbn), stdin);
                livro.isbn[strcspn(livro.isbn, "\n")] = 0;

                Node* encontradoISBN = buscarPorISBN(raiz, livro.isbn);
                if (encontradoISBN) {
                    printf("Encontrado - Título: %s\n", encontradoISBN->livro.titulo);
                } else {
                    printf("Livro não encontrado!\n");
                }
                break;

            case 3: // Consultar Livro por Título
                printf("Digite o título do livro: ");
                fgets(livro.titulo, sizeof(livro.titulo), stdin);
                livro.titulo[strcspn(livro.titulo, "\n")] = 0;

                Node* encontradoTitulo = buscarPorTitulo(raiz, livro.titulo);
                if (encontradoTitulo) {
                    printf("Encontrado - ISBN: %s\n", encontradoTitulo->livro.isbn);
                } else {
                    printf("Livro não encontrado!\n");
                }
                break;

            case 4: // Remover Livro por ISBN
                printf("Digite o ISBN do livro a ser removido: ");
                fgets(livro.isbn, sizeof(livro.isbn), stdin);
                livro.isbn[strcspn(livro.isbn, "\n")] = 0;

                raiz = remover(raiz, livro.isbn);
                printf("Livro removido com sucesso (se existia)!\n");
                imprimirArvore(raiz, 0);
                break;

            case 5: // Remover Livro por Título
                printf("Digite o título do livro a ser removido: ");
                fgets(livro.titulo, sizeof(livro.titulo), stdin);
                livro.titulo[strcspn(livro.titulo, "\n")] = 0;

                raiz = removerPorTitulo(raiz, livro.titulo);
                printf("Livro removido com sucesso (se existia)!\n");
                imprimirArvore(raiz, 0);
                break;

            case 6: // Exibir Estrutura da Árvore
                printf("Estrutura da Árvore:\n");
                imprimirArvore(raiz, 0);
                break;

            case 7: // Exibir Travessias
                printf("Travessia Em-Ordem:\n");
                emOrdem(raiz);
                printf("\n");

                printf("Travessia Pré-Ordem:\n");
                preOrdem(raiz);
                printf("\n");

                printf("Travessia Pós-Ordem:\n");
                posOrdem(raiz);
                printf("\n");
                break;

            case 8: // Sair
                printf("Saindo...\n");
                exit(0);
                break;

            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
        }
    }

    return 0;
}
 