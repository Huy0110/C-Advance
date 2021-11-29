#include <stdio.h>
#include <stdio_ext.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "cgraphv1.h"

int menuVaLuaChon(int a,int b)
{
  int luaChon,buff;
  char c;
  printf("+-----+--------------------------------------------------+\n");
  printf("|%25s%6s%25s|\n","","MENU","");
  printf("+-----+--------------------------------------------------+\n");
  printf("|%5d|%-50s|\n",1,"Y nghia");
  printf("+-----+--------------------------------------------------+\n");

  do
    {
      __fpurge(stdin);
      printf("Nhap vao lua chon cua ban:");
      buff=scanf("%d%c",&luaChon,&c);
      if(buff!=2&&c!='\n'||luaChon<a||luaChon>b) printf("Ban da nhap sai cu phap\n");
    }while(buff!=2&&c!='\n'||luaChon<a||luaChon>b);
  return luaChon;
}

typedef struct
{
  int out;
  int in;
  double trongSo;
  int id;
}vtx;

int docFileWeb(Graph g, char* tenFile)
{
  FILE* f;
  int n;
  int id;
  char str[100];
  f=fopen(tenFile,"r");
  if(f==NULL)
    {
      printf("File open failed");
      exit(1);
    }
  fscanf(f,"%d\n",&n);
  for(int i=0;i<n;i++)
    {
      fscanf(f,"%s %d\n",str,&id);
      addVertex(g,id,str);
    }
  return n;
}

void docFileConnect(Graph g, char* tenFile)
{
  FILE* f;
  int n;
  int id;
  int idKhac;
  char str[100];
   const char s[2] = " ";
   char *token;
  f=fopen(tenFile,"r");
  if(f==NULL)
    {
      printf("File open failed");
      exit(1);
    }
  fscanf(f,"%d\n",&n);
  for(int i=0;i<n;i++)
    {
      fscanf(f,"%d%*c",&id);
      fgets(str,100,f);
      if(strlen(str)==0||strlen(str)==1)
	continue;
      token = strtok(str, s);
      while( token != NULL )
	{
	  idKhac=atoi(token);
	  addEdge(g,id,idKhac,1.0);
	  // printf("%d %d\n",id,idKhac);
	  token = strtok(NULL, s);
	}
    }
}

void khoiTao(Graph g,vtx mangWeb[])
{
  JRB node;
  int id;
  double trongSo;
  int in;
  int out;
  int input[100];
  int output[100];
  vtx web;
  int i=0;
  jrb_traverse(node, g.vertices)
    {
      id=jval_i(node->key);
      trongSo=1.0;
      out= outdegree(g, id, output);
      in = indegree(g, id, input);
      web.id = id;
      web.trongSo = trongSo;
      web.in = in;
      web.out = out;
      mangWeb[i]= web;
      i++;
    }
}

int layPhanTuTuID(vtx mangWeb[], int id, int n)
{
  for(int i=0;i<n;i++)
    if(mangWeb[i].id==id)
      return i;
  return -1;
}

void pageRank(Graph g, vtx mangWeb[], int n)
{
  JRB node;
  JRB node2;
  JRB tree;
  double trongSo;
  int id;
  int id2;
  int i,k;
  vtx mangBuffer[100];
  int input[100];
  int in;
  for(i =0 ;i<n;i++)
    mangBuffer[i]= mangWeb[i];
  jrb_traverse(node, g.edges)
    {
      trongSo=0;
      id=jval_i(node->key);
      // printf("id: %d\n",id);
      k=layPhanTuTuID(mangWeb,id, n);
      in = indegree(g, id, input);
      for(int h=0;h<in;h++)
	{
	  id2=input[h];
	  //printf("%d\n",id2);
	  i=layPhanTuTuID(mangWeb,id2,n);
	  //  printf("%f // %f\n",mangBuffer[i].trongSo, (double) mangBuffer[i].out);
	  trongSo= trongSo + mangBuffer[i].trongSo / (double) mangBuffer[i].out;
	}
      mangWeb[k].trongSo=trongSo;
    }
}

int main()
{
  vtx mangWeb[100];
  Graph g = createGraph(DIRECT_GRAPH);
  int thoat,luaChon;
  thoat=0;
  int soLuongWeb;
  JRB node;
  int min = 1000;
  int max=0;
  int m;
  vtx web;
  soLuongWeb = docFileWeb(g,"webpages.txt");
  docFileConnect(g,"pageConnections.txt");
  do
    {
      luaChon= menuVaLuaChon(1,4);
      switch(luaChon)
	{
	case 1:
	  //ham1
	  docFileWeb(g,"webpages.txt");
	  docFileConnect(g,"pageConnections.txt");
	  printf("%d\n",soLuongWeb);
	  jrb_traverse(node,g.vertices)
	    printf("%s %d\n",jval_s(node->val),jval_i(node->key));
	  khoiTao(g,mangWeb);
	  printf("\nCac web co it lien ket toi nhat:\n");
	  for(int i=0; i<soLuongWeb;i++)
	    {
	      if(min>mangWeb[i].in)
		min=mangWeb[i].in;
	    }
	  for(int i=0; i<soLuongWeb;i++)
	    {
	      if(min==mangWeb[i].in)
		printf("%d %s\n",mangWeb[i].id,getVertex(g,mangWeb[i].id));
	    }
	  
	  printf("\nCac web co nhieu lien ket toi nhat:\n");
	  for(int i=0; i<soLuongWeb;i++)
	    {
	      if(max<mangWeb[i].in)
		max=mangWeb[i].in;
	    }
	  for(int i=0; i<soLuongWeb;i++)
	    {
	      if(max==mangWeb[i].in)
		printf("%d %s\n",mangWeb[i].id,getVertex(g,mangWeb[i].id));
	    }
	  break;
	case 2:
	  //ham2
	  pageRank(g, mangWeb,soLuongWeb);
	  for(int i = 0; i<soLuongWeb;i++)
	    {
	      printf("%d %f\n",mangWeb[i].id,mangWeb[i].trongSo);
	    }
	  break;
	case 3:
	  //ham3
	  do
	    {
	      printf("Nhap vao m ( >=10 && <=50):");
	      scanf("%d",&m);
	      if(m<10||m>50)
		printf("Nhap sai\n");
	    }while(m<10||m>50);
	    for(int i = 0 ;i<m;i++)
	      pageRank(g,mangWeb,soLuongWeb);
	  for(int i = 0; i<soLuongWeb;i++)
	    {
	      printf("%d %f\n",mangWeb[i].id,mangWeb[i].trongSo);
	    }
	  for(int i=0;i<soLuongWeb;i++)
	    for(int j=i;j<soLuongWeb;j++)
	      if(mangWeb[i].trongSo<mangWeb[j].trongSo)
		{
		  web=mangWeb[i];
		  mangWeb[i]=mangWeb[j];
		  mangWeb[j]=web;
		}
	  printf("Top 3 wab quan trong nhat:\n");
	  printf("%s\n%s\n%s\n",getVertex(g,mangWeb[0].id),getVertex(g,mangWeb[1].id),getVertex(g,mangWeb[2].id));
	  break;
	case 4:
	  thoat=1;
	  break;
	}	
    }while(thoat!=1);
}
