/* Eric Ventor
# Eventor@ucsc.edu
# CMPS 180
# Prof Finkelstein
# Combine 2.2 */

/* Hint from piazza:
    SQL UPDATE statement can have a FROM clause, the statement's WHERE clause
    can refer to both the updated table and the table(s) in the FROM clause, and
    the statement's SET clause can also refer to those tables.

    UPDATE Tickets
    SET SeatNum = NewTickets.SeatNum, Paid = FALSE
    FROM NewTickets
    WHERE Tickets.TicketID = NewTickets.TicketID
        AND Tickets.CustID = NewTickets.CustID
        AND Tickets.AirlineID = NewTickets.AirlineID
        AND Tickets.FlightNum = NewTickets.FlightNum;
    */

/*For each “new member” tuple in NewMemberships, there might already be a tuple
in Members that has the same primary key, memberID. If there isn’t a tuple with
that memberID, then this is a new member of Niles Books. If there already is a
tuple with that memberID, then this is a renewal of member’s membership. So here
are the actions that you should take.*/

BEGIN;

UPDATE Members
SET MemberName = NewMemberships.memberName, renewalDate = CURRENT_DATE
FROM NewMemberships
WHERE Members.memberID = NewMemberships.memberID;

INSERT INTO Members
SELECT NewMemberships.memberID,
    NewMemberships.memberName,
    CURRENT_DATE,
    NewMemberships.renewalDate,
    TRUE
FROM NewMemberships
WHERE NewMemberships.memberID NOT IN (SELECT memberID
                    FROM Members);

COMMIT;
