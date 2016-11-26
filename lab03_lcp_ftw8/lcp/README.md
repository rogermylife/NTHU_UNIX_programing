# lab03_lcp

## Content
Write a utility like cp(1), say lcp, that copies a file containing holes, without copying the hole to the target file. Instead, your utility writes “\0” to fill those holes in the target file.
* lcp only needs to support basic copy feature; copying a file to another file. (usage : lcp <source_file> <destination_file>)
* Your output file should be identical to the original input file (size, content), but the block usage on the disk is different.
* You need to check how to create a file with holes as explained in Chapter 3.
* The test cases will be regular files, and you don’t have to handle any unusual exceptions.

**simple explanation: Compared with command `cp`, lcp will not copy the holes in files.However, it will write `\0` to new file instead of a hole.**


