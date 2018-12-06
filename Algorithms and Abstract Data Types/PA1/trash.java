//Eric Ventor
//PA1
//10/11/17
//Prof. Tontalo
//ADT

public class List {
// Creates a new empty list.
   
   private class Node {
      int data;
      Node next;
      Node prev;
      //Constructor - creates a new Node
	   Node(int data) {
	      this.data = data;
	      next = null;
	      prev = null;
	   }
	  //debug purposes
	  public void displayNode() {
         System.out.print(data + " ");
      }
   }
   List(){
      head = null;
   	tail = null;
   	cursor = null;
   	length = 0;
   }
   //Fields
   Node cursor;
   Node head;
   Node tail;
   int length;
   int index = -1;

// Access functions
   int length(){ // Returns the number of elements in this List.
      return length;
   }
   int index(){ // If cursor is defined, returns the index of the cursor element,
 // otherwise returns -1.
      Node N = head; 
      int num =1;
      if(cursor !=null){
         if(N.data == cursor.data){ return num;}
      }
      while(N != null){
         if(N.next.data == cursor.data){
            return num; 
         }
         N = N.next;
      }
   return -1;
   } 
   
   int front(){ // Returns front element. Pre: length()>0
      if(length<0){
      	throw new RuntimeException("Precondition length not met");
      }
      return head.data;
   }
   int back(){ // Returns back element. Pre: length()>0
      if(length<=0){
         throw new RuntimeException("Precondition length not met");
      }
      return tail.data;  
   }
   int get(){ // Returns cursor element. Pre: length()>0, index()>=0
      if(length()<0){ //are the comparison symbols correct?
         throw new RuntimeException("Precondition length not met");
      }
      if((index() < 0)){
         System.out.println(index);
         throw new RuntimeException("Precondition index not met");
      }
      return cursor.data;
   }
   boolean equals(List A){ // Returns true if and only if this List and L are the same
 // integer sequence. The states of the cursors in the two Lists
 // are not used in determining equality.
      int count = 0;
      Node N = new Node(head.data);
      Node M = new Node(A.head.data);
      N = head;
      M = A.head;
      if(length == A.length){
         while(N.data == M.data){
         	N = N.next;
         	M = M.next;
            count++;
         }
         if(count == length){ 
	        return true;  
         }
      }
      return false;  
   }

// Manipulation procedures
   void clear(){// Resets this List to its original empty state.
      head = null;
      tail = null;
      cursor = null;
      index = -1; 
   }

   void moveFront(){// If List is non-empty, places the cursor under the front element,
 // otherwise does nothing.
      if(head!=null){
         cursor = head;
         index = 0;
      }
   }

   void moveBack(){// If List is non-empty, places the cursor under the back element,
 // otherwise does nothing.
      if(tail!=null){
         cursor = tail;
         index = length-1;
      }
   }

   void movePrev(){ // If cursor is defined and not at front, moves cursor one step toward
 // front of this List, if cursor is defined and at front, cursor becomes
// undefined, if cursor is undefined does nothing.
      if( (cursor!=null) && (index!=0) ){
      	cursor = cursor.prev;
      	index--;
      }else if(cursor != null && index==0){
          cursor.next=null;
          cursor.prev=null;
          index=-1;	
       }
   }

   void moveNext(){// If cursor is defined and not at back, moves cursor one step toward
 // back of this List, if cursor is defined and at back, cursor becomes
 // undefined, if cursor is undefined does nothing.
      if(cursor != null && index != length-1){
         cursor = cursor.next;
         index++;
      }else if(cursor != null && index == length-1){
         cursor.next = null;
         cursor.prev = null;
         index = -1;
      }
   }

   void prepend(int data){ //Insert new element into this List. If List is non-empty,insertion takes place before front element.
      Node N = new Node(data);
      if((head!=null) && (tail!=null)){
         N.next = head;
         head = N.prev;
      }else{
          head = N;
          tail = N;
      }
      length++;
      index++;
   }
   //make sure it works with List copy()
   void append(int data){ //Insert new element into this List. If List is non-empty,insertion takes place after back element.
      Node N = new Node(data);
      if((head!=null) && (tail!=null)){
         N.prev = tail;
         tail.next = N;
         tail = N;
      }else if(head == null){
          head = N;
          tail = N;
       }
       length++;
       if(index == -1){
         index++;
      }
   }
   void insertBefore(int data){ //Insert new element before cursor.Pre: length()>0, index()>=0
      if( (length<=0) || (index<0) ){
         throw new RuntimeException("Precondition: length or index not met");	
      }
      Node N = new Node(data);
      if(cursor.prev == null){
         N.next = head;
         head.prev = N;
         head = N;
      }else{
          N.prev = cursor.prev;
          N.next = cursor;
          cursor.prev.next = N;
          cursor.prev = N;
      }
      length++;
      index++;
   }
   void insertAfter(int data){ // Inserts new element after cursor.
 // Pre: length()>0, index()>=0
      if( (length<=0) || (index<0) ){
         throw new RuntimeException("Precondition: length or index not met");	
      }
      Node N = new Node(data);
      if(cursor.next == null){
         tail.next = N;
         tail = N;
         tail.prev = cursor;
      }else{
          N.prev = cursor;
          N.next = cursor.next;
          cursor.next.prev = N;
          cursor.next = N;
      }
      length++;
   }

   void deleteFront(){// Deletes the front element. Pre: length()>0
      if(length<=0){
         throw new RuntimeException("ERROR: precondition length not met");
      }else if(length==1){
          clear();
	   }else{
           head = head.next;
           head.prev = null;
        }
      length--;
      if(index!=-1){
      index--;
      }
   }

   void deleteBack(){ // Deletes the back element. Pre: length()>0
      if(length<=0){
         throw new RuntimeException("ERROR: precondition length not met");
      }
      Node temp = cursor.next;
      if(length == 1){
        if( (defined() == false) && (temp == null) ){
           clear();
           System.out.println("Warning: cursor object cleared");
        }else if(length > 1){
           tail = tail.prev;
           tail.next = null;
       }
     }
       length--;
   }

   void delete(){ // Deletes cursor element, making cursor undefined.
 // Pre: length()>0, index()>=0
      if( (length<=0) || (index()<0) ){
         throw new RuntimeException("ERROR: precondition length not met");
      }
      if(index==0){
      	deleteFront();
      }else if(index==length-1){
      	deleteBack();
      }else{
          Node P = cursor.prev;
          Node N = cursor.next;
          P.next = N;
          N.prev = P;
      }
      length--;
      index=-1;
   }
// Other methods

   public String toString(){
      String temp = "";
      for(Node N=head; N!=null; N = N.next){
         temp +=N.toString() + " ";
      }
      return temp;
      
   } // Overrides Object's toString method. Returns a String
 // representation of this List consisting of a space
// separated sequence of integers, with front on left.

   List copy(){ // Returns a new List representing the same integer sequence as this
 // List. The cursor in the new list is undefined, regardless of the
 // state of the cursor in this List. This List is unchanged.
      List L = new List();
      L.cursor = null;
      Node N = head;
      while(N != null){
         L.append(N.data);
         N = N.next;
      }
      return L; 
   }
   boolean defined(){
      if(cursor == null){
         return false;
      }
      return true;
   }
   
}


