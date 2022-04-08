#include <unistd.h>

/* code de la fonction
   int example();
   pas d'argument -> appel à la fonction d'enveloppe syscall0
*/
syscall0(int, example)
