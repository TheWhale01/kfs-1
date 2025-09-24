#!/bin/bash

set -e

if [ $# -le 1 ]; then
    echo "Usage: create_isr.sh <isrs_gen.c> <isrs_gen.s>"
    exit 1
fi

ISRS_GEN_C=$1
ISRS_GEN_ASM=$2
ISRS_WITH_ERROR_CODE="8 10 11 12 13 14 17 21"


echo "; !!! AUTO GENERATED FILE DO NOT MODIFY !!!" > $ISRS_GEN_ASM
echo "// !!! AUTO GENERATED FILE DO NOT MODIFY !!!
#include \"idt.h\"
" > $ISRS_GEN_C

for i in $(seq 0 255); do
    echo "extern void isr${i}(void);" >> $ISRS_GEN_C
done

echo "void init_idt_gates(void) {" >> $ISRS_GEN_C

for i in $(seq 0 255); do
    if echo "$ISRS_WITH_ERROR_CODE" | grep -q "\b${i}\b"; then
        echo "ISR_ERRORCODE ${i}" >> $ISRS_GEN_ASM
    else
        echo "ISR_NOERRORCODE ${i}" >> $ISRS_GEN_ASM
    fi
    echo "    set_idt_gate(${i}, &isr${i}, 0x08, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT);" >> $ISRS_GEN_C
done

echo "}" >> $ISRS_GEN_C
