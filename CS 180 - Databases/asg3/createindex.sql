/* Eric Ventor
# Eventor@ucsc.edu
# CMPS 180
# Prof Finkelstein
# Create an Index 2.7 */

/*
create an index named LookUpOrders over the bookID and memberID columns
(in that order) of the Orders table */

CREATE INDEX LookUpOrders
ON Orders (bookID, memberID);
