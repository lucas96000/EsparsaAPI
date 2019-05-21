#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "Esparsa.h"

struct Esparsa {
	int    linha, coluna;
	double valor;
	struct Esparsa* direita, * abaixo;
};

Esparsa* criaNoh(int linha, int coluna, double valor) {
	Esparsa* A = malloc(sizeof(Esparsa));

	A->linha   = linha;
	A->coluna  = coluna;
	A->valor   = valor;
	A->direita = NULL;
	A->abaixo  = NULL;

	return A;
}

Esparsa* cria(void) {
	Esparsa* A;
	A = criaNoh(-1, -1, -1); //inicializa i-1j-1
	return A;	
}

void criaMatriz(Esparsa* A, int linha, int coluna) {
	Esparsa* cabecalho_i, * cabecalho_j, * noh;

	cabecalho_i = A;
	cabecalho_j = A;

	int cont;

	//cria lista de LINHAS
	for (cont = 0; cont < linha; cont++) {
		noh = criaNoh(cont, -1, -1); //inicializa i0(++)j-1
		
		cabecalho_i->abaixo = noh;
		cabecalho_i         = noh;
	}


	//cria lista de COLUNAS
	for (cont = 0; cont < coluna; cont++) {
		noh = criaNoh(-1, cont, -1); //inicializa i-1j0(++)
		
		cabecalho_j->direita = noh;
		cabecalho_j          = noh;
	}
}

void insereNoh (Esparsa* A, Esparsa* noh) {
	Esparsa* i, * j;

	for (i = A; i->linha  != noh->linha;  i = i->abaixo);   //i->linha = noh->linha e i->coluna = -1
	//printf("i1 (%d,%d) a1 (%d,%d)\n", noh->linha, noh->coluna, i->linha, i->coluna);
	for (j = A; j->coluna != noh->coluna; j = j->direita); //j->linha = -1 e j->coluna = noh->coluna
	//printf("j1 (%d,%d) a1 (%d,%d)\n", noh->linha, noh->coluna, j->linha, j->coluna);

	for (; (i->direita != NULL) && (i->direita->coluna < noh->coluna); i = i->direita); //recebe o noh que vai apontar para direita desse novo noh
	//printf("i2 (%d,%d) a2 (%d,%d)\n", noh->linha, noh->coluna, i->linha, i->coluna);
	for (; (j->abaixo != NULL)  && (j->abaixo->linha   < noh->linha);  j = j->abaixo); //recebe o noh que vai apontar pra baixo desse novo noh
	//printf("j2 (%d,%d) a2 (%d,%d)\n", noh->linha, noh->coluna, j->linha, j->coluna);
	
	//Insere noh da linha, fazendo o apontamento pra direita
	if ((i->direita == NULL) && (i->coluna < noh->coluna)) { //se o proximo da direita for nulo e o i->coluna for anterior ao noh->coluna, insere-se no 'final'
		noh->direita = NULL;
		i->direita   = noh;
		//printf("f1 %.2lf \n", noh->valor);
	}else{ //insere no meio, apontando para o noh existente a direita
		noh->direita = i->direita;
		i->direita   = noh;
		//printf("m1 %.2lf \n", noh->valor);
	}
	//Insere noh da coluna, fazendo o apontamento pra baixo
	if ((j->abaixo == NULL) && (j->linha < noh->linha)) { //se o proximo de baixo for nulo e o j->linha for anterior ao noh->linha, insere-se no 'final'
		noh->abaixo = NULL;
		j->abaixo   = noh;
		//printf("f2 %.2lf \n", noh->valor);
	}else{ //insere no meio, apontando para o noh existente abaixo
		noh->abaixo = j->abaixo;
		j->abaixo   = noh;
		//printf("m2 %.2lf \n", noh->valor);
	}

}

void print_t (int quant) {
	int cont;
	//printf("quant %d ", quant);
	for (cont = 0; cont < quant; cont++) {
		if (cont == 0) {  //se cont == 0, eh soh o espaco entre os numeros
			printf("\t");
		}else{            //se não, existe um zero nesse lugar da matriz
			printf("0.00 \t");
		}
	}
}

void imprimir (Esparsa* A) {
	Esparsa* i, *j;

	int linhasQtdA  = MatrizLinhasQtd(A);
	int colunasQtdA = MatrizColunasQtd(A);

	int j_antes;

	for (i = A->abaixo; i != NULL; i = i->abaixo) { //anda pelo cabecalho de linha (0,-1 -> 1,-1 -> ... -> n, -1)
		j_antes = -1; //o anterior de j0 eh j-1
		for (j = i->direita; j != NULL; j = j->direita) { //anda pelos valores da linha (ex.: 0,0 -> 0,1 -> ... -> 0,n)
			print_t(j->coluna - j_antes);
			j_antes = j->coluna; //recebe o j q ja foi usado
			printf("%.2lf", j->valor);
			if (j->direita == NULL) {
				print_t(colunasQtdA - j->coluna); //se o prox eh nulo, imprime os zeros até completar a coluna
			}
		}
		if (j_antes != -1){ //se o j_antes mudou, então foi escrito na linha e só pula uma linha para a prox
			printf("\n"); 
		}else{
			print_t(colunasQtdA + 1); //se j_antes == 1, qr dizer q o for n foi efetuado, logo ha uma linha vazia, que eh preenchida com zeros
	    	printf("\n");
		}
	}
}

void somaNoh (Esparsa* A, int linha, int coluna, double valor) {
	Esparsa* i, * j;
	
	int aux = 0;

	for (i = A->abaixo; i != NULL; i = i->abaixo) { //cabecalho da linha (0,-1 -> 1,-1 -> ... -> n, -1)
		for (j = i->direita; j != NULL; j = j->direita) { //valores da linha (ex.: 0,0 -> 0,1 -> ... -> 0,n)
			if (j->linha == linha && j->coluna == coluna) { //Anda na matriz até achar a posicao ij p/ somar o valor
				j->valor += valor;
				aux++; //quer dizer que ele achou o Noh e somou o valor
			}
		}
	}
	
	if (aux == 0) {
		insereNoh(A, criaNoh(linha, coluna, valor)); //aux == 0, quer dizer q o Noh não foi encontrado, logo cria-se um novo	
	}
}

void somar (Esparsa* S, const Esparsa* A, const Esparsa* B) { 
	Esparsa* ia, *ib, *ja, *jb;
	
	int linhasQtdA, colunasQtdA, linhasQtdB, colunasQtdB;
	
	linhasQtdA  = MatrizLinhasQtd(A);
	colunasQtdA = MatrizColunasQtd(A);
	
	linhasQtdB  = MatrizLinhasQtd(B);
	colunasQtdB = MatrizColunasQtd(B);
	
	if (linhasQtdA != linhasQtdB || colunasQtdA != colunasQtdB){
		printf("Nao eh possivel calcular a soma de matrizes de tamanhos i, j diferentes!");
		return;
	}else{
		criaMatriz(S, linhasQtdA, colunasQtdA);
		//primeiro copia a matriz A na matriz S
		for (ia = A->abaixo; ia != NULL; ia = ia->abaixo) { //cabecalho da linha (0,-1 -> 1,-1 -> ... -> n, -1)
			for (ja = ia->direita; ja != NULL; ja = ja->direita) { //valores da linha (ex.: 0,0 -> 0,1 -> ... -> 0,n)
				insereNoh(S, criaNoh(ja->linha, ja->coluna, ja->valor)); //i0++, j0++, valor A 
			}
		}
		//depois usa a funcao somaNoh que soma nas posicoes existentes de S os valores de B ou cria um noh novo em S 
		for (ib = B->abaixo; ib != NULL; ib = ib->abaixo) { //cabecalho da linha (0,-1 -> 1,-1 -> ... -> n, -1)
			for (jb = ib->direita; jb != NULL; jb = jb->direita) { //valores da linha (ex.: 0,0 -> 0,1 -> ... -> 0,n)
				somaNoh(S, jb->linha, jb->coluna, jb->valor); //i0++, j0++, valor B
			}
		}
	}		
}

void transpor (Esparsa* T, const Esparsa* A) {
	Esparsa* i, *j;

	int colunasQtd, linhasQtd;
	
	colunasQtd = MatrizColunasQtd(A);
	
	linhasQtd  = MatrizLinhasQtd(A);

	criaMatriz(T, colunasQtd, linhasQtd);

	int i_antes, j_antes;

	i_antes = -1; //o anterior de i0 eh i-1
	for (i = A->abaixo; i != NULL; i = i->abaixo) { //cabecalho da linha (0,-1 -> 1,-1 -> ... -> n, -1)
		j_antes = -1; //o anterior de j0 eh j-1
		for (j = i->direita; j != NULL; j = j->direita) { //valores da linha (ex.: 0,0 -> 0,1 -> ... -> 0,n)
			j_antes = j->coluna; //recebe o j q ja foi usado
			insereNoh(T, criaNoh(j_antes, i_antes+1, j->valor)); //i e j invertidos, valor
		}
		i_antes = i->linha; //recebe o i q ja foi usado	
	}
}

void multiplicar (Esparsa* M, const Esparsa* A, const Esparsa* B) {
	Esparsa* ia, *ib, *ja, *jb;	
	
	int cont = 0, j_antes = -1, aux = 0;
	
	int colunasQtd, linhasQtd;
	
	colunasQtd = MatrizColunasQtd(B);
	
	linhasQtd  = MatrizLinhasQtd(A);
	
	if (colunasQtd != linhasQtd){
		printf("O numero de colunas da 2a matriz deve ser igual ao numero de linhas da 1a matriz");
		return;
	}else{
		criaMatriz(M, linhasQtd, colunasQtd);
		for (ia = A->abaixo; ia != NULL; ia = ia->abaixo) { //anda pelo cabecalho linha de A (0,-1 -> 1,-1 ...)
			j_antes = -1; //o anterior de j0 eh j-1
			cont = 0;
			for (ja = ia->direita, ib = B->abaixo; ib != NULL, ja != NULL; ja = ja->direita, ib = ib->abaixo) { //anda pelos valores da linha de A (ex.: 0,0 -> 0,1 -> ... -> 0,n) e anda pelo cabecalho linha de B (0,-1 -> 1,-1 ...)
				aux = ja->coluna - j_antes; //recebe a diferenca de antes e agora
				j_antes = ja->coluna; //atualiza o anterior
				while (aux > 1) { //se eh maior que 1, anda os valores de jb
					ib = ib->abaixo;
					jb = ib->direita;
					aux--;
				}
				//printf("aux %d\n", aux);
				for (jb = ib->direita; jb != NULL; jb = jb->direita) { //anda pelos valores da linha de B (ex.: 0,0 -> 0,1 -> ... -> 0,n)
					//printf("ja %.2lf, jb %.2lf\n", ja->valor, jb->valor);
					if (cont <= jb->coluna) { //se o contador eh <= a coluna de B, quer dizer q ainda nao foi inserido noh
						insereNoh(M, criaNoh(ja->linha, jb->coluna, ja->valor*jb->valor));
						//printf("valor %2.lf, cont %d, jb %d\n", ja->valor*jb->valor, cont, jb->coluna);
					}else{
						somaNoh(M, ja->linha, jb->coluna, ja->valor*jb->valor);	
					}
					cont++;
				}
			}
		}
	}
}

// Retorna a quantidade de Linhas na Matriz Esparsa
int MatrizLinhasQtd(const Esparsa *A){
	Esparsa* i;

	int linhas = 0;

	for (i = A->abaixo; i != NULL; i = i->abaixo) { //(i0; i < linha; i++;)
		linhas++;		
	}
	
	return linhas;
}

// Retorna a quantidade de colunas na Matriz Esparsa
int MatrizColunasQtd(const Esparsa *A){
	Esparsa* j;

	int colunas = 0;

	for (j = A->direita; j != NULL; j = j->direita) { //(j0; j < coluna; j++;)
		colunas++;		
	}
	return colunas;
}

// Retorna a quantidade de linhas no arquivo csv
int LinhasQtdArq(char* filePath){
	int linhas  = 0;
	
	FILE * file = fopen(filePath,"r");

	if(file == NULL){ //Se nao encontrar o arquivo
		printf("Arquivo nao encontrado.\n");
		exit(EXIT_FAILURE);
	}
	else{
		while(!feof(file)){ //enquanto nao terminar o arquivo
			char c = fgetc(file);

			if(c == EOF){
				if(ferror(file)){
				  perror("Nao foi possivel ler o arquivo");
				  fclose(file);
				}

				continue;
			}
			if(c == '\n'){
				++linhas;
			}
		}
	}
	
	fclose(file);

	return linhas;
}

// Separa as linhas do arquivo csv em um array de strings
// Parametros 
//[filePath]  -> caminho do arquivo 
//[Linhas]    -> Vetor de strings para aramazenar cada linha inteira
//[QtdLinhas] -> Quantidade de linhas no arquivo
void CsvSeparaLinhas(char* filePath, char Linhas[][STR_MAX], int LinhasQtd){
	// Lê o arquivo
	FILE * file = fopen(filePath,"r");
	
	if(file == NULL){ //Se nao encontrar o arquivo
		printf("Arquivo nao encontrado.\n");
		exit(EXIT_FAILURE);
	}
	else{
		int i = 0, j = 0;

		// Enquanto nao terminar o arquivo
		while(!feof(file)){ 
			// pega o char
			char c = fgetc(file);

			// Verifica se não é o final do arquivo
			if(c == EOF){
				if(ferror(file)){
				  perror("Nao foi possivel ler o arquivo");
				  fclose(file);
				}

				continue;
			}
			
			// Verifica se não é um '\n', se for vai para a próxima linha
			if(c == '\n'){
				// Inseri o caractere nulo para finalizar a string
				Linhas[i][j] = '\0';
				i++;				
				j = 0;
			}
			else{
				// Insere o char
				Linhas[i][j] = c;				
				j++;
			}
			
			// Insere o caractere nulo na ultima linha, pois não um '\n' na ultima linha
			Linhas[i][j+1] = '\0';
		}

		fclose(file);
	}
}

//Pega o array de strings e transforma e uma Mtriz esparsa
//Parametros
//[Linhas] 		-> Vetor contendo cada linha do arquivo
//[A]      		-> Matriz Esparsa no qual será inseridos os elementos
//[LinhasQtd]   -> Quantidade de linhas no arquivo
void LinhasParaEsparsa(char Linhas[][STR_MAX], Esparsa* A, int LinhasQtd){
	int i, j, elementos, n_elementos;
	
	// Verifica se cada linha tem o mesmo numero de elemento
	for(i = 0; i < LinhasQtd; ++i){
		elementos = 0;
		for(j = 0; j < strlen(Linhas[i]); ++j){	
			// É utilizado o ponto e virgula pra saber quantos elementos contem na linha		
			if(Linhas[i][j] == ';'){ 
				elementos++;
			}
		}
		
		if(i == 0){
			n_elementos = elementos;
		} else{
			if(elementos != n_elementos){
				printf("%d, %d\n", elementos, n_elementos);				
				printf("Erro ao ler arquivo, o numero de elementos de ser o mesmo em todas as linhas.\n");
    			exit(EXIT_FAILURE);
			}
		}		
	}
	
	n_elementos++; // Como não tem ponto e virgula no ultimo elemento
	
	criaMatriz(A, LinhasQtd, n_elementos); //tendo a quantidades de elementos por linhas já é possivel criar a matriz
	
	char *elementos_array[n_elementos*LinhasQtd]; //array de string com todos os elementos
	double elementos_float[n_elementos*LinhasQtd]; //array para guardar todos os elementos em float
	
	// Pega a linha do arquivo e faz um explode separando os elementos pelo delimitador ';'
	for(i = 0, j = 0; i < LinhasQtd; i++){
		char *tok = strtok(Linhas[i], ";");		
		
		while (tok != NULL) {
		    elementos_array[j] = tok;
		    tok = strtok(NULL, ";");
		    j++;
		}
	}
	
	// Valida o array passando por cada elemento e verificando se não caracteres invalidos
	for(i = 0; i < n_elementos * LinhasQtd; ++i){		
		for(j = 0; j < strlen(elementos_array[i]); j++){					
			if((int) elementos_array[i][j] < 48 || (int) elementos_array[i][j] > 57){ // se for diferente de numero
				if(elementos_array[i][j] != '.' && elementos_array[i][j] != '-'){ //se for diferente de ponto -> spota o erro
					printf("Erro ao ler arquivo, contem elemento(s) invalidos no arquivo.\n");
    				exit(EXIT_FAILURE);
				}
			}
		}				
	}
	
	// Passa para double quaso os elementos estejam validos e insere no array
	for(i = 0; i < n_elementos * LinhasQtd; ++i){
		elementos_float[i] = atof(elementos_array[i]);
	}
	
	int k;
	
	//Insere na Matriz
	for(i = 0; i < LinhasQtd; ++i){
		for(j = 0; j < n_elementos; ++j){
			if(elementos_float[k] != 0){
				insereNoh(A, criaNoh(i, j, elementos_float[k]));
			}							
			
			k++;
		}
	}
}

void Ler(char* filePath, Esparsa* A){
	// Quantidade de linhas no arquivo
	int linhasQtd = LinhasQtdArq(filePath);
	
	// Vetor de strings para as linhas separadas
	char Linhas[linhasQtd][STR_MAX];
	
	// Separa as linhas no vetor
	CsvSeparaLinhas(filePath, Linhas, linhasQtd);
	
	// Realiza a separacao dos elementos e cria/insere na matriz
	LinhasParaEsparsa(Linhas, A, linhasQtd);
}
