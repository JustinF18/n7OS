.text

# c'est le nombre total d'appels systeme
nr_syscall = 3

.globl handler_syscall

handler_syscall:
  # %eax contient le numero de l'appel system
  # si %eax > nr_syscall-1 alors erreur
  cmpl $nr_syscall-1, %eax
  ja bad_sys_call
  # %ebx = arg1, %ecx = arg2, %edx = arg3
  # on empile pour la fonction de traitement de l'AS
  pushl %edx
  pushl %ecx
  pushl %ebx
  # appel à la fonction de traitement de l'AS : syscall_table[i] (i==%eax)
  call *syscall_table(,%eax,4)
  # on re-dépile
  popl %ebx
  popl %ecx
  popl %edx
iret

bad_sys_call:
  # on retourne -1 = erreur
	movl $-1,%eax
	iret
