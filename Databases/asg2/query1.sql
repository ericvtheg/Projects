/* Eric Ventor
# Eventor@ucsc.edu
# CMPS 180
# Prof Finkelstein
# query1 */

/* Find the authorID and authorName for each author whose name doesn't
begin with the letter S. You result should appear in descending order of
authorName, and no duplicates should appear in your result. */

SELECT DISTINCT authorID, authorName
FROM authors
WHERE authorName NOT LIKE 'S%'
GROUP BY authorID, authorName
ORDER BY authorName DESC;
