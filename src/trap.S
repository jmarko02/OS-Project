.extern _ZN5Riscv18handleExcEcallTrapEv
.extern _ZN5Riscv15handleTimerTrapEv
.extern _ZN5Riscv18handleExternalTrapEv
.align 4
.global _ZN5Riscv14supervisorTrapEv
.type _ZN5Riscv14supervisorTrapEv, @function
_ZN5Riscv14supervisorTrapEv:
j ExceptionAndEcall
j Timer
.skip 28
j External
ExceptionAndEcall:
#iz nekog razloga kada je External bio odmah ispod Timer, iz Timer je propadalo u External samo prvi put,
#a kada bi clearovanje sip-a bilo zakomentarisano u handleru za Timer onda ne bi propadalo tj uvek bi se vrteo u tajmeru
#zato ovo moze biti stavljeno izmedju Timer i External
#ako se stavi tako - obrati paznju da li ce biti problema kod testiranja sis poziva (da li ce tada propadati ili nesto slicno)
    addi sp,sp, -256
    .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    sd x\index , \index * 8(sp)
    .endr

    call _ZN5Riscv18handleExcEcallTrapEv

    .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    ld x\index , \index * 8(sp)
    .endr
    addi sp,sp, 256
    sret

Timer:

    addi sp,sp, -256
    .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    sd x\index , \index * 8(sp)
    .endr

    call _ZN5Riscv15handleTimerTrapEv

    .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    ld x\index , \index * 8(sp)
    .endr
    addi sp,sp, 256
    sret
#sip reg je sa 0x202 clearovan na 0x200 u ovom slucaju, pa kad se odradi console_handler onda bude 0 i ne ulazi vise u External
#ako je ExceptionEcall labela i kod izmedju ova 2, onda se iz nekog razloga sip clearuje odmah na 0 i nikada ne ulazi u External
External:

    addi sp,sp, -256
    .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    sd x\index , \index * 8(sp)
    .endr

    call _ZN5Riscv18handleExternalTrapEv

    .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    ld x\index , \index * 8(sp)
    .endr
    addi sp,sp, 256
    sret