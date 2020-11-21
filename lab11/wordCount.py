import sys
import re

from pyspark import SparkContext,SparkConf

def splitDocument(document):
    """Returns a list of all words in the document"""
    return re.findall(r"\w+", document[1])

def toPairs(word):
    """ Creates `(key, value)` pairs where the word is the key and 1 is the value """
    return (word, 1)

def sumCounts(a, b):
    """ Add up the values for each word, resulting in a count of occurences """
    return a + b

def wordCount(file_name, output="spark-wc-out-wordCount"):
    sc = SparkContext("local[8]", "WordCount", conf=SparkConf().set("spark.hadoop.validateOutputSpecs", "false"))
    """ Reads in a sequence file FILE_NAME to be manipulated """
    file = sc.sequenceFile(file_name)

    """
    - Explanation:
    -
    - `flatMap` takes in a function that will take one input and outputs 0 or
    -   more items. All returned results are combined into a single list of
    -   items that future functions are run on. We use this function to
    -   transform our document into a list of words.
    -
    - `map` takes in a function take in one item, perform an action on it, and
    -   return the result. When called on a list, it applies the function to
    -   each item in the list. We use this function transform our words into
    -   `(key, value)` pairs, with the key being the word and the value being
    -   the number of times it occurs.
    -
    - `reduceByKey` groups a list of `(key, value)` pairs by keys and runs a
    -   function on each key which takes two values and returns a single value
    -   (i.e. "reducing" them two inputs into one). It will be called
    -   iteratively on each key until only a single value remains for that key.
    -   We use this function to sum the number of times a word occurs.
    """
    counts = file.flatMap(splitDocument) \
                 .map(toPairs) \
                 .reduceByKey(sumCounts)

    """ Takes the dataset stored in counts and writes everything out to OUTPUT """
    counts.coalesce(1).saveAsTextFile(output)

""" Do not worry about this """
if __name__ == "__main__":
    argv = sys.argv
    if len(argv) == 2:
        wordCount(argv[1])
    else:
        wordCount(argv[1], argv[2])
