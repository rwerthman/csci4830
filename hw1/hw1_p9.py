# Homework #1 Problem 9 
# Find inputs that cause a md5 saylor-6 collision

import hashlib
import string
import time

count = 0
print("\nRunning...\n")
t0 = time.time()

# Count up from one and cast numbers as strings and hash those strings
# Put the first six and last six hex digits of md5 hash into a key
# Store strings of numbers that create key as values
dictionary = dict()
while count < 17000000:
    b = str.encode(str(count))
    s2 = hashlib.md5(b)
    s2 = s2.hexdigest()
    s2_eight = s2[:6] + s2[-6:]
    dictionary.setdefault(s2_eight, []).append(b)
    count = count + 1
    
# Find the keys that have more than one value associated with them
# The values of those keys cause a collision    
for key, value in dictionary.items():
    if len(value) > 1:
        print ("The 12 hex match is: ", key, "\n", "The matching inputs are: ", value)
        print ("The saylor-6 hash collision is ", hashlib.md5(value[0]).hexdigest(), "\n")
        

t1 = time.time()
print("\nStopped!\n")
print("\nTotal time to run ", t1-t0, "seconds\n")

# Inputs that cause a Saylor-6 collision are: 4308906, 6857790



    
