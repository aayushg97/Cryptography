#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int dec(char str[],int l){
	int i,sum=0;
	for(i=0;i<l;i++){
		sum = 2*sum;
		sum += (str[i]=='1')?1:0;
	}
	return sum;
}

void c2str(char str[],int l,int x){
	int i;
	
	for(i=l-1;i>=0;i--){
		str[i] = (x%2==1)?'1':'0';
		x /= 2;
	}
}

int main() {
	char X[500],w[500],u[500],v[500],y[500];
	int r,i,j,k,l,m,pv,pu,length;
	int pis[16] = {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7};
	int pip[16] = {1,5,9,13,2,6,10,14,3,7,11,15,4,8,12,16};
	printf("Enter the binary string \n");
	scanf("%s",X);
	length = strlen(X);
	printf("Enter l and m as two space separated integers \n");
	scanf("%d %d",&l,&m);
	printf("Enter the number of keys \n");
	scanf("%d",&r);
	printf("Enter the r+1 lines, each line containing a key \n");
	char key[r+1][500];
	
	for(i=0;i<r+1;i++){
		scanf("%s",key[i]);
	}
	
	for(i=0;i<length;i++){
		w[i] = X[i];
	}
	
	char temp[l];
	for(i=1;i<r;i++){
		pv=0;
		pu=0;
		
		for(j=0;j<length;j++){
			u[j] = (w[j]==key[i-1][j])?'0':'1';
		}
		
		for(k=1;k<=m;k++){
			for(j=0;j<l;j++){
				temp[j] = u[pu+j];
			}
			
			c2str(temp,l,pis[dec(temp,l)]);
			
			for(j=0;j<l;j++){
				v[pv+j] = temp[j];
			}
			pu += l;
			pv += l;
		}
		
		for(j=0;j<length;j++){
			w[j] = v[pip[j]-1];
		}
		
	}
	
	for(j=0;j<length;j++){
		u[j] = (w[j]==key[r-1][j])?'0':'1';
	}
	
	pv=0;
	pu=0;
	
	for(k=1;k<=m;k++){
		for(j=0;j<l;j++){
			temp[j] = u[pu+j];
		}
		c2str(temp,l,pis[dec(temp,l)]);
		for(j=0;j<l;j++){
			v[pv+j] = temp[j];
		}
		pu += l;
		pv += l;
	}
	
	for(j=0;j<length;j++){
		y[j] = (v[j]==key[r][j])?'0':'1';
	}
	y[length] = '\0';
	
	printf("Output binary string is :- %s\n",y);
	return 0;
}
