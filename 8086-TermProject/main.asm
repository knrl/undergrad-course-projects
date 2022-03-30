;
;   Marmara University      :   Computer Engineering
;   @author Mehmet Kaan Erol

org 100h ; DOS com file                        

; @param stopCondition character indicating the stop condition
; @param argTempNumbers variable to assign output
;
; this macro takes characters until the stop condition. But the character limit is two.
take_inputs MACRO stopCondition,argTempNumbers
local last,repeat,backspace      ; local labels
    repeat:
        mov ah,07h               ; interupt' function value
        int 21h                  ; read character (al)
        
        cmp al,08h               ; if backspace                                                       
        je backspace             ; jump to backspace label
        
        cmp al,[stopCondition]   ; if enter jump to last
        je last     
        
        cmp [num_step],2
        jge repeat
        
        cmp al,30h               ; check if it is in the desired range
        jb repeat                ; if characters are not between 0-9 (30h-39h)
        cmp al,39h               ; jump to repeat label
        ja repeat
                       
        mov si,[num_step]
        mov [argTempNumbers+si],al
        inc [num_step]
        
        mov dl,al                ; write the character that in al register
        mov ah,02h
        int 21h
    jmp repeat:

    backspace:
        cmp [num_step],0         ; jump if the number of character equal to 0
        je repeat        
        dec [num_step]           ; decrease the number of character
        mov si,[num_step]     
        mov [argTempNumbers+si],0h

        mov dl,08h               ; take a step back with backspace
        mov ah,02h
        int 21h
        
        mov dl,' '               ; overwrite the previous character with the space character
        mov ah,02h
        int 21h

        mov dl,08h               ; take a step back with backspace
        mov ah,02h
        int 21h

    jmp repeat        
    last:
ENDM

; @param argTempNumbers characters convert from ascii to decimal
; @param argNumber variable to assign output
;
ascii_to_decimal MACRO argTempNumbers,argNumber       
;    example   :
; sample input : '1' and '2' ascii equivalents 31h, 32h 
; output       : al = (12)decimal_base

local check,last       
        mov al,0
        cmp [argTempNumbers],0    ; if digit is 0
        jne check
        cmp [argTempNumbers+1],0
        je last
    check:
        mov al,[argTempNumbers]   ; second digit
        sub al,30h                ; ascii equivalent
      
        cmp [num_step],1           
        je last                                        
        mov bl,10                 ; base 10
        mul bl                    ; multiply with
     
        mov bl,al         
        mov al,[argTempNumbers+1] ; number of first digit
        sub al,30h                ; ascii equivalent
        add al,bl                
    last:                        
        mov ah,0                  ; assign decimal number in al to variable 'argNumber'
        mov [argNumber],ax
ENDM

; @param argNumber the value to be calculated with factorial
; @param argResult variable to assign the factorial result
;
calc_factorial MACRO argNumber,argResult
local last,repeat,end_with          
    cmp [argNumber],0             ; jump last if argNumber is equal to 0
    je last
    mov ax,1                      ; value of multyplied will be kept in ax; initial value 1
    repeat:
        mov bx,dx                 ; ax * argNumber in case dx is not calculated
        mul [argNumber]           ; dx value is kept in a temp variable
        mov [temp],ax             ; calculated separately, no loss of value
        mov ax,bx     
        mov [temp2],dx            ; keep the overflow value thrown to dx as a result of multiplication
        mul [argNumber]           ; calculate overflow result keep in ax      
        add ax,[temp2]            ; add the result of the multiplier overflow with the result of the overflow from the last multiplication
        mov dx,ax                 ; assign this value to dx register
        mov ax,[temp]             ; ax * number value was kept in temp
                                  ; back assign this value to ax register
        cmp [argNumber],2
        jle last    
        dec [argNumber]
    jmp repeat
    last:                         ; assign values to argResult in order for later use            
        mov [argResult],al        ; argResult+0 = al, argResult+1 = ah 
        mov [argResult+1],ah      ; argResult+2 = dl, argResult+3 = dh
        cmp [basamak],1           ; if number of step (mean number of entered chracters) is equal to 1 jump next step
        je end_with                 
            mov [argResult+2],dl
            mov [argResult+3],dh        
        end_with:                       
ENDM         

; @param argText variable that to be displayed
;
; this macro reads argText all characters until '$' character
output_display MACRO argText 
local repeat,last
    mov ah,02h         
    mov dl,13                     ; assign 0 to column for cursor position
    int 21h   
     
    lea bx,argText
    repeat:    
        mov dl,[bx]
        cmp dl,dollar_ascii_value
        je bitir
        mov ah,02h
        int 21h
        inc bx  
    jmp repeat
    last:
ENDM

; @param argFile the name of the file to print
;
output_file MACRO argFile                
    lea dx,argFile                ; filename given as parameter
    mov cx,0                      ; function parameters required to create the file
    mov ah,3ch                     
    int 21h                       ; create the file
                                  ; file descriptor is assigned to ax
    mov bx,ax                     ; file descriptor is assigned
    mov dx,offset data            ; file writing process parameters
    mov cx,[data_size]            ; byte size
    mov ah,40h
    int 21h                       ; print to file
    
    mov ah,3eh                    ; the function param required to close the file
    int 21h                       ; close the file
ENDM

; @param argResult result value
; @param argVeri write output to data variable
;
decimal_to_ascii MACRO argResult,argData
local repeat,last
    mov si,3                      ; number of repeat for (al,ah,dl,dh)
    mov di,8                      ; toplam eleman sayisi
    repeat:
        mov ax,0                  ; 0 to avoid remaining value from previous transaction              
        mov al,[result+si]        ; get retained value of result variable in order
        mov bl,10h                ; take the remainder and the division from 10h division
        div bl 
        mov [tempNumbers],ah      ; the remainder is kept in ah
                                  ; kept division in al
        cmp al,0ah                ; first print al
        jl if_greater  
            add al,7h             ; if greater than 9 print as characters
        if_greater:
        add al,30h
        mov [argDatari+di],al  
        inc di  
           
        mov ah,[tempNumbers]      ; after al is printed
        cmp ah,0ah                ; compare with A hex
        jl if_greater2  
            add ah,7h             ; if greater than 9 print as characters
        if_greater2:
        add ah,30h
        mov [argData+di],ah        
        inc di
                         
        dec si
        cmp si,-1
        je last:
    jmp repeat    
     
    last:
ENDM

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;; Main ;;;;;;;;;;;;  
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

output_display firstText          ; send message to user
take_inputs enter_ascii_value,sayilar
                                  ; character reading process
                                  ; output is passed to numbers variable
                                  ; as parameter value of key for stop condition
ascii_to_decimal tempNumbers,number    
                                  ; convert two read characters to decimal
                                  ; factorial result
calc_factorial number,result
                                  ; calculate factorial and assign output to result variable
mov ah,02h                        ; new line
mov dl,10                         
int 21h

decimal_to_ascii result,data      ; convert factorial result to ascii format

output_display data               ; print the values held in result to the screen
output_file fileName              ; display

RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;; Values ;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

temp2 dw 0
temp dw 0     
num_step dw 0
number dw 0
tempNumbers db 2 dup(0)    
result db 4 dup(0)           

;;;;;;;; Parameters ;;;;;;;;;    

data db "Result: (",8 dup(" "),")hex$"
data_size dw 21

enter_ascii_value db 0Dh          ; enter ascii value for stop conditioning
dollar_ascii_value db 24h         ; character representing the termination of text
                         
firstText db "Input: $"           ; information message to user
resultText db "Result: $"         ; text expressing the result
fileName db "c:\result.txt",0     ; file with results

END
