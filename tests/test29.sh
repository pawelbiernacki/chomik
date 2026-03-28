#!/bin/sh

# a wrapper for the test29.chomik
exec chomik --set-the-chomik-string 1 "$(dirname $0)" "$(dirname $0)/test29.chomik"

