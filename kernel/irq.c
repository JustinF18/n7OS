#include <inttypes.h>
#include <n7OS/irq.h>

void init_irq_entry(int irq_num, uint32_t addr) {
    idt_entry_t * entry = (idt_entry_t *) &idt[irq_num];
    entry->offset_inf = addr & 0xFFFF;
    entry->sel_segment = KERNEL_CS;
    entry->zero = 0;
    entry->type_attr = 0b10001110;
    entry->offset_sup = addr >> 16;
}
