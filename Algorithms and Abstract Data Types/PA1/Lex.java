//Eric Ventor
//PA1
//10/11/17
//Prof. Tontalo

import java.io.*;
import java.util.Scanner;
import java.util.ArrayList;

public class Lex{
   public static void printWriter(List L, PrintWriter out,String[] token){
     L.moveFront();
     while(L.cursor!=null){
        out.println(token[L.get()]);
        L.moveNext();
     }   
   }
   
   public static void main(String[] args) throws IOException{
      Scanner in, counter = null;
      PrintWriter out = null;
      String line = "";
      String hold = "";
      String[] token;
      int i, n, lineNumber = 0;
      if((args.length >2) || (args.length <2) ){
         System.err.println("Error: proper usage use two string arguments");
         System.exit(1);
      }
      in = new Scanner(new File(args[0]));
      counter = new Scanner(new File (args[0]));
      out = new PrintWriter(new FileWriter(args[1]));

      lineNumber = lineCounter(counter);
      System.out.println("lines: ");
      System.out.println(lineNumber);
      counter.close();
      token = new String[lineNumber];

      for(int t = 0; t < lineNumber; t++){
         hold = in.nextLine();
         token[t] = hold;
      }
      System.out.println("token" + token[0] + token[1] + token[4]);
      List L = new List();
      L.prepend(0);
      for(i = 1; i<lineNumber; i++){
         String comp = token[i];
         System.out.println("comp: " + comp);
         for(L.moveFront();L.index()!=-1;L.moveNext() ){
            System.out.println("index: " + L.index());
            System.out.println("get test " + token[L.get()]);
            if(comp.compareTo(token[L.get()])<0){
               L.insertBefore(i);
            }else if(comp.compareTo(token[L.get()])==0){
                L.insertBefore(i);
                
            }
        }       
        } 
      printWriter(L, out, token);
      in.close();
      out.close();
   }
}
