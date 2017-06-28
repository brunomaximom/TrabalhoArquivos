#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct registro{
	char cnpj[18], dataRegistro[8], dataCancelamento[8], cnpjAuditor[18];
	char *nomeSocial, *nomeFantasia, *motivoCancelamento, *nomeEmpresa;	
};

void help() {
    fprintf(stderr, "\
            [uso]  <opcoes> <argumentos>\n\
            -h	mostra essa tela e sai.\n\
            -r	remove registro.\n\
            -i	insere registro.\n");
    exit(-1) ;
}


int main(int argc, char **argv){
	//Remover arquivos de execução passada
	if(remove("arquivo1.dat") == 0 && remove("indice1.dat") == 0
	&& remove("arquivo2.dat") == 0 && remove("indice2.dat") == 0
	&& remove("arquivo3.dat") == 0 && remove("indice3.dat") == 0){
		printf("Removido com sucesso!\n");
	}
	else{
		printf("Erro ao remover. Talvez os arquivos não existam. Ignorar...\n");
	}
	
	FILE *csv = fopen("../SCC0215012017projeto01turmaBdadosCompanhias.csv", "rt");
	FILE *arquivos[3] = {fopen("arquivo1.dat", "a+b"), fopen("arquivo2.dat", "a+b"), fopen("arquivo3.dat", "a+b")};
	FILE *indices[3] = {fopen("indice1.dat", "a+b"), fopen("indice2.dat", "a+b"), fopen("indice3.dat", "a+b")};
	char linha[500], *result, *campo;
	struct registro Registro;
	int i = 0, opcao, tamanho_registro = 0;
	
	Registro.nomeSocial = (char *) malloc(200);
	Registro.nomeFantasia = (char *) malloc(200);
	Registro.motivoCancelamento = (char *) malloc(200);
	Registro.nomeEmpresa = (char *) malloc(200);
	
	if (csv == NULL)  // Se houve erro na abertura
	{
		printf("Problemas na abertura do arquivo\n");
		return 0;
	}
	
	while(!feof(csv)){
		result = fgets(linha, 500, csv);
		if(result){
			campo = strtok(linha, ";");
			strcpy(Registro.cnpj, campo);
			puts(Registro.cnpj);
			i++;
			while(campo != NULL){
				campo = strtok(NULL, ";");
				if(i == 0){
					strcpy(Registro.cnpj, campo);
					puts(Registro.cnpj);
				}
				if(i == 1){
					strcpy(Registro.nomeSocial, campo);
					puts(Registro.nomeSocial);
				}
				if(i == 2){ 
					strcpy(Registro.nomeFantasia, campo); 
					puts(Registro.nomeFantasia); 
				}
				if(i == 3){ 
					strcpy(Registro.dataRegistro, campo); 
					puts(Registro.dataRegistro); 
					}
				if(i == 4){ 
					strcpy(Registro.dataCancelamento, campo); 
					puts(Registro.dataCancelamento); 
					}
				if(i == 5){ 
					strcpy(Registro.motivoCancelamento, campo);
					puts(Registro.motivoCancelamento);
				}
				if(i == 6){ 
					strcpy(Registro.nomeEmpresa, campo);
					puts(Registro.nomeEmpresa);
				}
				if(i == 7){
					campo = strtok(campo, "\r");
					//nomeSocial fica com pau aqui
					strcpy(Registro.cnpjAuditor, campo);
					puts(Registro.cnpjAuditor);
					printf("\n");
				}
				i++;
			}
			
			for(int j = 0; j < 3; j++){
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
			tamanho_registro += strlen(Registro.cnpj) + strlen(Registro.nomeSocial) +
			strlen(Registro.nomeFantasia) + strlen(Registro.dataRegistro) +
			strlen(Registro.dataCancelamento) + strlen(Registro.motivoCancelamento) +
			strlen(Registro.nomeEmpresa) + strlen(Registro.cnpjAuditor);
			printf("%d\n", tamanho_registro);
			sprintf(result, "%d", tamanho_registro);
			for(int j = 0; j < 3; j++){
				fwrite(result, 1, 5, indices[j]);	//5 porque é quantos caracteres no maximo ele terá
				fwrite("\n", 1, 1, indices[j]);
			}
			
			i = 0;
		}
	}
	
	/*******************************/
	/* FUNCIONALIDADES COMEÇAM AQUI*/
	/*******************************/
	
	//se nenhum parametro for passado printa a função de help
	if(argc < 2) help();
	
	while((opcao = getopt(argc, argv, "hr:i:")) > 0){
		switch(opcao){
			case 'h':
				help();
				break;
			case 'r':
				
				break;
			case 'i':
				
				break;
			default:
				printf("Opção inválida. Tente novamente\n");
		}
	}
	
	fclose(csv);
	for(int j = 0; j < 3; j++){
		fclose(arquivos[j]);
		fclose(indices[j]);
	}
    return 0;
}
