extern TaskletFunctionEntry : proto	;The main entry point, needs to have a reference to the current tasklet, this is currently global

.data


.code


StartNewTasklet proc
	; At this point the stack will contain the return instruction

	; Add the parent stack pointer to the start of the Tasklets stack memory
	; Store in r11
	mov r11,rsp

	;Switch rsp to the Tasklet's stack memory
	mov rsp, rdx
	;Add the parent stack pointer to the start of the new stack
	push r11

	;Prologue
	push rbp
	mov rbp, rsp

	;Add space for shadow memory
	sub rsp, 20h

	; rcx already contains the active Tasklet here, no need to set it again

	;Call to c++
	call TaskletFunctionEntry

	;Epilogue
	mov rsp, rbp
	pop rbp

	;Reinstate the rsp to the parent which should be the top of the stack
	pop r11
	mov rsp,r11

	ret				

StartNewTasklet endp


; External Procs

; ---Run Tasklet Entry---
RunTaskletASM proc
	
	; Expects input registers as
	; rcx - ActiveTasklet
	; rdx - StackMemoryPointer

	call StartNewTasklet

	;Prepare the return status code
	mov rax,0				;Clear RAX
	mov al,0				;Set lowest byte of RAX

	ret				

RunTaskletASM endp

; ---Yield Tasklet Entry---
YieldTaskletASM proc
	
	ret				

YieldTaskletASM endp

; ---Resume Tasklet Entry---
ResumeTaskletASM proc
	
	ret				

ResumeTaskletASM endp

end