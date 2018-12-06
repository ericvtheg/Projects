/* Eric Ventor
# Eventor@ucsc.edu
# CMPS 180
# Prof Finkelstein*/

import java.sql.*;
import java.util.*;

/**
 * The class implements methods of the NileBooks database interface.
 *
 * All methods of the class receive a Connection object through which all
 * communication to the database should be performed. Note: the
 * Connection object should not be closed by any method.
 *
 * Also, no method should throw any exceptions. In particular, in case
 * an error occurs in the database, then the method should print an
 * error message and call System.exit(-1);
 */

public class NileBooksApplication {

    private Connection connection;

    /*
     * Constructor
     */
    public NileBooksApplication(Connection connection) {
        this.connection = connection;
    }

    public Connection getConnection()
    {
        return connection;
    }

    /**
     * Takes as argument an integer called numberReviewedBooks.
     * Returns the authorID for each author in Authors that has at least numberReviewedBooks
     * different books that have at least one review.
     * A negative value for numberReviewedBooks is an error.
     */

    public List<Integer> getAuthorsWithManyReviewedBooks(int numberReviewedBooks) throws SQLException
    {
        List<Integer> result = new ArrayList<Integer>();
        List<Integer> fResult = new ArrayList<Integer>();
        // your code here
            Statement st;
            ResultSet rs;
            ResultSet fs;
        try{
            st = connection.createStatement();
            rs = st.executeQuery("SELECT DISTINCT a.authorID, b.bookID, r.bookID FROM Authors a, Books b, Reviews r WHERE "
            + "a.numBooksWritten >= "+numberReviewedBooks+" AND a.authorID = b.authorID AND r.bookID = "
            + "b.bookID GROUP BY a.authorID, b.bookID, r.bookID HAVING COUNT(DISTINCT r.bookID) >= 1 ");

            //adding our query results to our ArrayList
            while(rs.next()){
                result.add(rs.getInt("authorID"));
            }

            //error set: used for querying books where numReviewedBooks < 0
            ResultSet es = st.executeQuery("SELECT a.authorID FROM Authors a, Books b, Reviews r WHERE "
            + "a.numBooksWritten < 0 AND a.authorID = b.authorID AND r.bookID = b.bookID GROUP BY "
            + "a.authorID HAVING COUNT(DISTINCT r.bookID) >= 1");

            while(es.next()){
                int authorID = es.getInt("authorID");
                result.add(authorID);
                System.out.println("ERROR: less than 0 numReviewedBooks for "+ authorID + ".");
                result.remove(es.getInt("authorID"));
            }

            //collects and deletes duplicate in result value
            for( Integer n : result ) {
                if(Collections.frequency(result, n) >= numberReviewedBooks) {
                    if(!fResult.contains(n)){
                        fResult.add(n);
                    }

                }
            }

        } finally {
            //System.out.println(result);
        }
            return fResult;
    }


    /**
     * The fixTotalOrdered method has one integer argument, aPublisherIDtoFix, which is a publisherID.
     * fixTotalOrdered should change the totalOrdered values for each “bad book” that was published
     * by aPublisherIDtoFix, updating totalOrdered so that it is correct, i.e., so that it’s equal
     * to the sum of the quantity values for the Orders of that book.
     * fixTotalOrdered should return the number of bad books whose totalOrdered values were “fixed”.
     *
     * This method is relatively easy to write if you use the BadBookTotals view from Lab3, which has
     * been provided to you in the lab4_create.sql script for this assignment.
     */

    public int fixTotalOrdered(int aPublisherIDtoFix) throws SQLException
    {
        // your code here; return 0 appears for now to allow this skeleton to compile.
        List<Integer> result = new ArrayList<Integer>();

        int count = 0;
        String sql;
        Statement st;
        ResultSet rs;
        PreparedStatement updateTotals;
        try {
            st = connection.createStatement();

            rs = st.executeQuery("SELECT * FROM BadBookTotals bb, Books b WHERE "
            +aPublisherIDtoFix+ " = b.publisherID AND b.bookID = bb.bookID");

            sql = "UPDATE Books SET totalOrdered = ? WHERE publisherID = ? AND bookID = ?";

            updateTotals = connection.prepareStatement(sql);

            while(rs.next()){

                int publisherID = rs.getInt("publisherID");
                String bookID = rs.getString("bookID");
                int rOrder = rs.getInt("badQuantitySum");
                int wOrder = rs.getInt("totalOrdered");

                if(publisherID == aPublisherIDtoFix){
                    updateTotals.setInt(1, rOrder);
                    updateTotals.setInt(2, publisherID);
                    updateTotals.setString(3, bookID);
                    updateTotals.executeUpdate();
                    count = count + 1;
                }
            }

            //ns = st.executeQuery("SELECT * FROM BadBookTotals bb, Books b WHERE " +aPublisherIDtoFix+ " = b.publisherID AND b.bookID = bb.bookID");

            //System.out.println("count: " + count);
         } finally {
         }

        return count;

        // end of your code
    }


    /**
     * increasePublishersPrices has two integer parameters, thePublisherID and theCount, and invokes
     * a stored function increasePricesFunction that you will need to implement and store in the database
     * according to the description in Lab4 assignment.
     *
     * The increasePublishersPrices method must only invoke the stored function increasePricesFunction,
     * which does all of the assignment work; do not implement the increasePublishersPrices method using
     * a bunch of SQL statements through JDBC.  However, increasePublishersPrices should check to see
     * whether theCount is greater or equal to 0, and report an error if it’s not.
     *
     * increasePublishersPrices should return the same integer result as the increasePricesFunction
     * stored function.
     */

    public int increasePublishersPrices(int thePublisherID, int theCount) throws SQLException
    {
        // There's nothing special about the name storedFunctionResult
        int storedFunctionResult = 0;
        int result;

        // your code here
        if(theCount > 0){
            for (int i = 1; i <= theCount; i++){
                CallableStatement stmt = connection.prepareCall("{call increasePricesFunction(?,?)}");
                stmt.setInt(1, thePublisherID);
                stmt.setInt(2, i);
                stmt.execute();
                //System.out.println("Hit in increasePublisherPrices: ");
            }
        } else {
            System.out.println("ERROR: theCount < 0 in increasePublisherPrices()");
        }

        // end of your code
        return storedFunctionResult;

    }

};
