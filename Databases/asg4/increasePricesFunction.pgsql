/* Eric Ventor
# Eventor@ucsc.edu
# CMPS 180
# Prof Finkelstein
# Working with Views 2.6 */

/*
increase the price of Non-Fiction books by 1.50, and you’ll increase the price of Fiction books by 2.50.

a) If thePublisherID published 3 or more books, then the prices of the most expensive 3 books that thePublisherID published should be increased.
b) If thePublisherID published only 2 books, then the prices of the most expensive 2 books that thePublisherID published should be increased.
c) If thePublisherID published no books, then there should be no price increases.

*/

CREATE OR REPLACE FUNCTION increasePricesFunction(thePublisherID INTEGER, theCount INTEGER)
    RETURNS void AS $$

DECLARE
    result ALIAS FOR $1;
    bID CHAR(6) = NULL;

BEGIN

    SELECT bookID FROM (
        SELECT *
        FROM Books bs
        WHERE bs.publisherID = thePublisherID
        AND theCount <= (SELECT COUNT(*) FROM Books bss
                        WHERE bss.publisherID = thePublisherID)
        ORDER BY bs.price ASC
        limit theCount) as topRow
    limit 1
    into bID;

    UPDATE Books
        SET price = price + 1.5
        WHERE
        category = 'N' AND bID = bookID;

    UPDATE Books
        SET price = price + 2.5
        WHERE
        category = 'F' AND bID = bookID;


    /*UPDATE Books
         SET price = (CASE
            WHEN
                (category = 'N' AND bID = bookID)
                    THEN price + 1.5
            WHEN
                (category = 'F' AND bID = bookID)
                    THEN price + 2.5
        END);*/

END;
$$ LANGUAGE plpgsql;
