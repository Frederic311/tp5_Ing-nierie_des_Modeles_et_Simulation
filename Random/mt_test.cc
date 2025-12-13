// Générer des nombres dans un fichier binaire (pour test DIE HARD de G. Marsaglia) 
 
#include <sys/types.h> 
#include <sys/stat.h> 
 
#include <fcntl.h>   
#include <limits.h> 
#include <unistd.h>  
 
#include "CLHEP/Random/MTwistEngine.h" // Entête pour MT dans la bibliothèque CLHEP 
 
int main() 
{ 
   CLHEP::MTwistEngine * mtRng = new CLHEP::MTwistEngine();   
 
   int          fId;  
   double       fRn; 
   unsigned int iRn;    
 
   // Fichier binaire pour tests Die Hard  
   fId = open("./qrngb",O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR); 
 
   // Le test de Die Hard est sur 3 millions de nombres 
   for(int i = 1; i < 3000000; i++) 
   { 
      fRn = mtRng->flat(); // Génération uniforme avec MT 
      iRn = (unsigned int) (fRn * UINT_MAX); 
 
      write(fId, &iRn, sizeof(unsigned int)); 
   } 
 
   close(fId); 
 
   delete mtRng; 
 
   return 0; 
}