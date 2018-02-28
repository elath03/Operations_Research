#include<iostream>

using namespace std;

float** input(int m, int n)
{
	int i,j;
	float** array = new float* [m];
	for(i=0; i<m; i++)
    	array[i] = new float[n];
	for(i=0;i<m;i++)
	{
		cout<<"Input row "<< i <<": ";
		for(j=0;j<n;j++)
			cin>>array[i][j];
	}
	return array;
}

void view(float** array, int m, int n)
{
	int i,j;
	for(i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
			cout<<array[i][j]<<"\t";
		cout<<endl;
	}
	cout<<endl;
}

float** copy(float** array, int m, int n, int* bv_num,int aug)
{
	int i,j;
	float** a = new float* [m];
	for(i=0; i<m; i++)
    	a[i] = new float[m+aug];
	for(i=0;i<m;i++)
	{
		for(j=0;j<m;j++)
			a[i][j] = array[i][bv_num[j]];
		if(aug)
			a[i][m] = array[i][n-1];
	}
	return a;
}

float** reduce(float** array,int m, int n, int* bv_num, int aug)
{
	float** a = copy(array,m,n,bv_num,aug);
	int disp = 0;
	n = m+aug;
	for(int i=0; i+disp<n && i<m; i++)
	{
		float el = a[i][i+disp];
		if(el)
		{
			for(int j=0; j<n; j++)
				a[i][j]/=el;
			for(int j=0; j<m; j++)
			{
				if(i!=j)
				{
					float factor = a[j][i+disp];
					for(int k=0; k<n; k++)
						a[j][k] -= a[i][k]*factor;
				}
			}
		}
		else
		{
			int j = i;
			while(!el && i+disp<n)
			{
				if(j==m)
				{j=i;disp++;}
				el = a[j++][i+disp];
			}
			if(i+disp!=n)
			{
				if(--j!=i)
					for(int k=0; k<n; k++)
					{
						float temp = a[i][k];
						a[i][k] = a[j][k];
						a[j][k] = temp;
					}
				el = a[i][i+disp];
				for(int j=0; j<n; j++)
					a[i][j]/=el;
				for(int j=0; j<m; j++)
				{
					if(i!=j)
					{
						float factor = a[j][i+disp];
						for(int k=0; k<n; k++)
							a[j][k] -= a[i][k]*factor;
					}
				}
			}
		}
	}
	return a;
}

int rank_mat(float** array,int m, int n, int* bv_num, int aug)
{
	float** a = reduce(array,m,n,bv_num,aug);
	int r=0,disp=0;
	n=m+aug;
	for(int i=0;i+disp<n && i<m;i++)
	{
		while(!a[i][i+disp])
		{
			disp++;
			if(i+disp==n)
				break;
		}
		if(i+disp!=n)
			if(a[i][i+disp])
				r++;
	}
	return r;
}

int factorial(int n)
{
	if(n==0)
		return 1;
	else
		return n * factorial(n-1);
}

void next_bv_num(int* bv_num, int m)
{
	int k = m-1;
	bv_num[k]+=1;
	while(bv_num[k]==m && k>=0)
	{
		bv_num[k] = bv_num[k-1] + 2;
		bv_num[--k] += 1;
	}
}

void BS(float** array, int m, int n)
{
	int num_bfs = factorial(n)/(factorial(m)*factorial(n-m));
	int* bv_num = new int[m];
	for(int i=0;i<m;i++)
	{
		bv_num[i] = i;
	}
	for(int i=1; i<num_bfs+1; i++)
	{
		cout<<"Solution "<<i<<": "<<endl;
		int ra = rank_mat(array,m,n+1,bv_num,1);
		cout<<ra<<endl;
		int r = rank_mat(array,m,n+1,bv_num,0);
		cout<<r<<endl;
		if(ra==r)
		{
			if(r==m)
			{
				cout<<"Basic Variables: ";
				for(int j=0; j<m; j++)
					cout<<bv_num[j]+1<<" ";
				cout<<endl;
				float** sol = reduce(array,m,n+1,bv_num,1);
				for(int j=0; j<m; j++)
					cout<<sol[j][m]<<" ";
				cout<<endl;
				for(int j=0; j<m; j++)
				{
					if(sol[j][m]==0)
					{
						cout<<"Degenerate Solution"<<endl;
						break;
					}
					if(j==m-1)
						cout<<"Non-degenerate Solution"<<endl;
				}
				for(int j=0; j<m; j++)
				{
					if(sol[j][m]<0)
					{
						cout<<"NOT Basic Feasible Solution "<<endl;
						break;
					}
					if(j==m-1)
						cout<<"Basic Feasible Solution"<<endl;
				}
				cout<<endl;
			}
			else
				cout<< "infinite solutions"<<endl;
		}
		else
			cout<<"no solution"<<endl;
		next_bv_num(bv_num,m);
	}
}

int main()
{
	int m,n;
	cout<<"Input number of equations: ";
	cin>>m;
	cout<<"Input number of variables: ";
	cin>>n;
	float** array = input(m,n+1);
	cout<<"The input array is:"<<endl;
	view(array,m,n+1);
	BS(array, m, n);
}
