import sys
import re

from pyspark import SparkContext,SparkConf

def flatMapFunc(document):
    """
    Before we returned a list of words and used the map and reduce fucntions to
    determine how many times each word occurred (regardless of document ID).
    Now we want to know how many different documents contain the word.

    This function should create a list of items which we will then run various
    transformations on to eventually create a count of documents per word.

    document[0] is the document ID (distinct for each document)
    document[1] is a string of all text in that document

    You will need to modify this code.
    """
    documentID = document[0]
    words = re.findall(r"\w+", document[1])
    return words

def mapFunc(arg):
    """
    Create `(key, value)` pairs.
    You may need to modify this code.
    """
    return (arg, 1)

def reduceFunc(arg1, arg2):
    """
    Sum counts.
    You may need to modify this code.
    """
    return arg1 + arg2

def perWordDocumentCount(file_name, output="spark-wc-out-perWordDocumentCount"):
    sc = SparkContext("local[8]", "PerWordDocumentCount", conf=SparkConf().set("spark.hadoop.validateOutputSpecs", "false"))
    file = sc.sequenceFile(file_name)

    """
    You will need to add, remove, and/or modify function calls here.
    The function `distinct()` may be helpful...
    Be sure that your output ends up in alphabetial order.
    """
    counts = file.flatMap(flatMapFunc) \
                 .map(mapFunc) \
                 .reduceByKey(reduceFunc)

    counts.coalesce(1).saveAsTextFile(output)

""" Do not worry about this """
if __name__ == "__main__":
    argv = sys.argv
    if len(argv) == 2:
        perWordDocumentCount(argv[1])
    else:
        perWordDocumentCount(argv[1], argv[2])
