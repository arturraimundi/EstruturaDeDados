#include <stdio.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_BOOKS 100

typedef struct {
    char email[100];
    char nome[100];
} Usuario;

typedef struct {
    int id;
    char titulo[100];
    char autor[100];
    int ano;
    int status; // 0 disponivel, 1 emprestado
    char emailUsuario[100];
} Livro;

Usuario usuarios[MAX_USERS];
Livro livros[MAX_BOOKS];
int totalUsuarios = 0;
int totalLivros = 0;
int proximoIdLivro = 1;

void limparEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int encontrarUsuarioPorEmail(const char *email) {
    for (int i = 0; i < totalUsuarios; i++) {
        if (strcmp(usuarios[i].email, email) == 0) {
            return i;
        }
    }
    return -1;
}

int encontrarLivroPorId(int id) {
    for (int i = 0; i < totalLivros; i++) {
        if (livros[i].id == id) {
            return i;
        }
    }
    return -1;
}

void cadastrarUsuario() {
    if (totalUsuarios >= MAX_USERS) {
        printf("Limite de usuarios atingido.\n");
        return;
    }

    Usuario u;
    printf("Email: ");
    scanf("%99s", u.email);
    limparEntrada();
    if (encontrarUsuarioPorEmail(u.email) != -1) {
        printf("Usuario ja cadastrado.\n");
        return;
    }

    printf("Nome: ");
    scanf("%99s", u.nome);
    limparEntrada();

    usuarios[totalUsuarios++] = u;
    printf("Usuario cadastrado com sucesso.\n");
}

void cadastrarLivro() {
    if (totalLivros >= MAX_BOOKS) {
        printf("Limite de livros atingido.\n");
        return;
    }

    Livro l;
    l.id = proximoIdLivro++;
    printf("Titulo: ");
    scanf("%99s", l.titulo);
    limparEntrada();
    printf("Autor: ");
    scanf("%99s", l.autor);
    limparEntrada();
    printf("Ano de publicacao: ");
    scanf("%d", &l.ano);
    limparEntrada();
    l.status = 0;
    l.emailUsuario[0] = '\0';

    livros[totalLivros++] = l;
    printf("Livro cadastrado com sucesso. ID = %d\n", l.id);
}

void consultaLivros() {
    int op;
    printf("1. Por codigo\n");
    printf("2. Por autor\n");
    printf("0. Voltar\n");
    printf("Opcao: ");
    scanf("%d", &op);
    limparEntrada();

    if (op == 1) {
        int id;
        printf("Codigo do livro: ");
        scanf("%d", &id);
        limparEntrada();
        int idx = encontrarLivroPorId(id);
        if (idx == -1) {
            printf("Livro nao encontrado\n");
        } else {
            Livro *l = &livros[idx];
            printf("ID: %d\nTitulo: %s\nAutor: %s\nAno: %d\nStatus: %s\n",
                   l->id, l->titulo, l->autor, l->ano,
                   l->status == 0 ? "Disponivel" : "Emprestado");
            if (l->status == 1) {
                printf("Emprestado para: %s\n", l->emailUsuario);
            }
        }
    } else if (op == 2) {
        char autor[100];
        printf("Autor: ");
        scanf("%99s", autor);
        limparEntrada();
        int achou = 0;
        for (int i = 0; i < totalLivros; i++) {
            if (strcmp(livros[i].autor, autor) == 0) {
                printf("ID: %d, Titulo: %s, Ano: %d, Status: %s\n",
                       livros[i].id, livros[i].titulo, livros[i].ano,
                       livros[i].status == 0 ? "Disponivel" : "Emprestado");
                achou = 1;
            }
        }
        if (!achou) {
            printf("Livro nao encontrado\n");
        }
    }
}

void consultaUsuarios() {
    int op;
    printf("1. Por email\n");
    printf("2. Por nome\n");
    printf("0. Voltar\n");
    printf("Opcao: ");
    scanf("%d", &op);
    limparEntrada();

    if (op == 1) {
        char email[100];
        printf("Email: ");
        scanf("%99s", email);
        limparEntrada();
        int idx = encontrarUsuarioPorEmail(email);
        if (idx == -1) {
            printf("Usuario nao cadastrado\n");
        } else {
            printf("Email: %s\nNome: %s\n", usuarios[idx].email, usuarios[idx].nome);
        }
    } else if (op == 2) {
        char nome[100];
        printf("Nome: ");
        scanf("%99s", nome);
        limparEntrada();
        int achou = 0;
        for (int i = 0; i < totalUsuarios; i++) {
            if (strcmp(usuarios[i].nome, nome) == 0) {
                printf("Email: %s\nNome: %s\n", usuarios[i].email, usuarios[i].nome);
                achou = 1;
            }
        }
        if (!achou) {
            printf("Usuario nao cadastrado\n");
        }
    }
}

void consultaEmprestimos() {
    char email[100];
    printf("Email do usuario: ");
    scanf("%99s", email);
    limparEntrada();
    int idx = encontrarUsuarioPorEmail(email);
    if (idx == -1) {
        printf("Usuario nao cadastrado\n");
        return;
    }
    int achou = 0;
    for (int i = 0; i < totalLivros; i++) {
        if (livros[i].status == 1 && strcmp(livros[i].emailUsuario, email) == 0) {
            printf("ID: %d, Titulo: %s, Autor: %s, Ano: %d\n",
                   livros[i].id, livros[i].titulo, livros[i].autor, livros[i].ano);
            achou = 1;
        }
    }
    if (!achou) {
        printf("Nenhum livro emprestado para este usuario.\n");
    }
}

void menuConsulta() {
    int op;
    do {
        printf("\n--- Consulta ---\n");
        printf("1. Livros\n");
        printf("2. Usuarios\n");
        printf("3. Emprestimos\n");
        printf("0. Voltar\n");
        printf("Opcao: ");
        scanf("%d", &op);
        limparEntrada();
        switch (op) {
            case 1: consultaLivros(); break;
            case 2: consultaUsuarios(); break;
            case 3: consultaEmprestimos(); break;
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    } while (op != 0);
}

void menuCadastro() {
    int op;
    do {
        printf("\n--- Cadastro ---\n");
        printf("1. Livros\n");
        printf("2. Usuarios\n");
        printf("0. Voltar\n");
        printf("Opcao: ");
        scanf("%d", &op);
        limparEntrada();
        switch (op) {
            case 1: cadastrarLivro(); break;
            case 2: cadastrarUsuario(); break;
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    } while (op != 0);
}

void atualizarLivro() {
    int id;
    printf("Digite o codigo do livro: ");
    scanf("%d", &id);
    limparEntrada();
    int idx = encontrarLivroPorId(id);
    if (idx == -1) {
        printf("Livro nao encontrado.\n");
        return;
    }
    printf("Novo titulo: ");
    scanf("%99s", livros[idx].titulo);
    limparEntrada();
    printf("Novo autor: ");
    scanf("%99s", livros[idx].autor);
    limparEntrada();
    printf("Novo ano: ");
    scanf("%d", &livros[idx].ano);
    limparEntrada();
    printf("Livro atualizado.\n");
}

void atualizarUsuario() {
    char email[100];
    printf("Digite o email do usuario: ");
    scanf("%99s", email);
    limparEntrada();
    int idx = encontrarUsuarioPorEmail(email);
    if (idx == -1) {
        printf("Usuario nao cadastrado.\n");
        return;
    }
    printf("Novo nome: ");
    scanf("%99s", usuarios[idx].nome);
    limparEntrada();
    printf("Usuario atualizado.\n");
}

void menuAtualizacao() {
    int op;
    do {
        printf("\n--- Atualizacao ---\n");
        printf("1. Livros\n");
        printf("2. Usuarios\n");
        printf("0. Voltar\n");
        printf("Opcao: ");
        scanf("%d", &op);
        limparEntrada();
        switch (op) {
            case 1: atualizarLivro(); break;
            case 2: atualizarUsuario(); break;
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    } while (op != 0);
}

void excluirLivro() {
    int id;
    printf("Digite o codigo do livro: ");
    scanf("%d", &id);
    limparEntrada();
    int idx = encontrarLivroPorId(id);
    if (idx == -1) {
        printf("Livro nao encontrado.\n");
        return;
    }
    for (int i = idx; i < totalLivros - 1; i++) {
        livros[i] = livros[i + 1];
    }
    totalLivros--;
    printf("Livro excluido com sucesso.\n");
}

void excluirUsuario() {
    char email[100];
    printf("Digite o email do usuario: ");
    scanf("%99s", email);
    limparEntrada();
    int idx = encontrarUsuarioPorEmail(email);
    if (idx == -1) {
        printf("Usuario nao cadastrado.\n");
        return;
    }
    for (int i = 0; i < totalLivros; i++) {
        if (livros[i].status == 1 && strcmp(livros[i].emailUsuario, email) == 0) {
            printf("Usuario possui livros emprestados e nao pode ser excluido.\n");
            return;
        }
    }
    for (int i = idx; i < totalUsuarios - 1; i++) {
        usuarios[i] = usuarios[i + 1];
    }
    totalUsuarios--;
    printf("Usuario excluido com sucesso.\n");
}

void menuExclusao() {
    int op;
    do {
        printf("\n--- Exclusao ---\n");
        printf("1. Livros\n");
        printf("2. Usuarios\n");
        printf("0. Voltar\n");
        printf("Opcao: ");
        scanf("%d", &op);
        limparEntrada();
        switch (op) {
            case 1: excluirLivro(); break;
            case 2: excluirUsuario(); break;
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    } while (op != 0);
}

void emprestarLivro() {
    int id;
    char email[100];
    printf("Codigo do livro: ");
    scanf("%d", &id);
    limparEntrada();
    int idxLivro = encontrarLivroPorId(id);
    if (idxLivro == -1) {
        printf("Livro nao encontrado.\n");
        return;
    }
    if (livros[idxLivro].status == 1) {
        printf("Livro ja esta emprestado.\n");
        return;
    }
    printf("Email do usuario: ");
    scanf("%99s", email);
    limparEntrada();
    int idxUsuario = encontrarUsuarioPorEmail(email);
    if (idxUsuario == -1) {
        printf("Usuario nao cadastrado.\n");
        return;
    }
    livros[idxLivro].status = 1;
    strcpy(livros[idxLivro].emailUsuario, email);
    printf("Emprestimo registrado.\n");
}

void devolverLivro() {
    int id;
    printf("Codigo do livro: ");
    scanf("%d", &id);
    limparEntrada();
    int idxLivro = encontrarLivroPorId(id);
    if (idxLivro == -1) {
        printf("Livro nao encontrado.\n");
        return;
    }
    if (livros[idxLivro].status == 0) {
        printf("Livro ja esta disponivel.\n");
        return;
    }
    livros[idxLivro].status = 0;
    livros[idxLivro].emailUsuario[0] = '\0';
    printf("Devolucao registrada.\n");
}

int main() {
    int op;
    do {
        printf("\n--- Sistema de Biblioteca ---\n");
        printf("1. Cadastro\n");
        printf("2. Consulta\n");
        printf("3. Atualizacao\n");
        printf("4. Exclusao\n");
        printf("5. Emprestimo\n");
        printf("6. Devolucao\n");
        printf("0. Sair\n");
        printf("Opcao: ");
        scanf("%d", &op);
        limparEntrada();
        switch (op) {
            case 1: menuCadastro(); break;
            case 2: menuConsulta(); break;
            case 3: menuAtualizacao(); break;
            case 4: menuExclusao(); break;
            case 5: emprestarLivro(); break;
            case 6: devolverLivro(); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opcao invalida.\n");
        }
    } while (op != 0);
    return 0;
}
