#include "cgraphv1.h"
#define Max_String 100


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

void printGraph(Graph g)
{
  JRB node,node2;
  JRB tree;
  printf("Vertieces:\n");
  jrb_traverse(node,g.vertices)
    printf("%d %s\n",jval_i(node->key),jval_s(node->val));
  printf("Edges:\n");
  jrb_traverse(node,g.edges)
    {
      tree=jval_v(node->val);
      printf("%d with ",jval_i(node->key));
      jrb_traverse(node2,tree)
	printf("%d(val = %lf)   ",jval_i(node2->key),jval_d(node2->val));
      printf("\n");
    }

}

int main()
{
  Graph g=createGraph(DIRECT_GRAPH);
  JRB anhXa= make_jrb();
  int soIP,soCanh;
  docFileConnect(g,"pageConnections.txt");
  printGraph(g);
}
