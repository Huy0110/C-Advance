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

int docFileSanPham(Graph gSanPham, char* tenFile)
{
  int n;
  FILE* f;
  int id;
  char str[100];
  f=fopen(tenFile,"r");
  if(f==NULL)
    {
      printf("File open failed\n");
      exit(1);
    }
  fscanf(f,"%d\n",&n);
  for(int i=0;i<n;i++)
    {
      fscanf(f,"%s %d\n",str,&id);
      addVertex(gSanPham,id,str);
    }
}

int bienDoi(int idKhoHang)
{
  return idKhoHang*1000;
}

int bienDoiNguoc( int idKhoHangTrongG)
{
  return idKhoHangTrongG/1000;
}
int docFileKhoHang(Graph gSanPham, Graph gKhoHang, Graph g, char *tenFile, int m)
{
  int n;
  int k;
  FILE* f;
  int id;
  int i,j;
  char str[100];
  int idSanPham, soLuong;
  int id1, id2, doDai;
  f=fopen(tenFile,"r");
  if(f==NULL)
    {
      printf("File open failed\n");
      exit(1);
    }
  fscanf(f,"%d\n",&n);
  for(i=0;i<n;i++)
    {
      fscanf(f,"%s %d\n",str,&id);
      addVertex(gKhoHang,id,str);
      addVertex(g,bienDoi(id),str);
      for(j=0;j<m;j++)
	{
	  fscanf(f,"%d %d\n",&idSanPham,&soLuong);
	  addEdge(g,bienDoi(id),idSanPham,soLuong);
	}
    }
  fscanf(f,"%d\n",&k);
  for(i=0;i<k;i++)
    {
      fscanf(f,"%d %d %d\n",&id1,&id2,&doDai);
      addEdge(gKhoHang,id1,id2,(double)doDai);
    }
  return n;
}

int main()
{
  Graph g= createGraph(UNDIRECT_GRAPH);
  Graph gSanPham= createGraph(UNDIRECT_GRAPH);
  Graph gKhoHang= createGraph(UNDIRECT_GRAPH);
  int m = docFileSanPham(gSanPham,"sanpham.txt");
  int n = docFileKhoHang(gSanPham,gKhoHang,g,"khohang.txt",m);
  JRB node;
  JRB tree;
  int id1,id2;
  JRB node1, node2, node3, node4;
  JRB tree1, tree2;
  int thoat,luaChon;
  int idKhoHang,idSanPham;
  int soLuong, idKhoHangDuTru;
  int soHang, soHangDuTru;
  thoat=0;
  do
    {
      luaChon= menuVaLuaChon(1,7);
      switch(luaChon)
	{
	case 1:
	  //ham1
	  printf("C-Advanced HK20182\n");
	    break;
	case 2:
	  //ham2
	  jrb_traverse(node,gSanPham.vertices)
	    {
	      printf("%s %d\n",jval_s(node->val),jval_i(node->key));
	    }
	  break;
	case 3:
	  //ham3
	  printf("Nhap vao id1:");
	  scanf("%d",&id1);
	  printf("Nhap vao id2:");
	  scanf("%d",&id2);
	  node1=jrb_find_int(gKhoHang.vertices,id1);
	  node2=jrb_find_int(gKhoHang.vertices,id2);
	  if(node1==NULL||node2==NULL||node1==node2)
	    {
	      printf("-1\n");
	      break;
	    }
	  if(hasEdge(gKhoHang,id1,id2)==0)
	    {
	      printf("-1\n");
	    }
	  node1=jrb_find_int(gKhoHang.edges,id1);
	  tree1= (JRB) jval_v(node1->val);
	  node2=jrb_find_int(tree1,id2);
	  printf("Do dai 2 canh: %f (km)\n",jval_d(node2->val));
	  break;
	case 4:
	  //ham4
	  jrb_traverse(node,gKhoHang.vertices)
	    {
	      printf("%s\n",jval_s(node->val));
	      idKhoHang = bienDoi(jval_i(node->key));
	      node1=jrb_find_int(g.edges,idKhoHang);
	      tree1=(JRB) jval_v(node1->val);
	      jrb_traverse(node2,tree1)
		{
		  printf("%s %d\n",getVertex(gSanPham,jval_i(node2->key)),(int) jval_d(node2->val));
		}
	      printf("\n");
	    }
	  break;
	case 5:
	  //ham5
	  printf("Nhap vao idKhoHang:");
	  scanf("%d",&idKhoHang);
	  printf("Nhap vao idSanPham:");
	  scanf("%d",&idSanPham);
	  node = jrb_find_int(g.edges,bienDoi(idKhoHang));
	  tree = (JRB) jval_v(node->val);
	  node1= jrb_find_int(tree,idSanPham);
	  if(node1==NULL||node==NULL)
	    {
	      printf("Khong ton tai 2 id nay\n");
	      break;
	    }
	  printf("%s\n",getVertex(gKhoHang,idKhoHang));
	  printf("%s %d\n",getVertex(gSanPham,idSanPham),(int)jval_d(node1->val));
	  
	  printf("\nCac kho hang ke la:\n");
	  node = jrb_find_int(gKhoHang.edges, idKhoHang);
	  tree= (JRB) jval_v(node->val);
	  jrb_traverse(node,tree)
	    {
	      idKhoHang=jval_i(node->key);
	      node1=jrb_find_int(gKhoHang.vertices,idKhoHang);
	      //thac mc
	      if(node1==NULL)
		break;
	      //ben tren
	      node3 = jrb_find_int(g.edges,bienDoi(idKhoHang));
	      tree = (JRB) jval_v(node3->val);
	      node4= jrb_find_int(tree,idSanPham);
	      printf("%s\n",getVertex(gKhoHang,idKhoHang));
	      printf("%s %d\n",getVertex(gSanPham,idSanPham),(int)jval_d(node4->val));
	    }
	  break;
	case 6:
	  //ham6
	  printf("Nhap vao id kho gan nhat:");
	  scanf("%d",&idKhoHang);
	  printf("Nhap vao id san pham:");
	  scanf("%d",&idSanPham);
	  printf("Nhap vao so luong can mua:");
	  scanf("%d",&soLuong);
	  printf("Nhap vao id kho hang du tru:");
	  scanf("%d",&idKhoHangDuTru);
	  node= jrb_find_int(g.edges,bienDoi(idKhoHang));
	  if(node==NULL)
	    {
	      printf("Khong ton tai id kho hang \n");
	      break;
	    }
	  tree = (JRB) jval_v(node->val);
	  node1= jrb_find_int(tree,idSanPham);
	  soHang = (int) jval_d(node1->val);
	  if(soHang>=soLuong)
	    {
	      printf("Dat hang thanh cong voi thoi gian 30 phut\n");
	      break;
	    }

	  node2= jrb_find_int(g.edges,bienDoi(idKhoHangDuTru));
	  if(node2==NULL)
	    {
	      printf("Khong ton tai id kho hang \n");
	      break;
	    }
	  tree = (JRB) jval_v(node2->val);
	  node3= jrb_find_int(tree,idSanPham);
	  soHangDuTru = (int) jval_d(node3->val);
	  if(soHang+soHangDuTru<soLuong)
	    {
	      printf("Dat hang that bai");
	      break;
	    }
	  printf("Dat hang thanh cong voi thoi gian: %d phut\n", ((int) getEdgeValue(gKhoHang,idKhoHang,idKhoHangDuTru)*2 +30));
	  printf("\n");
	  break;
	case 7:
	  thoat=1;
	  break;
	}	
    }while(thoat!=1);
}
