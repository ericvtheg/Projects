/* Eric Ventor
# Eventor@ucsc.edu
# CMPS 180
# Prof Finkelstein
# query5 */

/*
Find the memberID and joinDate for each member who
reviewed a book that was published after April 1, 2010, yyyy-mm-dd
and that book was also written by an author who has the exact same name as the reviewer.
No duplicates should appear in your result. */

SELECT DISTINCT Members.memberID, Members.joinDate
FROM (((Members
INNER JOIN Reviews ON Reviews.reviewerID = Members.memberID)
INNER JOIN Books ON Books.bookID = Reviews.bookID)
INNER JOIN Authors ON Books.authorID = Authors.authorID)
WHERE books.pubDate > '2010-04-01' AND
    Authors.authorName = Members.memberName;
