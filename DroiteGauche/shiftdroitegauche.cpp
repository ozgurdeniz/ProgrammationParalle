#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

void shift_droite_gauche_circulaire(int n, int* in, int* out){
  //Declatarion & Initialisation
  int pid, nprocs;  
  MPI_Status status;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid ) ;
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs ) ;

  //Declaration & Initialisation des constants
  int taille = n/2;
  int droite = (pid +1)%nprocs;
  int gauche = (pid -1 + nprocs )%nprocs;

  //Declaration des tableaux
  int *tabGaucheRecv = new int[taille];
  int *tabDroiteRecv = new int[taille];
  int *tabGaucheEnvoi = new int[taille];
  int *tabDroiteEnvoi = new int[taille];
  
  //Initialisation des tableaux a envoyer
  for (int i = 0; i < taille; ++i){
    tabGaucheEnvoi[i] = in[i];   
    tabDroiteEnvoi[i] = in[i+taille];
  }

  //Echanges entre processeurs
  if( nprocs % 2 == 1 ) {
    if ((nprocs % 2 == 1) && (pid == 0)) {
      MPI_Recv(tabDroiteRecv,taille,MPI_INT,droite,0,MPI_COMM_WORLD,&status);
      MPI_Send(tabGaucheEnvoi,taille,MPI_INT,gauche,0,MPI_COMM_WORLD);
      MPI_Send(tabDroiteEnvoi,taille,MPI_INT,droite,0,MPI_COMM_WORLD);
      MPI_Recv(tabGaucheRecv,taille,MPI_INT,gauche,0,MPI_COMM_WORLD,&status);
    } else if (pid % 2 == 0){
      MPI_Recv(tabDroiteRecv,taille,MPI_INT,droite,0,MPI_COMM_WORLD,&status);
      MPI_Recv(tabGaucheRecv,taille,MPI_INT,gauche,0,MPI_COMM_WORLD,&status);
      MPI_Send(tabDroiteEnvoi,taille,MPI_INT,droite,0,MPI_COMM_WORLD);
      MPI_Send(tabGaucheEnvoi,taille,MPI_INT,gauche,0,MPI_COMM_WORLD);
    } else {
      MPI_Send(tabGaucheEnvoi,taille,MPI_INT,gauche,0,MPI_COMM_WORLD);
      MPI_Send(tabDroiteEnvoi,taille,MPI_INT,droite,0,MPI_COMM_WORLD);
      MPI_Recv(tabGaucheRecv,taille,MPI_INT,gauche,0,MPI_COMM_WORLD,&status);   
      MPI_Recv(tabDroiteRecv,taille,MPI_INT,droite,0,MPI_COMM_WORLD,&status);
    }
  } else {
    if (pid % 2 == 0){
      MPI_Send(tabDroiteEnvoi,taille,MPI_INT,droite,0,MPI_COMM_WORLD);
      MPI_Recv(tabDroiteRecv,taille,MPI_INT,droite,0,MPI_COMM_WORLD,&status);
      MPI_Send(tabGaucheEnvoi,taille,MPI_INT,gauche,0,MPI_COMM_WORLD);
      MPI_Recv(tabGaucheRecv,taille,MPI_INT,gauche,0,MPI_COMM_WORLD,&status);
      } else {
      MPI_Recv(tabGaucheRecv,taille,MPI_INT,gauche,0,MPI_COMM_WORLD,&status);   
      MPI_Send(tabGaucheEnvoi,taille,MPI_INT,gauche,0,MPI_COMM_WORLD);
      MPI_Recv(tabDroiteRecv,taille,MPI_INT,droite,0,MPI_COMM_WORLD,&status);
      MPI_Send(tabDroiteEnvoi,taille,MPI_INT,droite,0,MPI_COMM_WORLD);
    }
  }

  //Remplissages du tableau final
  for (int i = 0; i < taille; ++i){
    out[i] = tabGaucheRecv[i];
    out[i+taille] = tabDroiteRecv[i];
  }
}

void shift_droite_gauche(int n, int* in, int* out){
  //Declatarion & Initialisation
  int pid, nprocs;  
  MPI_Status status;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid ) ;
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs ) ;

  //Declaration & Initialisation des constants
  int taille = n/2;
  int droite = (pid +1)%nprocs;
  int gauche = (pid -1 + nprocs )%nprocs;

  //Declaration des tableaux
  int *tabGaucheRecv = new int[taille];
  int *tabDroiteRecv = new int[taille];
  int *tabGaucheEnvoi = new int[taille];
  int *tabDroiteEnvoi = new int[taille];

  //Initialisation des tableaux a envoyer
  for (int i = 0; i < taille; ++i){
    tabGaucheEnvoi[i] = in[i];   
    tabDroiteEnvoi[i] = in[i+taille];
  }

  //Echanges entre processeurs
  if (pid % 2 == 0){
    if (pid == 0){
      MPI_Send(tabDroiteEnvoi,taille,MPI_INT,droite,0,MPI_COMM_WORLD);
      MPI_Recv(tabDroiteRecv,taille,MPI_INT,droite,0,MPI_COMM_WORLD,&status);
      tabGaucheRecv = tabGaucheEnvoi;
    } else {
      MPI_Send(tabGaucheEnvoi,taille,MPI_INT,gauche,0,MPI_COMM_WORLD);
      MPI_Recv(tabGaucheRecv,taille,MPI_INT,gauche,0,MPI_COMM_WORLD,&status);
      MPI_Send(tabDroiteEnvoi,taille,MPI_INT,droite,0,MPI_COMM_WORLD);
      MPI_Recv(tabDroiteRecv,taille,MPI_INT,droite,0,MPI_COMM_WORLD,&status);
    }
  } else {
    if ( pid == nprocs-1){
      MPI_Recv(tabGaucheRecv,taille,MPI_INT,gauche,0,MPI_COMM_WORLD,&status);
      MPI_Send(tabGaucheEnvoi,taille,MPI_INT,gauche,0,MPI_COMM_WORLD);
      tabDroiteRecv = tabDroiteEnvoi;
    } else {
      MPI_Recv(tabGaucheRecv,taille,MPI_INT,gauche,0,MPI_COMM_WORLD,&status);
      MPI_Send(tabGaucheEnvoi,taille,MPI_INT,gauche,0,MPI_COMM_WORLD);
      MPI_Recv(tabDroiteRecv,taille,MPI_INT,droite,0,MPI_COMM_WORLD,&status);
      MPI_Send(tabDroiteEnvoi,taille,MPI_INT,droite,0,MPI_COMM_WORLD);
    }
  }

  //Remplissages du tableau final
  for (int i = 0; i < taille; ++i){
    out[i] = tabGaucheRecv[i];
    out[i+taille] = tabDroiteRecv[i];
  }
}

int main ( int argc , char **argv ) {
  int pid, nprocs;  
  MPI_Status status;
  MPI_Init (&argc , &argv) ;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid ) ;
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs ) ;
  
  int nlocal = 2*atoi(argv[1]);
  int* tablocal = new int[nlocal];
  int* tabres = new int[nlocal];

  srand(time(NULL)+pid);
  for (int i=0; i<nlocal; i++)
    tablocal[i] = rand()%10;
  
  cout << "je suis " << pid << " et j'ai le tableau initial : ";
  for (int i=0; i<nlocal; i++)
    cout << tablocal[i] << " ";
  cout << endl;


  //shift_droite_gauche(nlocal,tablocal,tabres);
  shift_droite_gauche_circulaire(nlocal,tablocal,tabres);
  
  cout << "je suis " << pid << " et j'ai le tableau final : ";
  for (int i=0; i<nlocal; i++)
    cout << tabres[i] << " ";
  cout << endl;
  
  delete[] tablocal;
  delete[] tabres;
  MPI_Finalize() ;
  return 0 ;
}
