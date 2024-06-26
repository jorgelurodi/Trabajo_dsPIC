;.....
;    File   pasobanda_IIR.s 
;.....

    .equ pasobanda_IIRNumTapsB, 90
    .equ pasobanda_IIRNumTapsA, 90

;.....
; Allocate and initialize filter taps

        .section .pasobanda_IIRconst, "x"
        .align 256
pasobanda_IIRTapsB:
.hword 0x7fff, 0x0000, 0x8000, 0x7fff, 0x0000, 0x8000, 0x7fff, 0x0000, 0x8000 
.hword 0x7fff, 0x0000, 0x8000, 0x7fff, 0x0000, 0x8000, 0x7fff, 0x0000, 0x8000 
.hword 0x7fff, 0x0000, 0x8000, 0x7fff, 0x0000, 0x8000, 0x7fff, 0x0000, 0x8000 
.hword 0x7fff, 0x0000, 0x8000, 0x7fff, 0x0000, 0x8000, 0x7fff, 0x0000, 0x8000 
.hword 0x7fff, 0x0000, 0x8000, 0x7fff, 0x0000, 0x8000, 0x7fff, 0x0000, 0x8000 
.hword 0x7fff, 0x0000, 0x8000, 0x7fff, 0x0000, 0x8000, 0x7fff, 0x0000, 0x8000 
.hword 0x7fff, 0x0000, 0x8000, 0x7fff, 0x0000, 0x8000, 0x7fff, 0x0000, 0x8000 
.hword 0x7fff, 0x0000, 0x8000, 0x7fff, 0x0000, 0x8000, 0x7fff, 0x0000, 0x8000 
.hword 0x7fff, 0x0000, 0x8000, 0x7fff, 0x0000, 0x8000, 0x7fff, 0x0000, 0x8000 
.hword 0x7fff, 0x0000, 0x8000, 0x7fff, 0x0000, 0x8000, 0x7fff, 0x0000, 0x8000 

pasobanda_IIRTapsA:
.hword 0x7fff, 0x8000, 0x7fdd, 0x7fff, 0x8000, 0x7fdf, 0x7fff, 0x8000, 0x7f97 
.hword 0x7fff, 0x8000, 0x7f9c, 0x7fff, 0x8000, 0x7f52, 0x7fff, 0x8000, 0x7f5b 
.hword 0x7fff, 0x8000, 0x7f10, 0x7fff, 0x8000, 0x7f1b, 0x7fff, 0x8000, 0x7ed0 
.hword 0x7fff, 0x8000, 0x7ede, 0x7fff, 0x8000, 0x7e94, 0x7fff, 0x8000, 0x7ea4 
.hword 0x7fff, 0x8000, 0x7e5d, 0x7fff, 0x8000, 0x7e6d, 0x7fff, 0x8000, 0x7e2a 
.hword 0x7fff, 0x8000, 0x7e3b, 0x7fff, 0x8000, 0x7dfd, 0x7fff, 0x8000, 0x7e0d 
.hword 0x7fff, 0x8000, 0x7dd6, 0x7fff, 0x8000, 0x7de5, 0x7fff, 0x8000, 0x7db5 
.hword 0x7fff, 0x8000, 0x7dc2, 0x7fff, 0x8000, 0x7d9a, 0x7fff, 0x8000, 0x7da6 
.hword 0x7fff, 0x8000, 0x7d87, 0x7fff, 0x8000, 0x7d8f, 0x7fff, 0x8000, 0x7d7b 
.hword 0x7fff, 0x8000, 0x7d80, 0x7fff, 0x8000, 0x7d75, 0x7fff, 0x8000, 0x7d77 

;.....
; Allocate delay line in (uninitialized) Y data space

        .section .ybss,  "b"
        .align 256

pasobanda_IIRDelay:
        .space (pasobanda_IIRNumTapsB+pasobanda_IIRNumTapsA)*2

; .....
; Allocate and initialize filter structure

        .section .data
        .global _pasobanda_IIRFilter

_pasobanda_IIRFilter:
.hword pasobanda_IIRNumTapsB
.hword pasobanda_IIRNumTapsA
.hword psvoffset(pasobanda_IIRTapsB)
.hword psvoffset(pasobanda_IIRTapsB)+pasobanda_IIRNumTapsB*2-1
.hword psvoffset(pasobanda_IIRTapsA)
.hword psvoffset(pasobanda_IIRTapsA)+pasobanda_IIRNumTapsA*2-1
.hword psvpage(pasobanda_IIRTapsB)
.hword pasobanda_IIRDelay
.hword pasobanda_IIRDelay+(pasobanda_IIRNumTapsB+pasobanda_IIRNumTapsA)*2-1
.hword pasobanda_IIRDelay

; ....................................
; ....................................
