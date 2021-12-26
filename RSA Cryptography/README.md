Public Key Cryptography
-----------------------

DISCLAIMER:
-------------------

THIS IS A MODIFIED VERSION OF AN ASSIGNMENT DONE FOR CSE13S AT UC SANTA CRUZ. IF YOU ARE A CSE13S STUDENT, DO NOT VIEW THE FOLLOWING SOURCE CODE!
-------------

Description
---------
This is a public key cryptography program. Public key cryptography utilizes two different pairs of keys. The first key is a public key, which is known to other people and the second key is a private key. The private key is only known by the owner of that key. These keys are generated using different cryptographic algorithms. With RSA encryption, a person encrypts a message using the receiver’s public key. However, this message can only be decrypted using the receiver’s private key. The RSA algorithm depends on the principle of factoring the product of two large prime numbers. By choosing two large prime numbers, their product can only have two factors, therefore making it nearly impossible to decipher someone’s private key.

Building the Program
-------------
By running make all, make keygen will make the keygen executable, make decrypt will make the decrypt executable, and make encrypt will make the encrypt executable, or make all will build all executables that function By using make format, all files will be formatted according to clang specifications. Make clean will delete all .o files and binaries

Running the Program
------------------
Run the program by selecting which test you would like to run. -i selects the infile, -o selects the outfile, -h prints the help menu, and -v prints out compression statistics. The same works with the decryption file. For keygen, -b specifies the number of bits, -i specifies the iterations, -n specifies the public file, -s specifies the seed, and -h prints the help.

Bugs/Issues
-----------------
There are no bugs to report.
