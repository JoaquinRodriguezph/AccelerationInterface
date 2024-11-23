section .data
km_to_m dd 0.27777778 ; Constant for converting km/h to m/s

section .text
global calculate_acceleration

calculate_acceleration:
    ; Initialize pointers
    xor rcx, rcx             ; Loop counter (row index)

loop_start:
    cmp rcx, rsi             ; Check if we are done
    jge loop_end             ; Exit loop if rcx >= rows

    ; Load values from matrix
    mov rax, rcx             ; Row index
    imul rax, 3              ; Multiply by 3 to get the starting index
    lea r8, [rdi + rax*4]    ; Pointer to row in matrix

    movss xmm0, [r8]         ; Load Vi (Initial Velocity) into xmm0
    movss xmm1, [r8 + 4]     ; Load Vf (Final Velocity) into xmm1
    movss xmm2, [r8 + 8]     ; Load T (Time) into xmm2

    ; Debugging: Validate loaded values
    ; (Optional: Output these values to memory for inspection)

    ; Convert velocities from KM/H to m/s
    movss xmm3, dword [rel km_to_m] ; Load conversion factor (1000/3600)
    mulss xmm0, xmm3         ; Vi * (1000/3600)
    mulss xmm1, xmm3         ; Vf * (1000/3600)

    ; Calculate acceleration
    subss xmm1, xmm0         ; (Vf - Vi)
    divss xmm1, xmm2         ; (Vf - Vi) / T

    ; Check for division errors (e.g., divide by zero)
    ; You can add error handling here if needed.

    ; Convert result to integer
    cvttss2si eax, xmm1      ; Convert to integer
    mov [rdx + rcx*4], eax   ; Store in output array

    ; Increment loop counter
    inc rcx
    jmp loop_start

loop_end:
    ret
