#include <iostream>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

struct max_loc {
    int max;
    int pos;
};


void generation_aleatoire(int n, int *tab, int graine, int max) {
    srand(time(NULL) + graine);
    //srand(2*graine+10);
    for (int i = 0; i < n; i++)
        tab[i] = rand() % (max + 1);
}

struct max_loc* calcul_max(int n, int* tab) {
  // à compléter
	int pid, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	max_loc *ml = new max_loc;
	int max, ind;
  	max = 0;
  	for (int i=0; i<n; i++) {
  	  if ( tab[i] > max){
  	    max = tab[i];
  	    ind = i;
   	  }
  	}
  	ml->max = max;
  	if (pid < (n % nprocs)){ // A VOIR !
  		ml->pos = ind + (pid * (n / nprocs));
  	} else {
  		ml->pos = ind + (pid * (n / (nprocs-1)));
  	}
  	

  	return ml;

}

int main(int argc, char **argv) {
    int pid, nprocs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Request request;
    MPI_Status status;

    int n = atoi(argv[1]); // la taille du tableau global
    int root = atoi(argv[2]); // le processeur root

    int *tab_global;

    if (pid == root) {
        tab_global = new int[n];
        generation_aleatoire(n, tab_global, pid, 50);
        cout << "je suis :" << pid << " et le tableau initial est ";
        for (int i = 0; i < n; i++)
            cout << tab_global[i] << " ";
        cout << endl;
    }

    // à compléter avec la distribution du tableau et le calcul du max associé à sa position dans le tableau global
    int* tab_nbElem = new int[nprocs];
    int* tab_indiceElem = new int[nprocs];

    int q = n / nprocs;
    int r = n % nprocs;

     for (int i = 0; i < nprocs; ++i) {
     	if ( i < r ){
     		tab_nbElem[i] = q + 1;
     		tab_indiceElem[i] = (q + 1) * i;
     	} else {
     		tab_nbElem[i] = q;
     		tab_indiceElem[i] = (q*i)+r;
     	}
    }		
    int* tab_recv = new int[q+1];
    int qq = q +1;
    MPI_Scatterv(tab_global,tab_nbElem,tab_indiceElem,MPI_INT, tab_recv,qq, MPI_INT,root,MPI_COMM_WORLD);
    max_loc *mlsend;
    if(pid < r){
    	mlsend = calcul_max(qq,tab_recv);
    } else {
    	mlsend = calcul_max(q,tab_recv);
    }

    max_loc *mlrecv = new max_loc;

    MPI_Reduce(mlsend, mlrecv, 1,MPI_2INT, MPI_MAXLOC,root,MPI_COMM_WORLD);
    if (pid  == root){
    	cout << "Le max global est " << mlrecv->max << " et sa position est " << mlrecv->pos << endl;

    }


    MPI_Finalize();
    return 0;
}
