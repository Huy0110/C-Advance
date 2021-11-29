#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>
#include  "jrb.h"
#include "jval.h"
#include "dllist.h"
#define Max_DoThi 100
#define Max_Ga 100
#define Max_String 30
#define Max_GaTrenTram 20


typedef JRB Graph;


typedef struct
{
  char kiHieu[Max_String];
  char ten[Max_String];
}Ga;


Graph createGraph()
{
  Graph graph;
  graph=make_jrb();
  return graph;
}

void addEdge(Graph graph, int v1,int v2)
{
  JRB node,tree,node2;
  node=jrb_find_int(graph,v1);
  if(node==NULL)
    {
      tree=make_jrb();
      jrb_insert_int(graph,v1,new_jval_v(tree));
      jrb_insert_int(tree,v2,new_jval_i(1));
    }
  else
    {
      tree=make_jrb();
      tree=(JRB)jval_v(node->val);
      node2=jrb_find_int(tree,v2);
      if(node2==NULL)
	{
	  jrb_insert_int(tree,v2,new_jval_i(1));
	}
      // jrb_insert_int(tree,v2,new_jval_i(1));
      node->val=new_jval_v(tree);
    }

  node=jrb_find_int(graph,v2);
  if(node==NULL)
    {
      tree=make_jrb();
      jrb_insert_int(graph,v2,new_jval_v(tree));
      jrb_insert_int(tree,v1,new_jval_i(1));
    }
  else
    {
      tree=make_jrb();
      tree=(JRB)jval_v(node->val);
      node2=jrb_find_int(tree,v1);
      if(node2==NULL)
	{
	  jrb_insert_int(tree,v1,new_jval_i(1));
	}
      jrb_insert_int(tree,v1,new_jval_i(1));
      node->val=new_jval_v(tree);
    }
  // addEdge(graph,v2,v1);
}

int getAdjacentVertices(Graph graph,int v,int *output)
{
  JRB node,tree;
  int total;
  node=jrb_find_int(graph,v);
  if(node==NULL)
    return 0;
  else
    {
      tree=(JRB) jval_v(node->val);
      total=0;
      jrb_traverse(node,tree)
	output[total++]=jval_i(node->key);
      return total;
    }
}

void dropGraph(Graph graph)
{
  JRB node;
  jrb_traverse(node, graph)
    jrb_free_tree(jval_v(node->val));
}


void BFS(Graph graph, int start, int stop, void (*func)(int,Ga[]),Ga ga[])
{
   int visited[1000] = {};
   int n, output[100], i, u, v;
   Dllist node, queue;
   
   queue = new_dllist();
   dll_append(queue, new_jval_i(start));

   while ( !dll_empty(queue) )
   {
      node = dll_first(queue);
      u = jval_i(node->val);
      dll_delete_node(node);
      if (!visited[u]) 
      {
	func(u,ga);
          visited[u] = 1;
          if ( u == stop ) return;           
          n = getAdjacentVertices(graph, u, output);
          for (i=0; i<n; i++)
          {
              v = output[i];
              if (!visited[v])
                 dll_append(queue, new_jval_i(v));
          }
      }
   }                            
}



void DFS(Graph graph, int start, int stop, void (*func)(int))
{
   int visited[1000] = {};
   int n, output[100], i, u, v;
   Dllist node, stack;
   
   stack = new_dllist();
   dll_append(stack, new_jval_i(start));

   while ( !dll_empty(stack) )
   {
      node = dll_last(stack);
      u = jval_i(node->val);
      dll_delete_node(node);
      if (!visited[u]) 
      {
          func(u);
          visited[u] = 1;
          if ( u == stop ) return;           
          n = getAdjacentVertices(graph, u, output);
          for (i=0; i<n; i++)
          {
              v = output[i];
              if (!visited[v])
                 dll_append(stack, new_jval_i(v));
          }
      }
   }                            
}

void printVertex(int v,Ga ga[])
{
  printf("%-15s",ga[v].ten);
}

Ga LayGaTuFile(char *str)
{
  Ga ga;
  const char s[2] = "=";
  char *token;
   
   //lay token dau tien
  //printf("%s\n",str);
   token = strtok(str, s);
   strcpy(ga.kiHieu,token);
   strcpy(ga.ten,strtok(NULL,s));
   // printf("%s:%s\n",ga.kiHieu,ga.ten);
   return ga;
}

int timSoGa(Ga ga[],char* kiHieu,int soGa)
{
  int i;
  for(i=0;i<soGa;i++)
    {
      if(strcmp(ga[i].kiHieu,kiHieu)==0)
	return i;
    }
  return -1;
}

int timSoGaDuaVaoTenGa(Ga ga[],char* ten,int soGa)
{
  int i;
  for(i=0;i<soGa;i++)
    {
      if(strcmp(ga[i].ten,ten)==0)
	return i;
    }
  return -1;
}


void DocFileCacTramM(Graph graph,char *str,Ga ga[],int soGa)
{
  int i;
  int n;
  int dayKiHieu[Max_GaTrenTram];
  const char s[2]= "=";
  const char ss[2]= " ";
  char *token;
  i=0;
  token=strtok(str,s);
  str=strtok(NULL,s);
  //printf("%s\n",str);
  token=strtok(str,ss);
  dayKiHieu[i]=timSoGa(ga,token,soGa);
  i++;
  while(token != NULL)
    {
      token = strtok(NULL, ss);
      if(token==NULL)
	break;
      dayKiHieu[i]=timSoGa(ga,token,soGa);
      addEdge(graph,timSoGa(ga,ga[dayKiHieu[i]].kiHieu,soGa),timSoGa(ga,ga[dayKiHieu[i-1]].kiHieu,soGa));    
      i++;
    }
}



int main()
{
  Graph graph;
  char* output[Max_Ga];
  Ga ga[Max_Ga];
  FILE *fptr;
  char str[Max_String];
  int i,n,soGa;
  char str1[Max_String],str2[Max_String];
  int soGa1,soGa2;
  i=0;

  graph=createGraph();
  
  fptr=fopen("file.txt","r");
  fgets(str,100,fptr);
  //printf("%s",str);
  
  while(1)
    {
      fgets(str,100,fptr);
      if(strcmp(str,"Line:\n") !=0)
	{
	  str[strlen(str)-1]='\0';
	  //printf("%s\n",str);
	  ga[i]=LayGaTuFile(str);
	  // printf("%d,%s:%s\n",i,ga[i].kiHieu,ga[i].ten);
	  i++;
	}
      else
	{
	  soGa=i;
	  break;
	}
    }
  while(fgets(str,100,fptr)!=NULL)
    {
      // fgets(str,100,fptr);
      str[strlen(str)-1]='\0';
      DocFileCacTramM(graph,str,ga,soGa);
    }
  
  printf("Tim duong di giua 2 nha ga:\n");
  __fpurge(stdin);
  printf("Nha ga dau:");
  gets(str1);
  __fpurge(stdin);
  printf("Nha ga cuoi:");
  gets(str2);
  __fpurge(stdin);
  if(str[strlen(str1)-1]=='\n')
    str[strlen(str)-1]='\0';
  if(str[strlen(str1)-2]=='\n')
    str[strlen(str)-1]='\0';
  // printf("%s\n",str);
  soGa1=timSoGaDuaVaoTenGa(ga,str1,soGa);
    soGa2=timSoGaDuaVaoTenGa(ga,str2,soGa);


    BFS(graph,soGa1,soGa2,printVertex,ga);
    printf("\n");
}
