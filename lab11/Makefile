# Build file for CS61C MapReduce Lab
# You should not need to edit this file.

# This file requires GNU make and depends on paths on instruction machines.

####

all:

generate-input:
	hadoop jar textImporter/wc.jar Importer $(myinput) seqFiles/

clean:
	rm -rf spark-wc-out*

.PHONY: clean all
