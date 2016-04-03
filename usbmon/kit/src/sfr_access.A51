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
                MOV     A,000H
				MOV     R7, A
				RET
                MOV     A,001H
				MOV     R7, A
				RET
                MOV     A,002H
				MOV     R7, A
				RET
                MOV     A,003H
				MOV     R7, A
				RET
                MOV     A,004H
				MOV     R7, A
				RET
                MOV     A,005H
				MOV     R7, A
				RET
                MOV     A,006H
				MOV     R7, A
				RET
                MOV     A,007H
				MOV     R7, A
				RET
                MOV     A,008H
				MOV     R7, A
				RET
                MOV     A,009H
				MOV     R7, A
				RET
                MOV     A,00AH
				MOV     R7, A
				RET
                MOV     A,00BH
				MOV     R7, A
				RET
                MOV     A,00CH
				MOV     R7, A
				RET
                MOV     A,00DH
				MOV     R7, A
				RET
                MOV     A,00EH
				MOV     R7, A
				RET
                MOV     A,00FH
				MOV     R7, A
				RET
                MOV     A,010H
				MOV     R7, A
				RET
                MOV     A,011H
				MOV     R7, A
				RET
                MOV     A,012H
				MOV     R7, A
				RET
                MOV     A,013H
				MOV     R7, A
				RET
                MOV     A,014H
				MOV     R7, A
				RET
                MOV     A,015H
				MOV     R7, A
				RET
                MOV     A,016H
				MOV     R7, A
				RET
                MOV     A,017H
				MOV     R7, A
				RET
                MOV     A,018H
				MOV     R7, A
				RET
                MOV     A,019H
				MOV     R7, A
				RET
                MOV     A,01AH
				MOV     R7, A
				RET
                MOV     A,01BH
				MOV     R7, A
				RET
                MOV     A,01CH
				MOV     R7, A
				RET
                MOV     A,01DH
				MOV     R7, A
				RET
                MOV     A,01EH
				MOV     R7, A
				RET
                MOV     A,01FH
				MOV     R7, A
				RET
                MOV     A,020H
				MOV     R7, A
				RET
                MOV     A,021H
				MOV     R7, A
				RET
                MOV     A,022H
				MOV     R7, A
				RET
                MOV     A,023H
				MOV     R7, A
				RET
                MOV     A,024H
				MOV     R7, A
				RET
                MOV     A,025H
				MOV     R7, A
				RET
                MOV     A,026H
				MOV     R7, A
				RET
                MOV     A,027H
				MOV     R7, A
				RET
                MOV     A,028H
				MOV     R7, A
				RET
                MOV     A,029H
				MOV     R7, A
				RET
                MOV     A,02AH
				MOV     R7, A
				RET
                MOV     A,02BH
				MOV     R7, A
				RET
                MOV     A,02CH
				MOV     R7, A
				RET
                MOV     A,02DH
				MOV     R7, A
				RET
                MOV     A,02EH
				MOV     R7, A
				RET
                MOV     A,02FH
				MOV     R7, A
				RET
                MOV     A,030H
				MOV     R7, A
				RET
                MOV     A,031H
				MOV     R7, A
				RET
                MOV     A,032H
				MOV     R7, A
				RET
                MOV     A,033H
				MOV     R7, A
				RET
                MOV     A,034H
				MOV     R7, A
				RET
                MOV     A,035H
				MOV     R7, A
				RET
                MOV     A,036H
				MOV     R7, A
				RET
                MOV     A,037H
				MOV     R7, A
				RET
                MOV     A,038H
				MOV     R7, A
				RET
                MOV     A,039H
				MOV     R7, A
				RET
                MOV     A,03AH
				MOV     R7, A
				RET
                MOV     A,03BH
				MOV     R7, A
				RET
                MOV     A,03CH
				MOV     R7, A
				RET
                MOV     A,03DH
				MOV     R7, A
				RET
                MOV     A,03EH
				MOV     R7, A
				RET
                MOV     A,03FH
				MOV     R7, A
				RET
                MOV     A,040H
				MOV     R7, A
				RET
                MOV     A,041H
				MOV     R7, A
				RET
                MOV     A,042H
				MOV     R7, A
				RET
                MOV     A,043H
				MOV     R7, A
				RET
                MOV     A,044H
				MOV     R7, A
				RET
                MOV     A,045H
				MOV     R7, A
				RET
                MOV     A,046H
				MOV     R7, A
				RET
                MOV     A,047H
				MOV     R7, A
				RET
                MOV     A,048H
				MOV     R7, A
				RET
                MOV     A,049H
				MOV     R7, A
				RET
                MOV     A,04AH
				MOV     R7, A
				RET
                MOV     A,04BH
				MOV     R7, A
				RET
                MOV     A,04CH
				MOV     R7, A
				RET
                MOV     A,04DH
				MOV     R7, A
				RET
                MOV     A,04EH
				MOV     R7, A
				RET
                MOV     A,04FH
				MOV     R7, A
				RET
                MOV     A,050H
				MOV     R7, A
				RET
                MOV     A,051H
				MOV     R7, A
				RET
                MOV     A,052H
				MOV     R7, A
				RET
                MOV     A,053H
				MOV     R7, A
				RET
                MOV     A,054H
				MOV     R7, A
				RET
                MOV     A,055H
				MOV     R7, A
				RET
                MOV     A,056H
				MOV     R7, A
				RET
                MOV     A,057H
				MOV     R7, A
				RET
                MOV     A,058H
				MOV     R7, A
				RET
                MOV     A,059H
				MOV     R7, A
				RET
                MOV     A,05AH
				MOV     R7, A
				RET
                MOV     A,05BH
				MOV     R7, A
				RET
                MOV     A,05CH
				MOV     R7, A
				RET
                MOV     A,05DH
				MOV     R7, A
				RET
                MOV     A,05EH
				MOV     R7, A
				RET
                MOV     A,05FH
				MOV     R7, A
				RET
                MOV     A,060H
				MOV     R7, A
				RET
                MOV     A,061H
				MOV     R7, A
				RET
                MOV     A,062H
				MOV     R7, A
				RET
                MOV     A,063H
				MOV     R7, A
				RET
                MOV     A,064H
				MOV     R7, A
				RET
                MOV     A,065H
				MOV     R7, A
				RET
                MOV     A,066H
				MOV     R7, A
				RET
                MOV     A,067H
				MOV     R7, A
				RET
                MOV     A,068H
				MOV     R7, A
				RET
                MOV     A,069H
				MOV     R7, A
				RET
                MOV     A,06AH
				MOV     R7, A
				RET
                MOV     A,06BH
				MOV     R7, A
				RET
                MOV     A,06CH
				MOV     R7, A
				RET
                MOV     A,06DH
				MOV     R7, A
				RET
                MOV     A,06EH
				MOV     R7, A
				RET
                MOV     A,06FH
				MOV     R7, A
				RET
                MOV     A,070H
				MOV     R7, A
				RET
                MOV     A,071H
				MOV     R7, A
				RET
                MOV     A,072H
				MOV     R7, A
				RET
                MOV     A,073H
				MOV     R7, A
				RET
                MOV     A,074H
				MOV     R7, A
				RET
                MOV     A,075H
				MOV     R7, A
				RET
                MOV     A,076H
				MOV     R7, A
				RET
                MOV     A,077H
				MOV     R7, A
				RET
                MOV     A,078H
				MOV     R7, A
				RET
                MOV     A,079H
				MOV     R7, A
				RET
                MOV     A,07AH
				MOV     R7, A
				RET
                MOV     A,07BH
				MOV     R7, A
				RET
                MOV     A,07CH
				MOV     R7, A
				RET
                MOV     A,07DH
				MOV     R7, A
				RET
                MOV     A,07EH
				MOV     R7, A
				RET
                MOV     A,07FH
				MOV     R7, A
				RET
                MOV     A,080H
				MOV     R7, A
				RET
                MOV     A,081H
				MOV     R7, A
				RET
                MOV     A,082H
				MOV     R7, A
				RET
                MOV     A,083H
				MOV     R7, A
				RET
                MOV     A,084H
				MOV     R7, A
				RET
                MOV     A,085H
				MOV     R7, A
				RET
                MOV     A,086H
				MOV     R7, A
				RET
                MOV     A,087H
				MOV     R7, A
				RET
                MOV     A,088H
				MOV     R7, A
				RET
                MOV     A,089H
				MOV     R7, A
				RET
                MOV     A,08AH
				MOV     R7, A
				RET
                MOV     A,08BH
				MOV     R7, A
				RET
                MOV     A,08CH
				MOV     R7, A
				RET
                MOV     A,08DH
				MOV     R7, A
				RET
                MOV     A,08EH
				MOV     R7, A
				RET
                MOV     A,08FH
				MOV     R7, A
				RET
                MOV     A,090H
				MOV     R7, A
				RET
                MOV     A,091H
				MOV     R7, A
				RET
                MOV     A,092H
				MOV     R7, A
				RET
                MOV     A,093H
				MOV     R7, A
				RET
                MOV     A,094H
				MOV     R7, A
				RET
                MOV     A,095H
				MOV     R7, A
				RET
                MOV     A,096H
				MOV     R7, A
				RET
                MOV     A,097H
				MOV     R7, A
				RET
                MOV     A,098H
				MOV     R7, A
				RET
                MOV     A,099H
				MOV     R7, A
				RET
                MOV     A,09AH
				MOV     R7, A
				RET
                MOV     A,09BH
				MOV     R7, A
				RET
                MOV     A,09CH
				MOV     R7, A
				RET
                MOV     A,09DH
				MOV     R7, A
				RET
                MOV     A,09EH
				MOV     R7, A
				RET
                MOV     A,09FH
				MOV     R7, A
				RET
                MOV     A,0A0H
				MOV     R7, A
				RET
                MOV     A,0A1H
				MOV     R7, A
				RET
                MOV     A,0A2H
				MOV     R7, A
				RET
                MOV     A,0A3H
				MOV     R7, A
				RET
                MOV     A,0A4H
				MOV     R7, A
				RET
                MOV     A,0A5H
				MOV     R7, A
				RET
                MOV     A,0A6H
				MOV     R7, A
				RET
                MOV     A,0A7H
				MOV     R7, A
				RET
                MOV     A,0A8H
				MOV     R7, A
				RET
                MOV     A,0A9H
				MOV     R7, A
				RET
                MOV     A,0AAH
				MOV     R7, A
				RET
                MOV     A,0ABH
				MOV     R7, A
				RET
                MOV     A,0ACH
				MOV     R7, A
				RET
                MOV     A,0ADH
				MOV     R7, A
				RET
                MOV     A,0AEH
				MOV     R7, A
				RET
                MOV     A,0AFH
				MOV     R7, A
				RET
                MOV     A,0B0H
				MOV     R7, A
				RET
                MOV     A,0B1H
				MOV     R7, A
				RET
                MOV     A,0B2H
				MOV     R7, A
				RET
                MOV     A,0B3H
				MOV     R7, A
				RET
                MOV     A,0B4H
				MOV     R7, A
				RET
                MOV     A,0B5H
				MOV     R7, A
				RET
                MOV     A,0B6H
				MOV     R7, A
				RET
                MOV     A,0B7H
				MOV     R7, A
				RET
                MOV     A,0B8H
				MOV     R7, A
				RET
                MOV     A,0B9H
				MOV     R7, A
				RET
                MOV     A,0BAH
				MOV     R7, A
				RET
                MOV     A,0BBH
				MOV     R7, A
				RET
                MOV     A,0BCH
				MOV     R7, A
				RET
                MOV     A,0BDH
				MOV     R7, A
				RET
                MOV     A,0BEH
				MOV     R7, A
				RET
                MOV     A,0BFH
				MOV     R7, A
				RET
                MOV     A,0C0H
				MOV     R7, A
				RET
                MOV     A,0C1H
				MOV     R7, A
				RET
                MOV     A,0C2H
				MOV     R7, A
				RET
                MOV     A,0C3H
				MOV     R7, A
				RET
                MOV     A,0C4H
				MOV     R7, A
				RET
                MOV     A,0C5H
				MOV     R7, A
				RET
                MOV     A,0C6H
				MOV     R7, A
				RET
                MOV     A,0C7H
				MOV     R7, A
				RET
                MOV     A,0C8H
				MOV     R7, A
				RET
                MOV     A,0C9H
				MOV     R7, A
				RET
                MOV     A,0CAH
				MOV     R7, A
				RET
                MOV     A,0CBH
				MOV     R7, A
				RET
                MOV     A,0CCH
				MOV     R7, A
				RET
                MOV     A,0CDH
				MOV     R7, A
				RET
                MOV     A,0CEH
				MOV     R7, A
				RET
                MOV     A,0CFH
				MOV     R7, A
				RET
                MOV     A,0D0H
				MOV     R7, A
				RET
                MOV     A,0D1H
				MOV     R7, A
				RET
                MOV     A,0D2H
				MOV     R7, A
				RET
                MOV     A,0D3H
				MOV     R7, A
				RET
                MOV     A,0D4H
				MOV     R7, A
				RET
                MOV     A,0D5H
				MOV     R7, A
				RET
                MOV     A,0D6H
				MOV     R7, A
				RET
                MOV     A,0D7H
				MOV     R7, A
				RET
                MOV     A,0D8H
				MOV     R7, A
				RET
                MOV     A,0D9H
				MOV     R7, A
				RET
                MOV     A,0DAH
				MOV     R7, A
				RET
                MOV     A,0DBH
				MOV     R7, A
				RET
                MOV     A,0DCH
				MOV     R7, A
				RET
                MOV     A,0DDH
				MOV     R7, A
				RET
                MOV     A,0DEH
				MOV     R7, A
				RET
                MOV     A,0DFH
				MOV     R7, A
				RET
                MOV     A,0E0H
				MOV     R7, A
				RET
                MOV     A,0E1H
				MOV     R7, A
				RET
                MOV     A,0E2H
				MOV     R7, A
				RET
                MOV     A,0E3H
				MOV     R7, A
				RET
                MOV     A,0E4H
				MOV     R7, A
				RET
                MOV     A,0E5H
				MOV     R7, A
				RET
                MOV     A,0E6H
				MOV     R7, A
				RET
                MOV     A,0E7H
				MOV     R7, A
				RET
                MOV     A,0E8H
				MOV     R7, A
				RET
                MOV     A,0E9H
				MOV     R7, A
				RET
                MOV     A,0EAH
				MOV     R7, A
				RET
                MOV     A,0EBH
				MOV     R7, A
				RET
                MOV     A,0ECH
				MOV     R7, A
				RET
                MOV     A,0EDH
				MOV     R7, A
				RET
                MOV     A,0EEH
				MOV     R7, A
				RET
                MOV     A,0EFH
				MOV     R7, A
				RET
                MOV     A,0F0H
				MOV     R7, A
				RET
                MOV     A,0F1H
				MOV     R7, A
				RET
                MOV     A,0F2H
				MOV     R7, A
				RET
                MOV     A,0F3H
				MOV     R7, A
				RET
                MOV     A,0F4H
				MOV     R7, A
				RET
                MOV     A,0F5H
				MOV     R7, A
				RET
                MOV     A,0F6H
				MOV     R7, A
				RET
                MOV     A,0F7H
				MOV     R7, A
				RET
                MOV     A,0F8H
				MOV     R7, A
				RET
                MOV     A,0F9H
				MOV     R7, A
				RET
                MOV     A,0FAH
				MOV     R7, A
				RET
                MOV     A,0FBH
				MOV     R7, A
				RET
                MOV     A,0FCH
				MOV     R7, A
				RET
                MOV     A,0FDH
				MOV     R7, A
				RET
                MOV     A,0FEH
				MOV     R7, A
				RET
                MOV     A,0FFH
				MOV     R7, A
				RET
                END
