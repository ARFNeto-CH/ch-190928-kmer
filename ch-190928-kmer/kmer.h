#pragma once
#define	 _CRT_SECURE_NO_WARNINGS

#include	<memory.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

struct K_mer
{
	char*			k_mer;
	struct  K_mer*	proximo;
	int				total;
};

struct Pesquisa
{
	char*			cadeia;
	unsigned short	k;			// 1024 max
	unsigned short	tamanho_cadeia;
	unsigned short	total_k_mer;
	struct K_mer*	k_mer;
};
typedef struct Pesquisa pesquisa;


// prototipos para referencia


pesquisa*	apaga_pesquisa(pesquisa*);
int			compila_k_mer(pesquisa*);
int			count(char*, char*);
pesquisa*	cria_lista_k_mer(char*, int);
void		insere_na_pesquisa(char*, pesquisa*);
void		mostra_pesquisa(pesquisa*);
pesquisa*	testa_compila_k_mer(char*, int);
int			testa_count(char*, int);
void		testa_com_count();
void		testa_sem_count();

// fim de kmer.h