win_bison -d $1'.y'
win_flex $1'.l'
gcc lex.yy.c $1'.tab.c'

echo ".model small
.stack 200h
.data

.code
mov ax , @data
mov ds , ax
mov ax , 0
   " > "C:\Tasm 1.4\Tasm\code.asm";

a < "test.rexx";

echo "mov ah,4ch 
int 21h

print proc             
    mov cx,0 
    mov dx,0 
    label1:  
        cmp ax,0 
        je print1       
        mov bx,10          
        div bx                    
        push dx               
        inc cx               
        xor dx,dx 
        jmp label1 
    print1:  
        cmp cx,0 
        je exit 
        pop dx  
        add dx,48  
        mov ah,02h 
        int 21h  
        dec cx 
        jmp print1   
        exit:
    mov dx,13
	mov ah,2
	int 21h  
	mov dx,10
	mov ah,2
	int 21h     
        ret 
print endp
end" >> "C:\Tasm 1.4\Tasm\code.asm";
