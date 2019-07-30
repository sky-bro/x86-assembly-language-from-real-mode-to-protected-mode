mov ax, 0xb800
mov ds, ax
; mov byte [0x00], 'a'
; mov byte [0x02], 's'
; mov byte [0x04], 'm'
mov word [0x00], 0xCE31 ; 0x31 数字1 1100 闪烁 红背景 1110 高亮 黄色 (不高亮是棕色)
mov word [0x02], 0x4632 ; 0x32 数字2 0100 不闪烁 红背景 0110 不高亮 棕色
mov word [0x04], 0x0733 ; 0x33 数字3 黑底白字 无闪烁 无加亮 默认
jmp $