#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

/*! 
 * Pour tester si une suite est une suite de syracuse (à partir du premier élément du tableau)
 * \param n la taille du tableau (la suite)
 * \param tab la suite à tester
 * \return 0 si ce n'est pas une suite de Syracuse 1 sinon
 */
int test_syracuse(int n, int* tab){
  if (tab[0]<=0){
    return 0;
  }
  for (size_t i = 1; i < n; i++) {
    if (tab[i-1]%2 ==0){
      if(tab[i]!=tab[i-1]/2) {
        return 0;
      }
    }else{
      if(tab[i] !=(3*tab[i-1]+1)){
        return 0;
      }
    }
  }
  return 1;
}


/*!
 * génération d'une suite de Syracuse à partir de U0=x
 * \param n la taille voulue
 * \param tab le tableau qui enregistre la suite
 * \param x la valeur initiale
 */
void syracuse(int n, int* tab, int x){
  tab[0] = x;
  for (int i=1; i<n; i++) {
    if (tab[i-1]%2==0)
      tab[i] = tab[i-1]/2;
    else
      tab[i] = 3*tab[i-1]+1;
  }
}

/*!
 * génération aléatoire d'une suite quelconque
 * \param n la taille voulue
 * \param tab le tableau qui enregistre la suite
 * \param graine pour le processus de génération aléatoire
 */
void non_syracuse(int n, int* tab, int graine){
  srand(time(NULL)+graine);
  //srand(2*graine+10);  
  for (int i=0; i<n; i++) 
    tab[i] = rand()%50;
}

int main ( int argc , char **argv ){
  ///DEBUT SQUELETTE///
  int pid, nprocs;  
  MPI_Init (&argc , &argv) ;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid ) ;
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs ) ;
  MPI_Request request;
  MPI_Status status;
  
  int n = atoi(argv[1]); // la taille du tableau global
  int root = atoi(argv[2]); // le processeur root
  int x = atoi(argv[3]); // le x pour générer une suite de syracuse
  int cas = atoi(argv[4]);  // les différentes générations possibles
  
  int* tab_global = new int[n];
  int* tab_recv = new int[n / nprocs +1];
  if (pid==root) {
    switch(cas) {
    case 0:
      cout << "test génération aléatoire"<< endl;
      non_syracuse(n,tab_global,pid);
      break;
    case 1:
      cout << "test syracuse partiel"<< endl;
      syracuse(n/2,tab_global,x); 
      non_syracuse(n-n/2,(tab_global+n/2),pid);
      break;
    default:
      cout << "test syracuse"<< endl;
      syracuse(n,tab_global,x);
    }
  }
  if (pid==root){
    cout << "je suis le processeur : "  << pid << "\nLe tableau est : ";
    for (int i=0; i<n; i++)
      cout << tab_global[i] << " ";
    cout << endl;
  }
  ///FIN SQUELETTE///

  // Declaration du..
  int* tab_nbElem = new int[nprocs]; //Tableau contenant le nb element de chaque proc
  int* tab_indiceElem = new int[nprocs]; //Tableau contenant l'indice de déb de chaque proc

  // Le quotion & le reste de DivEuc
  int q = n / nprocs;
  int r = n % nprocs;

  //Remplissages des tableaux avec nbElem et Indice
  for (int i = 0; i < nprocs; ++i) {
    if ( i < r ){
      tab_nbElem[i] = q + 1;
      tab_indiceElem[i] = (q + 1) * i;
    } else {
      tab_nbElem[i] = q;
      tab_indiceElem[i] = (q*i)+r;
    }
  }

  //Gestion des ghosts
  for (int i = 1; i < nprocs; ++i) {
		tab_nbElem[i] += 1;
		tab_indiceElem[i] -= 1;
  }

  //Envoi du tableau global aux processeurs
	MPI_Scatterv(tab_global,tab_nbElem,tab_indiceElem,MPI_INT,tab_recv,tab_nbElem[pid], MPI_INT,root,MPI_COMM_WORLD);

  //Reception du resultat par processeur
	int response = test_syracuse(tab_nbElem[pid],tab_recv);

  //Decision finale
  int res;
  MPI_Reduce(&response, &res, 1,MPI_INT, MPI_MIN,root,MPI_COMM_WORLD);

  //Affichage sur le processeur root
  if (pid==root) {
    cout << "test de syracuse : " << res << endl;
  }
  
  MPI_Finalize() ;
  return 0 ;
}
