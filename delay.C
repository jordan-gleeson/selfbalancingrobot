#include <delay.H>

//Delay function (appoximate milliseconds)
void dly(unsigned int dly_time){
	unsigned int i, j,k=0,l;
	for(i=0;i<=dly_time;i++){
		for(j=0;j<32;j++){
			j=j++;
			j=j--;
			for(k=0;k<15;k++){}
		}
	}
}

//Small delay function
void sdly(unsigned int dly_time){
	unsigned int i;
	for(i=0;i<=dly_time;i++){}
}