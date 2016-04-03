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
				PUBLIC  _SFR_WRITE

SFR_ACCESS	   SEGMENT   CODE


                RSEG    SFR_ACCESS
				
;---- Variable 'sfr_addr' assigned to Register 'R7' ----
_SFR_READ:
				MOV     DPTR,#SFR_READ_BASE
				MOV     A,R7
				MOV     B,#04H
				MUL     AB
				ADD     A,DPL
				MOV     DPL,A
				MOV     A,DPH
				ADDC    A,B
				MOV     DPH,A
				CLR		A
				JMP		@A+DPTR
SFR_READ_BASE:
``for {set i 0} {$i<256} {incr i} {``
                MOV     A,0`format "%02X" $i`H
				MOV     R7, A
				RET
``}``

;---- Variable 'value' assigned to Register 'R5' ----
;---- Variable 'sfr_addr' assigned to Register 'R7' ----
_SFR_WRITE:
				MOV     DPTR,#SFR_WRITE_BASE
				MOV     A,R7
				MOV     B,#04H
				MUL     AB
				ADD     A,DPL
				MOV     DPL,A
				MOV     A,DPH
				ADDC    A,B
				MOV     DPH,A
				CLR		A
				JMP		@A+DPTR
SFR_WRITE_BASE:
``for {set i 0} {$i<256} {incr i} {``
                MOV     A, R5
				MOV     0`format "%02X" $i`H,A
				RET
``}``
                END
