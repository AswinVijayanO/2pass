#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct hrecord
{
int stadr;
int ln;
char name[6];
}hrecord;
hrecord h;
typedef struct trecord
{
int stadr;
int ln;
long int trec[11];
}trecord;
trecord t1[10];
int tc,tcnt;
typedef struct erecord
{
int stadr;
}erecord;
char adr[6];
char opc[8];
char opr[8];
char labe[8];

typedef struct opt
{
char op[6];
int val;
}opt;
opt optab[20];
int opcount,symcount;
typedef struct symt
{
char sym[6];
int val;
}symt;
symt symtab[20];
int t;

void loadop()
{
	FILE *fp;
	int t,i,valu;
	t=0;
	fp=fopen("opco.txt","r");
	do
	{
		fscanf(fp,"%s %s",labe,opr);
		valu=(int)strtol(opr, NULL, 16); 
		strcpy(optab[t].op,labe);
		optab[t].val=valu;t++;
		printf("\n%s  %d",optab[t-1].op,optab[t-1].val);
	}while(!feof(fp));
	opcount=t;
}

void readsym()
{
	int i;
	int t,valu;
	FILE *fp=fopen("symtab.txt","r");
	t=0;
	do
	{
		fscanf(fp,"%s %s",labe,opr);
		valu = (int)strtol(opr, NULL, 16); 
		strcpy(symtab[t].sym,labe);
		symtab[t].val=valu;t++;
		//printf("\n%s  %x",symtab[t-1].sym,symtab[t-1].val);
	}while(!feof(fp));
	symcount=t;
}

int searc(char a[])
{
	for(int i=0;i<opcount;i++)
	if(!strcmp(optab[i].op,a))
		return(optab[i].val);
	return(-1);
}
int searsym(char a[])
{
	for(int i=0;i<symcount;i++)
	if(!strcmp(symtab[i].sym,a))
		return(symtab[i].val);

	return(0);
}
FILE *opj;

void fetch(int start)
{
FILE *fp=fopen("intr.txt","r");
FILE *save;
char str[28];
int i,l,count,j,ad,s,symc,tex,tloop,frg,temp,k,m,flx,g;
char ter[20];
frg=1;
flx=0;
tcnt=0x0;
count=0;
//printf("strts at %d",start);
symc=0;
ad=start;
t1[tc].stadr=start;
//t1[tc].stadr=ad;
do
{	if((tcnt==10||!strcmp(opc,"RESW")||!strcmp(opc,"RESB"))&&frg)
		{	
			//t1[tc].ln=tcnt*3;
			fprintf(opj,"\nt^%06x^%02x",t1[tc].stadr,t1[tc].ln);
			for(tloop=0;tloop<tcnt;tloop++)
			fprintf(opj,"^%06x",t1[tc].trec[tloop]);
			tcnt=0x0;
			tc++;
			t1[tc].stadr=ad;
			frg=0;
		}
	count=0;
	fgets(str,28,fp);
	//printf("\n%s",str);
	if(str[0]=='.')
		continue;
	l=strlen(str);
	for(i=0;i<l;i++)
	{
	if(str[i]==' ')
	count++;
	}
	str[l-1]='\0';

	tex=1;
	
	if(count==2)
	{
	for(i=0,j=0;str[i]!=' '&&i<l;i++,j++)
	adr[j]=str[i];
	adr[j]='\0';
		i++;
	for(j=0;str[i]!=' '&&i<l;i++,j++)
	opc[j]=str[i];
	opc[j]='\0';
	i++;
	for(j=0;(str[i]!=' ')&&i<l;i++,j++)
	opr[j]=str[i];
	opr[j-1]='\0';

		
	ad = (int)strtol(adr, NULL, 16); 
		if(!strcmp(opc,"START"))
			{printf("\n..");	continue;}
		if(!strcmp(opc,"END"))
			break;
		if(!strcmp(opc,"RSUB"))
		{
			printf("\nrsub error");
		}
		if(searc(opc)>=0&&searsym(opr))
		{	
			tex=((searc(opc)*65536)+searsym(opr));
			printf("\n%s",opr);

			if(frg==0)
			t1[tc].stadr=ad;
			frg=1;ad = (int)strtol(adr, NULL, 16); 
			//if(flx)
			//{ tex+=0x8000;flx=0;printf("index found");}
			t1[tc].ln+=3;
		}
		//printf("operant is present -%s-  -%s-",opc,opr);
		//printf("operant value -%x %s\n",searsym(opr),opr);	
		else if(!strcmp(opc,"WORD"))
				{	if(frg==0)
					t1[tc].stadr=ad;
					tex = (int)strtol(opr, NULL, 16);frg=1;ad = (int)strtol(adr, NULL, 16); 
					ad = (int)strtol(adr, NULL, 16); t1[tc].ln+=3;
				}
		else if(!strcmp(opc,"BYTE"))
				{ 	 tex=0;
					if(frg==0)
					t1[tc].stadr=ad;
					ad = (int)strtol(adr, NULL, 16); 
					if(opr[0]=='C')
					{	s=strlen(opr);
						tex=0;
						for(m=0,k=2;k<s-1;k++,m++)
						{		temp=opr[k];
								printf("%x",temp);
								tex=(tex*0x100)+temp;
								t1[tc].ln++;
						}
					}
					else if(opr[0]=='X')	
					{
						s=strlen(opr);	
						for(m=0,k=2;k<s-1;k++,m++)
							ter[m]=opr[k];
						        tex = (int)strtol(ter, NULL, 16);
					s-=3;
					s/=2;
					t1[tc].ln+=s;
					}
						
					frg=1;
				}
		else if(!strcmp(opc,"RESW")||!strcmp(opc,"RESB"))
				{printf("\nreswb");ad = (int)strtol(adr, NULL, 16); continue;}
		else
			{
				opr[j-3]='\0';
					if(searc(opc)>=0&&searsym(opr))
					{	
						tex=((searc(opc)*65536)+searsym(opr))+0x8000;
						printf("\n%s",opr);
						if(frg==0)
						t1[tc].stadr=ad;
						frg=1;ad = (int)strtol(adr, NULL, 16); t1[tc].ln+=3;
					}
				else
					printf("%s",opc);
			}	
		
		t1[tc].trec[tcnt]=tex;
			tcnt++;
	}
	if(count==1)
	{
	for(i=0,j=0;str[i]!=' '&&i<l;i++,j++)
	adr[j]=str[i];
	adr[j]='\0';
	i++;
	for(j=0;str[i]!=' '&&i<l;i++,j++)
	opc[j]=str[i];
	opc[j]='\0';
		
		if(searc(opc)>=0)
		{	tex=(searc(opc)*0x10000);
			//fprintf(opj,"^%06x",str,tex);
			t1[tc].trec[tcnt]=tex;
			tcnt++;
			ad = (int)strtol(adr, NULL, 16); 
			frg=1;t1[tc].ln+=3;
		}
		
	}

	printf("\n\n%d",count);
printf("\n%x  %s  %x",ad,str,tex);
}while(!feof(fp));
l=(tcnt+2)*6;
fprintf(opj,"\nt^%06x^%02x",t1[tc].stadr,t1[tc].ln);
for(tloop=0;tloop<tcnt;tloop++)
fprintf(opj,"^%06x",t1[tc].trec[tloop]);
}
int main()
{
char nam[12],st[12],ed[12];
FILE *h1;
int i,stat,ent;
h1=fopen("prodet.txt","r");
opj=fopen("object.txt","w");
tc=0;
fscanf(h1,"%s %s %s",st,ed,nam);
stat = (int)strtol(st, NULL, 16); 
ent = (int)strtol(ed, NULL, 16); 
fprintf(opj,"h^%s^%x^%x",nam,stat,ent-stat);
loadop();
readsym();
fetch(stat);
fprintf(opj,"\ne^%s",st);
return(0);
}
