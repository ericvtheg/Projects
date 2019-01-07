/* Eric Ventor
# Eventor@ucsc.edu
# CMPS 180
# Prof Finkelstein
# General 2.4 */

/*
1. In Orders, quantity must be positive. Please give a name to this positive
quantity constraint when you create it. We recommend that you use the name
positive_quantity, but you may use another name. The other general constraints
don’t need names.
2. In Books, lastOrderDate must be greater than or equal to pubDate.
3. In Members, if joinDate is NULL then isCurrentMember must also be NULL.
*/

ALTER TABLE Orders
ADD CONSTRAINT positive_quantity CHECK (quantity > 0);

ALTER TABLE Books
ADD CHECK (lastOrderDate >= pubDate);

ALTER TABLE Members
ADD CHECK (
    (joinDate is NULL AND isCurrentMember is NULL)
    OR
    (joinDate is NOT NULL));
