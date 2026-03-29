#!/bin/sh

# This test uses the --execute-for command line option, it is similar to the test58.sh
# but this time we have an actual control whether it works
# In order to check the test we use the test59.chomik file
# it should fail.

exec chomik --execute-for common string "let something=value integer 123; <print \"we assign something to\" <something>>;" \
			--execute-for instant string "<print \"something=\" <something>>;" \
			--execute-for common file "$(dirname $0)/test59.chomik" \
			--execute-for common string "<print \"the last execute still remembers something=\" <something>>; let the program return = value integer 1; <perform test <something> and 333>;"


