Firewall Simulation using Hash Tables, Bloom Filters and Binary Trees
-----------
DISCLAIMER:
-----------

THIS IS A MODIFIED ASSIGNMENT DONE FOR CSE13S AT UC SANTA CRUZ. IF YOU ARE A CSE13S STUDENT, DO NOT VIEW THE FOLLOWING SOURCE CODE!
-----------

Description
----
This runs a firewall simulation program. By first reading a list of bad speak words first, the program will now have an idea of what words and phrases to flag if they appear in any text forms. Now that the program knows which words to flag, the old speak and newspeak words can be scanned in. Only the old-speak words should be added to the Bloom filters. The old-speak and the newspeak words are added to the hash table. With the words now accounted for, the program can begin to filter words out. With each word that is read in, if the word has not been added to the bloom filter, then the word is not a flagged word. If the word has been added to the filter already, then a few of the following actions can be taken. If the hash table contains the word and it does but does not have a newspeak translation, then a thoughtcrime has occurred and it is inserted into the bad speak words. If the hash table contains the word, and the word has a newspeak translation, then the citizen needs right speak counseling. If the hash table does not contain the word, then no action is taken.

Building the Program
----------
By running make all, make banhammer, or make will build an executable that functions and works properly. By using make format, all files will be formatted according to clang specifications.

Running the Program
----------
Run the program by selecting which test you would like to run. -s will select the statistics option to be selected. The -f option will prompt the user to input the size of the bloom filter. The -t option will prompt the user to input the hash table size. The -h command will print out the help message. 

Bugs/Issues
-----------
There should be no major bugs or issues in this program that will cause the output to greatly differ from the binary reference.
