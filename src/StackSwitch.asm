extern FibreFunctionEntry : proto	;The main entry point, needs to have a reference to the current fibre, this is currently global

.data
	ReturnStatus db ?

.code

StartNewFibre proc
	; At this point the stack will contain the return instruction

	; Add the parent stack pointer to the start of the Fibres stack memory
	; Store in r11
	mov r11,rsp

	;Switch rsp to the Fibre's stack memory
	mov rsp, rdx
	;Add the parent stack pointer to the start of the new stack
	push r11

	;Prologue
	push rbp
	mov rbp, rsp

	;Add space for shadow memory
	sub rsp, 20h

	; rcx already contains the active Fibre here, no need to set it again

	;Call to c++
	call FibreFunctionEntry

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

StartNewFibre endp


ResumeFibre proc

	; Expects input registers as
	; rcx - parent rsp
	; rdx - rsp at yield which will be read

	; Update the parent location to new rsp
	mov [rcx], rsp
	
	; Set Return status back to finished
	; Which will be returned if not suspended
	mov [ReturnStatus],2
	
	; Reinstate rsp to Fibre
	mov rsp, [rdx]

	ret	

ResumeFibre endp


; External Procs

; ---Run Fibre Entry---
RunFibreASM proc
	
	; Expects input registers as
	; rcx - ActiveFibre
	; rdx - StackMemoryPointer

	; Set Return value
	; Set to FINISHED which is the value that is returned if Yield is not hit
	mov [ReturnStatus],2

	call StartNewFibre

	;Prepare the return status code
	mov rax,0							;Clear RAX
	mov al,[ReturnStatus]				;Set lowest byte of RAX

	ret				

RunFibreASM endp

; ---Yield Fibre Entry---
YieldFibreASM proc
	
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

YieldFibreASM endp

; ---Resume Fibre Entry---
ResumeFibreASM proc

	; Expects input registers as
	; rcx - parent rsp
	; rdx - rsp at yield which will be read

	call ResumeFibre

	mov rax,0							;Clear RAX
	mov al,[ReturnStatus]				;Set lowest byte of RAX

	ret				

ResumeFibreASM endp

end