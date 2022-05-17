.text
# cette directive sert a rendre l'etiquette publique
    .globl handler_TIMER
# debut du traitant
handler_TIMER:
# sauvegarde des registres importants
    pushl %eax
    pushl %edx
    pushl %ecx
# appel a la fonction C realisant le traitant
    call handler_timer
# restauration des registres importants
    popl %ecx
    popl %edx
    popl %eax
# fin du traitant
    iret
