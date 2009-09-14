/*
 * main.c
 *
 *  Created on: Sep 12, 2009
 *      Author: mits
 */

#include "huffman.h"
#include "math.h"
#include "stdlib.h"

extern struct listEntry *head;
extern int totalBlocks;


int main(int argc, char *argv[])
{
	int i,m48;
	uint32_t *encodedBlocks48;
	char **codes48;
	m48 = doHuffman("data.txt",4,8,encodedBlocks48,codes48);
	for (i=0;i<m48+1;i++)
	{
		printf("block: %d, code %s\n",encodedBlocks48[i],codes48[i]);
	}
	FILE *fin,*fout;
	fin = fopen("data.txt","r");
	fout = fopen("output.txt","w");
//	makeOutput(fin, fout, encodedBlocks48, codes48, 4,m48);
	fclose(fin);
	fclose(fout);













	return 0;
}
