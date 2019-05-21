#ifndef ESPARSA_H
#define ESPARSA_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define STR_MAX 100

/*
 *
 *  Api de Matrizes Esparsas
 *  Trabalho De Estrutura de Dados 2018-1
 *	Autores Lucas Ferreira e Sabrina Nawcki 
 *  
*/

// Definicao do TAD: Esparsa
typedef struct Esparsa Esparsa;

Esparsa* criaNoh(int linha, int coluna, double valor);
Esparsa* cria(void);
void criaMatriz(Esparsa* A, int linha, int coluna);
void insereNoh (Esparsa* A, Esparsa* noh);
void print_t (int quant);
void imprimir (Esparsa* A);
void somaNoh (Esparsa* A, int linha, int coluna, double valor);
void somar (Esparsa* S, const Esparsa* A, const Esparsa* B);
void transpor (Esparsa* T, const Esparsa* A);
void multiplicar (Esparsa* M, const Esparsa* A, const Esparsa* B);
void Ler(char* filePath, Esparsa* A);

#endif
