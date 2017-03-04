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
printf("\n%s  %x",optab[t-1].op,optab[t-1].val);
}while(!feof(fp));
opcount=t;
}
void writesym()
{int i;
FILE *f=fopen("symtab.txt","w");
for(i=0;i<symcount;i++)
{fprintf(f,"%s %x\n",symtab[i].sym,symtab[i].val);
printf("%s %x\n",symtab[i].sym,symtab[i].val);}
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
FILE *stab=fopen("symtab.txt","w");
char str[28],WOR[3][12];
 char * pch;
int i,l,count,j,ad,s,symc,k,m,cf;
int len[3];
count=0;
printf("strts at %x",start);
fprintf(op,"%6x %s %s\n",start,opr,opc);
ad=0x0;
symc=0;
ad=start;
do
{	count=0;cf=0;
	fgets(str,28,fp);
	printf("\n%s",str);
	//fprintf(op,"\n%s",str);
	l=strlen(str);
	if(str[0]=='.')
	{printf("comment line");continue;}
  	pch = strtok (str," \t");
  		while (pch != NULL)
  	{	l=strlen(pch);
		len[count]=l;
		printf("--%d",l);
  	 	printf ("%s\n",pch);
		//pch[l]='\0';
		strcpy(WOR[count],pch);
		printf("-%s-",WOR[count]);
		printf("\n:%c",WOR[count][l-1]);
		WOR[count][l]='\0';
		count++;
		pch = strtok (NULL, " \t");
 	 }

	printf(" count=%d",count);
	count--;
	if(count==2)
	{
		strcpy(labe,WOR[0]);
		strcpy(opc,WOR[1]);
		strcpy(opr,WOR[2]);
	fprintf(op,"%06x %s %s",ad,opc,opr);
	printf("\n%s-%s",labe,WOR[0]);
	printf("\n%s-%s",opc,WOR[1]);
	printf("\n%s-%s",opr,WOR[2]);
	strcpy(symtab[symc].sym,labe);
	symtab[symc].val=ad;symc++;
	//fprintf(stab,"%s %x\n",labe,ad);
	}
	if(count==1)
	{
		
		strcpy(opc,WOR[0]);
		strcpy(opr,WOR[1]);
		
	printf("\n%s-%s",opc,WOR[0]);
	printf("\n%s-%s",opr,WOR[1]);
	fprintf(op,"%06x %s %s",ad,opc,opr);
	printf("cc%scc",opc);
	}
	if(count==0)
	{
		strcpy(opc,WOR[0]);
		printf("\n%s-%s",opc,WOR[0]);
	
	l=strlen(opc);
	opc[l-1]='\0';
	fprintf(op,"%06x %s\n",ad,opc);
	printf("cc%scc",opc);
	}
	//printf("\n%s   %s   %s",labe,opc,opr);
	if(!strcmp(opc,"END"))
		{
			printf("\nend");break;
		}
	else if(searc(opc))
			ad+=3;
	else if(!strcmp(opc,"WORD"))
			ad+=3;
	else if(!strcmp(opc,"BYTE"))
			{
				s=strlen(opr);
				for(k=0;k<s&&opr[k]!='\'';k++);
				k++;
				for(m=0;k<s&&opr[k]!='\'';k++,m++);
				
				if(opr[0]=='C')
				ad+=m;
				else if(opr[0]=='X')
				ad+=(m/2);
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
	else
		{
			printf("\nopcode error");
			//break;
		}
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
FILE *f=fopen("pgm.txt","r");
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

