#include <stdio.h>

void chuanHoaTuyen(int tuyen[],int size, int diemDung[])
{
  int i,j;
  int k=0;
  i=0;
  for(i=0;i<size;i++)
    {
      j=i+1;
      if(tuyen[i]==tuyen[j])
	{
	  for(int m=i;m<size;m++)
	    tuyen[m]=tuyen[m+1];
	  i--;
	}
      else
	{
	  diemDung[k]=i;
	  printf("%d\n",i);
	  k++;
	}
    }
}

int main()
{
  int a[100]= {1,2,3,3,4,5,5,6,4};
  int diemDung[100]={};
  int ts[100];
  int i;
  int n = sizeof(a) / sizeof(a[0]);
  // printf("%d\n",n);
  chuanHoaTuyen(a,n,diemDung);
  for(i=0;i<n;i++)
    {
      if(a[i]==0) break;
      printf("%d ",a[i]);
    }
  printf("\n");
  i=0;
  while(diemDung[i]!=0)
    {
      printf("%d ",diemDung[i]);
      i++;
    }
}
