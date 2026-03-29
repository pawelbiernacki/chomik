#!/bin/sh

# This test uses the --execute-for command line option, it is similar to the test58.sh and test59.sh

# It is supposed to succeed, because 'the program return' will be assigned with 0 by the 'perform test ...' code stored in the test60.chomik file.

chomik --execute-for common file "$(dirname $0)/test60.chomik" --execute-for common string "let the program return = value integer 1; <perform test 45 and 45>; <print <the program return>>;"

