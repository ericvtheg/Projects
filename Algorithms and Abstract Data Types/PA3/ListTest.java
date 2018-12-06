//Eric Ventor 
//Eventor - 1492157
//PA3
//11/1/2017
//Prof. Tantalo
//Matrix.java Tester

public class ListTest{
	public static void main(String[] args){
		List P = new List();
		List Q = new List();
		
		System.out.println("P.length() = "+P.length());
		System.out.println("Q.length() = "+Q.length());
		
		for(int i=1; i<=20; i++){
			P.prepend(i);
			Q.append(i);
		}
		
		System.out.println(P);
		System.out.println(Q);
		
		P.moveFront();
		System.out.println("P.get() = "+P.get());
		System.out.println("P.index() = "+P.index());
		Q.moveBack();
		System.out.println("Q.get() = "+Q.get());
		System.out.println("Q.index() = "+Q.index());
		
		List C = P.copy();
		System.out.println(C);
		System.out.println(P.equals(C));
		System.out.println(Q.equals(C));
		System.out.println(P.equals(P));
		
		for(P.moveBack(); P.index()>=0; P.movePrev()){
			System.out.print(P.get()+" ");
		}
		
		System.out.println();
		
		for(Q.moveBack(); Q.index()>=0; Q.movePrev()){
			System.out.print(Q.get()+" ");
		}
		
	    P.moveFront();
		for(int i=0; i<5; i++){
			P.moveNext(); 
			P.insertBefore(-1); 
		}
		                 
		for(int i=0; i<9; i++){ 
			P.moveNext(); // at index 15
			P.insertAfter(-2);
		}
		for(int i=0; i<5; i++){ 
			P.movePrev(); // at index 10
			P.delete();
		}
		P.front();
		Q.back();
		P.deleteFront();
		Q.deleteBack();
		
		System.out.println(P);
		P.clear();
		System.out.println(P.length());
		
    }
}