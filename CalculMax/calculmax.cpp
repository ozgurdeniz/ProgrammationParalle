#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

void calcul_max(int n, int* tab, int root, int* maxtab){
  int max, ind, pid;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid ) ;
  max = 0;
  for (int i=0; i<n; i++) {
    if ( tab[i] > max){
      max = tab[i];
      ind = i;
    }
  }
  maxtab[0] = max;
  maxtab[1] = ind+pid*n;
}

int main ( int argc , char **argv )
{
  int pid, nprocs;  
  MPI_Status status;
  MPI_Init (&argc , &argv) ;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid ) ;
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs ) ;
  
  int nlocal = atoi(argv[1]);
  int root = atoi(argv[2]);
  
  int* tablocal = new int[nlocal];
  
  
  srand(time(NULL)+pid);
  for (int i=0; i<nlocal; i++)
    tablocal[i] = rand()%100;
  
  cout << "je suis " << pid << " et j'ai le tableau initial : ";
  for (int i=0; i<nlocal; i++)
    cout << tablocal[i] << " ";
  cout << endl;
  
  int* maxtab = new int[2];
  calcul_max(nlocal,tablocal,root,maxtab);

  if (pid==root) {
    int* inttab = new int[2];
    for (int i=0; i<nprocs-1; i++){
      MPI_Recv(inttab,2,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);
      if (maxtab[0] < inttab[0]) {
        maxtab[0] = inttab[0];
        maxtab[1] = inttab[1];
      }
    }
    cout << "le max est : " << maxtab[0] << endl;
    cout << "c'est l'élément " << maxtab[1] << " du tableau" << endl;
  } else {
    MPI_Send(maxtab,2,MPI_INT,root,0,MPI_COMM_WORLD);
  }
  
  delete[] tablocal;
  delete[] maxtab;
  MPI_Finalize() ;
  return 0 ;
}
