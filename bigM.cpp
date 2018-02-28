#include<iostream>
#include <iomanip> 
using namespace std;
#define M 1000
void print_table(float **a,int x_b[] ,float c_b[],float cost[],float b[], float delta[], int m, int n, int iteration){
	cout<<endl;
	cout<<"*************** SIMPLEX TABLE FOR "<<iteration<<"th ITERATION ***************"<<endl;
	cout<<endl;
	cout<<" \t \t";
	for(int i=1;i<=n;i++)
		cout<<cost[i]<<"\t";
	cout<<endl;
	cout<<"BV\tCB\t";
	for(int i=1;i<=n;i++)
		cout<<"X"<<i<<"\t";
	cout<<"B"<<endl;
	for(int i=0;i<m;i++){
		cout<<x_b[i+1]<<"\t"<<c_b[i+1]<<"\t";
		for(int j=0;j<n;j++){
			cout<<setprecision(4)<<a[i][j]<<"\t";
		}
		cout<<b[i]<<endl;
	}
	cout<<" \t \t";
	for(int i=1;i<=n;i++)
		cout<<delta[i]<<"\t";
	cout<<endl;

}

void change_table(float **a, float b[], int m, int n, int q, int p){
	float temp=a[p][q];
	b[p]=b[p]/a[p][q];
	for(int i=0;i<n;i++){
		a[p][i]=a[p][i]/temp;
	}
	for(int i=0;i<m;i++){
		if(i!=p){
			float f= a[i][q];
			for(int j=0;j<n;j++){
				a[i][j]=a[i][j]-f*a[p][j];
			}
			b[i]=b[i]-f*b[p];
		}
	}
}


void simplex_table(float **a,float b[],int n_dash, int n,int m,float obj_func[], int max_flag, int case_in){
	int iter;
	if(case_in==4 || case_in==3){
		
		cout<<"Enter the iteration number"<<endl;
		cin>> iter;
	}
	int x_b[m+1];
	float c_b[m+1];
	float cost[n+1];
	float delta[n+1];
	for(int i=1;i<=m;i++)
		x_b[i]=n_dash+i;
	for(int i=1;i<=m;i++)
		c_b[i]=obj_func[n_dash+i-1];
	for(int i=1;i<=n;i++){
		cost[i]=obj_func[i-1];
	}
	float min=0;
	float z[n+1];
	int min_index;
	float b_min=10000000;
	int b_min_index;
	int iteration_count=0;
	while(1){	
		for(int i=0;i<n;i++){
			z[i+1]=0;
			for(int j=0;j<m;j++){
				z[i+1]+=c_b[j+1]*a[j][i];
			}
		}
		for(int i=1;i<=n;i++){
			delta[i]=z[i]-cost[i];
		}
		if(case_in==5)
		print_table(a,x_b,c_b,cost,b,delta,m,n,iteration_count);
		min=0;
		for(int i=1;i<=n;i++){
			if(delta[i]<min){
				min=delta[i];
				min_index=i;
			}
		}
		if(case_in==4){
			if(iteration_count==iter){
				cout<<"Delta value are: ";
				for(int i=1;i<=n;i++){
					cout<<delta[i]<<" ";
				}
			}
		}
		if(case_in==3){
			if(iteration_count==iter){
				for(int i=1;i<=m;i++){
					cout<<"x"<<x_b[i]<<"= "<<b[i-1]<<"   ";
				}
			}
		}
		if(min==0 || iteration_count==5){
			break;
		}
		b_min=10000000;
		for(int i=0;i<m;i++){
			if(a[i][min_index-1]>0 && b[i]>=0){
				if(b_min> b[i]/a[i][min_index-1]){
					b_min=b[i]/a[i][min_index-1];
					b_min_index=i+1;
				}
			}
		}
		x_b[b_min_index]=min_index;
		c_b[b_min_index]=cost[min_index];
		change_table(a,b,m,n,min_index-1,b_min_index-1);
		iteration_count++;
	}
	float optimal=0;
	for(int i=0;i<m;i++){
		optimal+=c_b[i+1]*b[i];
	}
	//cout<<"optimal solution is "<<optimal*max_flag<<endl;
	if(case_in==6){
		cout<<"optimal solution is "<<optimal*max_flag<<endl;
	}
	if(case_in==2){
		cout<<"number of iterations is "<<iteration_count<<endl;
	}
}
int main(){
	int counter=0;
	int n,m;
	cout<<"Enter number of variables to maximize i.e n :";
	cin>>n;
	cout<<"Enter number of equations i.e value of m :" ;
	cin>>m;
	float **a = new float* [m];
	for(int i=0;i<m;i++)
		a[i]= new float[n+m+m];
	cout<<"Enter the coefficients on left: "<<endl;
	for(int i=0;i<m;i++)
		for(int j=0;j<n;j++)
			cin>>a[i][j];
	cout<<"Enter the values of bj's :"<<endl;
	float b[m];
	for(int i=0;i<m;i++)
		cin>>b[i];
	int max_flag=1;
	cout<<"if you want to maximize enter 1 else -1"<<endl;
	cin>>max_flag;
	cout<<"Enter the coefficients of the expression you want to maximize/minimize: "<<endl;
	float obj_func[n+m+m];
	for(int i=0;i<n;i++){
		cin>>obj_func[i];
		if(max_flag==-1){
			obj_func[i]=obj_func[i]*(-1);
		}
	}

	cout<<"enter 1 for <= , enter 0 for = , enter -1 for >= "<<endl;	
	int sign[m];	
	for(int i=0;i<m;i++){
		cout<<"For equation "<<i+1<<" ";
		cin>>sign[i];	
	}
	int n_dash =n;
	for(int i=0;i<m;i++){
		for(int j=0;j<m;j++){
				a[j][i+n]=0;
			}
		if(sign[i]==0){			
			a[i][n_dash+i+1]=1;
			obj_func[n_dash+i]=-1*M;
		}
		if(sign[i]==1){
			a[i][n_dash+i]=1;
			obj_func[n_dash+i]=0;
		}
		else{
			for(int j=0;j<m;j++){
				a[j][n_dash+m+counter]=0;
			}
			a[i][n_dash+i]=1;
			obj_func[n_dash+i]=-1*M;
			a[i][n_dash+m+counter]=-1;
			obj_func[n_dash+m+counter]=0;
			counter++;
		}
	}	
	cout<<"Select an option from the below : "<<endl;
	cout<<"1.List of all BFS\n2.Number of Iterations to solve the problem\n3.Net evaluations in ith iteration\n4.List of Basic variables along with min ratios in ith iteration\n5.simplex table of ith iteration\n6.optimal solution\n"<<endl;
	int option;
	cin>>option;
	switch(option){
		case 1:
			// BSF(a,b,m,n+m);
			break;
		case 3:	
		case 4:
		case 2:
		case 5:
		case 6:
			simplex_table(a,b,n,n+m+counter,m,obj_func,max_flag,option);
			break;
		default:
			return 0;
		}
	
}
