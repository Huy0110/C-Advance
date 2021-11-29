#include "cgraphv1.h"
#define Max_String 100


int layID(char* str)
{
  return atoi(&str[1]);
}

int docFile(char *tenFile, Graph g)
{
  FILE *f;
  char str[Max_String];
  char ten[Max_String];
  char ten2[Max_String];
  int soDinh=0;
  char *token, *token2;
  //note
  char s[2]=" ";
  char s2[2]= "-";
  //note
  int i;
  int j;
  f=fopen(tenFile,"r");
  if(f==NULL)
    {
      printf("File open failed\n");
      exit(1);
    }
  while(!feof(f))
    {
      fgets(str,Max_String,f);
      if(strlen(str)==1) break;
      //  printf("%s %ld\n",str,strlen(str));
      if(strlen(str)==0) break;
      str[strlen(str)-1]=0;
      soDinh++;
      for(i=0;i<strlen(str);i++)
	if(str[i]==':') break;  //note
      strncpy(ten,str,i);
      ten[i]=0;
      strcpy(str,&str[i+1]);
      if(ten[0]!='A') break; //note: co A
      addVertex(g,layID(ten),strdup(ten));
      if(strlen(str)==0)
	continue;
      
       token = strtok(str, s);
       while( token != NULL )
	 {
	   for(j=0;j<strlen(token);j++)
	     if(token[j]=='-')  //note
	       break;
	   strncpy(ten2,token,j);
	   ten2[j]=0;
	   strcpy(token,&token[j+1]);
	   addEdge(g,layID(ten2),layID(ten),atoi(token)); //ten2 cho den ten
	   token = strtok(NULL, s);
	 }
    }
   
  return soDinh-1;
  
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
  docFile("dataK55.txt",g);
  printGraph(g);
}
