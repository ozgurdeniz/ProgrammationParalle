#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;



void shift_circulaire_tab(int n, int* in, int* out)
{
  int pid, nprocs;  
  MPI_Comm_rank(MPI_COMM_WORLD, &pid );
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs );
  MPI_Status status;

  int pid_envoi = (pid+1)%nprocs;
  int pid_recu = ((pid-1)+nprocs)%nprocs;
  
  if (pid % 2 == 0) {
    MPI_Send(in,n,MPI_INT,pid_envoi,34,MPI_COMM_WORLD);
    MPI_Recv(out,n,MPI_INT,pid_recu,34,MPI_COMM_WORLD,&status);
  } else {
    MPI_Recv(out,n,MPI_INT,pid_recu,34,MPI_COMM_WORLD,&status);
    MPI_Send(in,n,MPI_INT,pid_envoi,34,MPI_COMM_WORLD);
  }
}


int main ( int argc , char **argv )
{
  int pid, nprocs;  
  MPI_Status status;
  MPI_Init (&argc , &argv) ;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid ) ;
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs ) ;
  
  int nlocal = atoi(argv[1]);
  int* tablocal = new int[nlocal];
  int* tabres = new int[nlocal];

  srand(time(NULL)+pid);
  for (int i=0; i<nlocal; i++)
    tablocal[i] = rand()%10;

  cout << "je suis " << pid << " et j'ai le tableau initial : ";
  for (int i=0; i<nlocal; i++)
    cout << tablocal[i] << " ";
  cout << endl;

  shift_circulaire_tab(nlocal,tablocal,tabres);
  
  cout << "je suis " << pid << " et j'ai le tableau final : ";
  for (int i=0; i<nlocal; i++)
    cout << tabres[i] << " ";
  cout << endl;
  
  delete[] tablocal;
  delete[] tabres;
  MPI_Finalize() ;
  return 0 ;
}
