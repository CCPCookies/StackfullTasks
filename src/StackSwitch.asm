extern TaskletFunctionEntry : proto	;The main entry point, needs to have a reference to the current tasklet, this is currently global

.data
	ReturnStatus db ?

.code

; ---Run Tasklet Entry---
RunTaskletASM proc
	
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