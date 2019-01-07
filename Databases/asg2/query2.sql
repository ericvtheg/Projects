/* Eric Ventor
# Eventor@ucsc.edu
# CMPS 180
# Prof Finkelstein
# query2 */

/* Output the quantity and orderDate for each order that was made by a
member whose isCurrentMember is TRUE. The second attribute in your result
should appear as oDate. Duplicates may appear in your result. */

SELECT Orders.quantity, Orders.orderDate as oDate
FROM Orders
INNER JOIN Members ON Orders.memberID=Members.memberID
WHERE Members.isCurrentMember = TRUE;
