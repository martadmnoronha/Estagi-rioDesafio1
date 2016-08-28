/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: MARTA DIAS MOREIRA NORONHA
 *
 * Created on 26 de Agosto de 2016, 16:50
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#define Instancias 5000
#define Atributos 41 /*0-39 recebe os atributos e 40 recebe a classe*/
#define Clusters 3

/*******************************************************************************************************/
/* VARIÁVEIS GLOBAIS */
float MatDados[Instancias][Atributos]; // Matriz de Armazenamento da BD

/* OBS: Exclui-se a última coluna de atributos que contém a classe nos cálculos necessários */
float vetDist[Instancias][Atributos]; // Vetor que armazena a Distância de cada atributo de cada Instância
float vetPadrao[Instancias][Atributos-1]; // Vetor que armazena o Padrão de cada Atributo de cada Instância
float desvioMedio[Atributos-1]; // Vetor que armazena o Desvio Médio de cada coluna de Atributo 
float medidaPadrao[Instancias][Atributos]; // Armazena o valor após padronização do BD de cada Atributo de cada Instância
float valorMedio[Atributos-1]; // Armazena o valor médio de cada coluna de Atributos
float vetClass1[Instancias][Atributos];// Armazena Classes separadas
float vetClass2[Instancias][Atributos];// Armazena Classes separadas
float vetClass3[Instancias][Atributos];// Armazena Classes separadas
float ClassTeste[Instancias/5][Atributos];// Armazena amostras de Classes -> Decidi usar 20% da base como amostra 
float vetDistTeste[Instancias][Atributos]; // Vetor que armazena a Distância de cada atributo de cada Instância da ClassTeste
float DistManTeste[Instancias];


/*******************************************************************************************************/

void MatrizDados(){ /* Preenchimento da matriz original */
        FILE *arquivo = fopen("waveformnoise.data","r");
        
        if(!arquivo){
            printf("O arquivo não foi localizado.");
            exit(0);
        }
        
	char info[300]; //Vetor para armazenamento de cada linha da BD
	char cNum[6]; // String auxiliar que recebe cada atributo de cada Instância da BD par armazená-lo como float em sua devida posição da matriz
	int i, j , k, l;
        /* Efetuando a leitura dos dados contidos na waveform.data */       
	for(i = 0; i< Instancias; i++){
		if(fgets(info, sizeof(info), arquivo) != NULL){ //Adquirindo os dados a cada linha                   
			k = 0;	//Cada incremento de i, deve 'zerar' a variável para capturar nova Instância com seus atributos	
			for(j = 0; j < Atributos; j++){//Em cada linha, separar os valores dos atributos e armazená-los na matriz				
				l = 0; // Cada incremento de J deve 'zerar' a variável para capturar um novo atributo				
				while( info[k] == '.' || info[k] == '-' || isdigit(info[k]) ){ // Se info[k] for diferente de ',' e '\n', então o valor é um número ou '.'
					cNum[l] = info[k]; // Se for número ou '.', armazena em cNum
					l++;
					k++;
				}											
                                if(info[k] == ',') // Incrementa para pegar o início do próximo atributo
					k++;
				if(info[k] == '\0') // Deve capturar a próxima linha que representa a próxima linha da matriz
					j = Atributos;
                                
				cNum[l]='\0'; // Fecha a string antes de converter para que não haja lixo na conversão
								
				MatDados[i][j] = atof(cNum);	// cNum convertido será armazenado em sua devida posição na MatDados			
			}
		}else{ 
			printf("Erro ao adquirir os dados da waveform\n");
			exit(0);
		}	                
	}
	
	fclose(arquivo); /* Fechando o arquivo */
        /**/
        /* Conferindo os elementos da Matriz */
	/*for(i = 0; i < Instancias; i++){
		printf("--------------------\n\n");
		for(j = 0; j < Atributos; j++)
			printf("%1.2f , ", MatDados[i][j]);
         }*/
}

/*******************************************************************************************************/

void DistManhattan(){  /* Cálculo da Distância de Manhattan */ 
    int i, j, n, distR;
        for(i = 0; i < Instancias; i++){		
		distR = 0;
		for(j = 0; j < (Atributos-1); j++){/* Exclui-se a última coluna de atributos que contém a classe*/
			for(n = 0; n < (Atributos-1); n++) /* Exclui-se a última coluna de atributos que contém a classe*/
				distR = fabs(MatDados[i][n] - MatDados[j][n]) + distR;
				
			vetDist[i][j] = distR;		
		}
	}
    
        /* Imprimir as distâncias de cada Atributo */
	/*for(i = 0; i < Instancias; i++){
		printf("\n");
		for(j = 0; j < Atributos-1; j++)
		printf("%f, ", vetDist[i][j]);
	}*/
	
}

/*******************************************************************************************************/

void DesvioMedioAbs(){ /* Calcula o desvio médio absoluto de cada atributo (coluna) */
    int i, j, l;
        float Sf; // valor associado à cada coluna de atributos Af
	float Mf; // Usado para cálculo do valor médio de cada coluna Af
	
	for(i = 0; i< (Atributos-1); i++){ /* Exclui-se a última coluna de atributos que contém a classe*/
		Mf = 0;
		Sf = 0;
		for(j = 0; j < Instancias; j++){                    
			Mf = MatDados[j][i] + Mf;                        
		}
                
		valorMedio[i] = Mf/Instancias; // Cálculo do valor Médio
		
		for(l = 0; l < Instancias; l++){
			Sf = Sf + fabs( MatDados[l][i] - valorMedio[i]); 
                       
                }
			
		desvioMedio[i] = Sf/Instancias; // Desvio Médio Absoluto
	}	
        
        /* Imprimir o desvio médio de cada atributo */
        /*for(i = 0; i < (Atributos-1); i++ )
            printf("%f, ", desvioMedio[i]);*/
}

/*******************************************************************************************************/

void PadronizaMatrizOriginal(){/* Calcula a medida padrão do atributo f de cada objeto i */
    int i, j;
	//(xif-mf)/sf
	for(i = 0; i< Atributos-1; i++){            
		for(j = 0; j < Instancias; j++){
			medidaPadrao[j][i] = (MatDados[j][i] - valorMedio[i])/desvioMedio[i];
		} 
	}
	
      	for(j = 0; j < Instancias; j++){ // Preenchendo os valores das classes originais
		medidaPadrao[j][Atributos-1] = MatDados[j][Atributos-1];
        } 
	/* Imprimir as medidas padrao da matriz original */
	/*for(i = 0; i < Instancias; i++){
		printf("\n");
		for(j = 0; j < Atributos; j++)
		printf("%f, ", medidaPadrao[i][j]);
	}	*/
}

/*******************************************************************************************************/

void amostra(){ 
    int i, j, l=0, opc;
    float op;
    int c1 = 0 , c2 = 0, c3 = 0; // Guarda a quantidade de classes que há na matriz original
    int count1 = 0 , count2 = 0, count3 = 0; // Compara com a quantidade de amostras de cada classe
    int AmC1, AmC2, AmC3; // Amostra de cada classe
    int PC1 = 0, PC2 = 0, PC3 = 0; // Passos de escolha de dentro de cada classe
    for(i = 0; i < Instancias; i++ ){ // Separando as classes
        op = medidaPadrao[i][Atributos-1];
        opc = (int)op;
        switch(opc){
            case 0:                
                for(j = 0; j < Atributos; j++){
                    vetClass1[c1][j] = medidaPadrao[i][j];                    
                }
                c1++;
                break;
            case 1:               
                for(j = 0; j < Atributos; j++){
                    vetClass2[c2][j] = medidaPadrao[i][j];                    
                }
                c2++;
                break;
            case 2:              
                for(j = 0; j < Atributos; j++){
                    vetClass2[c3][j] = medidaPadrao[i][j];                    
                }
                c3++;
                break;
            default: printf("Falha na aquisição da classe\n"); 
                     exit(0);
                break;
        }
    }
    
   // printf("Elementos de c1: %d, c2: %d, c3 = %d\n", c1,c2,c3); // Quantidade de cada classe 
    AmC1 = (c1*1000)/Instancias; 
    AmC2 = (c2*1000)/Instancias;
    AmC3 = (c3*1000)/Instancias;
    
  //  printf("Elementos de c1: %d, c2: %d, c3 = %d", AmC1,AmC2,AmC3); // Quantidade a ser amostrada proporcionalmente
    
    PC1 = c1/AmC1;
    PC2 = c2/AmC2;
    PC3 = c3/AmC3;
    
    for(i = 0; i < Instancias; i++ ){ // Separando a base de Teste
        op = medidaPadrao[i][Atributos-1];
        opc = (int)op;
        switch(opc){
            case 0:
                if(count1%PC1 == 0  && count1 < c1 && l < 0.2*Instancias){ //O resto da divisão deve ser zero e a c1 deve ser menor do que a qtd de amostras para a classe
                    for(j = 0; j < Atributos; j++){
                       ClassTeste[l][j] = medidaPadrao[i][j];                    
                    }
                    l++;
                }
                count1++;
                break;
            case 1: 
                if(count2%PC2 == 0 && count2<c2 && l < 0.2*Instancias){//O resto da divisão deve ser zero e a c1 deve ser menor do que a qtd de amostras para a classe
                    for(j = 0; j < Atributos; j++){
                       ClassTeste[l][j] = medidaPadrao[i][j];                    
                    }
                    l++;
                }
                count2++;
                break;
            case 2:   
                if(count3%PC3 == 0  && count3<c3 && l < 0.2*Instancias){//O resto da divisão deve ser zero e a c1 deve ser menor do que a qtd de amostras para a classe
                    for(j = 0; j < Atributos; j++){
                        ClassTeste[l][j] = medidaPadrao[i][j];                    
                    }
                    l++;
                }
                count3++;
                break;
            default: printf("Falha na aquisição da classe\n"); 
                     exit(0);
                break;
        }
    }
    
    /*for(i = 0; i < (Instancias*0.2); i++){
		printf("\n");
		for(j = 0; j < Atributos; j++)
		printf("%f, ", ClassTeste[i][j]);
	}*/
}

/*******************************************************************************************************/

void Normaliza(){
        int i, j;
        float vMin[Atributos-1], vMax[Atributos-1];
        for(i = 0; i < (Atributos-1); i++){/* Exclui-se a última coluna de atributos que contém a classe*/
            vMin[i] = MatDados[0][i];
            vMax[i] = MatDados[0][i];
		for(j = 0; j < Instancias; j++){
                    if(MatDados[j][i] < vMin[i])
                        vMin[i] = MatDados[j][i];
                    else if(MatDados[j][i] > vMax[i])
                        vMax[i] = MatDados[j][i];                    
		}
	}

       /* NORMALIZANDO */
        for(i = 0; i < (Atributos-1); i++){/* Exclui-se a última coluna de atributos que contém a classe*/	          
		for(j = 0; j < Instancias; j++){
                   MatDados[j][i] = (MatDados[j][i] - vMin[i])/(vMax[i]-vMin[i]);                   
		}
	}
	
        /*for(i = 0; i < (Instancias); i++){
		printf("\n\n");
		for(j = 0; j < Atributos; j++)
		printf("%f, ", MatDados[i][j]);
	}*/
}
/*******************************************************************************************************/

void DistManhattanT(){  /* Cálculo da Distância de Manhattan */ 
    int i, j, n, distR;
        for(i = 0; i < (Instancias*0.2); i++){		
		distR = 0;
		for(j = 0; j < (Atributos-1); j++){/* Exclui-se a última coluna de atributos que contém a classe*/
			for(n = 0; n < (Atributos-1); n++) /* Exclui-se a última coluna de atributos que contém a classe*/
				distR = fabs(ClassTeste[i][n] - ClassTeste[j][n]) + distR;
				
			vetDistTeste[i][j] = distR;		
		}
	}
    for(i = 0; i < Instancias; i++) // Colocando a classe no vetor
        vetDistTeste[i][Atributos-1] = ClassTeste[i][Atributos-1]; 
    
        /* Imprimir as distâncias de cada Atributo */
	/*for(i = 0; i < (Instancias*0.2); i++){
		printf("\n");
		for(j = 0; j < Atributos; j++)
		printf("%f, ", vetDistTeste[i][j]);
	}*/
}

/*******************************************************************************************************/

void KNNTeste(){
    int i, j;
    DistManhattanT();
    /* Escolhi K aleatoriamente como 43*/
    for(i = 0; i < (Instancias*0.2); i++){
        DistManTeste[i] = 0;
        for(j = 0; j < (Atributos-1); j++){
            DistManTeste[i] = DistManTeste[i] + vetDistTeste[i][j];
        }
        DistManTeste[i] = DistManTeste[i]/(Atributos-1);
    }
    
    /* não concluído: SELECIONA-SE OS K MAIS PRÓXIMOS.*/
}

/*******************************************************************************************************/

void main(){
	int i, j, k, l, distR, n, q;     
        int *p;
        /* TRANSFIRO OS DADOS DA MATRIZ COM RUÍDOS PARA UMA MATRIZ NA MEMÓRIA DO COMPUTADOR */
        MatrizDados(); /* Armazena os dados do arquivo na matriz de Instâncias x Atributos*/  
        /* APLICO O PROCESSO DE NORMALIZAÇÃO DOS DADOS COM VALORES ENTRE 0 E 1 PARA UTILIZAR O K-NN */
        Normaliza();
        /* O DESVIO MEDIO ABSOLUTO DE CADA ATRIBUTO (COLUNAS) É CALCULADO PARA PADRONIZAR A BASE */
        DesvioMedioAbs(); /* Calcula o desvio médio absoluto de cada atributo (coluna) */
        /* O RESULTADO DO DESVIO MÉDIO É UTILIZADO PARA PADRONIZAR A BASE */
        /* NÃO HÁ COMO REMOVER 100% DOS RUÍDOS SEM UTILIZAR DE MÉTODOS MAIS COMPLEXOS E COM MUITO ESTUDO, 
         * LOGO, NÃO FOI POSSÍVEL QUE EU OS IMPLEMENTASSE */
        PadronizaMatrizOriginal(); /* Calcula a medida padrão do atributo f de cada objeto i */
        /* ESCOLHE-SE UMA AMOSTRA DE TESTE PARA TREINAR O K-NN, UTILIZANDO-SE DE UMA PARTE DA BASE ORIGINAL. DEFINI 20% PARA AMOSTRAGEM */
        amostra();
        /* PARA SE UTILIZAR O K-NN DEVE-SE USAR UMA MEDIDA DE DISTÂNCIA ENTRE O K VIZINHOS MAIS PRÓXIMOS, IDEALMENTE, ESSE K DEVE SER ÍMPAR 
         E COM O RESULTADO, A CLASSE MAJORITÁRA DAS INSTÂNCIAS MAIS PRÓXIMAS IRÁ ROTULAR A INSTÂNCIA ATUAL */
        KNNTeste();
        /* não concluído: PROCEDE-SE COM O K-NN COM A BASE COMPLETA E VERIFICA-SE A TAXA DE ACERTO */
        /* não concluído:UTILIZA-SE UMA MATRIZ DE CONFUSÃO COM O RESULTADO GERADO PELO KNN PARA SE OBTER UM PERCENTUAL DE ACERTOS DO ALGORITMO. */
        
        
        /* OBS: NÃO FOI POSSÍVEL CONCLUIR O TESTE POIS NÃO TIVE A MATÉRIA DE MINERAÇÃO DE DADOS. IREI TER NA METADE DESTE SEMESTRE
         * LETIVO. NA INICIAÇÃO CIENTÍFICA, TRABALHEI COM PREDIÇÃO DE PROTEÍNAS UTILIZANDO O SOFTWARE WEKA. NÃO NECESSITEI FAZER
         * QUAISQUER TRATAMENTOS NA BASE UTILIZADA DURANTE A INICIAÇÃO. FOI DESAFIADOR FAZER ESSE TESTE E AGRADEÇO PELA OPORTUNIDADE
         * DE PARTICIPAR NO PROCESSO SELETIVO. */
}

