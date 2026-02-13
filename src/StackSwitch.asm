extern TaskletFunctionEntry : proto	;The main entry point, needs to have a reference to the current tasklet, this is currently global

.data
	ReturnStatus db ?

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

	;Prepare the return status code
	;mov rax,0				;Clear RAX
	;mov al,[ReturnStatus]				;Set lowest byte of RAX

	ret				

StartNewTasklet endp


ResumeTasklet proc

	; Expects input registers as
	; rcx - parent rsp
	; rdx - rsp at yield which will be read

	; Update the parent location to new rsp
	mov [rcx], rsp
	
	; Set Return status back to finished
	; Which will be returned if not suspended
	mov [ReturnStatus],2
	
	; Reinstate rsp to Tasklet
	mov rsp, [rdx]

	ret	

ResumeTasklet endp


; External Procs

; ---Run Tasklet Entry---
RunTaskletASM proc
	
	; Expects input registers as
	; rcx - ActiveTasklet
	; rdx - StackMemoryPointer

	; Set Return value
	; Set to FINISHED which is the value that is returned if Yield is not hit
	mov [ReturnStatus],2

	call StartNewTasklet

	;Prepare the return status code
	mov rax,0							;Clear RAX
	mov al,[ReturnStatus]				;Set lowest byte of RAX

	ret				

RunTaskletASM endp

; ---Yield Tasklet Entry---
YieldTaskletASM proc
	
	; Expects input registers as
	; rcx - parent rsp
	; rdx - rsp at yield which will be set

	; Save the current rsp location so it can be reinstated later
	mov [rdx], rsp

	; Reinstate the rsp from rcx
	; This should point to the parent rsp
	mov rsp, [rcx]

	; Set the status to SUSPENDED
	mov [ReturnStatus],1

	ret				

YieldTaskletASM endp

; ---Resume Tasklet Entry---
ResumeTaskletASM proc

	; Expects input registers as
	; rcx - parent rsp
	; rdx - rsp at yield which will be read

	call ResumeTasklet

	mov rax,0							;Clear RAX
	mov al,[ReturnStatus]				;Set lowest byte of RAX

	ret				

ResumeTaskletASM endp

end