#define	 _CRT_SECURE_NO_WARNINGS

#include	<memory.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include "kmer.h"


int main(int argc, char** argv)
{
	//// as cadeias do enunciado porque nao?
	//char* entrada1 = "ACAACTATGCATCACTATCGGGAACTATCCT";	// 5-mer "ACTAT"
	//char* entrada2 = "CGATATATCCATAG";					// 3-mer "ATA"
	//char* entrada3 = "ACGTTGCATGTCGCATGATGCATGAGAGCT";	// 4-mer = "GCAT" e "CATG"

	//testa_com_count(entrada1, 5);
	//testa_com_count(entrada2, 3);
	//testa_com_count(entrada3, 4);
	//testa_com_count("AAAAA", 3);
	//testa_com_count("ABCDABCDABCD", 3);

	testa_sem_count();
	return 0;
};	// end main()


pesquisa*	apaga_pesquisa(pesquisa* p)
{
	// apagar uma pesquisa significa seguir a estrutura  
	// na sequencia e ir apagando tudo
	if (p == NULL) return NULL;
	if (p->k_mer == NULL) return NULL;
	//printf("\nApagando pesquisa. Aguarde...\n");
	struct K_mer* pk = p->k_mer;
	// apaga a tabela
	while (pk != NULL)
	{
		struct K_mer* temp = pk->proximo;
		//printf("    apaga %d-mer '%s'\n", p->k, pk->k_mer);
		free(pk->k_mer);
		free(pk);
		pk = temp;
	}	// end while
	//printf("    apagando cadeia '%s'\n", p->cadeia);
	free(p->cadeia);
	//printf("    apagando registro da pesquisa\n");
	free(p);
	//printf("***** apagado *****\n");
	return NULL;
}	// end apaga_pesquisa()


int			compila_k_mer(pesquisa* p)
{
	printf("    compilando tabela\n");
	int n = p->tamanho_cadeia - p->k + 1;		// total de possiveis k-mer 1<=
	printf("    possiveis ate %d %d-mer diferentes em '%s'\n", n, p->k, p->cadeia);
	char* origem = p->cadeia;
	for (int i = 0; i < n; i+=1)
	{	// processa todos, duplicados ou nao

		// cria o atual k-mer em 'este_k_mer'
		char temp = origem[p->k];
		origem[p->k] = 0;
		int t = (1 + p->k) * sizeof(char);
		char* este_k_mer = (char*) malloc(t);
		strcpy(este_k_mer, origem);

		origem[p->k] = temp;	// restaura a string
		origem++;				// avanca o ponteiro

		insere_na_pesquisa(este_k_mer, p);
		free(este_k_mer);
	}	//	end for
	return 0;
};	// end compila_k_mer()


int			count(char* c_str, char* k_str)
{
	// count() dee devolver 'n' o numero de vezes em que 
	// k-mer aparece em cadeia

	int n = 0;

	char cadeia[1024];
	char k_mer[1024];
	strcpy(cadeia, c_str);
	strcpy(k_mer, k_str);
	char* inicio = cadeia;
	char* pos;
	int i = strlen(k_mer);

	//printf(
	//	"Buscando '%s(%d)' em '%s(%d)'\n",
	//	k_mer, strlen(k_mer),
	//	cadeia, strlen(cadeia)
	//);
	do
	{
		pos = strstr(inicio, k_mer);
		if (pos == NULL)
		{	// nao tem mais
			return n;
		}
		else
		{	// mais um
			n = n + 1;
			inicio = pos + i;
			//printf("cadeia de pesquisa agora [%s]\n", inicio);
		};	// end if
	} while (pos != NULL);	// end while(1)
	return n;
};	// end count()


pesquisa*	cria_lista_k_mer(char* cadeia, int k)
{
	short		jaExiste = 0;
	char*		origem;
	char		temp;
	printf("\n\nCriando a lista.....\n");

	pesquisa* p = (pesquisa*) malloc(sizeof(pesquisa));
	p->cadeia = (char*)malloc ( 1 + strlen(cadeia) );
	strcpy(p->cadeia, cadeia);
	p->tamanho_cadeia = (unsigned short)strlen(cadeia);
	p->k = k;
	p->total_k_mer = 0;
	p->k_mer = NULL;

	printf(
		"    Criando tabela para '%s(%d)' usando %d-mer\n",
		p->cadeia, strlen(p->cadeia), k
	);

	int n = p->tamanho_cadeia - p->k + 1;		// total de possiveis k-mer 1<=
	printf("    Possiveis ate %d %d-mer diferentes em '%s'\n", n, p->k, p->cadeia);
	origem = p->cadeia;
	for (int i = 0; i < n; i += 1)
	{	// processa todos os possiveis k-mer, duplicados ou nao
		jaExiste = 0;
		temp = origem[p->k];
		origem[p->k] = 0;
		char* este_k_mer = (char*)malloc((1 + p->k) * sizeof(char));
		strcpy(este_k_mer, origem);

		origem[p->k] = temp;	// restaura a string
		origem++;				// avanca o ponteiro

		struct K_mer* pk = p->k_mer;	// aponta para o primeiro k_mer
		while (pk != NULL)
		{	// seque a lista ate o fim ou ate encontrar este ja presente
			if (strcmp(este_k_mer, pk->k_mer) == 0)
			{	// ja tinha esse na lista
				jaExiste = 1;
				break;
			}
			pk = pk->proximo;
		}

		// esse k-mer e novo?
		if (jaExiste) continue;

		struct K_mer* novo = (struct K_mer*) malloc(sizeof(struct K_mer));
		novo->k_mer = este_k_mer;
		novo->proximo = p->k_mer;
		novo->total = 0;
		p->total_k_mer += 1;
		p->k_mer = novo;	// insere no inicio: mais facil
	}	//	end for
	return p;
};	// end cria_lista_k_mer()


void		insere_na_pesquisa(char* valor, pesquisa* p)
{
	struct K_mer* pk = p->k_mer;	// aponta para o primeiro k_mer
	while (pk != NULL)
	{	// seque a lista ate o fim ou ate encontrar este ja presente
		if (strcmp(valor, pk->k_mer) == 0)
		{	// ja tinha esse na lista
			pk->total += 1;
			return;
		}
		pk = pk->proximo;
	}
	// esse k-mer e novo
	struct K_mer* novo = (struct K_mer*) malloc(sizeof(struct K_mer));	// novo no
	novo->k_mer = (char*)malloc((1 + strlen(valor)) * sizeof(char));	// nova string
	strcpy(novo->k_mer, valor);
	novo->proximo = p->k_mer;
	novo->total = 1;
	p->total_k_mer += 1;
	p->k_mer = novo;	// insere no inicio: mais facil

	return;
};	// end insere_na_pesquisa()


void		mostra_pesquisa(pesquisa* p)
{
	if (p == NULL) return;
	if (p->k_mer == NULL) return;
	printf("\n\n***** pesquisa *****\n\n");
	printf("cadeia [%s](%d), %d-mer\n\n", p->cadeia, p->tamanho_cadeia, p->k);
	struct K_mer* pk = p->k_mer;
	int max_k_mer = 0;
	for (int i = 0; i < p->total_k_mer; i += 1)
	{
		if (pk->total > max_k_mer) max_k_mer = pk->total;
		printf("%4d........ %s .... %4d\n", i + 1, pk->k_mer, pk->total);
		pk = pk->proximo;
	}	//	end for
	pk = p->k_mer;
	printf("\n%d-mer mais frequentes do grupo de %d:\n\n", p->k, p->total_k_mer );
	for (int i = 0; i < p->total_k_mer; i += 1)
	{
		if( pk->total == max_k_mer)
		    printf("    %s .... %4d\n", pk->k_mer, pk->total);
		pk = pk->proximo;
	}	// end for
	printf("\n***** fim *****\n");

	return;
};	// end mostra_pesquisa()


pesquisa* testa_compila_k_mer(char* cadeia, int k)
{
	pesquisa* p = (pesquisa*)malloc(sizeof(pesquisa));
	if (p == NULL)
	{
		printf("malloc() erro ao alocar pesquisa\n");
		exit(1);
	};	// end if
	p->cadeia = (char*) malloc( 1 + strlen(cadeia) );
	strcpy(p->cadeia, cadeia);
	p->tamanho_cadeia = (unsigned short)strlen(cadeia);
	p->k = k;
	p->total_k_mer = 0;
	p->k_mer = NULL;

	printf(
		"\n\nCriando tabela para '%s(%d)' usando %d-mer\n",
		p->cadeia, strlen(p->cadeia), k
	);

	compila_k_mer(p);
	return p;
};	// end testa_compila_k_mer()


int			testa_count(char* cadeia, int n_kmer)
{
	char	parte[80];
	printf("\n\ntestando para '%s' e %d-mer\n", cadeia, n_kmer);
	int l = strlen(cadeia);
	if (l < n_kmer) return -1;		// nao pode ser menor
	if (l == n_kmer) return 1;		// sao do mesmo tamanho!
	// monta em parte o primeiro k-mer possivel
	memcpy(parte, cadeia, n_kmer);
	parte[n_kmer] = 0;
	// ok: em 'parte' esta o primeiro valor
	printf("Primeiro %d-mer em '%s' = '%s'\n", n_kmer, cadeia, parte);
	// testa count()
	int n = count(cadeia, parte);
	printf("count() retornou %d\n", n);
	return 0;
};	// end testa_count()


void		testa_com_count(char * cadeia, int k)
{
	pesquisa* p = NULL;
	char teste[1024];
	strcpy(teste, cadeia);
	p = cria_lista_k_mer(teste, k);
	//
	// foi criada a lista dos possiveis k-mer em p
	// mas desta vez nao esta preenchida com o numero de
	// ocorrencias. Agora entao chama count() para cada
	// k-mer para preencher a tabela dos k-mer com o total 
	// de ocorrencias de cada um na lista
	//
	// claro, ja podia ter contado durante a construcao da lista
	//
	struct K_mer* pk = p->k_mer;
	for (int i = 0; i < p->total_k_mer; i += 1)
	{
		int n = count(p->cadeia, pk->k_mer);
		pk->total = n;
		pk = pk->proximo;
	};	// end for
	mostra_pesquisa(p);
	p = apaga_pesquisa(p);
	return;
};	// end testa_com_count()


void		testa_sem_count()
{
	// as cadeias do enunciado porque nao?
	char* entrada1 = "ACAACTATGCATCACTATCGGGAACTATCCT";	// 5-mer "ACTAT"
	char* entrada2 = "CGATATATCCATAG";					// 3-mer "ATA"
	char* entrada3 = "ACGTTGCATGTCGCATGATGCATGAGAGCT";	// 4-mer = "GCAT" e "CATG"

	pesquisa* p = NULL;

	p = testa_compila_k_mer("AAAAA", 3);
	mostra_pesquisa(p);
	p = apaga_pesquisa(p);

	p = testa_compila_k_mer("ABCDABCDABCD", 3);
	mostra_pesquisa(p);
	p = apaga_pesquisa(p);

	p = testa_compila_k_mer(entrada1, 5);
	mostra_pesquisa(p);
	p = apaga_pesquisa(p);

	p = testa_compila_k_mer(entrada2, 3);
	mostra_pesquisa(p);
	p = apaga_pesquisa(p);

	p = testa_compila_k_mer(entrada3, 4);
	mostra_pesquisa(p);
	p = apaga_pesquisa(p);

	return;
};	// end testa_sem_count()

