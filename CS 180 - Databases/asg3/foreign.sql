/* Eric Ventor
# Eventor@ucsc.edu
# CMPS 180
# Prof Finkelstein
# Foreign 2.3 */

/*
• The authorID field in Books should reference the authorID primary key Authors.
• The bookID field in Orders should reference the bookID primary key in Books.
• The reviewerID field in Reviews should reference the memberID primary key in Members.
*/

ALTER TABLE Books
ADD FOREIGN KEY (authorID) REFERENCES Authors(authorID);

ALTER TABLE Orders
ADD FOREIGN KEY (bookID) REFERENCES Books(bookID);

ALTER TABLE Reviews
ADD FOREIGN KEY (reviewerID) REFERENCES Members(memberID);
