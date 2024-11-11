#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//sifra de cesar 
void Criptografar(char *text, int chave) {
    int i;
    for (i = 0; i < strlen(text); i++) {
        char c = text[i];
        
        
        if (c >= 'A' && c <= 'Z') {
            text[i] = ((c - 'A' + chave) % 26) + 'A';
        }
        
        else if (c >= 'a' && c <= 'z') {
            text[i] = ((c - 'a' + chave) % 26) + 'a';
        }
    }
}

void Descriptografar(char *text, int chave) {
    Criptografar(text, 26 - chave);
}

int validarSenhaForte(char *senha) {
    int temMaiuscula = 0;
    int temNumero = 0;
    
    int tamanho = strlen(senha);

    if (tamanho < 8) {
        printf("A senha deve ter pelo menos 8 caracteres.\n\n");
        return 0;
    }
    
    int i;
    for (i = 0; i < tamanho; i++) {
        if (senha[i] >= 'A' && senha[i] <= 'Z') {
            temMaiuscula = 1;
        }
        if (senha[i] >= '0' && senha[i] <= '9') {
            temNumero = 1;
        }
    }

    if (temMaiuscula && temNumero) {
        return 1;
    } else {
        printf("A senha deve conter pelo menos uma letra maiuscula e um numero.\n\n");
        return 0;
    }
}


typedef struct {
    int id;
    char nome[50];
    char senha[50];
    int nivelPermissao;
} Usuario;

void adicionarUsuario(FILE *arquivo, Usuario usuario, char chave) {
    	Criptografar(usuario.nome, chave);  
        Criptografar(usuario.senha, chave);
    fprintf(arquivo, "%d,%s,%s,%d\n", usuario.id, usuario.nome, usuario.senha, usuario.nivelPermissao);

}

void listarUsuarios(FILE *arquivo, char chave) {
    Usuario usuario;
    rewind(arquivo);
    while (fscanf(arquivo, "%d,%49[^,],%49[^,],%d", &usuario.id, usuario.nome, usuario.senha, &usuario.nivelPermissao) != EOF) {
        Descriptografar(usuario.nome, chave);  
        Descriptografar(usuario.senha, chave);
        
        char *nivel; 
        switch (usuario.nivelPermissao) {
            case 1: nivel = "Admin"; break;
            case 2: nivel = "Desenvolvedor"; break;
            case 3: nivel = "Consultor"; break;
            default: nivel = "Desconhecido"; break;
        }
        
        printf("\nID: %d, Nome: %s, Senha: %s, Nivel de Permissao: %s\n", usuario.id, usuario.nome, usuario.senha, nivel);
    }
}


void excluirUsuario(FILE *arquivo, int id) {
    FILE *temp = fopen("temp.txt", "wb");

    Usuario usuario;
    int encontrado = 0;
    rewind(arquivo);

    while (fread(&usuario, sizeof(Usuario), 1, arquivo)) {
        if (usuario.id != id) {
            fwrite(&usuario, sizeof(Usuario), 1, temp);
        } else {
            encontrado = 1;
        }
    }

    fclose(arquivo);
    fclose(temp);

    remove("usuarios.txt");
    rename("temp.txt", "usuarios.txt");

    if (encontrado) {
        printf("Usuario excluído com sucesso.\n");
    } else printf("Usuario não encontrado.\n");
}


int main() {
    int acao = 0; 
    FILE *arquivo = fopen("usuarios.txt", "w+");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }
    srand(time(NULL));
    
    int chave = (rand() % 25) + 1;
    
	while(1){ 
	    printf("-----------------------------------\nDigite a acao: \n\n1 (INCLUIR USUARIO)\n2 (EXCLUIR USUARIO)\n3 (LISTAR USUARIOS)\n4 (SAIR)\n"); 
	    scanf("%d", &acao);
	
        if (acao == 1) {
            Usuario usuario;
            printf("Digite o ID do usuario: ");
            scanf("%d", &usuario.id);
            
            printf("Digite o nome do usuario: ");
            scanf("%s", usuario.nome);
            
            do {
				printf("Digite a senha do usuario: ");
	            scanf("%s", usuario.senha);
	    	}while(validarSenhaForte(usuario.senha) == 0);
	    	
            printf("Digite o nivel de permissao 1(ADMIN), 2 (DESENVOLVEDOR), 3 (CONSULTOR): ");
            scanf("%d", &usuario.nivelPermissao);
            
			adicionarUsuario(arquivo, usuario, chave);
        } else if (acao == 2) {
            int id;
            printf("Digite o ID do usuario a ser excluido: ");
            scanf("%d", &id);
            excluirUsuario(arquivo, id);
            arquivo = fopen("usuarios.txt", "r+");
        } else if (acao == 3) {
            listarUsuarios(arquivo, chave);
        } else if (acao == 4) {
            break;
        } else printf("Acao nao suportada\n");
    }

    fclose(arquivo);

    return 0;
}
