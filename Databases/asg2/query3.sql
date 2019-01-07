/* Eric Ventor
# Eventor@ucsc.edu
# CMPS 180
# Prof Finkelstein
# query3 */

/* Category is 'N’ for non-fiction books and 'F' for fiction books.
Find the authorName and publisherName for each non-fiction book
whose price is greater than or equal to 19.99; no duplicates
should appear in your result. */

SELECT DISTINCT Authors.authorName, Publishers.publisherName
FROM ((Books
INNER JOIN Publishers ON Publishers.publisherID = Books.publisherID)
INNER JOIN Authors ON Authors.authorID = Books.authorID)
WHERE Books.category = 'N' AND
    Books.price >= 19.99;
