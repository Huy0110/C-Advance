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


void docFile(Graph g, char* tenFile)
{
  FILE* f;
  char str[100];
  int id;
  int n,m;
  int id1,id2;
  double w;
  f=fopen(tenFile,"r");
  if(f==NULL)
    {
      printf("File opened failed\n");
      return;
    }
  fscanf(f,"%d %d\n",&n,&m);
  for(int i=0;i<n;i++)
    {
      fscanf(f,"%d %s\n",&id,str);
      addVertex(g,id,str);
    }
  for(int i=0;i<m;i++)
    {
      fscanf(f,"%d %d %lf\n",&id1,&id2,&w);
      addEdge(g,id1,id2,w);
    }
}


int main()
{
  Graph g = createGraph(UNDIRECT_GRAPH);
  JRB node;
  JRB node1, node2;
  JRB tree1, tree2;
  docFile(g,"data.txt");
  int thoat,luaChon;
  thoat=0;
  int id;
  JRB tree;
  double max;
  int id1, id2;
  int length, path[100], s, t,w;
  int banBeChung[100];
  int count=0;
  do
    {
      luaChon= menuVaLuaChon(1,5);
      switch(luaChon)
	{
	case 1:
	  //ham1
	  jrb_traverse(node, g.vertices)
	    {
	      printf("%d %s\n",jval_i(node->key),jval_s(node->val));
	    }
	    break;
	case 2:
	  //ham2
	  printf("Nhap vao id:");
	  scanf("%d",&id);
	  node=jrb_find_int(g.edges,id);
	  if(node==NULL)
	    {
	      printf("Khong ton tai id nay\n");
	      break;
	    }
	  tree = (JRB) jval_v(node->val);
	  jrb_traverse(node,tree)
	    {
	      if(max<jval_d(node->val))
		max=jval_d(node->val);
	    }
	  printf("%f\n",max);
	  jrb_traverse(node,tree)
	    {
	      if(max==jval_d(node->val))
		{
		  id=jval_i(node->key);
		  printf("%d %s\n",id,getVertex(g,id));
		}
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
	      printf("1\n");
	      break;
	    }
	  printf("-1\n");
	  break;
	case 4:
	   printf("Nhap vao id1:");
	  scanf("%d",&id1);
	  printf("Nhap vao id2:");
	  scanf("%d",&id2);
	  node1=jrb_find_int(g.vertices,id1);
	  node2= jrb_find_int(g.vertices,id2);
	  if(node==NULL||node2==NULL||node==node2)
	    {
	      printf("Khong ton tai hoac 2 dinh trung nhau\n");
	      break;
	    }
	   node1=jrb_find_int(g.edges,id1);
	  node2= jrb_find_int(g.edges,id2);
	  tree1 = (JRB) jval_v(node1->val);
	  tree2 = (JRB) jval_v(node2->val);
	  jrb_traverse(node1, tree1)
	    {
	      id=jval_i(node1->key);
	      jrb_traverse(node2,tree2)
		{
		  if(id==jval_i(node2->key))
		    {
		      banBeChung[count]=id;
		      count++;
		    }
		}
	    }
	  printf("%d\n",count);
	  for(int j=0 ; j <count; j++)
	    printf("%d %s\n",banBeChung[j],getVertex(g,banBeChung[j]));
	  break;
	case 5:
	  thoat=1;
	  break;
	}	
    }while(thoat!=1);
}
