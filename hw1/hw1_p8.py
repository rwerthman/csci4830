'''
Homework #1 Problem 8

Receive 4 4-byte integers from a port, add them together, and send back the sum in bytes to
    receive a username and password

'''

import socket
import time

# Create Socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('hitchens.cs.colorado.edu', 1234))

# Receive Bytes from hitchens
time.sleep(1)
data = s.recv(1024)
print("\nBytes of four integers ", data)
print("Length of the first receive: ", len(data), "\n")

# Separate the 4 4-byte integers
num_one = data[:4]
num_two = data[4:8]
num_three = data[8:12]
num_four = data[-4:]

# Print length in bytes and actual bytes of each number
print("The first number from hitchens is ", num_one)
print(len(num_one))
print("The second num bytes are ", num_two)
print(len(num_two))
print("The third num bytes are ", num_three)
print(len(num_three))
print("The fourth num bytes are ", num_four) 
print(len(num_four), "\n")

# Convert bytes to integer
num_one = int.from_bytes(num_one, 'little')
print("first num: ", num_one)
num_two = int.from_bytes(num_two, 'little')
print("second num: ", num_two)
num_three = int.from_bytes(num_three, 'little')
print("third num ", num_three)
num_four = int.from_bytes(num_four, 'little')
print("num_four", num_four, "\n")

# Add all four integers
return_num = num_one + num_two + num_three + num_four
print("Sum of four numbers: ", return_num, "\n")

#Convert sum to bytes and send them back
return_num = return_num.to_bytes(5, 'little')
print("Bytes to be sent back ", return_num,"\n")
s.send(return_num)

# Receive and print the response Username: vortex1 Password: Gq#qu3bF3
data = s.recv(4096)
s.close() 
print(data.decode("utf-8", "ignore"))

