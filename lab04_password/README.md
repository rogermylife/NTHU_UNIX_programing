# lab04_password


## Content
Please write a tool to break a /etc/shadow line using dictionary attack. 

Your tool will load this dictionary into memory, and tries to concatenate three words into the plaintext password candidate. Use the crypt library to find the actually password of a person. The TA will time how fast your code can break the password. The top 10% of the student will receive 2 bonus points.

* Your tool, say decrypt, that reads the hashed password from a file, and output the decrypted password. (sample test case can be downloaded from the iLms)

* The password is composed of three random words from the given dictionary file.
*
