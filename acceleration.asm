section .data
    conversion_factor dd 0.277777778  ; 1000/3600 for km/h to m/s conversion

section .text
global calculate_acceleration

; Function to calculate acceleration for multiple cars
; Parameters:
; RCX - pointer to float matrix (Vi, Vf, T for each car)
; RDX - number of rows (cars)
; R8  - pointer to output integer array

calculate_acceleration:
    push rbp
    mov rbp, rsp
    
    ; Preserve non-volatile registers
    push rbx
    push r12
    
    ; Constants for conversion
    ; 1000/3600 = 0.277777778 (for km/h to m/s conversion)
    movss xmm7, dword [rel conversion_factor]
    
    ; Process each car
    xor rbx, rbx                    ; Initialize counter
    
.loop:
    cmp rbx, rdx                    ; Compare counter with number of rows
    jge .done                       ; If counter >= rows, we're done
    
    ; Load values for current car
    mov rax, rbx
    imul rax, 12                    ; Each row is 3 floats (12 bytes)
    
    ; Load Initial Velocity (Vi)
    movss xmm0, dword [rcx + rax]   ; Vi
    
    ; Load Final Velocity (Vf)
    movss xmm1, dword [rcx + rax + 4] ; Vf
    
    ; Load Time (T)
    movss xmm2, dword [rcx + rax + 8] ; T
    
    ; Calculate Vf - Vi
    subss xmm1, xmm0                ; xmm1 = Vf - Vi
    
    ; Convert km/h to m/s: multiply by 1000/3600
    mulss xmm1, xmm7                ; Convert to m/s
    
    ; Divide by time to get acceleration
    divss xmm1, xmm2                ; a = (Vf - Vi)/T
    
    ; Convert float to integer (round to nearest)
    cvtss2si eax, xmm1              ; Convert to integer
    
    ; Store result in output array
    mov dword [r8 + rbx*4], eax
    
    ; Increment counter and continue loop
    inc rbx
    jmp .loop
    
.done:
    ; Restore preserved registers
    pop r12
    pop rbx
    
    ; Function epilogue
    mov rsp, rbp
    pop rbp
    ret
