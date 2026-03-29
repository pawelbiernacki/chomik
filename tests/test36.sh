#!/bin/sh

# The wrapper was added because of the CI. The fix is similar to the one applied
# in the test29.sh . It relies on the new chomik features
# (the chomik strings ...) and concatenate.

# a wrapper for the test30.chomik
exec chomik --set-the-chomik-string 1 "$(dirname $0)" "$(dirname $0)/test36.chomik"
