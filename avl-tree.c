#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ESPACO 5  // Define o espaçamento entre níveis para exibição

typedef struct AVLNode {
    int matricula;
    char nome[100];
    int altura;
    struct AVLNode* esquerda;
    struct AVLNode* direita;
} AVLNode;

int altura(AVLNode* no) {
    return no ? no->altura : 0;
}

int fatorBalanceamento(AVLNode* no) {
    return no ? altura(no->esquerda) - altura(no->direita) : 0;
}

AVLNode* rotacaoDireita(AVLNode* y) {
    AVLNode* x = y->esquerda;
    AVLNode* T2 = x->direita;

    x->direita = y;
    y->esquerda = T2;

    y->altura = 1 + (altura(y->esquerda) > altura(y->direita) ? altura(y->esquerda) : altura(y->direita));
    x->altura = 1 + (altura(x->esquerda) > altura(x->direita) ? altura(x->esquerda) : altura(x->direita));

    return x;
}

AVLNode* rotacaoEsquerda(AVLNode* x) {
    AVLNode* y = x->direita;
    AVLNode* T2 = y->esquerda;

    y->esquerda = x;
    x->direita = T2;

    x->altura = 1 + (altura(x->esquerda) > altura(x->direita) ? altura(x->esquerda) : altura(x->direita));
    y->altura = 1 + (altura(y->esquerda) > altura(y->direita) ? altura(y->esquerda) : altura(y->direita));

    return y;
}

AVLNode* criarNo(int matricula, char* nome) {
    AVLNode* novoNode = (AVLNode*)malloc(sizeof(AVLNode));
    novoNode->matricula = matricula;
    strcpy(novoNode->nome, nome);
    novoNode->altura = 1;
    novoNode->esquerda = novoNode->direita = NULL;
    return novoNode;
}

AVLNode* inserirAluno(AVLNode* node, int matricula, char* nome) {
    if (!node)
        return criarNo(matricula, nome);

    if (matricula < node->matricula)
        node->esquerda = inserirAluno(node->esquerda, matricula, nome);
    else if (matricula > node->matricula)
        node->direita = inserirAluno(node->direita, matricula, nome);
    else
        return node;

    node->altura = 1 + (altura(node->esquerda) > altura(node->direita) ? altura(node->esquerda) : altura(node->direita));

    int balance = fatorBalanceamento(node);

    if (balance > 1 && matricula < node->esquerda->matricula)
        return rotacaoDireita(node);

    if (balance < -1 && matricula > node->direita->matricula)
        return rotacaoEsquerda(node);

    if (balance > 1 && matricula > node->esquerda->matricula) {
        node->esquerda = rotacaoEsquerda(node->esquerda);
        return rotacaoDireita(node);
    }

    if (balance < -1 && matricula < node->direita->matricula) {
        node->direita = rotacaoDireita(node->direita);
        return rotacaoEsquerda(node);
    }

    return node;
}

AVLNode* minValueNode(AVLNode* node) {
    AVLNode* current = node;
    while (current->esquerda != NULL)
        current = current->esquerda;
    return current;
}

AVLNode* removerAluno(AVLNode* root, int matricula) {
    if (!root)
        return root;

    if (matricula < root->matricula)
        root->esquerda = removerAluno(root->esquerda, matricula);
    else if (matricula > root->matricula)
        root->direita = removerAluno(root->direita, matricula);
    else {
        if (!root->esquerda || !root->direita) {
            AVLNode* temp = root->esquerda ? root->esquerda : root->direita;
            if (!temp) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;
            free(temp);
        } else {
            AVLNode* temp = minValueNode(root->direita);
            root->matricula = temp->matricula;
            strcpy(root->nome, temp->nome);
            root->direita = removerAluno(root->direita, temp->matricula);
        }
    }

    if (!root)
        return root;

    root->altura = 1 + (altura(root->esquerda) > altura(root->direita) ? altura(root->esquerda) : altura(root->direita));

    int balance = fatorBalanceamento(root);

    if (balance > 1 && fatorBalanceamento(root->esquerda) >= 0)
        return rotacaoDireita(root);

    if (balance > 1 && fatorBalanceamento(root->esquerda) < 0) {
        root->esquerda = rotacaoEsquerda(root->esquerda);
        return rotacaoDireita(root);
    }

    if (balance < -1 && fatorBalanceamento(root->direita) <= 0)
        return rotacaoEsquerda(root);

    if (balance < -1 && fatorBalanceamento(root->direita) > 0) {
        root->direita = rotacaoDireita(root->direita);
        return rotacaoEsquerda(root);
    }

    return root;
}

AVLNode* buscarAluno(AVLNode* node, int matricula) {
    if (!node || node->matricula == matricula)
        return node;

    if (matricula < node->matricula)
        return buscarAluno(node->esquerda, matricula);
    else
        return buscarAluno(node->direita, matricula);
}

void desenha_arvore_horiz(AVLNode *arvore, int profundidade, char *caminho, int direita) {
    if (arvore == NULL) return;

    profundidade++;

    desenha_arvore_horiz(arvore->direita, profundidade, caminho, 1);

    caminho[profundidade - 2] = 0;
    if (direita) caminho[profundidade - 2] = 1;
    if (arvore->esquerda) caminho[profundidade - 1] = 1;

    printf("\n");

    for (int i = 0; i < profundidade - 1; i++) {
        if (i == profundidade - 2)
            printf("+");
        else if (caminho[i])
            printf("|");
        else
            printf(" ");

        for (int j = 1; j < ESPACO; j++)
            printf(i < profundidade - 2 ? " " : "-");
    }

    printf("%d (%s)\n", arvore->matricula, arvore->nome);

    for (int i = 0; i < profundidade; i++) {
        if (caminho[i])
            printf("|");
        else
            printf(" ");

        for (int j = 1; j < ESPACO; j++)
            printf(" ");
    }

    desenha_arvore_horiz(arvore->esquerda, profundidade, caminho, 0);
}

void desenha_avl(AVLNode* raiz) {
    char caminho[255] = {};
    desenha_arvore_horiz(raiz, 0, caminho, 0);
}

int main() {
    AVLNode* raiz = NULL;

    // Adiciona cinco alunos padrão
    raiz = inserirAluno(raiz, 1, "Alice");
    raiz = inserirAluno(raiz, 2, "Bob");
    raiz = inserirAluno(raiz, 3, "Charlie");
    raiz = inserirAluno(raiz, 4, "Dave");
    raiz = inserirAluno(raiz, 5, "Eve");
    raiz = inserirAluno(raiz, 6, "Vítor");
    raiz = inserirAluno(raiz, 7, "Dino");
    raiz = inserirAluno(raiz, 15, "Diego");
    
    int opcao, matricula;
    char nome[100];

    do {
        printf("\nSistema de Gerenciamento de Alunos\n");
        printf("1. Inserir aluno\n");
        printf("2. Remover aluno\n");
        printf("3. Buscar aluno\n");
        printf("4. Imprimir árvore AVL\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite a matrícula do aluno: ");
                scanf("%d", &matricula);
                printf("Digite o nome do aluno: ");
                scanf(" %[^\n]", nome);
                raiz = inserirAluno(raiz, matricula, nome);
                printf("Aluno inserido com sucesso!\n");
                break;
            case 2:
                printf("Digite a matrícula do aluno a ser removido: ");
                scanf("%d", &matricula);
                raiz = removerAluno(raiz, matricula);
                printf("Aluno removido com sucesso!\n");
                break;
            case 3:
                printf("Digite a matrícula do aluno a ser buscado: ");
                scanf("%d", &matricula);
                AVLNode* aluno = buscarAluno(raiz, matricula);
                if (aluno)
                    printf("Aluno encontrado: %d - %s\n", aluno->matricula, aluno->nome);
                else
                    printf("Aluno não encontrado.\n");
                break;
            case 4:
                printf("Árvore AVL desenhada horizontalmente:\n");
                desenha_avl(raiz);
                break;
            case 5:
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 5);

    return 0;
}
