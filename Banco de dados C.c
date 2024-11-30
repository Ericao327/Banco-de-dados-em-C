#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "clientes.txt"

typedef struct {
    char nome[50];
    char cpf[15];
    char email[50];
    char telefone[15];
    char senha[20];
} Cliente;

// Funções
void menu();
void cadastrarCliente();
void loginCliente();
void editarCliente();
void excluirCliente();
void listarClientes();
int clienteExiste(char *cpf, char *senha);

int main() {
    menu();
    return 0;
}

void menu() {
    int opcao;
    do {
        printf("\n----- MENU -----\n");
        printf("1. Cadastrar Cliente\n");
        printf("2. Login\n");
        printf("3. Editar Cliente\n");
        printf("4. Excluir Cliente\n");
        printf("5. Listar Clientes\n");
        printf("6. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); 

        switch (opcao) {
            case 1:
                cadastrarCliente();
                break;
            case 2:
                loginCliente();
                break;
            case 3:
                editarCliente();
                break;
            case 4:
                excluirCliente();
                break;
            case 5:
                listarClientes();
                break;
            case 6:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 6);
}

void cadastrarCliente() {
    Cliente cliente;
    FILE *file = fopen(FILE_NAME, "a");

    if (!file) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    printf("Nome: ");
    fgets(cliente.nome, 50, stdin);
    cliente.nome[strcspn(cliente.nome, "\n")] = '\0';

    printf("CPF: ");
    fgets(cliente.cpf, 15, stdin);
    cliente.cpf[strcspn(cliente.cpf, "\n")] = '\0';

    printf("E-mail: ");
    fgets(cliente.email, 50, stdin);
    cliente.email[strcspn(cliente.email, "\n")] = '\0';

    printf("Telefone: ");
    fgets(cliente.telefone, 15, stdin);
    cliente.telefone[strcspn(cliente.telefone, "\n")] = '\0';

    printf("Senha: ");
    fgets(cliente.senha, 20, stdin);
    cliente.senha[strcspn(cliente.senha, "\n")] = '\0';

    fwrite(&cliente, sizeof(Cliente), 1, file);
    fclose(file);

    printf("Cliente cadastrado com sucesso!\n");
}

void loginCliente() {
    char cpf[15], senha[20];
    printf("CPF: ");
    fgets(cpf, 15, stdin);
    cpf[strcspn(cpf, "\n")] = '\0';

    printf("Senha: ");
    fgets(senha, 20, stdin);
    senha[strcspn(senha, "\n")] = '\0';

    if (clienteExiste(cpf, senha)) {
        printf("Login realizado com sucesso!\n");
    } else {
        printf("CPF ou senha incorretos.\n");
    }
}

int clienteExiste(char *cpf, char *senha) {
    Cliente cliente;
    FILE *file = fopen(FILE_NAME, "r");

    if (!file) {
        printf("Erro ao abrir o arquivo!\n");
        return 0;
    }

    while (fread(&cliente, sizeof(Cliente), 1, file)) {
        if (strcmp(cliente.cpf, cpf) == 0 && strcmp(cliente.senha, senha) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void editarCliente() {
    char cpf[15], senha[20];
    Cliente cliente;
    FILE *file = fopen(FILE_NAME, "r+");
    int encontrado = 0;

    if (!file) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    printf("CPF: ");
    fgets(cpf, 15, stdin);
    cpf[strcspn(cpf, "\n")] = '\0';

    printf("Senha: ");
    fgets(senha, 20, stdin);
    senha[strcspn(senha, "\n")] = '\0';

    while (fread(&cliente, sizeof(Cliente), 1, file)) {
        if (strcmp(cliente.cpf, cpf) == 0 && strcmp(cliente.senha, senha) == 0) {
            printf("Novo Nome: ");
            fgets(cliente.nome, 50, stdin);
            cliente.nome[strcspn(cliente.nome, "\n")] = '\0';

            printf("Novo E-mail: ");
            fgets(cliente.email, 50, stdin);
            cliente.email[strcspn(cliente.email, "\n")] = '\0';

            printf("Novo Telefone: ");
            fgets(cliente.telefone, 15, stdin);
            cliente.telefone[strcspn(cliente.telefone, "\n")] = '\0';

            fseek(file, -sizeof(Cliente), SEEK_CUR);
            fwrite(&cliente, sizeof(Cliente), 1, file);
            encontrado = 1;
            printf("Cliente editado com sucesso!\n");
            break;
        }
    }

    fclose(file);

    if (!encontrado) {
        printf("Cliente nao encontrado ou senha incorreta.\n");
    }
}

void excluirCliente() {
    char cpf[15], senha[20];
    Cliente cliente;
    FILE *file = fopen(FILE_NAME, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    int encontrado = 0;

    if (!file || !tempFile) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    printf("CPF: ");
    fgets(cpf, 15, stdin);
    cpf[strcspn(cpf, "\n")] = '\0';

    printf("Senha: ");
    fgets(senha, 20, stdin);
    senha[strcspn(senha, "\n")] = '\0';

    while (fread(&cliente, sizeof(Cliente), 1, file)) {
        if (strcmp(cliente.cpf, cpf) == 0 && strcmp(cliente.senha, senha) == 0) {
            encontrado = 1;
        } else {
            fwrite(&cliente, sizeof(Cliente), 1, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);

    if (encontrado) {
        printf("Cliente excluido com sucesso!\n");
    } else {
        printf("Cliente nao encontrado ou senha incorreta.\n");
    }
}

void listarClientes() {
    Cliente cliente;
    FILE *file = fopen(FILE_NAME, "r");

    if (!file) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    printf("\n--- Lista de Clientes ---\n");
    while (fread(&cliente, sizeof(Cliente), 1, file)) {
        printf("Nome: %s\n", cliente.nome);
        printf("CPF: %s\n", cliente.cpf);
        printf("E-mail: %s\n", cliente.email);
        printf("Telefone: %s\n\n", cliente.telefone);
    }

    fclose(file);
}

