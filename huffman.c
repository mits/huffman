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

size_t readLine(char *line, size_t maxBytes,FILE *f)
{
	int pos=0;
	while(pos<maxBytes+1) {
		line[pos] = getc(f);
		if (line[pos]=='\n'||line[pos]==EOF) {
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
	int totalBlocks = 0;
	int pos, index, done=0;
	size_t maxlinesize=10;
	char block[blocksize+1];
	block[blocksize] = '\0';
	char *num;
	num = (char *)malloc(maxlinesize*sizeof(char));
	readLine(num,maxlinesize,f);
	maxlinesize = atoi(num);
	printf("parseFile, linesize: %d\n",maxlinesize);
	char *line = (char *)malloc(sizeof(char)*(maxlinesize+1));
	while (!done) {

		if (readLine(line, maxlinesize,f)==0||line[0]=='E') {
			done = 1;
//			break;
		} else {
	//		printf("parseFile, line: %s\n",line);

			index = 0;
			for(pos=0;pos<maxlinesize;pos++) {
				block[index] = line[pos];
				index++;
				if (index==blocksize) {
					addBlock(block,blocksize);
					totalBlocks++;
					index = 0;
//					printf("total: %d, block: %s\n",totalBlocks, block);
				}
			}
			if (index) {
				for(pos=index;pos<blocksize;pos++) block[pos] = 'X';
				addBlock(block,blocksize);
				totalBlocks++;
			}
		}
	}
	free(line);
	return totalBlocks;
}

struct listEntry *head, *tail;

void addBlock(char *block, int blocksize)
{
	struct listEntry *temp = head;
	struct listEntry *temp2;
	while(temp!=NULL && strcmp(temp->string,block))	temp = temp->next;
	if (temp!=NULL) {
		temp->freq++;
		temp2 = findFreqPos(temp,temp);
		if (temp2!=temp) {
			removeEntry(temp);
			insertAfter(temp,temp2);
		}
	}
	else {
		temp = createEntry(block, blocksize);
		insertAfter(temp,tail);
	}
}


void removeEntry(struct listEntry *block)
{
	if (block!=tail&&block!=head) {
		(block->next)->prev = block->prev;
		(block->prev)->next = block->next;
	}
	if (block==tail&&block!=head) {
		(block->prev)->next = NULL;
		tail = block->prev;
	}
	if (block==head&&block!=tail) {
		head = block->next;
		head->prev = NULL;
	}
	if (block==head&&block==tail) {
		head = NULL;
		tail = NULL;
	}
}

void insertAfter(struct listEntry *block, struct listEntry *previous)
{
	if(previous!=NULL&&previous!=tail) {
		block->prev = previous;
		block->next = previous->next;
		previous->next = block;
		(block->next)->prev = block;
	}
	if (previous==NULL) {
		block->next = head;
		block->prev = NULL;
		if (head!=NULL)	head->prev = block;
		if (head==NULL) tail=block;
		head = block;
	}
	if (previous!=NULL&&previous==tail) {
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
	if (temp == NULL) return NULL;
	if (temp!=block->prev) return temp;
	else return block;
}

struct listEntry* createEntry(char *str, uint8_t blocksize)
{
	struct listEntry *entry;
	entry = (struct listEntry *)malloc(sizeof(struct listEntry));
	if (entry!=NULL) {
		entry->freq = 1;
		entry->string = (char *)malloc((blocksize+1)*sizeof(char));
		strcpy(entry->string,str);
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
	printf("Entry:\n");
	printf("%s\n",block->string);
	printf("frequency: %d\n",block->freq);
}

void printList(struct listEntry *head)
{
	struct listEntry *cur = head;
	while(cur!=NULL) {
		printEntry(cur);
		cur = cur->next;
	}
}

struct listEntry* findUnspecified(struct listEntry *block)
{
	if (block == NULL) return NULL;
	int i;
	struct listEntry *temp = block;
	while(temp!=NULL) {
		for (i=0;i<block->blocksize;i++) {
			if (temp->string[i] == 'X') {
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
	for (i=0;i<blocksize;i++) {
		if ((str1[i]=='0' && str2[i]=='1')||(str1[i]=='1' && str2[i]=='0')
				||(str1[i]=='U')||(str2[i]=='U')) return 0;
	}
	return 1;
}

void mergeX(struct listEntry *one, struct listEntry *two)
{
	int blocksize = one->blocksize;
	struct listEntry *temp;
	mergeStrings(one->string,two->string,blocksize);
	merge(one,two);
	temp=head;
	while ((temp!=NULL&&strcmp(one->string,temp->string))||(temp==one)) temp = temp->next;
	if (temp!=NULL) merge(one,temp);
}

void merge(struct listEntry *one, struct listEntry *two)
{
	one->freq += two->freq;
	removeEntry(two);
	deleteEntry(two);
	struct listEntry *temp;
	temp = findFreqPos(one,one);
	if (temp!=one) {
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

void mergeAll()
{
	struct listEntry *current,*temp;
	uint8_t blocksize = head->blocksize;
	int merged,i;
	current = findUnspecified(head);
//	printf("hello?\n");
	while(current!=NULL)
	{
//		printf("current:\n");
//		printEntry(current);
		temp = head;
		merged = 0;
		while((temp=temp->next)!=NULL) {
//			printf("merge???? %d %d\n",current->index,temp->index);
			if (canMerge(current->string,temp->string,blocksize)&&current!=temp) {
//				printf("merge!!!\n");
				mergeX(current,temp);
				temp = tail;
				merged = 1;
			}
		}
		if (merged == 0) {
			for (i=0;i<blocksize;i++) {
				if (current->string[i]=='X') current->string[i] = (rand() % 2)?'1':'0';
			}
			current = findUnspecified(current->next);
		}
	}
}

int chopList(int blocks2encode)
{
	int i,num=0,freq=0;
	struct listEntry *temp=head,*temp2,*temp3;
	for (i=0;i<blocks2encode;i++) {
		if (temp!=NULL) {
			num++;
			temp = temp->next;
		}
	}
	temp2=temp;
	while(temp2!=NULL) {
		temp3 = temp2;
		temp2 = temp2->next;
		removeEntry(temp3);
		freq +=temp3->freq;
		deleteEntry(temp3);
	}
	if (freq>0) {
		temp3 = createEntry("U",head->blocksize);
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
	while(head!=tail) {
		other = tail;
		one = tail->prev;
		parent = createEntry("PP",one->blocksize);
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

void searchTree(struct listEntry *node,int position, char *encoding, char **blocks, char **codes)
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
		while(strcmp(blocks[i],node->string)) {printf("%s!%s\n",blocks[i],node->string);i++;}
		printf("i: %d\n",i);
		for (j=0;j<position;j++) codes[i][j] = encoding[j];
		codes[i][position] = '\0';
		printf("encoded block:\n");
		printf("block: %s\n",node->string);
		printf("\nhuffman code: %s\n",codes[i]);
	}
}

void makeBlocksTable(char **table)
{
	struct listEntry *temp = head;
	int i=0;
	while(temp!=NULL) {
		printf("&&%s\n",temp->string);
		if (temp->string[0]!='U') strcpy(table[i++],temp->string);
		temp = temp->next;
	}
	table[i] = "U";
}

void encodeBlock(char *unenc, char *enc, char **blocks, char **codes, int blocksize, int encodedblocks, int distinctBlocks)
{
	int i=0;
	while(!canMerge(unenc,blocks[i],blocksize)&&(i<distinctBlocks)) i++;
//	printf("i=%d\n",i);
	if (i<distinctBlocks) {
		strcpy(enc,codes[i]);
	//	printf("OUT:%d\n",i);
	} else {
		for (i=0;i<blocksize;i++) if (unenc[i]=='X') unenc[i] = (rand() % 2)?'1':'0';
	//	printf("unencoded\n");
		sprintf(enc,"%s%s",codes[encodedblocks-1],unenc);
	}
}


void makeOutput(FILE *in, FILE *out, char **blocks, char **codes, int blocksize, int encodedblocks)
{
	if (in==NULL||out==NULL) return;
	int pos, index, done=0;
	int i;
	int linelen;
	int distinctBlocks;
	char num[10];
	char temp[encodedblocks+1];
	readLine(num,10,in);
	linelen = atoi(num);
	char *line = (char *)malloc(sizeof(char)*linelen);
	char block[blocksize+1];
	block[blocksize]='\0';
	i=0;
	while(blocks[i][0]!='U' && i<encodedblocks) i++;
	distinctBlocks = i;
	while (!done) {
		if (readLine(line, linelen,in)==0||line[0]=='E') {
			done = 1;
			break;
		}
		index = 0;
		for(pos=0;pos<linelen;pos++) {
			block[index] = line[pos];
			index++;
			if (index==blocksize) {
				encodeBlock(block, temp, blocks, codes, blocksize, encodedblocks, distinctBlocks);
				fprintf(out,"%s",temp);
				index = 0;
			}
		}
		if (index) {
			for(pos=index;pos<blocksize;pos++) block[pos] = 'X';
			encodeBlock(block, temp, blocks, codes, blocksize, encodedblocks, distinctBlocks);
			fprintf(out,"%s",temp);
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
	printf("---------------------------\n");
	mergeAll();
	printf("AFTER MERGES!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	printList(head);
	*listHead = head;
	*listTail = tail;
	return 0;
}

int doHuffman(struct listEntry **listHead, struct listEntry **listTail, int num2encode, char ***encodedBlocks, char ***codes)
{
	int m,i;
	head = *listHead;
	tail = *listTail;
	m = chopList(num2encode);
	printf("AFTER CHOP!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	printList(head);
	*codes = malloc(m*sizeof(char*));
	for(i=0;i<m;i++) {
		(*codes)[i] = malloc(m*sizeof(char));
	}
	*encodedBlocks = malloc(m*sizeof(char *));
	for(i=0;i<m;i++) {
		(*encodedBlocks)[i] = malloc((head->blocksize+1)*sizeof(char));
		strcpy((*encodedBlocks)[i],"");
	}
	printf("make tree?\n");
	makeBlocksTable(*encodedBlocks);
	makeTree();
	char encoding[20+1];
	searchTree(head,0,encoding,*encodedBlocks,*codes);
	return m;
}
