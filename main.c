#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* REGISTRO DE DADOS COM CAMPOS DE TAMANHO FIXO E CAMPOS DE TAMANHO VARIÁVEL */
struct registro{
    char cnpj[18], dataRegistro[10], dataCancelamento[10], cnpjAuditor[18];
    char *nomeSocial, *nomeFantasia, *motivoCancelamento, *nomeEmpresa;	
};
/* FUNÇÃO DE MENU */
void menu() {
    printf("(1) Remoção lógica dos registros\n");
    printf("(2) Inserir registros\n");
    printf("(3) Visualizar estatisticas sobre arquivo de índice\n");
    printf("(4) Visualizar estatisticas sobre registros removidos\n");
    printf("(0) Sair");
}

void inserir() {

}

int main(int argc, char **argv) {
    //Remover arquivos de execução passada
    if(remove("arquivo1.dat") == 0 && remove("indice1.dat") == 0
            && remove("arquivo2.dat") == 0 && remove("indice2.dat") == 0
            && remove("arquivo3.dat") == 0 && remove("indice3.dat") == 0) {
        printf("Removido com sucesso!\n");
    } else {
        printf("Erro ao remover. Talvez os arquivos não existam. Ignorar...\n");
    }

    FILE *csv = fopen("../SCC0215012017projeto01turmaBdadosCompanhias.csv", "rt");
    FILE *arquivos[3] = {fopen("arquivo1.dat", "a+b"), fopen("arquivo2.dat", "a+b"), fopen("arquivo3.dat", "a+b")};
    FILE *indices[3] = {fopen("indice1.dat", "a+b"), fopen("indice2.dat", "a+b"), fopen("indice3.dat", "a+b")};
    char linha[500], *result, *campo, cnpj[18];
    struct registro Registro;
    int i = 0, tamanho_registro = 0, opcao, qual, bytes, posicao = 0;

    Registro.nomeSocial = (char *) malloc(200);
    Registro.nomeFantasia = (char *) malloc(200);
    Registro.motivoCancelamento = (char *) malloc(200);
    Registro.nomeEmpresa = (char *) malloc(200);

    if (csv == NULL) {  // Se houve erro na abertura
        printf("Problemas na abertura do arquivo\n");
        return 0;
    }

    while(!feof(csv)) {
        result = fgets(linha, 500, csv);
        if(result) {
            campo = strtok(linha, ";");
            strcpy(Registro.cnpj, campo);
            puts(Registro.cnpj);
            i++;
            while(campo != NULL) {
                campo = strtok(NULL, ";");
                if(i == 0) {
                    strcpy(Registro.cnpj, campo);
                    puts(Registro.cnpj);
                }
                if(i == 1) {
                    strcpy(Registro.nomeSocial, campo);
                    puts(Registro.nomeSocial);
                }
                if(i == 2) { 
                    strcpy(Registro.nomeFantasia, campo); 
                    puts(Registro.nomeFantasia); 
                }
                if(i == 3) { 
                    strcpy(Registro.dataRegistro, campo); 
                    puts(Registro.dataRegistro); 
                }
                if(i == 4) { 
                    strcpy(Registro.dataCancelamento, campo); 
                    puts(Registro.dataCancelamento); 
                }
                if(i == 5) { 
                    strcpy(Registro.motivoCancelamento, campo);
                    puts(Registro.motivoCancelamento);
                }
                if(i == 6) { 
                    strcpy(Registro.nomeEmpresa, campo);
                    puts(Registro.nomeEmpresa);
                }
                if(i == 7) {
                    campo = strtok(campo, "\r");
                    //nomeSocial fica com pau aqui
                    strcpy(Registro.cnpjAuditor, campo);
                    puts(Registro.cnpjAuditor);
                    printf("\n");
                }
                i++;
            }

            for(int j = 0; j < 3; j++) {
                fwrite(Registro.cnpj, 1, 18, arquivos[j]);
                fwrite(Registro.nomeSocial, 1, strlen(Registro.nomeSocial), arquivos[j]);
                fwrite("|", 1, 1, arquivos[j]);
                fwrite(Registro.nomeFantasia, 1, strlen(Registro.nomeFantasia), arquivos[j]);
                fwrite("|", 1, 1, arquivos[j]);
                fwrite(Registro.dataRegistro, 1, 8, arquivos[j]);
                fwrite(Registro.dataCancelamento, 1, 8, arquivos[j]);
                fwrite(Registro.motivoCancelamento, 1, strlen(Registro.motivoCancelamento), arquivos[j]);
                fwrite("|", 1, 1, arquivos[j]);
                fwrite(Registro.nomeEmpresa, 1, strlen(Registro.nomeEmpresa), arquivos[j]);
                fwrite("|", 1, 1, arquivos[j]);
                fwrite(Registro.cnpjAuditor, 1, 18, arquivos[j]);
                fwrite("#\n", 1, 2, arquivos[j]); //delimitador de registro é #\n

                fwrite(Registro.cnpj, 1, 18, indices[j]);
                fwrite("\t", 1, 1, indices[j]);
            }

            //grava o tamanho do registro no arquivo de índice
            tamanho_registro += sizeof(Registro.cnpj) + strlen(Registro.nomeSocial) +
            strlen(Registro.nomeFantasia) + sizeof(Registro.dataRegistro) +
            sizeof(Registro.dataCancelamento) + strlen(Registro.motivoCancelamento) +
            strlen(Registro.nomeEmpresa) + sizeof(Registro.cnpjAuditor) + 1;
            printf("%d\n", tamanho_registro);
            sprintf(result, "%d", posicao); //reusando result que está inútil

            //evitar que o posicao seja atualizado a qualquer custo!

            posicao = tamanho_registro;
            for(int j = 0; j < 3; j++) {
                fwrite(result, 1, 5, indices[j]);	//5 porque é quantos caracteres no maximo ele terá
                fwrite("#", 1, 1, indices[j]);
                fwrite("\n", 1, 1, indices[j]);
            }

            i = 0;
        }
    }

    fclose(csv);
    for(int j = 0; j < 3; j++) {
        fclose(arquivos[j]);
        fclose(indices[j]);
    }
    /*******************************/
    /* FUNCIONALIDADES COMEÇAM AQUI*/
    /*******************************/

    while(1) {
        printf("Digite a opcao desejada do menu\n");
        menu();
        scanf("%d", &opcao);
        switch(opcao) {
            case 1:
                printf("Qual arquivo deseja remover um registro?\n");
                scanf("%d", &qual);
                if(qual > 3 || qual < 1) {
                    printf("Escolha entre arquivos 1, 2 ou 3\n");
                    break;
                }

                if(qual == 1) {
                    indices[qual-1] = fopen("indice1.dat", "r+b");
                    arquivos[qual-1] = fopen("arquivo1.dat", "r+b");
                }
                if(qual == 2) {
                    indices[qual-1] = fopen("indice2.dat", "r+b");
                    arquivos[qual-1] = fopen("arquivo2.dat", "r+b");
                }
                if(qual == 3) {
                    indices[qual-1] = fopen("indice3.dat", "r+b");
                    arquivos[qual-1] = fopen("arquivo3.dat", "r+b");
                }

                rewind(indices[qual-1]);
                rewind(arquivos[qual-1]);
                printf("HUE%ld\n", ftell(arquivos[qual-1]));
                printf("Digite o CNPJ desejado: \n");
                scanf("%s", cnpj);
                while(!feof(indices[qual-1])) {
                    result = fgets(linha, 500, indices[qual-1]);
                    if(result) {
                        strtok(linha, "\t");
                        if(!strcmp(linha, cnpj)) {
                            campo = strtok(NULL, "#");	//reutilizando campo que está inútil
                            bytes = atoi(campo);
                        }
                    }
                }
                fseek(arquivos[qual-1], bytes, SEEK_SET);
                printf("HUE%ld\n", ftell(arquivos[qual-1]));
                fwrite("*", 1, 1, arquivos[qual-1]);
                break;
            case 2:
                inserir();
                break;
            case 3:

                break;
            case 4:

                break;
            case 0:
                return 0;
        }
    }
}
