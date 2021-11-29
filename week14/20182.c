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
  printf("|%5d|%-50s|\n",2,"Y nghia");
  printf("|%5d|%-50s|\n",3,"Y nghia");
  printf("|%5d|%-50s|\n",4,"Y nghia");
  printf("|%5d|%-50s|\n",5,"Y nghia");
  printf("|%5d|%-50s|\n",6,"Thoat");
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

void docFile(char *tenFile,Graph g,Graph gDiBo, int* soDinh,int* soCanh)
{
  int id1,id2;
  double doDai;
  int n,m;
  int i;
  FILE *f=fopen(tenFile,"r");
  if(f==NULL)
    {
      printf("File open failed!\n");
      return;
    }
  fscanf(f,"%d %d\n",&n,&m);
  for(i=0;i<m;i++)
    {
      fscanf(f,"%d %d %lf\n",&id1,&id2,&doDai);
      addVertex(g,id1,"1");
      addVertex(g,id2,"1");
      addEdge(g,id1,id2,doDai);
      if(doDai>=50)
	{
	  addVertex(gDiBo,id1,"1");
	  addVertex(gDiBo,id2,"1");
	  addEdge(gDiBo,id1,id2,doDai);
	}
    }
  *soDinh=n;
  *soCanh=m;
}

int chiCoTheDiBo(Graph g, int id)
{
  JRB node,node2;
  JRB tree;
  node= jrb_find_int(g.edges,id);
  tree= (JRB) jval_v(node->val);
  jrb_traverse(node,tree)
    if(jval_d(node->val)<50)
      {
	//	printf("id %d loi o %d voi %lf\n",id,jval_i(node->key),jval_d(node->val));
	return 0;
      }
  return 1;
}


int main()
{

  int thoat,luaChon;
  thoat=0;
  Graph g=createGraph(UNDIRECT_GRAPH);
  Graph gDiBo=createGraph(UNDIRECT_GRAPH);
  int soDinh,soCanh;
  JRB node, node2, node3;
  JRB node4;
  int id1,id2;
  JRB tree;
  int max=0;
  int output[100];
  int n;
  int s,t;
  double w;
  int length;
  int path[100];
  int path2[100];
  int length2;
  int i;
  docFile("dothi.txt",g,gDiBo,&soDinh,&soCanh);
  do
    {
      luaChon= menuVaLuaChon(1,6);
      switch(luaChon)
	{
	case 1:
	  //ham1
	   docFile("dothi.txt",g,gDiBo,&soDinh,&soCanh);
	   printf("     ");
	   jrb_traverse(node3,g.edges)
	     printf("id %-2d",jval_i(node3->key));
	   printf("\n");
	   jrb_traverse(node,g.edges)
	     {
	        printf("id %-2d",jval_i(node->key));
	       id1=jval_i(node->key);
	       tree=(JRB) jval_v(node->val);
	       jrb_traverse(node2,g.edges)
		 {
		   id2=jval_i(node2->key);
		   if(id1==id2)
		      printf("%-5d",1);
		   else
		     {
		   if(hasEdge(g,id1,id2))
		     printf("%-5d",1);
		   else
		     printf("%-5d",0);
		     }
		 }
	       printf("\n");
	     }
	    break;
	case 2:
	  //ham2
	  jrb_first(g.edges);
	  jrb_traverse(node,g.edges)
	    {
	      id1=jval_i(node->key);
	      printf("Castle %d:",id1);
	       tree=(JRB) jval_v(node->val);
	       jrb_traverse(node2,tree)
		 {
		   printf("%-5d",jval_i(node2->key));
		 }
	       printf("\n");
	    }
	  break;
	case 3:
	  //ham3
	  jrb_first(g.edges);
	  printf("Chi co di bo: ");
	  jrb_traverse(node4,g.edges)
	    {
	      id1=jval_i(node4->key);
	      //printf("%d\n",jval_i(node4->key));
	      if(chiCoTheDiBo(g,id1))
		printf("%-5d",id1);
	    }
	  printf("\n");


	  jrb_first(g.edges);
	   jrb_traverse(node,g.edges)
	     {
	       id1=jval_i(node->key);
	       n=indegree(g,id1,output);
	       if(n>max)
		 max=n;
	     }
	   printf("So duong den max: ");
	   jrb_first(g.edges);
	   jrb_traverse(node,g.edges)
	     {
	       id1=jval_i(node->key);
	       n=indegree(g,id1,output);
	       if(n==max)
		 printf("%-5d",id1);
	     }
	     printf("  voi indegree= %d\n",max);
	  
	  break;
	case 4:
	  //ham4
	  printf("Nhap s:");
	  scanf("%d",&s);
	  printf("Nhap t:");
	  scanf("%d",&t);
	  if(jrb_find_int(g.edges,s)==NULL||jrb_find_int(g.edges,t)==NULL)
	    {
	      printf("id khong hop le\n");
	      break;
	    }
	  w = Dijkstra(g, s, t, path, &length);
	  if (w == INFINITIVE_VALUE)
	    {
	      printf("No path from %d to %d\n", s, t);
	    }
	  else
	    {
	      printf("Path from %d to %d (with total distance %f)\n", s, t, w);
         for (i=0; i<length; i++)
             printf(" => %d", path[i]);
	    }
	  printf("\n");
	  break;
	case 5:
	  //ham5
	   printf("Nhap s:");
	  scanf("%d",&s);
	  printf("Nhap t:");
	  scanf("%d",&t);
	  if(jrb_find_int(gDiBo.edges,s)==NULL||jrb_find_int(gDiBo.edges,t)==NULL)
	    {
	      printf("id khong hop le hoac id nay khong co duong di bo\n");
	      break;
	    }
	  w = Dijkstra(gDiBo, s, t, path2, &length2);
	  if (w == INFINITIVE_VALUE)
	    {
	      printf("No path from %d to %d\n", s, t);
	    }
	  else
	    {
	      printf("Path from %d to %d (with total distance %f)\n", s, t, w);
         for (i=0; i<length2; i++)
             printf(" => %d", path2[i]);
	    }
	  printf("\n");
	  break;
	case 6:
	  thoat=1;
	  dropGraph(g);
	  dropGraph(gDiBo);
	  break;
	}	
    }while(thoat!=1);
}
