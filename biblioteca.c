// Sistema de Biblioteca usando listas ligadas:
// - Usuarios: lista ligada simples (insercao no inicio)
// - Livros: lista ligada dupla (insercao no fim)
// Entradas sem espacos (scanf "%s").
// Comentario sobre uso de LLM: o esqueleto do menu e exemplos de funcoes, 
// e descricao das funcoes
// foram criados com auxilio de LLM; logica e testes ajustados manualmente.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_MAX 100

typedef struct UsuarioNode {
    char email[STR_MAX];
    char nome[STR_MAX];
    struct UsuarioNode *next;
} UsuarioNode;

typedef struct LivroNode {
    int id;
    char titulo[STR_MAX];
    char autor[STR_MAX];
    int ano;
    int status; // 0 disponivel, 1 emprestado
    char emailUsuario[STR_MAX];
    struct LivroNode *prev, *next;
} LivroNode;

UsuarioNode *headUsuarios = NULL;
LivroNode *headLivros = NULL;
int proximoIdLivro = 1;

// Remove leftover input up to end of line to avoid scanf issues
void limparEntrada() { int c; while ((c = getchar()) != '\n' && c != EOF); }

// Busca um usuário pela chave email. Retorna ponteiro ou NULL se não achar.
UsuarioNode *findUsuario(const char *email) {
    for (UsuarioNode *p = headUsuarios; p; p = p->next) if (strcmp(p->email, email) == 0) return p;
    return NULL;
}

// Busca um livro pelo ID. Retorna ponteiro para o nó ou NULL se não existir.
LivroNode *findLivroById(int id) {
    for (LivroNode *p = headLivros; p; p = p->next) if (p->id == id) return p;
    return NULL;
}

// Cadastra um novo usuário no início da lista ligada de usuários.
// Verifica duplicação pelo email antes de inserir.
void cadastrarUsuario() {
    char email[STR_MAX], nome[STR_MAX];
    printf("Email (sem espacos): "); if (scanf("%99s", email) != 1) { limparEntrada(); return; } limparEntrada();
    if (findUsuario(email)) { printf("Usuario ja existe\n"); return; }
    printf("Nome (sem espacos): "); if (scanf("%99s", nome) != 1) { limparEntrada(); return; } limparEntrada();
    UsuarioNode *n = malloc(sizeof(UsuarioNode)); if (!n) { printf("Erro memoria\n"); return; }
    strcpy(n->email, email); strcpy(n->nome, nome); n->next = headUsuarios; headUsuarios = n;
    printf("Usuario cadastrado\n");
}

// Cadastra um novo livro ao final da lista dupla de livros.
// Atribui um ID sequencial automaticamente.
void cadastrarLivro() {
    char titulo[STR_MAX], autor[STR_MAX]; int ano;
    printf("Titulo (sem espacos): "); if (scanf("%99s", titulo) != 1) { limparEntrada(); return; } limparEntrada();
    printf("Autor (sem espacos): "); if (scanf("%99s", autor) != 1) { limparEntrada(); return; } limparEntrada();
    printf("Ano: "); if (scanf("%d", &ano) != 1) { limparEntrada(); return; } limparEntrada();
    LivroNode *n = malloc(sizeof(LivroNode)); if (!n) { printf("Erro memoria\n"); return; }
    n->id = proximoIdLivro++;
    strcpy(n->titulo, titulo);
    strcpy(n->autor, autor);
    n->ano = ano;
    n->status = 0;
    n->emailUsuario[0] = '\0';
    n->prev = n->next = NULL;
    if (!headLivros) headLivros = n; else { LivroNode *p = headLivros; while (p->next) p = p->next; p->next = n; n->prev = p; }
    printf("Livro cadastrado. ID=%d\n", n->id);
}

// Consulta livros: por código (ID) ou por autor, e exibe resultados.
void consultaLivros() {
    int op; printf("1-codigo 2-autor 0-voltar: "); if (scanf("%d", &op) != 1) { limparEntrada(); return; } limparEntrada();
    if (op == 1) {
        int id; printf("Codigo: "); if (scanf("%d", &id) != 1) { limparEntrada(); return; } limparEntrada();
        LivroNode *l = findLivroById(id); if (!l) { printf("Livro nao encontrado\n"); return; }
        printf("ID:%d T:%s A:%s Ano:%d St:%s\n", l->id, l->titulo, l->autor, l->ano, l->status==0?"Disp":"Emp");
        if (l->status) printf("Emprestado para: %s\n", l->emailUsuario);
    } else if (op == 2) {
        char autor[STR_MAX]; printf("Autor: "); if (scanf("%99s", autor) != 1) { limparEntrada(); return; } limparEntrada();
        int ach = 0; for (LivroNode *p = headLivros; p; p = p->next) if (strcmp(p->autor, autor) == 0) { printf("ID:%d T:%s Ano:%d St:%s\n", p->id, p->titulo, p->ano, p->status==0?"Disp":"Emp"); ach = 1; }
        if (!ach) printf("Livro nao encontrado\n");
    }
}

// Consulta usuários: por email ou por nome, e exibe resultados.
void consultaUsuarios() {
    int op; printf("1-email 2-nome 0-voltar: "); if (scanf("%d", &op) != 1) { limparEntrada(); return; } limparEntrada();
    if (op == 1) {
        char email[STR_MAX]; printf("Email: "); if (scanf("%99s", email) != 1) { limparEntrada(); return; } limparEntrada();
        UsuarioNode *u = findUsuario(email); if (!u) printf("Usuario nao cadastrado\n"); else printf("Email:%s Nome:%s\n", u->email, u->nome);
    } else if (op == 2) {
        char nome[STR_MAX]; printf("Nome: "); if (scanf("%99s", nome) != 1) { limparEntrada(); return; } limparEntrada();
        int ach = 0; for (UsuarioNode *p = headUsuarios; p; p = p->next) if (strcmp(p->nome, nome) == 0) { printf("Email:%s Nome:%s\n", p->email, p->nome); ach = 1; }
        if (!ach) printf("Usuario nao cadastrado\n");
    }
}

// Lista os empréstimos de um usuário (busca por email).
void consultaEmprestimos() {
    char email[STR_MAX]; printf("Email do usuario: "); if (scanf("%99s", email) != 1) { limparEntrada(); return; } limparEntrada();
    if (!findUsuario(email)) { printf("Usuario nao cadastrado\n"); return; }
    int ach = 0; for (LivroNode *p = headLivros; p; p = p->next) if (p->status == 1 && strcmp(p->emailUsuario, email) == 0) { printf("ID:%d T:%s A:%s Ano:%d\n", p->id, p->titulo, p->autor, p->ano); ach = 1; }
    if (!ach) printf("Nenhum livro emprestado\n");
}

// Atualiza título, autor e ano de um livro a partir do ID.
void atualizarLivro() {
    int id; printf("Codigo do livro: "); if (scanf("%d", &id) != 1) { limparEntrada(); return; } limparEntrada();
    LivroNode *l = findLivroById(id); if (!l) { printf("Livro nao encontrado\n"); return; }
    printf("Novo titulo: "); if (scanf("%99s", l->titulo) != 1) { limparEntrada(); return; } limparEntrada();
    printf("Novo autor: "); if (scanf("%99s", l->autor) != 1) { limparEntrada(); return; } limparEntrada();
    printf("Novo ano: "); if (scanf("%d", &l->ano) != 1) { limparEntrada(); return; } limparEntrada();
    printf("Livro atualizado\n");
}

// Atualiza o nome de um usuário identificado pelo email.
void atualizarUsuario() {
    char email[STR_MAX]; printf("Email do usuario: "); if (scanf("%99s", email) != 1) { limparEntrada(); return; } limparEntrada();
    UsuarioNode *u = findUsuario(email); if (!u) { printf("Usuario nao cadastrado\n"); return; }
    printf("Novo nome: "); if (scanf("%99s", u->nome) != 1) { limparEntrada(); return; } limparEntrada();
    printf("Usuario atualizado\n");
}

// Remove um livro da lista dupla pelo seu ID e libera memória.
void excluirLivro() {
    int id; printf("Codigo do livro: "); if (scanf("%d", &id) != 1) { limparEntrada(); return; } limparEntrada();
    LivroNode *l = findLivroById(id); if (!l) { printf("Livro nao encontrado\n"); return; }
    if (l->prev) l->prev->next = l->next; else headLivros = l->next;
    if (l->next) l->next->prev = l->prev;
    free(l);
    printf("Livro excluido\n");
}

// Remove um usuário da lista simples pelo email se não tiver livros emprestados.
void excluirUsuario() {
    char email[STR_MAX]; printf("Email: "); if (scanf("%99s", email) != 1) { limparEntrada(); return; } limparEntrada();
    UsuarioNode *u = findUsuario(email); if (!u) { printf("Usuario nao cadastrado\n"); return; }
    for (LivroNode *p = headLivros; p; p = p->next) if (p->status == 1 && strcmp(p->emailUsuario, email) == 0) { printf("Usuario tem livros emprestados. Nao pode excluir\n"); return; }
    UsuarioNode *prev = NULL;
    for (UsuarioNode *p = headUsuarios; p; prev = p, p = p->next) {
        if (p == u) {
            if (prev) prev->next = p->next; else headUsuarios = p->next;
            free(p);
            printf("Usuario excluido\n");
            return;
        }
    }
}

// Registra um empréstimo: marca livro como emprestado e guarda email do usuário.
void emprestarLivro() {
    int id; char email[STR_MAX]; printf("Codigo do livro: "); if (scanf("%d", &id) != 1) { limparEntrada(); return; } limparEntrada();
    LivroNode *l = findLivroById(id); if (!l) { printf("Livro nao encontrado\n"); return; }
    if (l->status == 1) { printf("Livro ja emprestado\n"); return; }
    printf("Email do usuario: "); if (scanf("%99s", email) != 1) { limparEntrada(); return; } limparEntrada();
    if (!findUsuario(email)) { printf("Usuario nao cadastrado\n"); return; }
    l->status = 1; strcpy(l->emailUsuario, email); printf("Emprestimo registrado\n");
}

// Registra a devolução do livro (marca disponível e limpa email do usuário).
void devolverLivro() {
    int id; printf("Codigo do livro: "); if (scanf("%d", &id) != 1) { limparEntrada(); return; } limparEntrada();
    LivroNode *l = findLivroById(id); if (!l) { printf("Livro nao encontrado\n"); return; }
    if (l->status == 0) { printf("Livro ja disponivel\n"); return; }
    l->status = 0; l->emailUsuario[0] = '\0'; printf("Devolucao registrada\n");
}

// Libera toda a memória alocada das listas de livros e usuários.
void liberarTudo() {
    while (headLivros) { LivroNode *t = headLivros->next; free(headLivros); headLivros = t; }
    while (headUsuarios) { UsuarioNode *t = headUsuarios->next; free(headUsuarios); headUsuarios = t; }
}

void menuCadastro() { int op; do{ printf("\n--- Cadastro ---\n1-Livros 2-Usuarios 0-Voltar\nOpcao: "); if(scanf("%d", &op)!=1){ limparEntrada(); op=0;} limparEntrada(); if(op==1) cadastrarLivro(); else if(op==2) cadastrarUsuario(); } while(op!=0); }
void menuConsulta() { int op; do{ printf("\n--- Consulta ---\n1-Livros 2-Usuarios 3-Emprestimos 0-Voltar\nOpcao: "); if(scanf("%d", &op)!=1){ limparEntrada(); op=0;} limparEntrada(); if(op==1) consultaLivros(); else if(op==2) consultaUsuarios(); else if(op==3) consultaEmprestimos(); } while(op!=0); }
void menuAtualizacao() { int op; do{ printf("\n--- Atualizacao ---\n1-Livros 2-Usuarios 0-Voltar\nOpcao: "); if(scanf("%d", &op)!=1){ limparEntrada(); op=0;} limparEntrada(); if(op==1) atualizarLivro(); else if(op==2) atualizarUsuario(); } while(op!=0); }
void menuExclusao() { int op; do{ printf("\n--- Exclusao ---\n1-Livros 2-Usuarios 0-Voltar\nOpcao: "); if(scanf("%d", &op)!=1){ limparEntrada(); op=0;} limparEntrada(); if(op==1) excluirLivro(); else if(op==2) excluirUsuario(); } while(op!=0); }

int main(){
    int op;
    do{
        printf("\n--- Sistema de Biblioteca (lista ligada) ---\n");
        printf("1-Cadastro 2-Consulta 3-Atualizacao 4-Exclusao 5-Emprestimo 6-Devolucao 0-Sair\nOpcao: ");
        if(scanf("%d", &op)!=1){ limparEntrada(); continue; }
        limparEntrada();
        if(op==1) menuCadastro();
        else if(op==2) menuConsulta();
        else if(op==3) menuAtualizacao();
        else if(op==4) menuExclusao();
        else if(op==5) emprestarLivro();
        else if(op==6) devolverLivro();
        else if(op==0) { printf("Saindo...\n"); }
        else printf("Opcao invalida\n");
    } while(op!=0);

    liberarTudo();
    return 0;
}
