//Eric Ventor 
//Eventor - 1492157
//PA3
//11/4/2017
//Prof. Tantalo
//Takes in input, runs calculations, and prints output

import java.io.*;
import java.util.Scanner;
import java.util.ArrayList;

public class Sparse{   
	public static void main(String[] args) throws IOException{
		
		if( (args.length > 2) || (args.length < 2) ){
			System.err.println("ERROR: proper usage use two string arguments");
			System.exit(1);
        }
		int n = 0;
		int a = 0;
		int b = 0;
		String[] token = null;
		String[] line = null;
		int num_lines = 0;
		int temp_row, temp_col;
		double temp_val;
		Matrix A = null;
		Matrix B = null;
		Matrix sM = null; //scalarMult
		Matrix AaddB = null;  //A + B
		Matrix AaddA = null;  //A + A
		Matrix BsubA = null;  //B - A
		Matrix AsubA = null;  //A - A
		Matrix Atrans = null; //A transposed
		Matrix AxB = null;    //A x B
		Matrix BxB = null;    //B x B
		
		
		Scanner in = new Scanner(new File(args[0]));
		PrintWriter out = new PrintWriter(new FileWriter(args[1]));
		
		
		while(in.hasNextLine()){
			num_lines++;
			//System.out.println(num_lines);
			if(num_lines == 1){
				n = in.nextInt();
				//System.out.println(n);
				a = in.nextInt();
				//System.out.println(a);
				b = in.nextInt();
				//System.out.println(b);
				A = new Matrix(n);
				B = new Matrix(n);
			}	
			for(int i = 1; i <= a; i++){
				temp_row = in.nextInt();
				temp_col = in.nextInt();
				temp_val = in.nextDouble();
				A.changeEntry(temp_row, temp_col, temp_val);
			}
			for(int i = 1; i <= b; i++){
				temp_row = in.nextInt();
				temp_col = in.nextInt();
				temp_val = in.nextDouble();
				B.changeEntry(temp_row, temp_col, temp_val);
			}
		}
		
		//Output
		out.printf("A has %d non-zero entries:", A.getNNZ());
		out.println();
		out.println(A);
		
		
		out.printf("B has %d non-zero entries:", B.getNNZ());
		out.println();
		out.println(B);
		
		//Math Matrix Functions
		out.println("(1.5)*A ="); 	//1.5 * A
		sM = A.scalarMult(1.5);
		out.println(sM);
		out.println();
		
		out.println("A+B =");
		AaddB = A.add(B);     		//A + B
		out.println(AaddB);
		out.println();
		
		out.println("A+A =");
		AaddA = A.add(A);     		//A + A
		out.println(AaddA);
		out.println();
		
		out.println("B-A =");
		BsubA = B.sub(A);         	//B - A 
		out.println(BsubA);
		out.println();
		
		out.println("A-A =");
		AsubA = A.sub(A);         	//A - A
		out.println(AsubA);
		out.println();
		
		out.println("Tranpose(A) =");
		Atrans = A.transpose();     //A transposed
		out.println(Atrans);
		out.println();
		
		out.println("A*B =");
		AxB = A.mult(B);           //A x B
		out.println(AxB);
		out.println();
		
		out.println("B*B =");
		BxB = B.mult(B);           //B x B
		out.println(BxB);
		out.println();
		
		in.close();
		out.close();
		
	}
}