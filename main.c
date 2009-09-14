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
	int blocksize,i,m;
	m=12;
	FILE *f;
	f = fopen("data.txt","r");
	blocksize = 12;
	initList();
//	table = (uint32_t*)malloc(pow(3,blocksize)*sizeof(uint32_t));
	parseFile(f,blocksize);
	fclose(f);
	struct listEntry *temp=head;
	while (temp!=NULL)
	{
		printEntry(temp);
		temp = temp->next;
	}
	mergeAll();
	printf("================================\n");
	temp = head;
	while (temp!=NULL)
	{
		printEntry(temp);
		temp = temp->next;
	}
	mergeRest();
	printf("================================\n");
	temp = head;
	while (temp!=NULL)
	{
		printEntry(temp);
		temp = temp->next;
	}
	chopList(m);
	printf("================================\n");
	temp = head;
	while (temp!=NULL)
	{
		printEntry(temp);
		temp = temp->next;
	}
	printf("================================\n");
	uint32_t blocks[m+1];
	char *codes[m+1];
	for(i=0;i<(m+1);i++)
	{
		codes[i] = (char *)malloc((m+1)*sizeof(char));
	}
	makeBlocksTable(blocks);
	makeTree();
	printf("total: %d\n",totalBlocks);
	char encoding[8+1];
	searchTree(head,0,encoding,blocks,codes);
	for (i=0;i<m+1;i++)
	{
		printf("block: %d, code %s\n",blocks[i],codes[i]);
	}
	FILE *fin,*fout;
	fin = fopen("data.txt","r");
	fout = fopen("output.txt","w");
	makeOutput(fin, fout, blocks, codes, blocksize,m);
	fclose(fin);
	fclose(fout);













	return 0;
}
