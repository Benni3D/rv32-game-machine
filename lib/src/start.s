.section .text
.extern main
.global _start
.global abort
.global exit
_start:
la t0, num_exit_handlers
sw x0, 0(t0)
la sp, __stack_top
mv fp, zero
call main
exit:
# save exit status
mv s1, a0

# call exit handlers
lw s2, num_exit_handlers
slli s2, s2, 2
la s3, exit_handlers

_start_loop:
beq s2, zero, _start_end

addi s2, s2, -4
add t0, s3, s2
lw t0, 0(t0)

jalr ra, t0

j _start_loop

_start_end:

# move exit status to x1
mv x1, s1
abort:
ebreak

.global atexit
atexit:
la t0, num_exit_handlers
lw t1, 0(t0)
li t2, 32
beq t1, t2, atexit_failed

addi t1, t1, 1
sw t1, 0(t0)

slli t1, t1, 2
la t0, exit_handlers
add t1, t1, t0
sw a0, -4(t1)

mv a0, zero
ret

atexit_failed:
li a0, 1
ret

.section .bss
# reserve 4kB for stack
.skip 4096
__stack_top:

# 16 bytes for safety
.skip 16

# reserve exit handlers
exit_handlers:
.skip 32*4
num_exit_handlers:
.skip 4


