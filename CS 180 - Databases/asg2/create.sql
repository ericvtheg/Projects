/* Eric Ventor
# Eventor@ucsc.edu
# CMPS 180
# Prof Finkelstein
# Tables */

/* Found on Lab1 pdf */
DROP SCHEMA Lab2 CASCADE;
CREATE SCHEMA Lab2;

/* Authors(authorID, authorName, address, numBooksWritten, mostRecentPubDate) */
CREATE TABLE Authors(
   authorID INT PRIMARY KEY,
   authorName VARCHAR(30),
   address VARCHAR(30),
   numBooksWritten INT,
   mostRecentPubDate DATE
);


/* # Books(bookID, authorID, bookName, publisherID, pubDate, price, category, lastOrderDate, totalOrdered) */
CREATE TABLE Books(
   bookID CHAR(6) PRIMARY KEY,
   authorID INT,
   bookName VARCHAR(30),
   publisherID INT NOT NULL,
   pubDate DATE,
   price DECIMAL(6,2),
   category CHAR(1),
   lastOrderDate DATE,
   totalOrdered INT,
   UNIQUE(bookName, authorID)
);

/* # Publishers(publisherID, publisherName, address) */
CREATE TABLE Publishers(
   publisherID INT PRIMARY KEY,
   publisherName VARCHAR(30),
   address VARCHAR(30),
   UNIQUE(publisherName)
);

/* # Members(memberID, memberName, joinDate, renewalDate, isCurrentMember) */
CREATE TABLE Members(
   memberID INT PRIMARY KEY,
   memberName VARCHAR(30),
   joinDate DATE,
   renewalDate DATE,
   isCurrentMember BOOLEAN,
   UNIQUE(memberName, joinDate)
);

/* # Orders(memberID, bookID, orderDate, quantity) */
CREATE TABLE Orders(
   memberID INT,
   bookID CHAR(6),
   orderDate DATE,
   quantity INT NOT NULL,
   PRIMARY KEY(memberID, bookID, orderDate)
);

/* # Reviews(reviewerID, bookID, reviewDate, reviewStars) */
CREATE TABLE Reviews(
   reviewerID INT,
   bookID CHAR(6),
   reviewDate DATE,
   reviewStars INT NOT NULL,
   PRIMARY KEY(reviewerID, bookID)
);
