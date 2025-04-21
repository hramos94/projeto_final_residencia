import sys
import subprocess

# Command we want to execute
command = ['make', 'test']

# Executing the command in the terminal
with open('test/scripts/test_capture.txt', 'w') as f:
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    
    # Reading and capturing the command's output
    for line in process.stdout:
        print(line, end='')   # Printing to the terminal
        f.write(line)         # Writing to the file

    # Reading and capturing errors, if any
    for line in process.stderr:
        print(line, end='', file=sys.stderr)  # Printing errors to the terminal
        f.write(line)         # Writing to the file

    # Waiting for the process to finish
    process.wait()

print("Execution completed. See the output in 'test_capture.txt'.")