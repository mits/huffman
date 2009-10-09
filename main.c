/*
 * main.c
 *
 *  Created on: Sep 12, 2009
 *      Author: mits
 */

#include "huffman.h"
#include "math.h"
#include "stdlib.h"

extern int totalBlocks;

int main(int argc, char *argv[])
{
	int i,m48,blocksize,num2encode;
	uint32_t *encodedBlocks48;
	struct listEntry *head,*tail;
	char **codes48;
	blocksize = 4;
	num2encode = 10;
	FILE *fin,*fout;
	fin = fopen("datasmall.vec","r");
	if (fin==NULL) return -1;
	makeBlocksList(fin, blocksize, &head, &tail);
	fclose(fin);
	m48 = doHuffman(&head, &tail, num2encode, &encodedBlocks48, &codes48);
	printf("done?\n");
	for (i=0;i<m48;i++)
	{
		printf("block: %d, code %s\n",encodedBlocks48[i],codes48[i]);
	}
	fin = fopen("dataaa.txt","r");
	fout = fopen("output.txt","w");
//	makeOutput(fin, fout, encodedBlocks48, codes48, blocksize,m48);
	fclose(fin);
	fclose(fout);
	return 0;
}
