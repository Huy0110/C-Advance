#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "jrb.h"
#include "dllist.h"
#include "jval.h"
#include "queue.h"
#include <stdio_ext.h>


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
void edgesDijkstra(Graph g, int diemdau, int diemcuoi, int sodinh)
{
    int *d = (int *)malloc(sodinh * sizeof(int)); //khoang cach tu diem dau
    for(int i = 0; i < sodinh; i++)
    {
        d[i] = 1000;
    }
    d[diemdau] = 0;
    int *diemtruoc = (int *)malloc(sodinh * sizeof(int));
    int *check = (int *)calloc(sodinh, sizeof(int)); //visited
    Queue q;
    makeNullQueue(&q);
    enQueue(&q, diemdau);
    int u;
    int n;
    int output[1000];
    JRB tmp, tmp1, tmp2;
    while(!isEmpty(q))
    {
        deQueue(&q, &u);
        n = outdegree(g, u, output);
        tmp = jrb_find_int(g.edges, u);
        if(check[u] == 0)
        {
            for(int i = 0; i < n; i++)
            {
                enQueue(&q, output[i]);
                tmp1 = (JRB)tmp->val.v;
                tmp2 = jrb_find_int(tmp1, output[i]);
                if(d[output[i]] == 1000)
                {
                    d[output[i]] = tmp2->val.i + d[u];
                    diemtruoc[output[i]] = u;
                }
                else if(tmp2->val.i + d[u] < d[output[i]])
                {
                    d[output[i]] = tmp2->val.i + d[u];
                    diemtruoc[output[i]] = u;
                }

            }
        }
        check[u] = 1;
    }
    int arr[100];
    int x = 0;
    int m = diemcuoi;
    while(m != diemdau)
    {
        arr[x++] = m;
        m = diemtruoc[m];
    }
    arr[x++] = diemdau;
    for(int i = x - 1; i >= 0; i--)
    {
        printf("%s ", getVertex(g, arr[i]));
    }
//inDuong(g, diemtruoc, diemdau, diemcuoi);
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
      addVertex(g,ip,strdup(ten));
    }
  fclose(f);
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
       strncpy(str,&str[1],strlen(str)-1);
       str[strlen(str)-1]=0;
      const char s[2] = " ";
      char *token;
      if(strlen(str)==4)
	addEdge(g,ip,atoi(str),1.0);
      else
	{
      token = strtok(str, s);
      while( token != NULL )
	{
	  printf("%s\n",token);
	  ipKhac=atoi(token);
	  printf("%d %d\n",ip,ipKhac);
	  addEdge(g,ip,ipKhac,1.0);
	  token = strtok(NULL, s);
	}
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


int menuVaLuaChon(int a,int b)
{
  int luaChon,buff;
  char c;
  printf("+-----+--------------------------------------------------+\n");
  printf("|%25s%6s%25s|\n","","MENU","");
  printf("+-----+--------------------------------------------------+\n");
  printf("|%5d|%-50s|\n",1,"case1 ");
  printf("|%5d|%-50s|\n",2,"case2 ");
  printf("|%5d|%-50s|\n",3,"case3 ");
  printf("|%5d|%-50s|\n",4,"case4 ");
  printf("|%5d|%-50s|\n",5,"case5 ");
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

int main()
{

  int thoat,luaChon;
  thoat=0;
    int i, length, path[100], s, t;
    JRB ptr;
    double w;
    int soIP;
    int count;
    Graph g = createGraph();
    vtx v[Max_Vtx];
        int max=0;
    int ipMax;
    int ipMin;
    int min=100;
    vtx vMoi[Max_Vtx];
    vtx tmp;
    int soCanLap;
      int chiDen=0;
    int chiDi=0;
      int id1,id2;
    double doDai;
    int path2[100];
    int length2;
    JRB node,node2;
    JRB tree;

  do
    {
      luaChon= menuVaLuaChon(1,6);
      switch(luaChon)
	{
	case 1:
	  //ham1
	   soIP=docFileWeb("webpages.txt",g);
	   docFileConnect("pageConnections.txt",g);
	   count= capNhatVtx(g,v);

    jrb_traverse(ptr,g.vertices)
      printf("%s %d\n",jval_s(ptr->val),jval_i(ptr->key));
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
    printf("\n\n");
    	  	printf("Vertiecs:\n");
	jrb_traverse(node,g.vertices)
	  printf("%d %s\n",jval_i(node->key),jval_s(node->val));
	printf("Edges:\n");
	jrb_traverse(node,g.edges)
	  {
	    printf("%d with ",jval_i(node->key));
	    tree=(JRB)jval_s(node->val);
	    jrb_traverse(node2,tree)
	      printf("%d(val= %f)   ",jval_i(node2->key),jval_d(node2->val));
	    printf("\n");
	  }

	    break;
	case 2:
	  //ham2
	  
    capNhatLanLap(g,count,v,vMoi);
    Gan(v,count,vMoi);
    for(i=0;i<count;i++)
      printf("%d %f\n",v[i].ip,v[i].trongSo);
    
    printf("\n");
    for( i=0;i<count;i++)
      for(int j=i;j<count;j++)
	if(vMoi[i].trongSo<vMoi[j].trongSo)
	  {
	    tmp=vMoi[i];
	    vMoi[i]=vMoi[j];
	    vMoi[j]=tmp;
	  }
    printf("\n");
    printf("Page quan trong nhat:\n");
    printf("%s %f\n",jval_s(jrb_find_int(g.vertices,vMoi[0].ip)->val),vMoi[0].trongSo);
    printf("Page it quan trong nhat:\n");
    printf("%s %f\n",jval_s(jrb_find_int(g.vertices,vMoi[count-1].ip)->val),vMoi[count-1].trongSo);
    
	  break;
	case 3:
	  //ham3
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
    printf("%s %f\n%s %f\n%s %f\n",jval_s(jrb_find_int(g.vertices,vMoi[0].ip)->val),vMoi[0].trongSo,jval_s(jrb_find_int(g.vertices,vMoi[1].ip)->val),vMoi[1].trongSo,jval_s(jrb_find_int(g.vertices,vMoi[2].ip)->val),vMoi[2].trongSo);

	  break;
	case 4:
	  //ham3
	  printf("\n");
    for(i=0;i<count;i++)
      {
	if(v[i].out==0 && v[i].in!=0)
	  chiDen++;
	if(v[i].in==0 && v[i].out!=0)
	  chiDi++;
      }
    printf("So luong page chi co out ma khong co in: %d\n",chiDi);
    printf("So luong page chi co in ma khong co out: %d\n",chiDen);

	  break;
	case 5:
	  //ham3
	  // JRB tree;
      	printf("Vertiecs:\n");
	jrb_traverse(node,g.vertices)
	  printf("%d %s\n",jval_i(node->key),jval_s(node->val));
	printf("Edges:\n");
	jrb_traverse(node,g.edges)
	  {
	    printf("%d with ",jval_i(node->key));
	    tree=(JRB)jval_s(node->val);
	    jrb_traverse(node2,tree)
	      printf("%d ( val=%f)   ",jval_i(node2->key),jval_d(node2->val));
	    printf("\n");
	  }
	    
    printf("Nhap vao id1:");
    scanf("%d",&id1);
    printf("Nhap vao id2:");
    scanf("%d",&id2);
    __fpurge(stdin);
    if(jrb_find_int(g.vertices,id1)==NULL||jrb_find_int(g.vertices,id2)==NULL)
      printf("Khong ton tai 1 trong 2 id nay\n");
    else
      {
	printf("%d %dxxx\n",id1,id2);
	edgesDijkstra(g,id1,id2,6);

	  }
	
	/*
      	doDai= shortestPath(g,id1,id2,path2,&length2);
	printf("Here\n");
	if(doDai==INFINITIVE_VALUE)
	  printf("Khong ton tai duong di\n");
	else
	  printf("Do dai duong di: %f\n", doDai);
	*/
  
	  break;
	case 6:
	  thoat=1;
	  dropGraph(g);
	  break;
	}	
    }while(thoat!=1);
}
