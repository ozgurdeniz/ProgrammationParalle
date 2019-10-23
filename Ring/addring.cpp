#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int addition_ring(int a, int root){
  int pid, valeur, nprocs;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs);
  MPI_Status status;
  int* envoi = new int[1];
  int* recu = new int[1];
  if (pid == root){
    envoi[0] = a;
    MPI_Send(envoi,1,MPI_INT,(pid +1)%nprocs,0,MPI_COMM_WORLD);
    MPI_Recv(recu,1,MPI_INT,(pid -1)%nprocs,0,MPI_COMM_WORLD,&status);
    valeur = recu[0];
  } else {
    MPI_Recv(recu,1,MPI_INT,(pid -1)%nprocs,0,MPI_COMM_WORLD,&status);
    envoi[0] = a + recu[0];
    MPI_Send(envoi,1,MPI_INT,(pid +1)%nprocs,0,MPI_COMM_WORLD);   
  }
  return valeur;
}

int main ( int argc , char **argv ){
  int pid, nprocs;  
  MPI_Status status;
  MPI_Init (&argc , &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs);
  
  int root = atoi(argv[1]);
  
  srand(time(NULL)+pid);
  int a = rand()%10;

  cout << "je suis " << pid << " et la valeur est " << a << endl;
  
  int somme = addition_ring(a,root);
  if (pid==root){
    cout << "je suis " << pid << " et la somme est " << somme << endl;  
  }
  
  
  MPI_Finalize() ;
  return 0 ;
}
