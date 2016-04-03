$NOMOD51

; <o> PPAGE number <0x0-0xFF> 
; <i> uppermost 256-byte address of the page used for PDATA variables.
PPAGE           EQU     0
;
; <o> SFR address which supplies uppermost address byte <0x0-0xFF> 
; <i> most 8051 variants use P2 as uppermost address byte
PPAGE_SFR       DATA    0A0H
SFR_DE			DATA    0DEH
;

; Standard SFR Symbols 
ACC     DATA    0E0H
B       DATA    0F0H
SP      DATA    81H
DPL     DATA    82H
DPH     DATA    83H

                NAME    SFR_ACCESS
				PUBLIC  _SFR_READ

SFR_ACCESS	   SEGMENT   CODE


                RSEG    SFR_ACCESS

_SFR_READ:
				MOV     DPTR,#SFR_READ_BASE
				JMP		@A+DPTR
SFR_READ_BASE:
``for {set i 0} {$i<256} {incr i} {``
                MOV     A,0`format "%02X" $i`H
				MOV     R7, A
				RET
``}``
                END
