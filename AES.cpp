#include <bits/stdc++.h>
#include <iostream>
using namespace std;

string rev(string a){
	char temp;
	for(int i=0;i<a.length()/2;i++){
		temp = a[i];
		a[i] = a[a.length()-i-1];
		a[a.length()-i-1] = temp;
	}
	return a;
}

string exor(string a,string b){
	string c = "";
	for(int i=0;i<a.length();i++){
		c += (a[i]==b[i])?'0':'1';
	}
	
	return c;
}

string esbox(string a){
	int sum;
	string b = "",d = "";
	int arr[8][8] = {{1,0,0,0,1,1,1,1},{1,1,0,0,0,1,1,1},{1,1,1,0,0,0,1,1},{1,1,1,1,0,0,0,1},
					 {1,1,1,1,1,0,0,0},{0,1,1,1,1,1,0,0},{0,0,1,1,1,1,1,0},{0,0,0,1,1,1,1,1}
					};
	string c = "11000110";
	for(int i=0;i<8;i++){
		sum = 0;
		for(int j=0;j<8;j++){
			sum += arr[i][j]*(int)(a[j]-'0');
		}
		b += '0' + (sum%2);
	}
		
	return exor(b,c);
}

string sbox32(string s){
	string a = "";
	a += esbox(s.substr(0,8)) + esbox(s.substr(8,8)) + esbox(s.substr(16,8)) + esbox(s.substr(24,8));
	return a;
}

string rot(string w){
	w = w.substr(8,8) + w.substr(16,8) + w.substr(24,8) + w.substr(0,8);
	return w;
}

void eschedule(string arr[],string key){
	string w[44];
	string c[10] = {"00000001000000000000000000000000","00000010000000000000000000000000","00000100000000000000000000000000",
					"00000100000000000000000000000000","00010000000000000000000000000000","00100000000000000000000000000000",
					"01000000000000000000000000000000","10000000000000000000000000000000","00011011000000000000000000000000",
					"00110110000000000000000000000000"};
	
	for(int i=0;i<4;i++){
		w[i] = "";
		for(int j=0;j<32;j++){
			w[i] += key[32*i+j];
		}
	}
	
	arr[0] = w[0] + w[1] + w[2] + w[3];
	
	for(int i=4;i<44;i+=4){
		for(int j=0;j<4;j++){
			w[i+j] = "";    
			if((i+j)%4==0){
				w[i+j] = exor(exor(sbox32(rot(w[i+j-1])),c[(i/4)-1]),w[i+j-4]);    
			}
			else{
				w[i+j] = exor(w[i+j-1],w[i+j-4]);		
			}
		}
		arr[i/4] = w[i] + w[i+1] + w[i+2] + w[i+3];
	}
}

void esubByte(string state[][4]){
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			state[i][j] = esbox(state[i][j]);
		}
	}
}

void eshiftRow(string state[][4]){
	string temp;

	temp = state[1][0];
	state[1][0] = state[1][1];
	state[1][1] = state[1][2];
	state[1][2] = state[1][3];
	state[1][3] = temp;	
	
	temp = state[2][0]; state[2][0] = state[2][2]; state[2][2] = temp;
	temp = state[2][1]; state[2][1] = state[2][3]; state[2][3] = temp;
	
	temp = state[3][3];
	state[3][3] = state[3][2];
	state[3][2] = state[3][1];
	state[3][1] = state[3][0];
	state[3][0] = temp;	
}

void addRoundKey(string state[][4],string key){
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			state[j][i] = exor(state[j][i],key.substr(8*j+32*i,8));
		}
	}
}

string fieldMult(string a,string b){
	char arr[15];
	for(int i=0;i<15;i++)arr[i] = '0';
	string c;
	int start,temp;
	a = rev(a);
	b = rev(b);
	for(int i=0;i<8;i++){
		for(int j=0;j<8;j++){
			if(a[i]=='1' && b[j]=='1'){
				if(arr[i+j]=='1')arr[i+j] = '0';
				else arr[i+j] = '1';
			}
		}
	}
	
	for(int i=14;i>=0;i--){
		if(arr[i]=='1'){
			start = i;
			break;
		}
	}	
	
	while(start>7){
		temp = start-8;
		arr[start] = '0';
		arr[temp+4] = (arr[temp+4]=='1')?'0':'1';
		arr[temp+3] = (arr[temp+3]=='1')?'0':'1';
		arr[temp+1] = (arr[temp+1]=='1')?'0':'1';
		arr[temp] = (arr[temp]=='1')?'0':'1';
		
		for(int i=start;i>=0;i--){
			if(arr[i]=='1'){
				start = i;
				break;
			}
		}
	}
	
	c = "";
	for(int i=7;i>=0;i--){
		c += arr[i]; 
	}
	
	return c;
}

void emixColumn(string state[][4]){
	string t0,t1,t2,t3;
	for(int j=0;j<4;j++){
		t0 = exor(exor(fieldMult("00000010",state[0][j]),fieldMult("00000011",state[1][j])),exor(state[2][j],state[3][j]));
		t1 = exor(exor(fieldMult("00000010",state[1][j]),fieldMult("00000011",state[2][j])),exor(state[0][j],state[3][j]));
		t2 = exor(exor(fieldMult("00000010",state[2][j]),fieldMult("00000011",state[3][j])),exor(state[0][j],state[1][j]));
		t3 = exor(exor(fieldMult("00000010",state[3][j]),fieldMult("00000011",state[0][j])),exor(state[2][j],state[1][j]));
		
		state[0][j] = t0;
		state[1][j] = t1;
		state[2][j] = t2;
		state[3][j] = t3;
	}
}

string dsbox(string a){
	int sum;
	string b = "",d = "";
	int arr[8][8] = {{0,0,1,0,0,1,0,1},{1,0,0,1,0,0,1,0},{0,1,0,0,1,0,0,1},{1,0,1,0,0,1,0,0},
					 {0,1,0,1,0,0,1,0},{0,0,1,0,1,0,0,1},{1,0,0,1,0,1,0,0},{0,1,0,0,1,0,1,0}
					};
	string c = "10100000";
	for(int i=0;i<8;i++){
		sum = 0;
		for(int j=0;j<8;j++){
			sum += arr[i][j]*(int)(a[j]-'0');
		}
		b += '0' + (sum%2);
	}
		
	return exor(b,c);
}

void dschedule(string arr[],string key){
	string w[44];
	string c[10] = {"00000001000000000000000000000000","00000010000000000000000000000000","00000100000000000000000000000000",
					"00000100000000000000000000000000","00010000000000000000000000000000","00100000000000000000000000000000",
					"01000000000000000000000000000000","10000000000000000000000000000000","00011011000000000000000000000000",
					"00110110000000000000000000000000"};
	
	for(int i=0;i<4;i++){
		w[i] = "";
		for(int j=0;j<32;j++){
			w[i] += key[32*i+j];
		}
	}
	arr[10] = w[0] + w[1] + w[2] + w[3];
	
	for(int i=4;i<44;i+=4){
		for(int j=0;j<4;j++){
			w[i+j] = "";
			if((i+j)%4==0){
				w[i+j] = exor(exor(sbox32(rot(w[i+j-1])),c[(i/4)-1]),w[i+j-4]);
			}
			else{
				w[i+j] = exor(w[i+j-1],w[i+j-4]);
			}
		}
		arr[10-(i/4)] = w[i] + w[i+1] + w[i+2] + w[i+3];
	}
}

void dsubByte(string state[][4]){
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			state[i][j] = dsbox(state[i][j]);
		}
	}
}

void dshiftRow(string state[][4]){
	string temp;

	temp = state[1][3];
	state[1][3] = state[1][2];
	state[1][2] = state[1][1];
	state[1][1] = state[1][0];
	state[1][0] = temp;	
	
	temp = state[2][0]; state[2][0] = state[2][2]; state[2][2] = temp;
	temp = state[2][1]; state[2][1] = state[2][3]; state[2][3] = temp;
	
	temp = state[3][0];
	state[3][0] = state[3][1];
	state[3][1] = state[3][2];
	state[3][2] = state[3][3];
	state[3][3] = temp;	
}

void dmixColumn(string state[][4]){
	string t0,t1,t2,t3;
	for(int j=0;j<4;j++){
		t0 = exor(exor(fieldMult("00001110",state[0][j]),fieldMult("00001011",state[1][j])),exor(fieldMult("00001101",state[2][j]),fieldMult("00001001",state[3][j])));
		t1 = exor(exor(fieldMult("00001001",state[0][j]),fieldMult("00001110",state[1][j])),exor(fieldMult("00001011",state[2][j]),fieldMult("00001101",state[3][j])));
		t2 = exor(exor(fieldMult("00001101",state[0][j]),fieldMult("00001001",state[1][j])),exor(fieldMult("00001110",state[2][j]),fieldMult("00001011",state[3][j])));
		t3 = exor(exor(fieldMult("00001011",state[0][j]),fieldMult("00001101",state[1][j])),exor(fieldMult("00001001",state[2][j]),fieldMult("00001110",state[3][j])));
		
		state[0][j] = t0;
		state[1][j] = t1;
		state[2][j] = t2;
		state[3][j] = t3;
	}
}

string encrypt(string plain,string key){
	string arr[11];
	eschedule(arr,key);
	string state[4][4],c="";
	
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			state[j][i] = plain.substr(8*j+32*i,8);
		}
	}
	
	addRoundKey(state,arr[0]);
	
	for(int i=1;i<10;i++){
		esubByte(state);
		eshiftRow(state);
		emixColumn(state);
		addRoundKey(state,arr[i]);
	}
	
	esubByte(state);
	eshiftRow(state);
	addRoundKey(state,arr[10]);
	
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			c += state[j][i];
		}
	}
	
	return c;
}

string decrypt(string plain,string key){
	string arr[11];
	dschedule(arr,key);
	string state[4][4],c="";
	
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			state[j][i] = plain.substr(8*j+32*i,8);
		}
	}
	
	addRoundKey(state,arr[0]);
	
	for(int i=1;i<10;i++){
		dshiftRow(state);
		dsubByte(state);
		addRoundKey(state,arr[i]);
		dmixColumn(state);
	}
	
	dshiftRow(state);
	dsubByte(state);
	addRoundKey(state,arr[10]);
	
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			c += state[j][i];
		}
	}
	
	return c;
}

int main() {
	string plain,key,cipher;
	
	cout<<"Enter the plain text \n";
	cin>>plain;
	
	cout<<"Enter the key \n";
	cin>>key;
	
	cipher = encrypt(plain,key);
	
	cout<<cipher<<endl;
	cipher = decrypt(cipher,key);
	cout<<cipher<<endl;
	
	return 0;
}
