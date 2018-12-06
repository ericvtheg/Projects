/* Eric Ventor
# Eventor@ucsc.edu
# CMPS 180
# Prof Finkelstein
# Unit Tests 2.5 */

/*
For each of the 3 foreign key constraints specified in section 2.3, write one unit test:
• An INSERT command that violates the foreign key constraint (and elicits an error).

Also, for each of the 3 general constraints, write 2 unit tests:
• An UPDATE command that meets the constraint.
• An UPDATE command that violates the constraint (and elicits an error).
*/

INSERT INTO Books (bookID, authorID, publisherID)
VALUES ('asffdf', 5463, 23464);

INSERT INTO Orders (memberID, bookID, orderDate, quantity)
VALUES (32452, 'fasgjk', CURRENT_DATE, 0);

INSERT INTO Reviews (reviewerID, bookID, reviewStars)
VALUES (38233, 'blahba', 5);

UPDATE Books
SET authorID = 643
WHERE bookID = 'kgoetm';

UPDATE Books
SET authorID = 6434
WHERE bookID = 'kgoetm';

UPDATE Orders
SET bookID = 'dmfnth'
WHERE memberID = 4843;

UPDATE Orders
SET bookID = 'wrttsw'
WHERE memberID = 1794;

UPDATE Reviews
SET reviewerID = 7447
WHERE reviewDate = '2006-05-26';

UPDATE Reviews
SET reviewerID = 96541
WHERE reviewDate = '2017-08-23';
