/* Eric Ventor
# Eventor@ucsc.edu
# CMPS 180
# Prof Finkelstein
# Query Views 2.6.2 */

/*
Some publishers who are GreatPublishers published books that have BadBookTotals.
For each publisher that has at least one book that appears in BadBookTotals, output
that publisher’s publisherID, the number of books they published (numReviewedPublished
from GreatPublishers),and the number of books with BadBookTotals that they
published. The third attribute in your result should be called numBad
*/

SELECT DISTINCT g.publisherID, g.numReviewedPublished,
    (SELECT COUNT(DISTINCT b1.bookID)
     FROM BadBookTotals b1, Publishers p
     WHERE bo.bookID = b1.bookID
       AND p.publisherID = bo.publisherID) as numBad
FROM GreatPublishers g, BadBookTotals b, Books bo

/*before deletion:
publisherid | numreviewedpublished | numbad
-------------+----------------------+--------
      31725 |                    2 |      0
      31725 |                    2 |      1
*/

DELETE ORDERS WHERE memberID = 8844 AND bookID = 'jgzhwq'
DELETE ORDERS WHERE memberID = 2161 AND bookID = 'rrrrrr'

/*after deletion
publisherid | numreviewedpublished | numbad
-------------+----------------------+--------
      31725 |                    2 |      0
      31725 |                    2 |      1 */
