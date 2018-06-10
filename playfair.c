#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim(char st[]){
	int i,j,l = strlen(st);
	char out[l];
	j = 0;
	for(i=0;i<l;i++){
		if(isalpha(st[i])){
			out[j] = st[i];
			j++;	
		} 
	}
	out[j] = '\0';
	for(i=0;i<j+1;i++){
		st[i] = out[i];
	}
}

int main() {
	int i,j,r,c,l=0;
	char ct;
	char key[50];
	char table[5][5];
	char input[500];
	char output[500];
	int alpha[26],beta[26][2];
	
	for(i=0;i<5;i++){
		for(j=0;j<5;j++){
			table[i][j] = 'A';
		}
	}
	
	for(i=0;i<26;i++){
		alpha[i] = 1;
	}
	printf("Enter the key \n");
	scanf(" %[^\n]",key);
	trim(key);
	r=0;
	c=0;
	for(i=0;i<strlen(key);i++){
		if(alpha[tolower(key[i])-97]==1){
			if(table[r][c]!='A')ct = table[r][c];
			table[r][c] = tolower(key[i]);
			beta[tolower(key[i])-97][0] = r;
			beta[tolower(key[i])-97][1] = c;
			alpha[tolower(key[i])-97] = 0;
			if(c<4)c++;
			else{
				r++;
				c = 0;
				if(r==5){
					r = 4;
					c = 4;
				}
			}
		}
	}
	
	for(i=0;i<26;i++){
		if(alpha[i]==1){
			if(table[r][c]!='A')ct = table[r][c];
			table[r][c] = 'a' + i;
			beta[i][0] = r;
			beta[i][1] = c;
			alpha[i] = 0;
			if(c<4)c++;
			else{
				r++;
				c = 0;
				if(r==5){
					r = 4;
					c = 4;
				}
			}
		}
	}
	
	printf("Enter the plaintext \n");
	scanf(" %[^\n]",input);
	trim(input);
	
	j=0;
	for(i=0;i<strlen(input);i+=2){
		if(i<strlen(input)-1){
			output[j] = input[i];  
			if(input[i+1]==input[i]){
				output[j+1] = ct;  
				output[j+2] = input[i+1];   
				output[j+3] = ct;   
				j+=4;
				l = j;
			}
			else {
				output[j+1] = input[i+1]; 
				j+=2;
				l = j;	
			}
		}
		else{
			output[j] = input[i];    
			output[j+1] = ct;     
			l = j+2;
		}
	}
	output[l] = '\0';
	
	for(i=0;i<strlen(output)-1;i+=2){
		if(beta[output[i]-'a'][0]==beta[output[i+1]-'a'][0]){
			output[i] = table[beta[output[i]-'a'][0]][(beta[output[i]-'a'][1]+1)%5];
			output[i+1] = table[beta[output[i+1]-'a'][0]][(beta[output[i+1]-'a'][1]+1)%5];
		}
		else{
			if(beta[output[i]-'a'][1]==beta[output[i+1]-'a'][1]){
				output[i] = table[(beta[output[i]-'a'][0]+1)%5][beta[output[i]-'a'][1]];
				output[i+1] = table[(beta[output[i+1]-'a'][0]+1)%5][beta[output[i+1]-'a'][1]];
			}
			else{
				output[i] = table[beta[output[i]-'a'][0]][beta[output[i+1]-'a'][1]];
				output[i+1] = table[beta[output[i+1]-'a'][0]][beta[output[i]-'a'][1]];
			}
		}
	}
	
	printf("Output is as follows \n");
	printf("%s \n",output);
	
	return 0;
}
