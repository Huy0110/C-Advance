#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "jrb.h"
#include "dllist.h"
#include "jval.h"

#define INFINITIVE_VALUE  10000000
#define Max_Vtx 50
#define Max_Str 1000
typedef struct {
   JRB edges;
   JRB vertices;
} Graph;

Graph createGraph()
{
   Graph g; 
   g.edges = make_jrb();  
   g.vertices = make_jrb();  
   return g;
}

typedef struct
{
  int ip;
  int out;
  int in;
  float trongSo;
}vtx;

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


void addEdge(Graph graph, int v1, int v2, double weight)
{
     JRB node, tree;
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
        jrb_insert_int(tree, v2, new_jval_d(weight));
     }
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

double shortestPath(Graph g, int s, int t, int* path, int*length)
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
   
   if (total != INFINITIVE_VALUE)
   {
       tmp[0] = t;
       n = 1;              
       while (t != s)
       {
             t = previous[t];
             tmp[n++] = t;
       }
       for (i=n-1; i>=0; i--)
	 path[n-i-1] = tmp[i]; //mang path la dao lon cua mang t
       *length = n;                
   }
   return total;   
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

int timIDtuIP(vtx* v,int n, int ip)
{
  int i;
  for(i=0;i<n;i++)
    if(v[i].ip==ip)
      return i;
  return -1;
}

int docFileWeb(char *tenFileWeb, Graph g)
{
  FILE *f;
  char ten[Max_Str];
  int ip;
  int n;
  f=fopen(tenFileWeb,"r");
  if(f==NULL)
    {
      printf("File open failed\n");
      exit(1);
    }
  fscanf(f,"%d\n",&n);
  char str[Max_Str];
  for(int i=0;i<n;i++)
    {
      fscanf(f,"%s %d\n",ten,&ip);
      strcat(ten," ");
      ten[strlen(ten)-1]=0;
      addVertex(g,ip,ten);
    }
  return n;
}

int docFileConnect(char *tenFile, Graph g)
{
  FILE *f;
  int ip,ipKhac;
  char str[Max_Str];
  f=fopen(tenFile,"r");
  if(f==NULL)
    {
      printf("File open failed\n");
      exit(1);
    }
  int n;
  fscanf(f,"%d\n",&n);
  for(int i=0;i<n;i++)
    {
      fscanf(f,"%d",&ip);
      fgets(str,Max_Str,f);
      str[strlen(str)-1]=0;
      strcpy(str,&str[1]);
      const char s[2] = " ";
      char *token;
      token = strtok(str, s);
      while( token != NULL )
	{
	  ipKhac=atoi(token);
	  addEdge(g,ip,ipKhac,1);
	  token = strtok(NULL, s);
	}
    }
  return n;
}

int capNhatVtx(Graph g,vtx* v)
{
  JRB ptr;
  int count=0;
  int output[100];
  jrb_traverse(ptr,g.vertices)
    {
      v[count].ip=jval_i(ptr->key);
      v[count].out=outdegree(g,jval_i(ptr->key),output);
      v[count].in= indegree(g,jval_i(ptr->key),output);
      v[count].trongSo=1.0;
      count++;
    }
  return count;
}

void capNhatLanLap(Graph g,int n, vtx* v, vtx* vMoi)
{
  int input[100];
  int k;
  int i;
  for(i=0;i<n;i++)
    {
      vMoi[i].ip=v[i].ip;
      vMoi[i].out=v[i].out;
      vMoi[i].in=v[i].in;
      vMoi[i].trongSo=0;
      k=indegree(g,v[i].ip,input);
      for(int j=0;j<k;j++)
	{
	  vMoi[i].trongSo=vMoi[i].trongSo+ v[timIDtuIP(v,n,input[j])].trongSo / v[timIDtuIP(v,n,input[j])].out;
	}
    }
}

void Gan(vtx* v, int n, vtx* vMoi)
{
  for(int i=0;i<n;i++)
    v[i]=vMoi[i];
}

int main()
{
    int i, length, path[100], s, t;
    JRB ptr;
    double w;
    int soIP;
    int count;
    Graph g = createGraph();
    vtx v[Max_Vtx];
    soIP=docFileWeb("webpages.txt",g);
    docFileConnect("pageConnections.txt",g);
    count= capNhatVtx(g,v);

    jrb_traverse(ptr,g.vertices)
      printf("%s %d\n",jval_s(ptr->val),jval_i(ptr->key));

    int max=0;
    int ipMax;
    int ipMin;
    int min=100;
    //case
    for(i=0;i<count;i++)
      {
	if(max<v[i].in)
	  {
	    max=v[i].in;
	    ipMax=i;
	  }
	
	if(min>v[i].in)
	  {
	    min=v[i].in;
	    ipMin=i;
	  }
      }
    printf("\n");
    printf("Web co nhieu lien ket toi nhat: %s voi %d lien ket\n",jval_s(jrb_find_int(g.vertices,v[ipMax].ip)->val),max);
    
    printf("Web co it lien ket toi nhat: %s voi %d lien ket\n",jval_s(jrb_find_int(g.vertices,v[ipMin].ip)->val),min);
    printf("\n");

    //case

    vtx vMoi[Max_Vtx];
    capNhatLanLap(g,count,v,vMoi);
    Gan(v,count,vMoi);
    for(i=0;i<count;i++)
      printf("%d %f\n",v[i].ip,v[i].trongSo);
    
    printf("\n");

    //case
    int soCanLap;
    do
      {
	printf("Nhap vao so m:");
	scanf("%d",&soCanLap);
	if(soCanLap<10||soCanLap>50)
	  printf("Yeu cau >=10 va <=50");
	else
	  break;
      }while(1);
    for(i=0;i<soCanLap;i++)
      {
	capNhatLanLap(g,count,v,vMoi);
	Gan(v,count,vMoi);
      }
    for(i=0;i<count;i++)
      printf("%d %f\n",v[i].ip,v[i].trongSo);

    vtx tmp;
    for( i=0;i<count;i++)
      for(int j=i;j<count;j++)
	if(vMoi[i].trongSo<vMoi[j].trongSo)
	  {
	    tmp=vMoi[i];
	    vMoi[i]=vMoi[j];
	    vMoi[j]=vMoi[i];
	  }
    printf("\n");
    printf("Top 3 page quan trong nhat:\n");
    printf("%s %f\n%s %f\n%s %f\n",jval_s(jrb_find_int(g.vertices,vMoi[0].ip)->val),v[0].trongSo,jval_s(jrb_find_int(g.vertices,vMoi[1].ip)->val),v[1].trongSo,jval_s(jrb_find_int(g.vertices,vMoi[2].ip)->val),v[2].trongSo);

    //case
    printf("\n");
    int chiDen=0;
    int chiDi=0;
    for(i=0;i<count;i++)
      {
	if(v[i].out==0 && v[i].in!=0)
	  chiDen++;
	if(v[i].in==0 && v[i].out!=0)
	  chiDi++;
      }
    printf("So luong page chi co out ma khong co in: %d\n",chiDi);
    printf("So luong page chi co in ma khong co out: %d\n",chiDen);

    //case
    int id1,id2;
    double doDai;
    int path2=0;
    int length2=0;
    printf("Nhap vao id1:");
    scanf("%d",&id1);
    printf("Nhap vao id2:");
    scanf("%d",&id2);
    if(jrb_find_int(g.vertices,id1)==NULL||jrb_find_int(g.vertices,id2)==NULL)
      printf("Khong ton tai 1 trong 2 id nay\n");
    else
      {
	doDai= shortestPath(g,id1,id2,&path2,&length2);
	if(doDai==INFINITIVE_VALUE)
	  printf("Khong ton tai duong di\n");
	else
	  printf("Do dai duong di: %f\n", doDai);
      }
      
    
}
