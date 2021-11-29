#include <stdio.h>
#include <stdio_ext.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "cgraphv1.h"
#define Max_Str 100

typedef struct
{
  int out;
  double trongSo;
  int in;
  int id;
}vtx;

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

int docFileWeb(char* tenFile, Graph g)
{
  FILE *f;
  int n;
  int i;
  int id;
  char ten[Max_Str];
  f=fopen(tenFile,"r");
  if(f==NULL)
    {
      printf("File open failed\n");
      exit(1);
    }
  fscanf(f,"%d\n",&n);
  for(i=0;i<n;i++)
    {
      fscanf(f,"%s %d\n",ten,&id);
      addVertex(g,id,ten);
    }
  return n;
}

int docFileConnect(char* tenFile, Graph g)
{
  FILE *f;
  int n;
  int i;
  int id;
  int idKhac;
  char ten[Max_Str];
  char str[Max_Str];
  const char s[2] = " ";
  char *token;
  f=fopen(tenFile,"r");
  if(f==NULL)
    {
      printf("File open failed\n");
      exit(1);
    }
  fscanf(f,"%d\n",&n);
  for(i=0;i<n;i++)
    {
      fscanf(f,"%d ",&id);
      fgets(str,100,f);
      str[strlen(str)-1]=0;
   
   /* lay token dau tien */
   token = strtok(str, s);
   
   /* duyet qua cac token con lai */
   while( token != NULL ) 
   {
     idKhac= atoi(token);
     addEdge(g,id,idKhac,1.0);
     token = strtok(NULL, s);
   }
    }
  return n;
}

void khoiTao(Graph g, vtx* gVtx)
{
  JRB node;
  int output[100];
  int id,out,in;
  vtx v;
  int i=0;
  jrb_traverse(node,g.vertices)
    {
      id=jval_i(node->key);
      out=outdegree(g,id,output);
      in=indegree(g,id,output);
      
      v.trongSo=1;
      v.out=out;
      v.in=in;
      v.id=id;
      gVtx[i]=v;
      i++;
    }
}

int layHeSoVtxTuId(int id, vtx* gVtx, int soDinh)
{
  for(int i=0;i<soDinh;i++)
    {
      if(gVtx[i].id==id)
	return i;
    }
  return -1;
}

void pageRank(Graph g, vtx* gVtx,int soDinh)
{
  int output[100];
  vtx gVtxMoi[100];
  int n;
  int i,j;
  int k;
  int id,id2;
  for(i=0;i<soDinh;i++)
    {
      gVtxMoi[i]=gVtx[i];
    }
  for(i=0;i<soDinh;i++)
    {
      id=gVtxMoi[i].id;
      gVtxMoi[i].trongSo=0;
      n=indegree(g,id,output);
      for(j=0;j<n;j++)
	{
	  id2=output[j];
	  k=layHeSoVtxTuId(id2,gVtxMoi,soDinh);
	  gVtxMoi[i].trongSo=gVtxMoi[i].trongSo+ (double) gVtx[k].trongSo/ gVtx[k].out;
	}
    }

    for(i=0;i<soDinh;i++)
    {
      gVtx[i]=gVtxMoi[i];
    }
}

int main()
{

  Graph g=createGraph(DIRECT_GRAPH);
  vtx gVtx[100];
  JRB node;
  int thoat,luaChon;
  int soDinh;
  int max=0;
  int min=10000;
  int i;
  int m;
  thoat=0;
  do
    {
      luaChon= menuVaLuaChon(1,4);
      switch(luaChon)
	{
	case 1:
	  //ham1
	  soDinh=docFileWeb("webpages.txt",g);
	  docFileConnect("pageConnections.txt",g);

	  printf("So luong webpages: %d\n",soDinh);
	  jrb_traverse(node,g.vertices)
	    {
	      printf("%d %s\n",jval_i(node->key),jval_s(node->val));
	    }
	  printf("\n\n");
	  
	  khoiTao(g,gVtx);
	  for(i=0;i<soDinh;i++)
	    {
	      if(gVtx[i].in>max)
		max=gVtx[i].in;
	      
	      if(gVtx[i].in<min)
		min=gVtx[i].in;
	    }
	  
	  printf("Max= %d\n",max);
	  for(i=0;i<soDinh;i++)
	    {
	      if(gVtx[i].in==max)
		printf("%d %s\n",gVtx[i].id,getVertex(g,gVtx[i].id));	      
	    }

	  
	   printf("Min= %d\n",min);	   
	  for(i=0;i<soDinh;i++)
	    {
	      if(gVtx[i].in==min)
		printf("%d %s\n",gVtx[i].id,getVertex(g,gVtx[i].id));	      
	    }
	  
	    break;
	case 2:
	  //ham2
	  pageRank(g,gVtx,soDinh);
	  for(i=0;i<soDinh;i++)
	    {
	      printf("%d %f\n",gVtx[i].id,gVtx[i].trongSo);
	    }
	  break;
	case 3:
	  //ham3
	  printf("Nhap vao m:");
	  scanf("%d",&m);
	  for(i=0;i<m;i++)
	    pageRank(g,gVtx,soDinh);
	  for(i=0;i<soDinh;i++)
	    {
	      printf("%d %f\n",gVtx[i].id,gVtx[i].trongSo);
	    }
	  break;
	case 4:
	  thoat=1;
	  break;
	}	
    }while(thoat!=1);
}
