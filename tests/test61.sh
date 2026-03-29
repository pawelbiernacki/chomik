#!/bin/sh

# This test shows why the instant execution mode cool - the instant machines are newly initialized every time we use them
# In this test we use also the command line option --set-the-chomik-string:
# The symbols 'the chomik "string" (NUMBER:range 1..10)' are predefined.

# This test does not fail. It is supposed merely to demonstrate a feature.

echo "Each instant machine is initialized every time we create it, so we can tell pass some values to them (using --set-the-chomik-string) and they will see the current value:"

chomik --set-the-chomik-string 1 "alpha" --execute-for instant string "<print <the chomik \"string\" 1>>;" --set-the-chomik-string 1 "beta" --execute-for instant string "<print <the chomik \"string\" 1>>;"


echo "This does not work in the common execution mode, only the common machine is initialized only when it is used for the first time:"

chomik --set-the-chomik-string 1 "alpha" --execute-for common string "<print <the chomik \"string\" 1>>;" --set-the-chomik-string 1 "beta" --execute-for common string "<print <the chomik \"string\" 1>>;"

