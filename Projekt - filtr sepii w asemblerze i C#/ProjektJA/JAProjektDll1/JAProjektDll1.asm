; Funkcja: ModifyPixel (Implementacja ASM SIMD)
; Równoleg³a implementacja filtru sepii z wykorzystaniem instrukcji SSE/AVX.
; Dzia³anie: Program ³aduje paczki po 4 piksele (128 bitów) do rejestrów XMM.
;           Za pomoc¹ operacji pshufb i pmovzxbd rozdziela kana³y kolorów,
;           nastêpnie wykonuje obliczenia matematyczne jednoczeœnie dla 4 pikseli (SIMD).
;           Nasycenie (clamping) realizowane jest sprzêtowo przez instrukcje minps.

.data
    align 16
    ; Maski dla pshufb (128-bitowe)
    mask_B db 0, 4, 8, 12, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    mask_G db 1, 5, 9, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    mask_R db 2, 6, 10, 14, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    mask_A db 3, 7, 11, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1

    ; Wspó³czynniki Sepii (4x float dla ka¿dego kana³u)
    cRR dd 4 dup(0.393)
    cRG dd 4 dup(0.769)
    cRB dd 4 dup(0.189)
    
    cGR dd 4 dup(0.349)
    cGG dd 4 dup(0.686)
    cGB dd 4 dup(0.168)
    
    cBR dd 4 dup(0.272)
    cBG dd 4 dup(0.534)
    cBB dd 4 dup(0.131)

    v255 dd 4 dup(255.0)

.code
ModifyPixel proc pixelData:QWORD, count:DWORD
    push rsi
    push rbx
    
    mov rsi, rcx         ; rsi = pixelData
    mov r8d, edx         ; r8d = count

    ; Za³adowanie masek do rejestrów (Poprawiona sk³adnia dla MASM)
    movdqa xmm10, xmmword ptr [mask_B]
    movdqa xmm11, xmmword ptr [mask_G]
    movdqa xmm12, xmmword ptr [mask_R]
    movdqa xmm13, xmmword ptr [mask_A]
    movaps xmm15, xmmword ptr [v255]

main_loop:
    cmp r8d, 4
    jl scalar_cleanup

    ; 1. Za³aduj 4 piksele (16 bajtów)
    movdqu xmm0, xmmword ptr [rsi]

    ; 2. Shuffling - rozdziel kana³y i konwertuj na float
    ; Kana³ B
    movdqa xmm1, xmm0
    pshufb xmm1, xmm10
    pmovzxbd xmm1, xmm1
    cvtdq2ps xmm1, xmm1   ; xmm1 = [B0, B1, B2, B3] jako float
    
    ; Kana³ G
    movdqa xmm2, xmm0
    pshufb xmm2, xmm11
    pmovzxbd xmm2, xmm2
    cvtdq2ps xmm2, xmm2   ; xmm2 = [G0, G1, G2, G3] jako float
    
    ; Kana³ R
    movdqa xmm3, xmm0
    pshufb xmm3, xmm12
    pmovzxbd xmm3, xmm3
    cvtdq2ps xmm3, xmm3   ; xmm3 = [R0, R1, R2, R3] jako float

    ; 3. Obliczenia Sepii
    ; --- R' ---
    movaps xmm4, xmm3
    mulps  xmm4, xmmword ptr [cRR]
    movaps xmm8, xmm2
    mulps  xmm8, xmmword ptr [cRG]
    addps  xmm4, xmm8
    movaps xmm8, xmm1
    mulps  xmm8, xmmword ptr [cRB]
    addps  xmm4, xmm8
    minps  xmm4, xmm15    ; xmm4 = R' (nasycone)

    ; --- G' ---
    movaps xmm5, xmm3
    mulps  xmm5, xmmword ptr [cGR]
    movaps xmm8, xmm2
    mulps  xmm8, xmmword ptr [cGG]
    addps  xmm5, xmm8
    movaps xmm8, xmm1
    mulps  xmm8, xmmword ptr [cGB]
    addps  xmm5, xmm8
    minps  xmm5, xmm15    ; xmm5 = G' (nasycone)

    ; --- B' ---
    movaps xmm6, xmm3
    mulps  xmm6, xmmword ptr [cBR]
    movaps xmm8, xmm2
    mulps  xmm8, xmmword ptr [cBG]
    addps  xmm6, xmm8
    movaps xmm8, xmm1
    mulps  xmm8, xmmword ptr [cBB]
    addps  xmm6, xmm8
    minps  xmm6, xmm15    ; xmm6 = B' (nasycone)

    ; 4. Pakowanie z powrotem do bajtów
    ; Konwersja float -> int32
    cvtps2dq xmm4, xmm4   ; R' ints
    cvtps2dq xmm5, xmm5   ; G' ints
    cvtps2dq xmm6, xmm6   ; B' ints

    ; Zachowujemy oryginalny kana³ Alpha
    movdqa xmm7, xmm0
    pshufb xmm7, xmm13    ; xmm7 = [A0, A1, A2, A3] bajty

    ; Sk³adanie BGRABGRA
    ; U¿ywamy pêtli skalarnej do zapisu lub instrukcji pack
    xor rbx, rbx
store_loop:
    vmovd ecx, xmm6       ; Pobierz B'
    vmovd edx, xmm5       ; Pobierz G'
    vmovd r9d, xmm4       ; Pobierz R'
    vmovd r10d, xmm7      ; Pobierz Alpha
    
    mov [rsi + rbx*4], cl
    mov [rsi + rbx*4 + 1], dl
    mov [rsi + rbx*4 + 2], r9b
    mov [rsi + rbx*4 + 3], r10b

    psrldq xmm4, 4        ; Przesuñ rejestry, by pobraæ kolejny piksel
    psrldq xmm5, 4
    psrldq xmm6, 4
    psrldq xmm7, 4
    inc rbx
    cmp rbx, 4
    jl store_loop

    add rsi, 16
    sub r8d, 4
    jmp main_loop

scalar_cleanup:
    ; Obs³uga pozosta³ych pikseli (0-3) - pêtla skalarna
    test r8d, r8d
    jle done
    ; Przygotowanie sta³ych do obliczeñ skalarnych (u¿ywamy xmm8-xmm10 jako pomocniczych)
    ; £adujemy sta³e float (tylko pierwszy element bêdzie u¿ywany)
    movss xmm11, dword ptr [cRR]
    movss xmm12, dword ptr [cRG]
    movss xmm13, dword ptr [cRB]

next_pixel:
    ; Pobierz kana³y B, G, R, A (1 piksel = 4 bajty)
    movzx eax, byte ptr [rsi]       ; B
    movzx ecx, byte ptr [rsi + 1]   ; G
    movzx edx, byte ptr [rsi + 2]   ; R
    movzx r9d, byte ptr [rsi + 3]   ; A

    ; Konwersja na float (u¿ywamy xmm0, xmm1, xmm2 dla B, G, R)
    cvtsi2ss xmm0, eax
    cvtsi2ss xmm1, ecx
    cvtsi2ss xmm2, edx

    ; Obliczenia Sepii (R')
    movss xmm3, xmm2
    mulss xmm3, dword ptr [cRR]
    movss xmm4, xmm1
    mulss xmm4, dword ptr [cRG]
    addss xmm3, xmm4
    movss xmm4, xmm0
    mulss xmm4, dword ptr [cRB]
    addss xmm3, xmm4
    minss xmm3, xmm15      ; Nasycenie do 255.0

    ; Obliczenia Sepii (G')
    movss xmm5, xmm2
    mulss xmm5, dword ptr [cGR]
    movss xmm4, xmm1
    mulss xmm4, dword ptr [cGG]
    addss xmm5, xmm4
    movss xmm4, xmm0
    mulss xmm4, dword ptr [cGB]
    addss xmm5, xmm4
    minss xmm5, xmm15

    ; Obliczenia Sepii (B')
    movss xmm6, xmm2
    mulss xmm6, dword ptr [cBR]
    movss xmm4, xmm1
    mulss xmm4, dword ptr [cBG]
    addss xmm6, xmm4
    movss xmm4, xmm0
    mulss xmm4, dword ptr [cBB]
    addss xmm6, xmm4
    minss xmm6, xmm15

    ; Konwersja float -> int i zapis
    cvtss2si eax, xmm6     ; B'
    cvtss2si ecx, xmm5     ; G'
    cvtss2si edx, xmm3     ; R'

    mov [rsi], al
    mov [rsi + 1], cl
    mov [rsi + 2], dl
    ; Kana³ Alpha [rsi + 3] zostaje bez zmian

    add rsi, 4
    dec r8d
    jnz next_pixel

done:
    vzeroupper
    pop rbx
    pop rsi
    ret
ModifyPixel endp
end