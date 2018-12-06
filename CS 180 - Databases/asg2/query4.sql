/* Eric Ventor
# Eventor@ucsc.edu
# CMPS 180
# Prof Finkelstein
# query4 */

/*
Find the reviewerID and bookID for each review that has all of the following three properties:
• The number of reviewStars isn't 4.
• The book is named 'Jane Eyre'.
• The reviewer is a member whose name is 'John Smith'.
No duplicates should appear in your result.
*/

SELECT DISTINCT Reviews.reviewerID, Reviews.bookID
FROM ((Reviews
INNER JOIN Books ON Reviews.bookID = Books.bookID)
INNER JOIN Members ON Reviews.reviewerID = Members.memberID)
WHERE Reviews.reviewStars <> 4 AND
    Books.bookName = 'Jane Eyre' AND
    Members.memberName = 'John Smith';
