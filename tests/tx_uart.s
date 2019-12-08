.global  _start

# Start a special section called .text.prologue making it
# allocatable and executable
.section .text.prologue, "ax"

_start:
# set stack pointer to 8192
lui sp, 2
j main

.text
