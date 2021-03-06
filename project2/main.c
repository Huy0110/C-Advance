#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "week4/jrb.h"
#include "week4/dllist.h"
#define Max_String 1000
#define Max_Ga 1000
#define Max_Tuyen 1000

#define INFINITIVE_VALUE  10000000

typedef struct {
   JRB edges;
   JRB vertices;
} Graph;

typedef struct
{
  int id;
  char tenGa[Max_String];
}Ga;
Ga ga[Max_Ga];
int idTuyen[Max_Tuyen]={};
int tongSoGa=0;

Graph createGraph()
{
   Graph g; 
   g.edges = make_jrb();  
   g.vertices = make_jrb();  
   return g;
}

void addVertex(Graph g, int id, char* name)
{
     JRB node = jrb_find_int(g.vertices, id);
     if (node==NULL) // only add new vertex 
         jrb_insert_int(g.vertices, id, new_jval_s(strdup(name)));            
}

char *getVertex(Graph g, int id)
{
     JRB node = jrb_find_int(g.vertices, id);
     if (node==NULL) 
        return NULL;
     else                
        return jval_s(node->val);
}     

void addEdge(Graph graph, int v1, int v2, double weight,int idTuyen)
{
  JRB node, tree,tree2;
     if (getEdgeValue(graph, v1, v2)==INFINITIVE_VALUE)
     {
        node = jrb_find_int(graph.edges, v1);
        if (node==NULL) {
           tree = make_jrb();
           jrb_insert_int(graph.edges, v1, new_jval_v(tree));
        } else
        {
           tree = (JRB) jval_v(node->val);   
        }
	tree2=make_jrb();
	jrb_insert_intt(tree2,idTuyen,new_jval_d(weight));
        jrb_insert_int(tree, v2, new_jval_v(tree2));
     }
}

double getEdgeValue(Graph graph, int v1, int v2)
{
    JRB node, tree;
    node = jrb_find_int(graph.edges, v1);
    if (node==NULL)
       return INFINITIVE_VALUE;
    tree = (JRB) jval_v(node->val);
    node = jrb_find_int(tree, v2);
    if (node==NULL)
       return INFINITIVE_VALUE;
    else
       return jval_d(node->val);       
}

int* getIDTuyenByEdge(Graph graph,int v1, int v2)
{
  JRB node, tree,tree2;
  int n=0;
  int output[Max_Tuyen];
    node = jrb_find_int(graph.edges, v1);
    if (node==NULL)
      return NULL;           ;
    tree = (JRB) jval_v(node->val);
    tree2 = jrb_find_int(tree, v2);
    if (tree2==NULL)
       return NULL;
    else
      {
	jrb_traverse(node,tree2)
	  {
	    output[n]=jval_i(node->key);
	    n++;
	  }
      }
    return output;
}


int indegree (Graph graph, int v, int* output)
{
    JRB tree, node;
    int total = 0;   
    jrb_traverse(node, graph.edges)
    {
       tree = (JRB) jval_v(node->val);
       if (jrb_find_int(tree, v))
       {
          output[total] = jval_i(node->key);
          total++;
       }                
    }
    return total;   
}

int outdegree (Graph graph, int v, int* output)
{
    JRB tree, node;
    int total;
    node = jrb_find_int(graph.edges, v);
    if (node==NULL)
       return 0;
    tree = (JRB) jval_v(node->val);
    total = 0;   
    jrb_traverse(node, tree)
    {
       output[total] = jval_i(node->key);
       total++;                
    }
    return total;   
}

double shortestPath(Graph g, int s, int t, int pathTuyen[][], int path[])
{
   double distance[1000], min, w, total;
   int previous[1000], tmp[1000];
   int n, output[100], i, u, v, start;
   Dllist ptr, queue, node;

   for (i=0; i<1000; i++)
       distance[i] = INFINITIVE_VALUE;
   distance[s] = 0;
   previous[s] = s;
       
   queue = new_dllist();
   dll_append(queue, new_jval_i(s));

   while ( !dll_empty(queue) )
   {
      // get u from the priority queue   
      min = INFINITIVE_VALUE;
      //ham tim ra trong queue diem co do dai tu s den min
      dll_traverse(ptr, queue)
      {
          u = jval_i(ptr->val);              
          if (min > distance[u])
          {
             min = distance[u];
             node = ptr;
          }                 
      }
      
      dll_delete_node(node);
      
      if (u == t) break; // stop at t
      n = outdegree(g, u, output);
      for (i=0; i<n; i++)
      {
	//xet cac lan can v cua u
          v = output[i];
          w = getEdgeValue(g, u, v);
          if ( distance[v] > distance[u] + w ) //neu s.v > s.u thi cap nhat lai
          {    
              distance[v] = distance[u] + w;
              previous[v] = u;
          }     
          dll_append(queue, new_jval_i(v));  //cho v vao trong queue
      }
   }
   
   total = distance[t]; //tim duoc khoang cach
   int tuyen[Max_Tuyen][Max_Tuyen];
   int m=0;
   if (total != INFINITIVE_VALUE)
   {
     tmp[0] = t;
       n = 1;    
       while (t != s)
       {
	 tuyen[m]=getIDTuyenByEdge(graph,prvious(t),t);
	 t = previous[t];
	 tmp[n++]=t;
         m++;
       }       
       for (i=m-1; i>=0; i--)
	 {
	   pathTuyen[m-i-1] = tuyen[i]; //mang pathTuyen la dao lon cua mang tuyen
	   path[n-i-1] = tmp[i]; //mang path la dao lon cua mang t
	 }
   }
   return total;   
}

int* chuanHoaTuyen(int tuyen[],int size)
{
  int i,j;
  int k=0;
  int out[100];
  i=0;
  for(i=0;i<size;i++)
    {
      j=i+1;
      if(tuyen[i]==tuyen[j])
	{
	  for(int m=i;m<size;m++)
	    tuyen[m]=tuyen[m+1];
	}
      else
	{
	  out[k]=i;
	  k++;
	}
    }
  return tuyen;
}



void dropGraph(Graph graph)
{
    JRB node, tree;
    jrb_traverse(node, graph.edges)
    {
        tree = (JRB) jval_v(node->val);
        jrb_free_tree(tree);
    }
    jrb_free_tree(graph.edges);
    jrb_free_tree(graph.vertices);
}
