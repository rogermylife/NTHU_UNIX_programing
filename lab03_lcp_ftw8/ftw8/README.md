# lab03_ftw

## Content

 In Section 4.22, our version of ftw, called ftw8.c, never changes its directory. Modify this routine so that each time it encounters a directory, it uses the chdir function to change to that directory, allowing it to use the filename and not the pathname for each call to lstat. When all the entries in a directory have been processed, execute chdir(".."). Compare the time consumed by this version and the version in the text book.
* Trace the ftw8 source code given by TA. (compile with “make ftw8”)
* Modify the code with chdir 

This is a program to surf all files in <path> and record their file type.

There are some legal command
* ./ftw <path>
* ./ftw ~/
