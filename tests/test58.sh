#!/bin/sh

# This is a standalone test - no Chomik code files are involved
# It demonstrates the usage of the --execute-for command line option

exec chomik --execute-for common string "let something=value integer 123; <print \"we assign something to\" <something>>;" \
			--execute-for instant string "<print \"something=\" <something>>;" \
			--execute-for common string "<print \"the last execute still remembers something=\" <something>>; <compare \"integer\" <something> 123>; <print <the compare result>>;"


