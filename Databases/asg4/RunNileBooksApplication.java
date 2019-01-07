/* Eric Ventor
# Eventor@ucsc.edu
# CMPS 180
# Prof Finkelstein*/

import java.sql.*;
import java.io.*;
import java.util.*;

/**
 * A class that connects to PostgreSQL and disconnects.
 * You will need to change your credentials below, to match the usename and password of your account
 * in the PostgreSQL server.
 * The name of your database in the server is the same as your username.
 * You are asked to include code that tests the methods of the NileBooksApplication class
 * in a similar manner to the sample RunStoresApplication.java program.
*/


public class RunNileBooksApplication
{
    public static void main(String[] args) {

    	Connection connection = null;
    	try {
    	    //Register the driver
    		Class.forName("org.postgresql.Driver");
    	    // Make the connection.
            // You will need to fill in your real username
            // and password for your Postgres account in the arguments of the
            // getConnection method below.
            connection = DriverManager.getConnection(
                                                     "jdbc:postgresql://cmps180-db.lt.ucsc.edu/",
                                                     "",
                                                     "");

            if (connection != null)
                System.out.println("Connected to the database!");

            /* Include your code below to test the methods of the NileBooksApplication class
             * The sample code in RunStoresApplication.java should be useful.
             * That code tests other methods for a different database schema.
             * Your code below: */

             List<Integer> result = new ArrayList<Integer>();
             int ftoCount;

             NileBooksApplication n = new NileBooksApplication(connection);

             result = n.getAuthorsWithManyReviewedBooks(1);
             System.out.println("Result of getAuthorsWithManyReviewedBooks(3): ");
             System.out.println(result);
             System.out.println();
             System.out.println("***********************************************");
             System.out.println();

             ftoCount = n.fixTotalOrdered(94519);
             System.out.println("Result of fixTotalOrdered(94159): ");
             System.out.println(ftoCount);
             System.out.println();
             System.out.println("***********************************************");
             System.out.println();


             n.increasePublishersPrices(98035, 2);
             System.out.println("Result of increasePublishersPrices(98035, 2): ");
             System.out.println("Result is nil but values have been updated appropriately.");
             System.out.println();
             System.out.println("***********************************************");
             System.out.println();

             n.increasePublishersPrices(98035, 4);
             System.out.println("Result of increasePublishersPrices(98035, 4): ");
             System.out.println("Result is nil but values have been updated appropriately.");
             System.out.println();
             System.out.println("***********************************************");
             System.out.println();
            /*******************
            * Your code ends here */

    	}
    	catch (SQLException | ClassNotFoundException e) {
    		System.out.println("Error while connecting to database: " + e);
    		e.printStackTrace();
    	}
    	finally {
    		if (connection != null) {
    			// Closing Connection
    			try {
					connection.close();
				} catch (SQLException e) {
					System.out.println("Failed to close connection: " + e);
					e.printStackTrace();
				}
    		}
    	}
    }
}
