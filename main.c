/*
 * main.c
 *
 *  Created on: Sep 12, 2009
 *      Author: mits
 */

#include "huffman.h"
#include "math.h"
#include "stdlib.h"

int main(int argc, char *argv[])
{
	int i,m48,blocksize,num2encode;
	char **encodedBlocks48;
	struct listEntry *head,*tail;
	char **codes48;
	blocksize = 4;
	num2encode = 8;
	FILE *fin,*fout;
	fin = fopen("datasmall.vec","r");
	if (fin==NULL) return -1;
	makeBlocksList(fin, blocksize, &head, &tail);
	fclose(fin);
	m48 = doHuffman(&head, &tail, num2encode, &encodedBlocks48, &codes48);
	printf("done? m48=%d\n",m48);
	for (i=0;i<m48;i++)
	{
	  printf("%d\n",i);
		printf("block: %s, code %s\n",encodedBlocks48[i],codes48[i]);
	}
	fin = fopen("datasmall.vec","r");
	fout = fopen("output.txt","w");
	makeOutput(fin, fout, encodedBlocks48, codes48, blocksize,m48);
	fclose(fin);
	fclose(fout);
	return 0;
}
