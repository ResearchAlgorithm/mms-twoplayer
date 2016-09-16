#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define MAX 300
#define INF -999999
#define NoE 12

class Good{
	public:
		int data[MAX];
		int size;
		int fullsize;
		int valid;
		
		bool Has(int x){
			return data[x] == 1;
		}
		void Delete(int x){
			if(data[x] == 1)
				data[x] = 0, size -= 1;
			return;
		}
		void Add(int x){
			if(data[x] == 0) 
				data[x] = 1, size += 1;
			return;
		}
		bool Exist(){ 
			// If it is $\bot$, this function returns false;
			return valid == 1;
		}
		void SetFull(int full){
			for(int i = 1; i <= full; i ++){
				data[i] = 1;
			}
			size = full;
			fullsize = full;
			valid = 1;
			return;
		}
		void SetEmpty(int full){
			for(int i = 1; i <= full; i ++)
				data[i] = 0;
			size = 0;
			valid = 1;
			fullsize = full;
			return;
		}
		void SetInvalid(int full){
			valid = 0;
		}
		void Print(){
			for(int i = 1; i <= fullsize; i ++)
				if(data[i] == 1)
					printf("%d ", i);
			printf("\n");
		}
		int Weight(int *value){
			if(valid == 0)
				return INF;
			int weight = 0;
			for(int i = 1; i <= fullsize; i++)
				if(data[i] == 1)
					weight += value[i];
			return weight;
		}
};

int n = 50;
int *V1, *V2, *V1backup, *V2backup;
int V1sum, V2sum;
int MMS1 = 25000;
int u = 5000, SS = 500;
int q;
int verbose = 1;
int Recurence = 1;
float e[NoE] = {0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1, 0.05, 0.03, 0.02, 0.01};
int f = 0;
char filepath[100];


Good DP(Good G, int MMS1, int q){
	Good *S[2];
	int *opt[2];
	Good value;
 	
	 // init
	opt[0] = (int *)malloc(sizeof(int) * (q + 1));
	opt[1] = (int *)malloc(sizeof(int) * (q + 1));
	S[0] = (Good *)malloc(sizeof(Good) * (q + 1));
	S[1] = (Good *)malloc(sizeof(Good) * (q + 1));

	// when i=0
	opt[0][0] = 0;
	S[0][0].SetEmpty(n);
	for(int j = 1; j <= q; j ++)
		opt[0][j] = INF, S[0][j].SetInvalid(n);

	for(int i = 1; i <= n; i ++)
	{
		opt[i % 2][0] = 0;
		S[i % 2][0].SetEmpty(n);
		for(int j = 1; j <= q; j ++)
		{
			opt[i % 2][j] = opt[(i-1) % 2][j];
			S[i % 2][j] = S[(i-1) %2][j];
			if( j >= V2[i] && opt[(i-1) % 2][j - V2[i]] != INF && opt[(i-1) % 2][j - V2[i]] + V1[i] >= opt[i % 2][j])
			{
				opt[i % 2][j] = opt[(i-1) % 2][j-V2[i]] + V1[i];
				S[i % 2][j] = S[(i-1) % 2][j - V2[i]];
				if(S[i % 2][j].Exist())
					S[i % 2][j].Add(i);
			}
		}
	}
	int c = -1;
	int flag = 0;
	for(int i = 0; i <= q; i++)
	{	
		if(opt[n % 2][i] != INF && opt[n % 2][i] >= MMS1){
			c = i, flag = 1;
			break;
		}
	}
	if(flag){
		value = S[ n % 2][c];
	}
	else{
			S[n % 2][q].SetInvalid(n);
			value = S[n % 2][q];
		}
	free(opt[0]), free(opt[1]), free(S[0]), free(S[1]);
	return value;
}

void Scale(int *a, int *b, float factor)
{
	for(int i = 1; i <=n; i ++)
	{
		b[i] = (int)(a[i] / factor);
	}
}
void Copy(int *a, int *b){
	for(int i = 1; i <=n; i++)
		b[i] = a[i];
}
Good Scaling(Good G, int MMS1, float e, int p)
{
	float Q = e * p / n;
	Scale(V2backup, V2, Q);
	return DP(G, MMS1, (int)(2*p / Q));
}
int Mini(int * a){
	int min = 1;
	for(int i = 1; i <=n; i ++){
		if(a[i] < a[min])
			min = i;
	}
	return a[min];
}
void Print(Good S){
	int v1 = 0, v2 = 0;
	int v2_discard = 0;
//	printf("MMS1 is: %d\n\n", MMS1);
	printf("P1:\t");
	S.Print();
	printf("value: %d\n", S.Weight(V1backup));
	
	printf("\nP2:\t");
	for(int i = 1; i <= n; i++)
		if(!S.Has(i))
			printf("%d ",i);
	printf("\nvalue: %d\n", V2sum - S.Weight(V2backup));
//	printf("\nplayer2 discard: %d\n", S.Weight(V2backup));
}
int gaussrand()
{
	srand( (unsigned)time( NULL ) ); 
	static double V1, V2, S;
	static int phase = 0;
	double X;

	if(phase == 0) {
		do{
		    double U1 = (double)rand() / RAND_MAX;
		    double U2 = (double)rand() / RAND_MAX;

		    V1 = 2 * U1 - 1;
		    V2 = 2 * U2 - 1;
		    S = V1 * V1 + V2 * V2;
		} while(S >= 1 || S == 0);
		
		X = V1 * sqrt(-2 * log(S) / S);
	} else{
		X = V2 * sqrt(-2 * log(S) / S);
	}
	phase = 1 - phase;

	return (int)(X * SS + u);
}
void Init(){	
	V1sum = V2sum = 0;
	if(f == 1){
		if(verbose)
			printf("The data are read from %s\n\n", filepath);
		FILE *fp = fopen(filepath,"r");
		fscanf(fp, "%d %d", &n, &MMS1);
		for(int i = 1; i <= n; i ++)
			fscanf(fp, "%d", &V1[i]), V1sum += V1[i];
		for(int i = 1; i <= n; i ++)
			fscanf(fp, "%d", &V2[i]), V2sum += V2[i];	
		fclose(fp);
	}
	else{
		MMS1 = n * u / 2;
		for(int i = 1; i <= n; i++)
			V1[i] = gaussrand(), V1sum += V1[i];
		for(int i = 1; i <= n; i ++)
			V2[i] = gaussrand(), V2sum += V2[i];
	}
	Copy(V1, V1backup);
	Copy(V2, V2backup);
	q = n * u + n * u / 10;
	if(verbose) {
		printf("n=%d, u=%d, SS=%d, MMS1=%d\n", n, u, SS, MMS1);
		printf("Recurance=%d\n", Recurence);
		
		printf("G :\t");
		for(int i = 1; i <= n; i ++)
			printf("%d ", i);
		printf("\nV1:\t");
		for(int i = 1; i <= n; i ++)
			printf("%d ", V1[i]);
		printf("\nV2:\t");
		for(int i = 1; i <= n; i ++)
			printf("%d ", V2[i]);
		printf("\n");
	}
}

int main(int argc, char * argv[]){
	time_t begin, end;
	double OptCost = 0, eCost[NoE] = {0};
	int ans[NoE];
	double runtime[NoE];
	int EE = NoE;
	if(argc != 1){
		for(int i = 1; i < argc; i++){
			if(argv[i][1] == 'm')
				n = atoi(argv[i + 1]);
			if(argv[i][1] == 'e')
				EE = 1, e[0] = atof(argv[ i + 1]);
			if(argv[i][1] == 'f'){
				for(int j = 0; argv[i + 1][j] != '\0'; j++)
					filepath[j] = argv[i + 1][j];
				f = 1;
			}
			if(argv[i][1] == 'r')
				Recurence = atoi(argv[ i + 1]), f = 0;
			if(argv[i][1] == 'v')
				verbose = 0;
		}
	}
	Good G, S;
	V1 = (int *)malloc(sizeof(int) * (n + 1));
	V2 = (int *)malloc(sizeof(int) * (n + 1));
	V1backup = (int *)malloc(sizeof(int) * (n + 1));
	V2backup = (int *)malloc(sizeof(int) * (n + 1));
	for(int tt = 0; tt < Recurence; tt ++){	
		Init();
		G.SetFull(n);
		Copy(V2backup, V2), Copy(V1backup, V1);	
		begin = clock();
/*		if(verbose)	printf("****************optimal is:**********************************\n");
		S = DP(G, MMS1, q);
		if(verbose)
		{
			if(S.Exist())
				Print(S);
			else
				printf("No valid solution!\n");
		}
		end = clock();
		OptCost += double(end-begin);
		if(verbose)	printf("Compute optimal solution costs %f seconds\n", double(end-begin)/CLOCKS_PER_SEC);
*/		
		for(int i = 0; i < EE; i ++){
			if(verbose)	printf("*******************the case of e = %f*******************\n", e[i]);
			begin = clock();
			G.SetFull(n);
			Copy(V2backup, V2), Copy(V1backup, V1);	
			
			int count = 0, p = Mini(V2backup);
			while(true)
			{
				S = Scaling(G, MMS1, e[i], p);
				if(S.Exist())
				{
					ans[i] = V2sum - S.Weight(V2backup);
					if(verbose)	printf("p = %d\n", p);
					if(verbose)	Print(S);
					break;
				}else{
					if(verbose)	printf("p = %d, no valid solution\n", p);
				}
				p *= 2;
				count ++;
				if(count > 10)
				{
					if(verbose) printf("iteration to many times!\n\n");
					break;
				}
			}

			end = clock();
			eCost[i] += double(end - begin);
			runtime[i] = double(end-begin)/CLOCKS_PER_SEC;
			if(verbose)	printf("***************e=%f costs %f seconds**************\n", e[i], double(end-begin)/CLOCKS_PER_SEC);
		}
	}
	if(verbose && Recurence != 1){
		printf("Total time of %d recurance:\n", Recurence);
		for(int i = 0; i < EE; i++){
			printf("e: %f, cost %f seconds\n", e[i], eCost[i] / CLOCKS_PER_SEC);
		}
	}
	if(Recurence != 1){
//		printf("\nTotal time:\n Optimal costs %f seconds\n", OptCost / CLOCKS_PER_SEC);
		printf("e\t\tMean time(s)\n");
		for(int i = 0; i < EE; i++){
			printf("%f\t%f\n", e[i], eCost[i] /(CLOCKS_PER_SEC * EE));
		}
	}else{
		printf("e\t\tsolution\ttime(s)\n");
		for(int i = 0; i < EE; i++)
			printf("%f\t%d\t%f\n", e[i], ans[i], runtime[i]);
/*		for(int i = 0; i < EE; i ++){
			printf("%f\t", e[i]);
		}
		printf("\nn=%d\n", n);
		for(int i = 0; i < EE; i ++){
			printf("%d\t", ans[i]);
		}
		printf("\n");
		for(int i = 0; i < EE; i++){
			printf("%f\t", runtime[i]);
		}
		printf("\n");
*/
	}
	return 0;	
}
