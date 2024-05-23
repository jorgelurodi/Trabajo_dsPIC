;.....
;    File   pasobanda_Hamming.s 
;.....

    .equ pasobanda_HammingNumTaps, 61

;.....
; Allocate and initialize filter taps

		.section .pasobanda_Hammingconst, "x"
		.align 128
pasobanda_HammingTaps:
.hword 0xfffe, 0xffc3, 0xff86, 0xff48, 0xff11, 0xfeee, 0xfef1, 0xff2a, 0xffa3 
.hword 0x005a, 0x013d, 0x022c, 0x02fd, 0x037f, 0x0388, 0x02fe, 0x01dd, 0x003c 
.hword 0xfe4f, 0xfc5d, 0xfab7, 0xf9ab, 0xf972, 0xfa29, 0xfbc6, 0xfe18, 0x00cf 
.hword 0x0386, 0x05d5, 0x0760, 0x07eb, 0x0760, 0x05d5, 0x0386, 0x00cf, 0xfe18 
.hword 0xfbc6, 0xfa29, 0xf972, 0xf9ab, 0xfab7, 0xfc5d, 0xfe4f, 0x003c, 0x01dd 
.hword 0x02fe, 0x0388, 0x037f, 0x02fd, 0x022c, 0x013d, 0x005a, 0xffa3, 0xff2a 
.hword 0xfef1, 0xfeee, 0xff11, 0xff48, 0xff86, 0xffc3, 0xfffe 

;.....
; Allocate delay line in (uninitialized) Y data space

		.section .ybss,  "b"
		.align 128

pasobanda_HammingDelay:
		.space pasobanda_HammingNumTaps*2

; .....
; Allocate and intialize filter structure

		.section .data
		.global _pasobanda_HammingFilter

_pasobanda_HammingFilter:
.hword pasobanda_HammingNumTaps
.hword psvoffset(pasobanda_HammingTaps)
.hword psvoffset(pasobanda_HammingTaps)+pasobanda_HammingNumTaps*2-1
.hword psvpage(pasobanda_HammingTaps)
.hword pasobanda_HammingDelay
.hword pasobanda_HammingDelay+pasobanda_HammingNumTaps*2-1
.hword pasobanda_HammingDelay

; ....................................
; ....................................
