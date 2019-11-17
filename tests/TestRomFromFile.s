.global  _start

# Start a special section called .text.prologue making it
# allocatable and executable
.section .text.prologue, "ax"

_start:
j main

.text
