#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* REGISTRO DE DADOS COM CAMPOS DE TAMANHO FIXO E CAMPOS DE TAMANHO VARIÁVEL */
struct registro{
	char cnpj[18], dataRegistro[10], dataCancelamento[10], cnpjAuditor[18];
	char *nomeSocial, *nomeFantasia, *motivoCancelamento, *nomeEmpresa;	
};

/* DADOS PARA LISTA (ESTÁTICA) DE REGISTROS REMOVIDOS */
struct dados{
	int posicao;
	int tamanho;
};

/* FUNÇÃO DE MENU */
void menu(){
	printf("(1) Remoção lógica dos registros\n");
	printf("(2) Inserir registros\n");
	printf("(3) Visualizar estatisticas sobre arquivo de índice\n");
	printf("(4) Visualizar estatisticas sobre registros removidos\n");
	printf("(0) Sair");
}

int main(int argc, char **argv){
	//Remover arquivos de execução passada
	if(remove("arquivo1.dat") == 0 && remove("indice1.dat") == 0 && remove("tmp1.dat") == 0
	&& remove("arquivo2.dat") == 0 && remove("indice2.dat") == 0 && remove("tmp2.dat") == 0
	&& remove("arquivo3.dat") == 0 && remove("indice3.dat") == 0 && remove("tmp3.dat") == 0){
		printf("Removido com sucesso!\n");
	}
	else{
		printf("Erro ao remover. Talvez os arquivos não existam. Ignorar...\n");
	}
	
	FILE *csv = fopen("../SCC0215012017projeto01turmaBdadosCompanhias.csv", "rt");
	FILE *arquivos[3] = {fopen("arquivo1.dat", "a+b"), fopen("arquivo2.dat", "a+b"), fopen("arquivo3.dat", "a+b")};
	FILE *indices[3] = {fopen("indice1.dat", "a+b"), fopen("indice2.dat", "a+b"), fopen("indice3.dat", "a+b")};
	FILE *tmp[3] = {fopen("tmp1.dat", "a+b"), fopen("tmp2.dat", "a+b"), fopen("tmp3.dat", "a+b")}; 	//arquivo temporário
	
	//armazena linha do arquivo, result valida leitura, itera entre campos do registro, cnpj passado pelo usuario
	char linha[500], *result, *campo, cnpj[18];
	struct registro Registro;
	
	/*iterador, tamanho do registro, opcao do menu, bytes contados no arquivo
	posicao recebe antigo tamanho do registro, quantidade de remoções, flag para decidir remoção do índice*/
	int i = 0, tamanho_registro = 0, opcao, bytes, posicao = 0, remocoes = 0, flag = 1;
	
	//Lista estática de registros removidos e inseridos
	struct dados Removidos[200], Inseridos[200];
	
	//populando Removidos com -1 como sendo vazio
	for(int i = 0; i < 200; i++){
		Removidos[i].posicao = -1;
		Removidos[i].tamanho = -1;
	}
	
	Registro.nomeSocial = (char *) malloc(150);
	Registro.nomeFantasia = (char *) malloc(150);
	Registro.motivoCancelamento = (char *) malloc(150);
	Registro.nomeEmpresa = (char *) malloc(150);
	
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
				//campos de tamanho fixo nao possuem delimitadores entre os outros
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
			
			//grava a quantidade de bytes até um registro no arquivo de índice
			tamanho_registro += sizeof(Registro.cnpj) + strlen(Registro.nomeSocial) +
			strlen(Registro.nomeFantasia) + sizeof(Registro.dataRegistro) +
			sizeof(Registro.dataCancelamento) + strlen(Registro.motivoCancelamento) +
			strlen(Registro.nomeEmpresa) + sizeof(Registro.cnpjAuditor) + 1;
			printf("%d\n", tamanho_registro);
			sprintf(result, "%d", posicao); //reusando result que está inútil
			
			posicao = tamanho_registro;
			for(int j = 0; j < 3; j++){
				fwrite(result, 1, 5, indices[j]);	//5 porque é quantos caracteres no maximo ele terá
				fwrite("#", 1, 1, indices[j]);
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
	/*******************************/
	/* FUNCIONALIDADES COMEÇAM AQUI*/
	/*******************************/
	
	arquivos[0] = fopen("arquivo1.dat", "r+b");
	arquivos[1] = fopen("arquivo2.dat", "r+b");
	arquivos[2] = fopen("arquivo3.dat", "r+b");
	indices[0] = fopen("indice1.dat", "r+b");
	indices[1] = fopen("indice2.dat", "r+b");
	indices[2] = fopen("indice3.dat", "r+b");
	
	while(1){
		printf("Digite a opcao desejada do menu\n");
		menu();
		scanf("%d", &opcao);
		switch(opcao){
			case 1:
				printf("Digite o CNPJ desejado: \n");
				scanf("%s", cnpj);
				
				for(int j = 0; j < 3; j++){
					while(!feof(indices[j])){
						result = fgets(linha, 100, indices[j]);
						if(result){
							strtok(linha, "\t");
							campo = strtok(NULL, "#");	//reutilizando campo que está inútil
							if(!strcmp(linha, cnpj)){
								flag = 0;
								bytes = atoi(campo);
							}
						}
						//faz a remoção física no arquivo de índice aqui para evitar código repetido
						if(flag){
							fwrite(linha, 18, 1, tmp[j]);
							fwrite("\t", 1, 1, tmp[j]);
							fwrite(campo, 5, 1, tmp[j]);
							fwrite("#\n", 2, 1, tmp[j]);
							flag = 1;
						}
					}
					
					//faz a remoção lógica no arquivo de dados
					fseek(arquivos[j], bytes, SEEK_SET);
					fseek(arquivos[j], -1, SEEK_CUR);	//sobrescreve caracter
					fwrite("*", 1, 1, arquivos[j]);
					remocoes++;		//incrementa remoções para estatisticas
					Removidos[(remocoes/3)-1].posicao = bytes;	//(remocoes/3)-1 representa a posição correta do vetor
					Removidos[(remocoes/3)-1].tamanho = tamanho_registro - bytes;
				}
				break;
			case 2:
				
				break;
			case 3:
				printf("-----------------------------------------------------------------------\n");
				printf("-Remoções\t| Posição da remoção\t| Tamanho do registro removido-\n");
				for(int k = 0; k < 200; k++)
					if(Removidos[k].posicao != -1)
						printf("%d\t\t| %d\t\t\t| %d\n", remocoes/3, Removidos[k].posicao, Removidos[k].tamanho);
				printf("-----------------------------------------------------------------------\n");
				break;
			case 4:
				
				break;
			case 0:
				fclose(tmp[0]);
				fclose(tmp[1]);
				fclose(tmp[2]);
				return 0;
		}
	}
}
