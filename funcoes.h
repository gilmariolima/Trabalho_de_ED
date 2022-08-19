#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define VERMELHO "\x1b[31m"
#define VERDE "\x1b[32m"
#define RESET "\x1b[0m"

typedef struct Vertice{
    int id;
    char nome_aluno[100];
    int matricula;
    char descricao[200];

    struct Vertice * esq;
    struct Vertice * dir;
}VERTICE;

typedef struct fila{
    int id;
    char * nome_aluno;
    int matricula;
    char * descricao;

    char campus_origem[50];
    char campus_destino[50];
    int responsavel;
    int prioridade;

    struct fila * prox;
    struct fila * ant;
}fila;

typedef struct funcionario{
    char * nome_fun;
    int cpf_fun;
    int senha_fun;
    int cargo; //1 - transportador / 2 - secretario / 3 - gerente

    struct funcionario * prox;
}funcionario;

VERTICE * raiz = NULL;

fila * inicio_fila = NULL;
fila * fim_fila = NULL;
int tam_fila = 0;

funcionario * inicio_fun = NULL;
funcionario * fim_fun = NULL;
int tam_fun = 0;

VERTICE * buscar(int id, VERTICE * aux){
    if(aux == NULL){
        return NULL; 
    }else if(id == aux->id){
        return aux; 
    }else if(id < aux->id){ //buscar no lado esquerdo
        if(aux->esq != NULL){
            return buscar(id, aux->esq);
        }else{// esquerda ta vazia
            return aux;
        }
    }else{//buscar pelo lado direito
        if(aux->dir != NULL){
            return buscar(id, aux->dir);
        }else{//direita ta vazia
            return aux;
        }
    }
}

char * buscar_resp(int cpf_resp){
    funcionario * resp = inicio_fun;
    for(int i=0; i < tam_fun; i++){
        if(resp->cpf_fun == cpf_resp){
            return resp->nome_fun;
        }else{
            resp = resp->prox;
        }
    }
    return "0";
}

int cadastrar_encomenda(int id){
    VERTICE * busca = buscar(id, raiz);
    if(busca == NULL || busca->id != id){// vazia OU a adição é permitida porquê o busca->ID é diferente do X    
        VERTICE * novo = malloc (sizeof(VERTICE));
        novo->id = id;

        printf("----CADASTRAR----\n\nId da encomenda %d\n", id);
        printf("Aluno: ");
        scanf(" %[^\n]s",novo->nome_aluno);
        printf("Matricula: ");
        scanf("%d", &novo->matricula);
        printf("Descricao: ");
        scanf(" %[^\n]s",novo->descricao);
        printf("\n");

        novo->esq = NULL;
        novo->dir = NULL;

        if(busca == NULL){ //add na raiz
            raiz = novo;
        }else{
            if(id < busca->id){
                busca->esq = novo;
            }else{
                busca->dir = novo;
            }
        }
        system("cls");
        printf(VERDE"Encomenda de %s cadastrada"RESET"\n", novo->nome_aluno);
    }else{
        printf("ID ""%d"" DUPLICADO!! A adição não foi realizada pois já existe um ID ""%d"". \n", id, id);
        return 0;
    }
}

void add_funcionario(char * nome, int cpf, int senha, int cargo){
    funcionario * novo_fun = malloc(sizeof(funcionario));
    novo_fun->nome_fun = nome;
    novo_fun->cpf_fun = cpf;
    novo_fun->senha_fun = senha;
    novo_fun->cargo = cargo;
    novo_fun->prox = NULL;

    if(inicio_fun == NULL){
        inicio_fun = novo_fun;
        fim_fun = novo_fun;
    }else{
        fim_fun->prox = novo_fun;
        fim_fun = novo_fun;
    }
    tam_fun++;
}

void add_fila(int id, char * nome_aluno, int matricula, char * descricao, int cpf_resp){
    fila * novo = malloc(sizeof(fila));
    novo->id = id;
    novo->nome_aluno = nome_aluno;
    novo->matricula = matricula;
    novo->descricao = descricao;
    novo->responsavel = cpf_resp;

    printf("---DADOS---\n"VERMELHO"-------------------------------"RESET"\n");
    printf("Id: %d\nNome: %s\nMatricula: %d\nDescricao: %s\nResponsavel: %s\n", id, nome_aluno, matricula, descricao, buscar_resp(cpf_resp));
    printf(VERMELHO"-------------------------------"RESET"\n");
    printf("Campus onde o livro esta: ");
    scanf(" %[^\n]s",novo->campus_origem);
    printf("Campus de destino: ");
    scanf(" %[^\n]s",novo->campus_destino);
    printf("Prioridade [0-100]: ");
    scanf("%d", &novo->prioridade);
    printf("\n");

    novo->prox = NULL;
    novo->ant = NULL;

    if(inicio_fila == NULL){
        inicio_fila = novo;
        fim_fila = novo;
    }else{
        if(novo->prioridade > inicio_fila->prioridade || novo->prioridade == inicio_fila->prioridade){
            novo->prox = inicio_fila;
            inicio_fila->ant = novo;
            inicio_fila = novo;
        }else if(novo->prioridade < fim_fila->prioridade || novo->prioridade == fim_fila->prioridade){
            fim_fila->prox = novo;
            novo->ant = fim_fila;
            fim_fila = novo;
        }else{
            fila * aux = inicio_fila->prox;
            while(novo->prioridade < aux->prioridade || novo->prioridade == aux->prioridade){
                aux = aux->prox;
            }
            aux->ant->prox = novo;
            novo->ant = aux->ant;
            novo->prox = aux;
            aux->ant = novo;
        }
    }
    tam_fila++;
    system("cls");
}

VERTICE * remover(VERTICE * aux, int id, int cpf_resp){
    if(raiz != NULL){
        VERTICE * busca = buscar(id, raiz);
        if(busca != NULL){
            if(aux == NULL){
                printf(VERMELHO"Não encontrado"RESET"\n");
                return NULL;
            }else if(aux->id > id){
                aux->esq = remover(aux->esq, id, cpf_resp);
            }else if(aux->id < id){
                aux->dir = remover(aux->dir, id, cpf_resp);
            }else{
                if(aux->esq == NULL && aux->dir == NULL){
                    add_fila(aux->id, aux->nome_aluno, aux->matricula, aux->descricao, cpf_resp);
                    aux = NULL;
                    printf(VERMELHO"Id: %d Removido"RESET"\n", id);
                    return aux;
                }else if(aux->esq == NULL){
                    add_fila(aux->id, aux->nome_aluno, aux->matricula, aux->descricao, cpf_resp);
                    aux = aux->dir;
                    printf(VERMELHO"Id: %d Removido"RESET"\n", id);
                    return aux;
                }else if(aux->dir == NULL){
                    add_fila(aux->id, aux->nome_aluno, aux->matricula, aux->descricao, cpf_resp);
                    aux = aux->esq;
                    printf(VERMELHO"Id: %d Removido"RESET"\n", id);
                    return aux;
                }else{
                    VERTICE * f = aux->esq;
                    while(f->dir != NULL){
                        f = f->dir;
                    }
                    aux->id = f->id;
                    strcpy(aux->nome_aluno, f->nome_aluno);
                    aux->matricula = f->matricula;
                    strcpy(aux->descricao, f->descricao);
                
                    f->id = id;
                    aux->esq = remover(aux->esq, id, cpf_resp);
                }
            }
            return aux;
            free(aux);
        }
    }else{
        printf(VERMELHO"Inválido nada removido"RESET"\n");
        return 0;
    }
}

void remover_fila(){
    if(tam_fila == 0){
        printf(VERMELHO"A lista esta vazia"RESET"\n");
    }else{
        fila * lixo = inicio_fila;
        printf(VERDE"Encomenda de %s - Id: %d foi retirado"RESET"\n", inicio_fila->nome_aluno, inicio_fila->id);
        if(tam_fila == 1){
            inicio_fila = NULL;
            fim_fila = NULL;
        }else{
            inicio_fila = inicio_fila->prox;
            inicio_fila->ant = NULL;
        }
        free(lixo);
        tam_fila--;    
    }
}

void ver_fila(){
    if(tam_fila > 0){
        fila * aux = inicio_fila;
        printf("----FILA----\n");
        for(int i = 0; i < tam_fila; i++){ 
            printf(VERMELHO"-------------------------------"RESET"\n");
            printf("id: %d\nNome: %s\nMatricula: %d\nDescricao: %s\nResponsavel: %s\nCampus origem: %s\nCampus destino: %s\nPrioridade: "VERDE"%d"RESET"\n", aux->id, aux->nome_aluno, aux->matricula, aux->descricao,buscar_resp(aux->responsavel),aux->campus_origem, aux->campus_destino, aux->prioridade);
            aux = aux->prox;
        }
        printf(VERMELHO"------------------------------'"RESET"\n");
    }else{
        printf(VERMELHO"A fila esta vazia"RESET"\n");
    }
}

int login(int cargo){
    int cpf;
    int senha;

    printf("----LOGIN----\n");
    printf("Cpf: ");
    scanf("%d",&cpf);
    printf("Senha: ");
    scanf("%d",&senha);

    funcionario * aux_fun = inicio_fun;

    for(int i=0; i < tam_fun; i++){
        if(aux_fun->cpf_fun == cpf && aux_fun->senha_fun == senha){
            if(aux_fun->cargo == cargo || aux_fun->cargo == 3){
                system("cls");
                printf(VERDE"Bem vindo(a) %s"RESET"\n", aux_fun->nome_fun);
                return cpf;
            }else{
                if(cargo == 1){
                    system("cls");
                    printf(VERMELHO"Apenas Transportadores podem fazer isso"RESET"\n");
                    return 1;
                }else if(cargo == 2){
                    system("cls");
                    printf(VERMELHO"Apenas Secretarios podem fazer isso"RESET"\n");
                    return 1;
                }else if(cargo == 3){
                    system("cls");
                    printf(VERMELHO"Apenas Gerentes podem fazer isso"RESET"\n");
                    return 1;
                }
            }
        }
        aux_fun = aux_fun->prox;
    }
    return 0;
}

void in_ordem(VERTICE * aux){
    if(raiz == NULL){
        printf(VERMELHO"Não tem nenhum pedido"RESET"\n");
        return;
    }
    if(aux->esq != NULL){
        in_ordem(aux->esq);
    }
    printf("Id: %d\nAluno: %s\nMatricula: %d\nDescricao: %s\n"VERMELHO"-------------------------------"RESET"\n", aux->id, aux->nome_aluno, aux->matricula, aux->descricao);
    if(aux->dir != NULL){
        in_ordem(aux->dir);
    }
}

int menos500(){
    srand(time(NULL));
    int id = rand() % 498;
    id += 1;
    return id;
}

int mais500(){
    srand(time(NULL));
    int id = rand() % 499;
    id += 501;
    return id;
}

int menu(){
    add_funcionario("Gilmario", 123, 12, 3);
    add_funcionario("Alonso", 125, 25, 2);
    add_funcionario("Ze", 888, 88, 2);
    add_funcionario("Yuri", 124, 24, 1);
    add_funcionario("Paula", 126, 26, 1);
    add_funcionario("Felipe", 222, 22, 1);
    add_funcionario("Levi", 555, 55, 1);
    add_funcionario("Alice", 333, 33, 1);
    system("cls");
    int cpf_resp;
    int opc = -1;
    int cont = 0;
    char nome_fun[100];
    int id, cpf_fun, senha_fun, cargo;

    while(opc != 0){
        printf("\n[ 1 ] Cadastrar encomenda\n[ 2 ] Mostrar encomendas\n[ 3 ] Deletar encomenda\n[ 4 ] Ver fila de prioridade\n[ 5 ] Remover da fila\n[ 6 ] Cadastrar funcionario\n[ 0 ] Sair\nEscolha uma opcao >> ");
        scanf("%d", &opc);
        printf("\n");
        switch(opc){
            case 1:
                system("cls");
                if(raiz == NULL)
                    id = 500;
                else if(cont % 2 == 0)
                    id = mais500();
                else
                    id = menos500();

                cont++;
                cadastrar_encomenda(id);
                break;
            case 2:
                system("cls");
                if(raiz != NULL){
                    printf("----ENCOMENDAS----\n");
                    printf(VERMELHO"-------------------------------"RESET"\n");
                    in_ordem(raiz);
                    cont = 1;
                    while(cont != 0){
                        printf(VERMELHO"Digite 0 para sair: "RESET);
                        scanf("%d", &cont); 
                    }      
                    system("cls");
                }else{
                    printf(VERMELHO"Nenhuma encomenda cadastrada"RESET"\n");
                }
                break;
            case 3:
                if(raiz != NULL){
                    system("cls");
                    cpf_resp = login(2);
                    if(cpf_resp > 1){
                        printf("\n----ENCOMENDAS----\n");
                        printf(VERMELHO"-------------------------------"RESET"\n");
                        in_ordem(raiz);
                        printf("\n----REMOVER----\n");
                        printf("Digite o id: ");
                        scanf("%d", &id);
                        
                        printf("\n");
                        system("cls");

                        raiz = remover(raiz, id, cpf_resp);
                    }else if(cpf_resp == 0){
                        system("cls");
                        printf(VERMELHO"Cpf ou senha incorreto"RESET"\n");
                    }
                }else{
                    system("cls");
                    printf(VERMELHO"Nenhuma encomenda cadastrada"RESET"\n");
                }
                break;
            case 4:
                system("cls");
                ver_fila();
                int i = 1;
                if(tam_fila > 0){
                    while(i != 0){
                        printf(VERMELHO"Digite 0 para sair: "RESET);
                        scanf("%d", &i);
                    }
                    system("cls");
                }
                break;
            case 5:
                system("cls");
                if(tam_fila == 0){
                    printf(VERMELHO"A fila esta vazia"RESET"\n");
                }else{
                    cpf_resp = login(1);
                    if(cpf_resp > 1){
                        remover_fila();
                    }else if(cpf_resp == 0){
                        system("cls");
                        printf(VERMELHO"Cpf ou senha incorrreto"RESET"\n");
                    }
                }
                break;
            case 6:
                system("cls");
                cpf_resp = login(3);
                if(cpf_resp > 1){
                    printf("\n----CADASTRAR FUNCIONARIO----\n\n");
                    printf("Nome: ");
                    scanf(" %[^\n]s",nome_fun);
                    printf("Cpf: ");
                    scanf("%d", &cpf_fun);
                    printf("Senha: ");
                    scanf("%d", &senha_fun);
                    printf("\n1- Transportador\n2- Secretario\nCargo >> ");
                    scanf("%d", &cargo);

                    if(buscar_resp(cpf_fun) == "0"){
                        add_funcionario(nome_fun, cpf_fun, senha_fun, cargo);
                        system("cls");
                        printf(VERDE"Funcionario Cadastrado"RESET"\n");
                    }else{
                        system("cls");
                        printf(VERMELHO"Esse funcionario ja esta cadastrado"RESET"\n");
                    }
                }else if(cpf_resp == 0){
                    system("cls");
                    printf(VERMELHO"Cpf ou senha incorrreto"RESET"\n");
                }    
                break;
            case 0:
                return 0;
                break;
            default:
                system("cls");
                printf(VERMELHO"Opção Invalida"RESET"\n");
                break;
        }
    }
}