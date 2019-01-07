/* Eric Ventor
# Eventor@ucsc.edu
# CMPS 180
# Prof Finkelstein
# Working with Views 2.6 */

CREATE VIEW GreatPublishers AS
    SELECT p.publisherID,
        ( SELECT COUNT( DISTINCT b1.bookID)
          FROM Reviews r1
            INNER JOIN Books b1 ON b1.bookID = r1.bookID
            INNER JOIN Publishers p1 ON p1.publisherID = b1.publisherID
          WHERE p.publisherID = b1.publisherID
            AND b1.bookID = r1.bookID) AS numReviewedPublished
   FROM Publishers p, Reviews r
   GROUP BY (p.publisherID, numReviewedPublished)
   HAVING 2 <=
        ( SELECT COUNT( DISTINCT b2.bookID)
          FROM Books b2, Reviews r2
          WHERE p.publisherID = b2.publisherID
            AND b2.bookID = r2.bookID)
       AND 3 <=
       ( SELECT MIN(r3.reviewStars)
         FROM Reviews r3, Books b3
         WHERE p.publisherID = b3.publisherID
           AND b3.bookID = r3.bookID);

CREATE VIEW BadBookTotals AS
    SELECT b.bookID, b.totalOrdered, SUM(o.quantity) as badQuantitySum
    FROM Books b, Orders o
    WHERE b.bookID = o.bookID
    GROUP BY b.bookID
    HAVING b.totalOrdered != (SELECT SUM(o1.quantity)
                                FROM Orders o1
                                WHERE b.bookID = o1.bookID);
