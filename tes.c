#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char labe[10],opc[10],opr[10];
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
void loadop()
{
FILE *fp;
int t,i,valu;
t=0;
fp=fopen("opco.txt","r");
do
{
fscanf(fp,"%s %s",labe,opr);
valu=atoi(opr);
strcpy(optab[t].op,labe);
optab[t].val=valu;t++;
printf("\n%s  %d",optab[t-1].op,optab[t-1].val);
}while(!feof(fp));
opcount=t;
}
void writesym()
{int i;
FILE *f=fopen("symtab.txt","w");
for(i=0;i<symcount;i++)
fprintf(f,"%s %x\n",symtab[i].sym,symtab[i].val);
}
int endad;
int searc(char a[])
{
for(int i=0;i<opcount;i++)
if(!strcmp(optab[i].op,a))
	return(1);
return(0);
}
void fetch(int start,FILE *fp)
{
//FILE *fp=fopen("progm.txt","r");
FILE *op=fopen("intr.txt","w");
char str[28];
int i,l,count,j,ad,s,symc,k,m;
count=0;
printf("strts at %d",start);
fprintf(op,"%6x %s %s\n",start,opr,opc);
ad=0x0;
symc=0;
ad+=start;
do
{	count=0;
	fgets(str,28,fp);
	//printf("\n%s",str);
	l=strlen(str);
	if(str[0]=='.')
	{printf("comment line");continue;}
	for(i=0;i<l;i++)
	{
	if(str[i]==' ')
	count++;
	}
	if(count==2)
	{
	for(i=0,j=0;str[i]!=' '&&i<l;i++,j++)
	labe[j]=str[i];
	labe[j]='\0';
		i++;
	for(j=0;str[i]!=' '&&i<l;i++,j++)
	opc[j]=str[i];
	opc[j]='\0';
	i++;
	for(j=0;str[i]!=' '&&i<l;i++,j++)
	opr[j]=str[i];
	opr[j]='\0';
	fprintf(op,"%06x %s %s",ad,opc,opr);
	strcpy(symtab[symc].sym,labe);
	symtab[symc].val=ad;symc++;
	}
	if(count==1)
	{
	
	for(i=0,j=0;str[i]!=' '&&i<l;i++,j++)
	opc[j]=str[i];
	opc[j]='\0';
	i++;
	for(j=0;str[i]!=' '&&i<l;i++,j++)
	opr[j]=str[i];
	opr[j]='\0';
	fprintf(op,"%06x %s %s",ad,opc,opr);
	}
	if(count==0)
	{
	for(i=0,j=0;str[i]!=' '&&i<l&&str[i]!='\n';i++,j++)
	opc[j]=str[i];
	opc[j]='\0';
	fprintf(op,"%06x %s\n",ad,opc);
	}
	//printf("\n%s   %s   %s",labe,opc,opr);
	
	if(searc(opc))
			ad+=3;
	else if(!strcmp(opc,"WORD"))
			ad+=3;
	else if(!strcmp(opc,"BYTE"))
			{
				s=strlen(opr);
				for(k=0;k<s&&opr[k]!='\'';k++);
				k++;
				for(m=0;k<s&&opr[k]!='\'';k++,m++);
				ad+=m;
			}
	else if(!strcmp(opc,"RESB"))
			{
				s=atoi(opr);
				ad+=s;
			}
	else if(!strcmp(opc,"RESW"))
			{
				s=atoi(opr)*3;
				ad+=s;
			}
	else if(!strcmp(opc,"END"))
	{printf("\nend");break;}
		strcpy(labe,"");
	strcpy(opc,"");
	strcpy(opr,"");
	
}while(!feof(fp));
symcount=symc;
endad=ad;
}
int main()
{
int st;
char name[6];
st=0;
loadop();
FILE *f=fopen("progm.txt","r");
FILE *f2=fopen("prodet.txt","w");
fscanf(f,"%s %s %s\n",labe,opr,opc);
if(!strcmp(opr,"START"))
{
st = (int)strtol(opc, NULL, 16); 
printf("\nstart found..........");
strcpy(name,labe);
}
else
	strcpy(name,"");
printf("\n %d",st);
fetch(st,f);
writesym();
fprintf(f2,"%x %x %s",st,endad,name);
return(0);
}

