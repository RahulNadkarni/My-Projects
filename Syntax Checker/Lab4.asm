##########################################################################
# Created by: Nadkarni, Rahul
# rnadkarn
# 22 February 2021
#
# Assignment: Lab 4: Syntax Checker
# CSE 12, Computer Systems and Assembly Language
# UC Santa Cruz, Winter 2021
#
# Description: This program reads a file and checks the input file for brace mismatches.
#
# Notes: This program is intended to be run from the MARS IDE.
#
#
#Pseudocode
#
# .data - asciiz values, buffer value, etc.
# .text
#   main:
#	print("You Entered the File: \n")
#	Get the file input name
#	Go to a new line
#  number:
#	if filename[0] >=65 and filename[0] < 90:
#		continue
#	if filename[0] < 65:
#		error
#	if filename[0] > 90 and filename[0] < 96:
#		error
#	if filename [0] >96 and filename[0] < 123:
#		continue
#  count:
#	for index in range of filename length
#		if index > 20:
#			error
#  continue:	
#	Get the contents of the file
#  	open the file
#  readfile:
#	Read the file using syscall 14
#	Store the first 128 bytes into the buffer
#	Store the current character index $v0 into register $t0
#  IsBrace:
#	Go through the entire 128 bytes
#	When the count reaches 126, go back to readfile to get the rest of the contents of the file
#	check if value loaded is '(', '[','{' or ')',']',or'}'
#	if left bracket, go to countbracketleft
#	if left brace, go to countbraceleft
#	if left parentheses, go to countparenthesesleft
#	if right bracket, parentheses, or brace, go to the check  
#	add one to the index
#  CountLeftParentheses:
#	xor by 6 to get matching pair '}' 
#	store the opposite in the stack
#	add one to the pair count
#	go to right add
# CountLeftParenteheses:
#	xor by 1 to get matching pair ')'
#	store opposite in the stack
#	add one to pair count
#	go to right add
# CountLeftBrace:
#	xor by 6 to get maching pair ']'
#	store opposite in the stack
#	add one to pair count
#	go to left add
# leftadd:
#	add one to the total left brace count
# Check:
#	add one to the right brace count
#	pop last value off the stack
#	If that value equals the next indexed value of the contents '), }, or ]'
#	If not, go to immediate error
#	go to subtractpair
# subtractpair:
#	subtract total pairs by one
# immediate error:
#	the value being held in the stack is correct, the one that has been loaded is not
#	that value is in $a0
#	load the value of $a0 into a register
#	go to MismatchMessagePrint
# isError:
#	After indexing through, if errors have not been caught yet, check again
#	if left braces are greater than right braces, jump to StillonStack
#	if the amount of register for correct pairs is greater than or equal to one, go to MismatchMessagePrint
#	if amount of register for correct pairs is zero, go to SuccessMessagePrint
# MismatchMessagePrint:
#	print the mismatch message "ERROR - There is a brace mismatch: "
#	then print the stored error brace 
#	get the index from isBrace
# SuccessMessagePrint:
#	print the message: "SUCCESS - There are "
#	get the total number of pairs 
#	print the rest of the statement
# StillonStack:
#	Print the message saying that braces are still on stack
# 	go to pop all the values out
# popall:
#	pop all the values out
#	if '}, ), or ]' revert back to original
# revert:
#	xor by 6 to get original form back for braces and brackets
# revertp:
#	xor by 1 to get original form back for parentheses
# Error:
#	print the error message if filename greater than 20 characters, or if first character is not a letter
# Ending:
#	Go to a new line
#	terminate the program
#
#
#
#
#
#
#
#
#
#
#
##########################################################################





									#Register Usage
									# $t0 - Holds the Filename character count
									# $t1 - Holds the correct amount of pairs to be subtracted from
									# $t2 - Holds the correct amount of pairs to be shown if there are no errors
									# $t3 - Holds the index value
									# $t4 - Holds the incorrect brace 
									# $t5 - Used to increase the byte count in the IsBrace function
									# $t6 - Index through the remaining values on the stack 
									# $t7 - Adds the number of right braces
									# $t8 - Adds the number of left braces
									# $t9 - Holds the reverted value of the popped value on the stack 

.data 									#Always Start ASM file with .data
whichFile:        .asciiz "You entered the file: \n"               	#String for which file 
buffer:           .space   128						#Buffer Value of 128 Bytes
ErrorMessage:     .asciiz "\nERROR: Invalid program argument."		#Error Message for invalid argument
MismatchMessage:  .asciiz "\nERROR - There is a brace mismatch: "	#Error Message for brace mismatch
SuccessMessage:   .asciiz "\nSUCCESS: There are "			#Success Message
atIndex:          .asciiz " at index "					#After the incorrect brace is shown, line for which index
PairsOfBraces:    .asciiz " pairs of braces"				#Second part of success message stating amount of pairs
stillonstack:     .asciiz "\nERROR - Brace(s) still on stack: "		#Error if there are still braces on stack
newLine:          .asciiz "\n"						#New Line
.text									#.text now has all the main code
	main: 								#Main Function
		li $v0 4						#Get Ready to Print a String 
		la $a0 whichFile					#whichFile string is the one the computer needs to print
		syscall 						#Execute


		lw $s0 ($a1)						#Get the input file name
		move $a0 , $s0						#Move the file name to get ready to print
		li $v0, 4						#Get ready to print a string
		syscall  						#Print the file name

		la $a0 newLine
		syscall
		
	number: 
		lb   $a0   0 ($s0)					#Get the first character of the file name					#A
		bge  $a0  65  nextcheck					#Is the character greater than the ascii value of 65(A) If yes, go to nextcheck
		nop
		ble  $a0  65  Error 					#Anything less than A is an error
		nop
	nextcheck:
		ble  $a0  90  count					#If the chracter is greater than A but less than Z, move on to the next
		nop
		bge  $a0  90  thirdcheck				#If the chracter is greater than Z go to the thirdcheck
		nop
	thirdcheck:
		bge  $a0  96  fourthcheck				#If the character is greater than a move on to the next check
		nop
		ble  $a0  96  Error					#If the character is greater than Z but less than a, then error
		nop
	fourthcheck:
		bge  $a0 123  Error					#If the chracter is greater than { then error
		nop
		j     count						#At the end, if there are no errors, move on
			
	count:
		lb   $a0,  0 ($s0)					#Load the first character to be counted
		beqz $a0,     continue					#If the character is nothing, then the characters have been counted and now can move on
   		nop
   		addi $s0 $s0  1						#After the getting the first character, go to the next one
    		addi $t0 $t0  1						#Add One to the Count
    		bge  $t0  20  Error					#If the count is greater than 20, then Error message
    		nop
    		j     count						#Go to the next character

	continue:
		lw   $s0,    ($a1)					#Get the contents of the file
			
		
		li   $v0      13					#Syscall 13 opens the file
		la   $a0     ($s0)					#Get the file name to see where to read from
		li   $a1       0					#I am telling the computer with the flag of 0 to read only
		syscall 						#Execute
		move $s0      $v0 					#Save the flag 
		
	readfile:	
		li   $v0       14					#Syscall 14 reads the file
		move $a0      $s0					#Get the flag from earlier
		la   $a1      buffer					#Store the contents of the file in the buffer
		la   $a2      128					#128 byte buffer value to go through only 128 bytes
		syscall							#Execute
		la   $t0    ($v0)					#The syscall 14 returns the amount of characters it has gone through in register $v0
									#With this in mind, store the last value of v0 into a register.
									#Later this will used as the value to iterate to. Go to the same number of characters that the file has read so far
		beq  $t0   00 SuccessMessagePrint			#If the value of $t0 is 0, then the full file has been read without errorsand now can go to the success message
		nop
		j IsBrace
	closeFile:
		li $v0 16						#After done reading, syscall 16 closes the file
		move $a0 $s0						#Get the flag that its over
		syscall							#Execute

	
	IsBrace:         
	        beq  $t5  $t0  readfile  				#Iterate through the entire file from 0 to the current return value of the readfile function
		nop
		lb   $a0    0 ($a1)					#Get the first character of the file contents 
		addi $a1  $a1    1					#The next iteration through, will be the next character of the file contents
		beqz $a0       isError					#After done going through the file, check for errors
		nop
		addi $t3  $t3    1					#Add the index value by one
		beq  $a0  123  CountBracketLeft				#If a left bracket, go to CountBracketLeft
		nop
		beq  $a0  125  Check					#If a right bracket, then check if it matches the stack
		nop
		beq  $a0   40  CountParenthesesLeft			#If a left parentheses, go to CountParenthesesLeft
		nop
		beq  $a0   41  Check 					#If a right parentheses, check if it matches the stack
		nop
		beq  $a0   91  CountBraceLeft				#If a left brace, go to CountBraceLeft
		nop
		beq  $a0   93  Check 					#If a right brace, check if it matches the stack
		nop
		addi $t5  $t5  01					#Add one to the byte count. When this value is equivalent to t0, then go back to the readfile to see if there is anything more to read
		j    IsBrace 						#Go to the next character
		
		
									#Store the correct amount of braces in the stack.
									#For every left brace, there should be a right brace
									#Store the right braces in the stack 
									#The amount of braces in the stack should equal the amount of left in the file
									#If it is different, then the error message should be outputted
	CountBracketLeft:
		xor  $a0    6						#Instead of storing the left bracket, store its opposite
		addi $sp  $sp  -4					#Change the stack pointer by -4
		sw   $a0 ($sp)						#Push a right bracket in the stack
		addi $t1  $t1  01 					#Adds a Pair to later be subtracted from if a match is found
		addi $t2  $t2  01					#Adds a Pair to later be shown if there is a success
		j    leftadd 						#Go to Right Add Subroutine
		
	CountParenthesesLeft:
		xor  $a0   01						#Instead of storing left parentheses, store its opposite
		addi $sp  $sp  -4					#Change the stack pointer by -4
		sw   $a0 ($sp)						#Push the right parentheses in the stack
		addi $t1  $t1  01 					#Adds a Pair to later be subtracted from if a match is found
		addi $t2  $t2  01					#Adds a Pair to later be shown if there is a success
		j    leftadd						#Go to Right Add Subroutine
			
	CountBraceLeft:							
		xor  $a0   06						#Instead of storing left bracket, store its opposite
		addi $sp  $sp  -4					#Change stack pointer by -4
		sw   $a0 ($sp)						#Push the right brace in the stack
		addi $t1  $t1  01 					#Adds a pair to later be subtracted from if a match is found
		addi $t2  $t2  01					#Adds a Pair to later be shown if there is a success
		j    leftadd 						#Go to Right Add Subroutine
	
	leftadd:
		addi $t8  $t8  01					#Add the amount of left braces by one
		j    IsBrace						#Go to the next character		
	
	Check: 
		addi $t7  $t7  01					#Add one to the right brace counter
		lw   $s0 ($sp)						#Pop the latest value from the stack
		addi $sp  $sp  04					#Increase the stack pointer by four
		beq  $s0  $a0  subtractpair 				#If the value popped out from the stack is equal to a right brace, right bracket, or right parentheses, then subtract a pair
		nop
		bne  $s0  $a0  immediateerror				#If the values are different, then an error has occured
		nop
		j    IsBrace						#Go to the next character of the file contents
	
	subtractpair:
		sub  $t1  $t1  01					#If a match is found, subtract the total pairs by one.
									#If it is successful, this register value should be zero
									#Zero in this register means that all pairs have been found
									#If not, then there will be an error
		j    IsBrace 						#Go to the next character of the file contents
			
	immediateerror:
		move $t4  $a0 						#If there is an error, then the current character is the error, store that value in the register
		j    MismatchMessagePrint				#Go to the MismatchMessagePrint Subroutine
		

		
	isError:							#isError only accessed after the full string is indexed
		bgt  $t8  $t7  StillonStack				#If the total left braces are greater than the right braces, then go to StillonStack subroutine
		nop
		bge  $t1   01  MismatchMessagePrint 			#If the total number of pairs is greater than or equal to one, then go to the mismatch message
		nop
		beq  $t1   00  SuccessMessagePrint			#If the total number of pairs is zero, then there are no errors
		nop
	MismatchMessagePrint:	
		sub  $t3  $t3  01					#Subtract the index by one to get the proper index with zero as the starting value
		li   $v0   04						#Get Ready to Print a String
		la   $a0   MismatchMessage				#The system should print the Mismatch Message
		syscall							#Print
		li   $v0   11						#Get Ready to print the improper brace character
		move $a0  $t4						#Move the improper brace to the arguments register so the system knows to print that value
		syscall 						#Print
		li   $v0   04						#Get Ready to Print a String
		la   $a0   atIndex					#The System should print the atIndex string
		syscall 						#Print
		li   $v0   01						#Get Ready to Print an Integer
		move $a0  $t3						#Move the index value into the arguments register so the system knows what value to print 
		syscall 						#Print
		j    Ending						#Go to the End
	
	SuccessMessagePrint:  
		li   $v0    04						#Get ready to print a string
		la   $a0    SuccessMessage				#Load the SuccessMessage
		syscall 						#Print it
		li   $v0    01						#Get ready to print the amount of pairs
		move $a0   $t2						#Move the pairs amount into the arguments register
		syscall 						#Print
		li   $v0    04						#Get ready to print a string
		la   $a0    PairsOfBraces				#Load the PairsofBraces message
		syscall 						#Print
		j    Ending						#Go to the End
	
	StillonStack:
		li   $v0   04						#Get ready to print a String
		la   $a0   stillonstack					#Load the stillonstack asciiz value 
		syscall 						#Print
		j    popall						#Go to pop all the values out from the stack
		
	popall:
		bge  $t6   $t8 Ending					#The total number of braces on the stack is equal to the amount of braces total
		nop							#So by going from 0 to the total left braces value, all braces will be popped
		lw   $s0  ($sp)						#Get the last value pushed from the stack
		addi $sp   $sp 04					#Increase the stack pointer by four
		beq  $s0   125 revert					#Because we are storing opposites, go back to its original form
									#If a right bracket, go to the revert subroutine
		beq  $s0    93 revert					#If a right brace, go to the revert subroutine
		beq  $s0    41 revertp					#If a right parerntheses, go to its specific subroutine
		addi $t6   $t6 01
		j    popall
		
	revert:
		
		xor  $t9   $s0 06					#Braces and Brackets are both xored by 6 to get its original value 
									#Store the reverted brace in the t9 register
		li   $v0    11						#Get Ready to print a character
		move $a0   $t9						#Move the brace to the arguments register
		syscall							#Print
		j    popall						#Get the next value from the stack
			
	revertp:
		xor  $t9   $s0 01					#To go from left to right parentheses and vice versa, xor by one 
									#Store the reverted parentheses in $t9 register
		li   $v0    11						#Get ready to print a character
		move $a0   $t9						#Move the brace to the arguments register
		syscall							#Print
		j    popall						#Get the next value in the stack
		
			
	Error:
		li   $v0    04						#Get ready to print a string
		la   $a0    ErrorMessage				#Load the Invalid Program Argument error message
		syscall 						#Print
		
	Ending:
		
		li   $v0    04						#Get ready to print a string
		la   $a0    newLine					#Load the newLine 
		syscall							#Print a new line at the end
		li   $v0    10						#Syscall 10 tells the program to end
		syscall 						#Execute
