
#include <iostream> 
#include <pthread.h> 
#include <vector> 
#define dimension 3 
#define max_threads 10

using namespace std; 

// input matrix
int mat[][dimension] = { { 2, -1, 5 }, 
					{ 1, 3, 2 }, 
					{ 4, 0, 6 } }; 

int det[dimension]; //to store determinant values of submatrices
pthread_t thread_pool1[max_threads];
pthread_t thread_pool2[max_threads]; 




//============== Determinant Sectioin =================================//

// function to compute determinants of submatrices
int determinant(vector<vector<int> > mat2, int d) 
{ 

	if (d == 2) { 
		
		// if dimension of matrix is 2X2 
		return mat2[0][0] * mat2[1][1] - 
			mat2[0][1] * mat2[1][0]; 
	} 
	else { 
		
		// else divide the matrix into more smaller parts.
		vector<vector<int> > mat1(d - 1), 
					mat3(d - 1), mat4(d - 1); 
		int k, l, m, i, j; 
		
		for (i = 0; i < d - 1; i++) { 
			mat1[i] = vector<int>(d - 1); 
			mat3[i] = vector<int>(d - 1); 
			mat4[i] = vector<int>(d - 1); 
		} 
		
		for (i = 1; i < d; i++) { 
			k = 0; 
			l = 0; 
			m = 0; 
			for (j = 0; j < d; j++) { 
				if (j != 0) { 
					mat1[i - 1][k] = mat2[i][j]; 
					k++; 
				} 
				if (j != 1) { 
					mat3[i - 1][l] = mat2[i][j]; 
					l++; 
				} 
				if (j != 2) { 
					mat4[i - 1][m] = mat2[i][j]; 
					m++; 
				} 
			} 
		} 

		return mat2[0][0] * determinant(mat1, d - 1) - 
			mat2[0][1] * determinant(mat3, d - 1) + 
			mat2[0][2] * determinant(mat4, d - 1); 
	} 
} 

// the funtion which will be used for multithreading. No of threads = dimension of matrix . 
void* determ_thread(void* sub_matrix) 
{ 
	int *sb_matrix = (int *)sub_matrix, i, j, k; 
	vector<vector<int> > mat2(dimension - 1); 

	for (i = 0; i < dimension - 1; i++) 
		mat2[i] = vector<int>(dimension - 1); 
		
	// logic to find the elements of the submatrix
	for (i = 1; i < dimension; i++) { 
		k = 0; 
		for (j = 0; j < dimension; j++) { 
			if (j != (*sb_matrix)) { 
				mat2[i - 1][k] = mat[i][j]; 
				k++; 
			} 
		} 
	} 
	
	// to find the determinant
	det[*sb_matrix] = det[*sb_matrix] * determinant(mat2, dimension - 1); 

    void *p;
    return p;
} 


void* calculate_determinant(void *arg){

    int i, j, detfin = 0; 
	int p[dimension]; 
	
	// storing the first row in a array 
	// for later multipying with the determinant 
	// of smaller matrix 
	for (i = 0; i < dimension; i++) 
		det[i] = mat[0][i]; 
		
	// creating threads to calculate determinants of submatrices
	for (i = 0; i < dimension; i++) { 
		p[i] = i; 
		pthread_create(&thread_pool2[i], NULL, &determ_thread, (void*)&p[i]); 
	} 
    //join all threads
	for (i = 0; i < dimension; i++) { 
		pthread_join(thread_pool2[i], NULL); 

	} 
    //compute final determinant from determinants of submatrices
	for (i = 0; i < dimension; i++) { 
		if (i % 2 == 0) 
			detfin += det[i]; 
		else
			detfin -= det[i]; 
	} 
	
	cout <<"Determinant of the matrix is "<< detfin << endl; 
}


// =================== Transpose Section =========================//



void* calculate_transpose(void *arg){
    
    vector<vector<int> > trans = {
        {0,0,0},
        {0,0,0},
        {0,0,0} //initialise with dummy values
    };

    for(int i=0;i< dimension;i++){
        for(int j=0;j< dimension;j++)
            trans[j][i] = mat[i][j]; //where mat[][] is the global matrix 
    }
    cout <<"\n The transpose of the matrix is \n ";
    for(int i=0;i<dimension; i++){

        for(int j=0;j<dimension;j++)   
            cout<<trans[i][j]<<"  ";
        cout<<"\n";
    }

}

// driver function 
int main() 
{ 
	pthread_create(&thread_pool1[0], NULL, &calculate_determinant, NULL); 
    pthread_create(&thread_pool1[1], NULL, &calculate_transpose, NULL); 

	pthread_join(thread_pool1[0], NULL);
    pthread_join(thread_pool1[1], NULL);
	return 0;  
} 
