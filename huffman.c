/*
 * huffman.c
 *
 *  Created on: Sep 12, 2009
 *      Author: mits
 */


#include "huffman.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

int totalBlocks;
/*
int parseFile(FILE *f, int blocksize)
{
	if (f==NULL) return -1;
	totalBlocks = 0;
	int pos, index, doneline,done=0;
	char *line = (char *)malloc(sizeof(char)*MAXLINESIZE);
	char block[blocksize];
	size_t maxlinesize=MAXLINESIZE;
	while (!done)
	{
		if (getline(&line, &maxlinesize,f)==-1)
		{
			done = 1;
			break;
		}
		printf("%s",line);
		doneline=0;
		pos = 0;
		index = 0;
		while(!doneline)
		{
			if (line[pos]=='\0')
			{
				doneline = 1;
				if (index>0)
				{

				}
			}
			else
			{
				block[index] = line[pos];
				index++;
				pos++;
				if (index==blocksize)
				{
					addBlock(block,blocksize);
					totalBlocks++;
//					printf("%s   %d\n",block,pos);
					index = 0;
				}
			}
		}
	}
	free(line);
	return 0;
}
*/
size_t readLine(char *line, size_t maxBytes,FILE *f)
{
	int pos=0;
	while(pos<maxBytes+1)
	{
		line[pos] = getc(f);
		if (line[pos]=='\n'||line[pos]==EOF)
		{
			line[pos] = '\0';
			return pos;
		}
		pos++;
	}
	return pos;
}

int parseFile(FILE *f, int blocksize)
{
	if (f==NULL) return -1;
	totalBlocks = 0;
	int pos, index, done=0;
	size_t maxlinesize=10;
	char block[blocksize];
	char *num;
	num = (char *)malloc(maxlinesize*sizeof(char));
	readLine(num,maxlinesize,f);
	maxlinesize = atoi(num);
	printf("parseFile, linesize: %d\n",maxlinesize);
	char *line = (char *)malloc(sizeof(char)*(maxlinesize+1));
	while (!done)
	{

		if (readLine(line, maxlinesize,f)==0||line[0]=='E')
		{
			done = 1;
			break;
		}
//		printf("parseFile, line: %s\n",line);

		index = 0;
		for(pos=0;pos<maxlinesize;pos++)
		{
			block[index] = line[pos];
			index++;
			if (index==blocksize)
			{
				addBlock(block,blocksize);
				totalBlocks++;
				index = 0;
			}
		}
		if (index)
		{
			for(pos=index;pos<blocksize;pos++) block[pos] = 'X';
			addBlock(block,blocksize);
			totalBlocks++;
		}
	}
	free(line);
	return 0;
}

int findIndex(char *s, int blocksize)
{
	int i,num=0;
	for (i=blocksize-1;i>=0;i--)
	{
		switch (s[i])
		{
		case '0':
			break;
		case '1':
			num += pow(3,blocksize-i-1);
			break;
		case 'X':
			num += 2*pow(3,blocksize-i-1);
			break;
		default:
			return -1;
		}
	}
	return num;
}

int findString(char *s,int blocksize,uint32_t index)
{
	int i,j,temp;
	temp = index;
	if (index==UINT32_MAX) return -1;
	for (i=blocksize-1;i>=0;i--)
	{
		j = temp % 3;
		temp = temp / 3;
		switch (j)
		{
		case 0:
			s[i] = '0';
			break;
		case 1:
			s[i] = '1';
			break;
		case 2:
			s[i] = 'X';
			break;
		}
	}
	return 0;
}

struct listEntry *head, *tail;

void addBlock(char *block, int blocksize)
{
	int blockIndex;
	struct listEntry *temp = head;
	struct listEntry *temp2;
	blockIndex = findIndex(block,blocksize);
	while(temp!=NULL && temp->index!=blockIndex) temp = temp->next;
	if (temp!=NULL)
	{
		temp->freq++;
		temp2 = findFreqPos(temp,temp);
		if (temp2!=temp)
		{
			removeEntry(temp);
			insertAfter(temp,temp2);
		}
	}
	else
	{
		temp = createEntry(blockIndex, blocksize);
		insertAfter(temp,tail);
	}
}


void removeEntry(struct listEntry *block)
{
	if (block!=tail&&block!=head)
	{
		(block->next)->prev = block->prev;
		(block->prev)->next = block->next;
	}
	if (block==tail&&block!=head)
	{
		(block->prev)->next = NULL;
		tail = block->prev;
	}
	if (block==head&&block!=tail)
	{
		head = block->next;
		head->prev = NULL;
	}
	if (block==head&&block==tail)
	{
		head = NULL;
		tail = NULL;
	}
}
void insertAfter(struct listEntry *block, struct listEntry *previous)
{
	if(previous!=NULL&&previous!=tail)
	{
		block->prev = previous;
		block->next = previous->next;
		previous->next = block;
		(block->next)->prev = block;
	}
	if (previous==NULL)
	{
		block->next = head;
		block->prev = NULL;
		if (head!=NULL)	head->prev = block;
		if (head==NULL) tail=block;
		head = block;
	}
	if (previous!=NULL&&previous==tail)
	{
		block->prev = previous;
		block->next = NULL;
		previous->next = block;
		tail = block;
	}
}

struct listEntry* findFreqPos(struct listEntry *block, struct listEntry *start)
{
	if (start==NULL) return NULL;
	struct listEntry *temp;
	int blockfreq=block->freq;
	if (start==block) temp=block->prev;
	else temp=start;
	while(temp!=NULL && temp->freq<blockfreq) temp = temp->prev;
	if (temp!=block->prev) return temp;
	else return block;
}

struct listEntry* createEntry(int index, uint8_t blocksize)
{
	struct listEntry *entry;
	entry = (struct listEntry *)malloc(sizeof(struct listEntry));
	if (entry!=NULL)
	{
		entry->freq = 1;
		entry->index = index;
		entry->blocksize = blocksize;
		entry->childs[0] = NULL;
		entry->childs[1] = NULL;
		entry->prev = NULL;
		entry->next = NULL;
	}
	return entry;
}

void initList(struct listEntry **h, struct listEntry **t)
{
	*h = NULL;
	*t = NULL;
}

void printEntry(struct listEntry *block)
{
	int i;
	printf("Entry:\n");
	char str[block->blocksize];
	findString(str,block->blocksize,block->index);
	for (i=0;i<block->blocksize;i++) printf("%c",str[i]);
	printf("\nindex: %d\n",block->index);
	printf("frequency: %d\n",block->freq);
}

void printList(struct listEntry *head)
{
	struct listEntry *cur = head;
	while(cur!=NULL)
	{
		printEntry(cur);
		cur = cur->next;
	}
}

void mergeAll()
{
	struct listEntry *current,*temp, *temp2;
	uint8_t blocksize = head->blocksize;
	char str1[blocksize], str2[blocksize];
	current = head;
//	printf("hello?\n");
	while((current=findUnspecified(current))!=NULL)
	{
//		printf("current:\n");
//		printEntry(current);
		findString(str1,blocksize,current->index);
		temp = current;
		while((temp=findUnspecified(temp))!=NULL)
		{
//			printf("merge???? %d %d\n",current->index,temp->index);
			findString(str2,blocksize, temp->index);
			if (canMerge(str1,str2,blocksize))
			{
//				printf("merge!!!\n");
				temp2 = temp->prev;
				mergeX(current,temp);
				temp = temp2;
			}
		}
	}
}


struct listEntry* findUnspecified(struct listEntry *block)
{
	if (block == NULL) return NULL;
	char str[block->blocksize];
	int i;
	struct listEntry *temp = block;
	while(temp!=NULL)
	{
		findString(str,temp->blocksize,temp->index);
		for (i=0;i<block->blocksize;i++)
		{
			if (str[i] == 'X')
			{
				return temp;
			}
		}
		temp = temp->next;
	}
	return NULL;
}

int canMerge(char *str1,  char *str2, int blocksize)
{
	int i;
	for (i=0;i<blocksize;i++)
	{
		if ((str1[i]=='0' && str2[i]=='1')||(str1[i]=='1' && str2[i]=='0')) return 0;
	}
	return 1;
}

void mergeX(struct listEntry *one, struct listEntry *two)
{
	int blocksize = one->blocksize;
	char str1[blocksize],str2[blocksize];
	struct listEntry *temp;
	findString(str1,blocksize,one->index);
	findString(str2,blocksize,two->index);
	mergeStrings(str1,str2,blocksize);
	one->index = findIndex(str1,blocksize);
	merge(one,two);
	temp=head;
	while ((temp!=NULL&&temp->index!=one->index)||(temp==one)) temp = temp->next;
	if (temp!=NULL)
	{
		merge(one,temp);
	}
}

void merge(struct listEntry *one, struct listEntry *two)
{
	one->freq += two->freq;
	removeEntry(two);
	deleteEntry(two);
	struct listEntry *temp;
	temp = findFreqPos(one,one);
	if (temp!=one)
	{
		removeEntry(one);
		insertAfter(one,temp);
	}
}


void mergeStrings(char *str1, char *str2, int blocksize)
{
	int i;
	for(i=0;i<blocksize;i++) if (str1[i]=='X') str1[i]=str2[i];
}

void deleteEntry(struct listEntry *entry)
{
	free(entry);
}

void mergeRest()
{
	struct listEntry *current,*temp;
	uint8_t blocksize = head->blocksize;
	char str1[blocksize], str2[blocksize];
	int merged,i;
	current = findUnspecified(head);
//	printf("hello?\n");
	while(current!=NULL)
	{
//		printf("current:\n");
		printEntry(current);
		findString(str1,blocksize,current->index);
		temp = head;
		merged = 0;
		while((temp=temp->next)!=NULL)
		{
//			printf("merge???? %d %d\n",current->index,temp->index);
			findString(str2,blocksize, temp->index);
			if (canMerge(str1,str2,blocksize)&&current!=temp)
			{
//				printf("merge!!!\n");
				mergeX(current,temp);
				temp = tail;
				merged = 1;
			}
		}
		if (merged == 0)
		{
			for (i=0;i<blocksize;i++)
			{
				if (str1[i]=='X') str1[i] = (rand() % 2)?'1':'0';
			}
			current->index = findIndex(str1,blocksize);
			current = findUnspecified(current->next);
		}
	}
}

int chopList(int blocks2encode)
{
	int i,num=0,freq=0;
	struct listEntry *temp=head,*temp2,*temp3;
	for (i=0;i<blocks2encode;i++)
	{
		if (temp!=NULL)
		{
			num++;
			temp = temp->next;
		}
	}
	temp2=temp;
	while(temp2!=NULL)
	{
		temp3 = temp2;
		temp2 = temp2->next;
		removeEntry(temp3);
		freq +=temp3->freq;
		deleteEntry(temp3);
	}
	if (freq>0)
	{
		temp3 = createEntry(UINT32_MAX,head->blocksize);
		temp3->freq = freq;
		temp2 = findFreqPos(temp3,tail);
		if (temp2!=temp3) insertAfter(temp3,temp2);
		else insertAfter(temp3,tail);
		num++;
	}
	return num;
}

void makeTree()
{
	struct listEntry *one,*other,*parent,*temp;
	while(head!=tail)
	{
		other = tail;
		one = tail->prev;
		parent = createEntry(UINT32_MAX,one->blocksize);
		parent->freq = one->freq + other->freq;
		parent->childs[0] = one;
		parent->childs[1] = other;
		removeEntry(other);
		removeEntry(one);
		temp = findFreqPos(parent,tail);
		if (temp!=parent) insertAfter(parent,temp);
		else insertAfter(parent,tail);
	}
}

void searchTree(struct listEntry *node,int position, char *encoding, uint32_t *blocks, char **codes)
{
	int i,j;
	printf("position %d\n",position);
	if (node->childs[0]!=NULL)
	{
		printf("has childs\n");
		encoding[position] = '0';
		searchTree(node->childs[0],position+1,encoding,blocks,codes);
		printf("child2\n");
		encoding[position] = '1';
		searchTree(node->childs[1],position+1,encoding,blocks,codes);
	}
	else
	{
		i=0;
//		printf("asdf\n");
		while(blocks[i]!=node->index) i++;
		printf("i: %d\n",i);
//		printf("asdf %d\n",position);
		for (j=0;j<position;j++)
		{
//			printf("i%d j%d\n",i,j);
			codes[i][j] = encoding[j];
		}
//		printf("asdf\n");
		codes[i][position] = '\0';

		printf("encoded block:\n");
		char str[node->blocksize];
		if (node->index!=UINT32_MAX) findString(str,node->blocksize,node->index);
		else for (i=0;i<node->blocksize;i++) str[i]='X';
		printf("block: ");
		for(i=0;i<node->blocksize;i++) printf("%c",str[i]);
		printf("\nhuffman code: ");
		for (i=0;i<position;i++) printf("%c",encoding[i]);
		printf("\n");

	}
}

void makeBlocksTable(uint32_t *table)
{
	struct listEntry *temp=head;
	int i=0;
	while(temp!=NULL)
	{
		if (temp->index!=UINT32_MAX) table[i++]=temp->index;
		temp = temp->next;
	}
	table[i] = UINT32_MAX;
}

void encodeBlock(char *unenc, char *enc, uint32_t *blocks, char **codes, int blocksize, int encodedblocks, char *unencodedCode)
{
	char str[blocksize];
	int i=0;
	findString(str,blocksize,blocks[i]);
	while(!canMerge(unenc,str,blocksize)&&i<encodedblocks)
	{
		i++;
		findString(str,blocksize,blocks[i]);
	}
	if (i<encodedblocks)
	{
		strcpy(enc,codes[i]);
//		printf("%s %s\n",unenc, codes[i]);
	}
	else
	{
		for (i=0;i<blocksize;i++) if (unenc[i]=='X') unenc[i] = (rand() % 2)?'1':'0';
		sprintf(enc,"%s%s",unencodedCode,unenc);
	}
}


void makeOutput(FILE *in, FILE *out, uint32_t *blocks, char **codes, int blocksize, int encodedblocks)
{
	if (in==NULL||out==NULL) return;
	int pos, index, doneline,done=0;
	int i;
	int linelen;
	char num[10];
	char temp[encodedblocks+1];
	readLine(num,10,in);
	linelen = atoi(num);
	char *line = (char *)malloc(sizeof(char)*linelen);
	char block[blocksize+1],str[blocksize];
	char unencoded[encodedblocks+1];
	i=0;
	while(blocks[i]<UINT32_MAX && i<encodedblocks) i++;
	if (i<encodedblocks) strcpy(unencoded,codes[i]);
	block[blocksize]='\0';
	while (!done)
	{
		if (readLine(line, linelen,in)==0||line[0]=='E')
		{
			done = 1;
			break;
		}
		index = 0;
		for(pos=0;pos<linelen;pos++)
		{
			block[index] = line[pos];
			index++;
			if (index==blocksize)
			{
				encodeBlock(block, temp, blocks, codes, blocksize, encodedblocks, unencoded);
				fprintf(out,"%s",temp);
				totalBlocks++;
				index = 0;
			}
		}
		if (index)
		{
			for(pos=index;pos<blocksize;pos++) block[pos] = 'X';
			encodeBlock(block, temp, blocks, codes, blocksize, encodedblocks, unencoded);
			fprintf(out,"%s",temp);
			totalBlocks++;
		}
		fprintf(out,"\n");
	}
	free(line);
}

int makeBlocksList(FILE *fin, int blocksize, struct listEntry **listHead, struct listEntry **listTail)
{
	initList(listHead, listTail);
	parseFile(fin,blocksize);
	printf("AFTER PARSE FILE!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	printList(head);
	//mergeAll();
//	printf("AFTER MERGES!!!!!!!!!!!!!!!!!!!!!!!!!\n");
//	printList(head);
	mergeRest();
	printf("AFTER MERGEREST!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	printList(head);
	*listHead = head;
	*listTail = tail;
	return 0;
}

int doHuffman(struct listEntry **listHead, struct listEntry **listTail, int num2encode, uint32_t **encodedBlocks, char ***codes)
{
	int m,i;
	head = *listHead;
	tail = *listTail;
	m = chopList(num2encode);
	printf("AFTER CHOP!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	printList(head);
	*codes = malloc(m*sizeof(char*));
	for(i=0;i<m;i++)
	{
		(*codes)[i] = malloc(m*sizeof(char));
	}
	*encodedBlocks = (uint32_t*)malloc(m*sizeof(uint32_t));
	makeBlocksTable(*encodedBlocks);
	printf("make tree?\n");
	makeTree();
	printf("total: %d\n",totalBlocks);
	char encoding[20+1];
	searchTree(head,0,encoding,*encodedBlocks,*codes);
	return m;
}