#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct registro{
	char cnpj[18], dataRegistro[10], dataCancelamento[10], cnpjAuditor[18];
	char *nomeSocial, *nomeFantasia, *motivoCancelamento, *nomeEmpresa;	
};

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
	char linha[500], *result, *campo, *tamanho_registro;
	struct registro Registro;
	int i = 0;
	
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
			
			//conteúdo gravado ta todo bugado
			for(int j = 0; j < 3; j++){
				fwrite(Registro.cnpj, 1, 18, arquivos[j]);
				fwrite(Registro.nomeSocial, 1, sizeof(Registro.nomeSocial), arquivos[j]);
				fwrite("|", 1, 1, arquivos[j]);
				fwrite(Registro.nomeFantasia, 1, sizeof(Registro.nomeFantasia), arquivos[j]);
				fwrite("|", 1, 1, arquivos[j]);
				fwrite(Registro.dataRegistro, 1, 10, arquivos[j]);
				fwrite(Registro.dataCancelamento, 1, 10, arquivos[j]);
				fwrite(Registro.motivoCancelamento, 1, sizeof(Registro.motivoCancelamento), arquivos[j]);
				fwrite("|", 1, 1, arquivos[j]);
				fwrite(Registro.nomeEmpresa, 1, sizeof(Registro.nomeEmpresa), arquivos[j]);
				fwrite("|", 1, 1, arquivos[j]);
				fwrite(Registro.cnpjAuditor, 1, 18, arquivos[j]);
				fwrite("#", 1, 1, arquivos[j]);
				
				fwrite(Registro.cnpj, 1, 18, indices[j]);
				fwrite("\t", 1, 1, indices[j]);
				
				//reaproveita o i que nesse for é inútil
				i = sizeof(Registro); 
				sprintf(&tamanho_registro, "%d", i);
				
				//grava o tamanho do registro no arquivo de índice
				fwrite(&tamanho_registro, 1, sizeof(tamanho_registro), indices[j]);
				fwrite("\n", 1, 1, indices[j]);
			}
			i = 0;
		}
	}
	
	fclose(csv);
	for(int j = 0; j < 3; j++){
		fclose(arquivos[j]);
		fclose(indices[j]);
	}
    return 0;
}
