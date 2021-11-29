#include <stdio.h>
#include <stdio_ext.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "cgraphv1.h"
#define Max_Str 100

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

void docFile(char *tenFile, Graph g,int* soDinh,int* soCanh)
{
  int n,m;
  int i,j;
  int id;
  int id1,id2;
  double trongSo;
  char ten[Max_Str];
  FILE *f;
  f=fopen(tenFile,"r");
  if(f==NULL)
    {
      printf("File open failed!\n");
      return;
    }
  fscanf(f,"%d %d\n",&n,&m);
  for(i=0;i<n;i++)
    {
      fscanf(f,"%d %s\n",&id,ten);
      addVertex(g,id,ten);
    }
  for(i=0;i<m;i++)
    {
      fscanf(f,"%d %d %lf\n",&id1,&id2,&trongSo);
      addEdge(g,id1,id2,trongSo);
    }
  *soDinh=n;
  *soCanh=m;
}

int main()
{
  Graph g=createGraph(UNDIRECT_GRAPH);
  int soCanh,soDinh;
  docFile("data.txt",g,&soDinh,&soCanh);
  int thoat,luaChon;
  JRB node,node2;
  JRB node3,node4;
  int id;
  JRB tree,tree2;
  double max=0;
  int id1,id2;
  char* ten[Max_Str];
  int length, path[100], s, t,w;
  int count=0;
  int banBeChung[100];
  thoat=0;
  do
    {
      luaChon= menuVaLuaChon(1,5);
      switch(luaChon)
	{
	case 1:
	  //ham1
	  docFile("data.txt",g,&soDinh,&soCanh);
	  jrb_traverse(node,g.vertices)
	    {
	      printf("%d %s\n",jval_i(node->key),jval_s(node->val));
	    }
	    break;
	case 2:
	  //ham2
	  printf("Nhap vao id:");
	  scanf("%d",&id);
	  node=jrb_find_int(g.edges,id);
	  tree=(JRB) jval_v(node->val);
	  jrb_traverse(node,tree)
	    {
	      if(max<jval_d(node->val))
		max=jval_d(node->val);
	    }
	  printf("Max: %.1f\n",max);
	  jrb_traverse(node,tree)
	    {
	      if(max==jval_d(node->val))
		printf("%d %s\n",jval_i(node->key),getVertex(g,jval_i(node->key)));
	    }
	  break;
	case 3:
	  //ham3
      	  printf("Nhap vao id1:");
	  scanf("%d",&s);
	  printf("Nhap vao id2:");
	  scanf("%d",&t);
	  node=jrb_find_int(g.edges,s);
	  node2= jrb_find_int(g.edges,t);
	  if(node==NULL||node2==NULL||node==node2)
	    {
	      printf("-1\n");
	      break;
	    }
	  //  w = shortestPath(g, s, t, path, &length);
	  w=Dijkstra(g,s,t,path,&length);
	  if(w!=INFINITIVE_VALUE && hasEdge(g,s,t)==0)
	    {
	      // printf("%d %d %f\n",s,t,getEdgeValue(g,s,t));
	      printf("1\n");
	      break;
	    }
	  printf("-1\n");
	  break;
	case 4:
	  //ham4
	   printf("Nhap vao id1:");
	  scanf("%d",&id1);
	  printf("Nhap vao id2:");
	  scanf("%d",&id2);
	  node=jrb_find_int(g.vertices,id1);
	  node2= jrb_find_int(g.vertices,id2);
	  if(node==NULL||node2==NULL||node==node2)
	    {
	      printf("Khong ton tai hoac 2 dinh trung nhau\n");
	      break;
	    }

	  node=jrb_find_int(g.edges,id1);
	  node2= jrb_find_int(g.edges,id2);
	  tree=(JRB) jval_v(node->val);
	  tree2=(JRB) jval_v(node2->val);
	  jrb_traverse(node3,tree)
	    {
	      id=jval_i(node3->key);
	      jrb_traverse(node4,tree2)
		{
		  if(id==jval_i(node4->key))
		    {
		      banBeChung[count]=id;
		      count++;
		      // printf("%d %s\n",id,getVertex(g,id));
		    }
		}
	    }
	  printf("%d\n",count);
	  for(int i=0;i<count;i++)
	    printf("%d %s\n",banBeChung[i],getVertex(g,banBeChung[i]));
	  break;
	case 5:
	  dropGraph(g);
	  thoat=1;
	  break;
	}	
    }while(thoat!=1);
}
