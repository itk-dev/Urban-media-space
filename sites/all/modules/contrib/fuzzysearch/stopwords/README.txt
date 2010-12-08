// $Id: README.txt,v 1.1.2.1 2010/10/06 13:25:07 awolfey Exp $

The directory fuzzysearch/stopwords contains text files that can be used to keep
common words out of the fuzzysearch index. Stop words are also stripped from
search terms. This has performance benefits and can make search results more
useful.

Do not modify files in this directory. First move them as described below.

How to use stopwords files:
The files here will have no effect until they are copied to the directory
sites/all/libraries/fuzzysearch/stopwords. Only when copied to that directory
will fuzzysearch be aware of them.

You can modify stopwords files after you copy them to
sites/all/libraries/fuzzysearch/stopwords, and you can also add your own
custom stopwords files there. You might want to keep offensive words out of your
index and create a file of these bad words.

How to name stopwords files:
Stopwords files must follow this naming convention to be recognized by
fuzzysearch: fuzzysearch_stopwords_xx.txt, where "xx" is any number of file-safe
characters that you use to identify your file. Some examples:

fuzzysearch_stopwords_en.txt for English stop words
fuzzysearch_stopwords_de.txt for German stop words
fuzzysearch_stopwords_dirty.txt for offensive stop words

How to build stopwords files:
See fuzzysearch_stopwords_en.txt for an example. Separate words with spaces.
Words should be all lowercase, with no punctuation. If you want to
stop "doesn't" include "doesnt" in your file. Do not use commas or quote marks.

How to contribute stopwords files:
If you have made a stopwords file for a language that is missing, please
contribute it in the fuzzysearch issue queue.