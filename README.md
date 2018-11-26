# Product-search
Search tool for specific product over a big range of dataset

qsearch: a search tool for finding the most relevant product item you are looking for
# Purpose:
     qsearch is a search tool, written in C++11, for finding the most relevant product information
     a client is looking for. The products are stored in a CSV file with the following format:
     category,title,description,price
     
     The client inputs some search keys from a computer keyboard, and the search tool then find
     the related items, ranking in decreasing order of relevance to the search words.

     The search results will be written into a file, which uses the search words (replacing the spaces with
     '-' sign) as file name. The score of relevance, together with the item number (the sequence of the
     item in the data file), the item's title, will be written into another file named 'scorefile.tex'. The first
     m (an integer with default value 6) items with the highest scores are printed on the screen.

# Installation:

There is no installation needed to build the tool, no external dependances are needed. Just
     place the zipped file into any directory in your computer, unzip it, goto the folder containing
     this readme.md file,  execute the mksearch.sh  by typing:
             sh mksearch.sh

     An executable file named qsearch will be created in the bin/ directory
     The sample data file named "products.csv" is located in the data/ directory. The client can extend this file,
     or uses his/her own data file of csv format, with the following line as its first line, which indicates the data structure:
             "category","title","description","price"
             
#  Build the search system
      To build the search system, the  data file will fist be read into the memory, and tokenized into
      vectors of words. The 'stopwords' in the description field of the data will be removed before
      inputing it into the vector. For the test data file of about 93M bites, it took 14 seconds to
      build the system on my laptop which operates on Linux Ubuntu 16.04.
      
      
# Use the tool
      To use the search tool, the client can either copy the qsearch executable into a folder where
      the operating system can find, such as ~/bin, /usr/local/bin, or create a folder inside
      the direacory where this readme.md file sits, and cd into the directory.

      The command to build the search system is some like:
                 ../bin/qsearch  ../data/products.csv
       or simply
              qsearch  data-file-name
      if  qsearch is set in the search path.  Once the search system is build,
      you can just type the search words. A typical example is given below:
 
threads available:4
m:6
Building search system, please wait.....
Number of threads used: 1
contents:125585
number of items read:125585
elapse time:14 seconds.
Please input query string,words are separated by space, Press return key to end input,  input -1 to end session.
        $ ipad mini case
      
Output:
Category: Computers & Accessories/Accessories/Tablet Accessories
Title: Targus Click-In Case for iPad Mini 1/2/3/4 Black
Description: 
   Slim sleek and lightweight - this Click-In Case by Targus is an everyday essential for your iPad mini 1/2/3/4.
   Not only will it keep it safe from bumps and scratches 
   but opening and closing the case will automatically activate and deactivate the tablet saving you power on the
   move. Designed specifically for iPad mini 1/2/3/4 
   it doesn't block the speakers or camera. It also acts as a multi-positional stand 
   so you can enjoy your apps and media at any angle. - Black 
Price: 21.99
      
Category: Computers & Accessories/Accessories/Tablet Accessories
Title: Targus Versavu iPad Mini 4/3/2/1 Case - Black
Description: 
   The ultra slim rotating case lets you turn your iPad mini 360 degrees inside the case for a choice of viewing in landscape or
   portrait. With automatic wake and sleep functionality the durable cover transforms into a multi position stand for handsfree
   viewing and a comfortable typing position. 360 degrees rotating iPad mini case with auto wake and sleep. Ultra slim form fit
    case with multi position 360 degrees stand for use in portrait and landscape. Secure front cover closure and soft interior
    lining protects iPad mini screen on the go. Comfortable typing position. (not included). Manufacturers limited lifetime
    guarantee. - Technology > iPad
   tablets and E-readers > iPad and tablet covers and cases - Black 
Price: 39.99

Category: Computers & Accessories/Accessories/Tablet Accessories
Title: Targus Click-In iPad Mini 4 / 3 / 2 / 1 Tablet Case - Grey
Description: 
   All round protection for your iPad mini. The Click-In case features a hard back cover
   a shock absorbing tray which clicks securely around your iPad mini to protect the back and sides 
   and a durable front cover with a soft interior to protect the device screen. Perfect fit protection case for iPad mini 1 
   2 3 and 4. Shock absorbing SMART-FIT tray for front  rear and side protection. Hard back cover design. Soft internal lining.
   Protects from bumps and scratches. Allows full accessibility to all functions and ports. Lightweight. Camera lens cut-out.
    iPad. Polyurethane. Manufacturers limited lifetime guarantee. - Technology > iPad 
   tablets and E-readers > iPad and tablet covers and cases - Grey 
Price: 21.99
full search results are written to file :ipad-mini-case.txt
Please input query string,words are separated by space, Press return key to end input,  input -1 to end session.

# Help on using the tool
     It is straightford to use this tool. help info can be found by just typing the command "qsearch", or type
             $ qsearch -h
     
# Score schemes

Two score schemes are implemented: the classical TF-IDF (term frequency, inverse document frequency)
scheme and a noval scheme, named 'keymatch', proposed by myself, are implemented.
It has been shown that the keymatch scheme I designed more precisely selects the most relevant items
for the client, giving those items higher score.

The novality of the 'keymatch' scheme is to emphasise the title field, match it against the search keys.

When full match is found, it gives the highest score for this field. However, if a redundant 'descriptive'
 word is found, then the score will be reduced. For example, when a client input a query:
           ipad mini
he/she wants to find information about  'ipad mini', not 'ipad mini case'. However, items whose title
include 'ipad mini' and 'ipad mini case' will get the same score. The latter one's score needs to be reduced,
because it contains a redudant word 'case', which makes the title mean another item : a case used for
ipad mini, this is obviously not what the client wants. Some common descriptive words such as 'unit',
'bag','box','cover','case', etc are included in the source code.

# Test runs
    Two directories, named   keymatch/    and  tf-idf/ are created.  Results for a few sample searches  are
    placed in each directory. In the keymatch/  directory, searches are conducted using the 'keymatch' scheme,
    while in the  tf-idf/ directory, searches are conducted using the  tf-idf/  scheme.
    
# Time  and space complexity
     Time complexity for building the search system is O(N), where N is the number of words 
     in the data file. A way to speed up is by parallelization. Since the search system can be built
     in the background, without the need for the customer to wait, this is not an issue for
     customers.

 In the implementation of this project, the product items are first filtered out for 'effective
     items', which requires their title words to match at least one search key word. This filtering greatly
     reduces  the size of the 'effective items'. The ranking procedure is conducted over the 'effective items',
      thus is quick.

     Time complexity for search is of O(N), because building the 'effective item' list needs to loop
     over the whole dataset. This process can be parallelized over many processors. Searching efficiency
     is crucial, since it takes the customers'waiting time. A customer usually does not have the patience
     to wait for more than 30 seconds for the results to come out.

# Build of 'effective items' for a search query   
  For current data of around 125,000 product items, search a query takes less than 1 second.
     This is efficient enough. For a million data items, it may take a few seconds, still not an issue.
     However, for hundreds of millions deta items, search a query may take a few minutes.
     This is more than a customer can wait. Parallelization is the easiest way to solve the problem.
     Another solution is to store the scores for all the possible searching words over
     the whole product dataset. The commonly-used English words are less than 10,000. Time to seach
     over a list of 10,000 items is negligible. This means that the list of  'effective items' for all
     possible queries can be build and stored before a query is submitted, thus greatly reduced
     the customer's waiting time for very large datasets, say 1billion items.

# Space complexity is of O(M)
     where M is the total length of the data file in bytes.
     To handle very large data set, the Hadoop big data framework may be used.
     Storage is cheap, so space complexity is not a big issue to solve.
# Removal of stopwords in search query
     To reduce the time complexity, it is helpful to remove the unnecessary words
     a customer may put in his/her search query. For example a customer  type:
     I really want to buy a beautiful Apple Iphone
     In this query, the only relevant words are apple, iphone. The rest are all
     redundant, and should be removed from the search. This functionality has been implemented
     in the current search tool.




             
