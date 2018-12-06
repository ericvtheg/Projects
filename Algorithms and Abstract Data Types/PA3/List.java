//Eric Ventor 
//Eventor - 1492157
//PA3
//11/4/2017
//Prof. Tantalo
//Doubly Linked List of Objects

import java.lang.Exception;

public class List {
// Creates a new empty list.
   
   private class Node {
       
        Object data;
        Node next;
        Node prev;
	  
      //Constructor - creates a new Node
	   Node(Object o) {
            data = o;
            next = null;
            prev = null;
	   }
       public String toString(){
            return String.valueOf(data);
		}
   }
   
   //Fields
   private Node cursor;
   private Node head;
   private Node tail;
   private int length;
   private int index;

// Constructor

// Creates a new empty list.
    List(){
        head = null;
        tail = null;
        cursor = null;
        length = 0;
        index = -1;
    }
    
// Access functions

// Returns the number of elements in this List.
    public int length()throws RuntimeException{
        return length;
    }
    
// If cursor is defined, returns the index of the cursor element,
 // otherwise returns -1
    public int index(){
        if(index <= -1){
            index = -1;
            return index;
        }
        return index;
    }
    
// Returns front element. Pre: length()>0 
    public Object front(){
        if(head == null){
            throw new RuntimeException("front() ERROR: head = null");
        }else if(length <= 0){
            throw new RuntimeException("front() ERROR: length <= 0");
        }
        return head.data;
    }
    
// Returns back element. Pre: length()>0
    public Object back(){
        if(tail == null){
            throw new RuntimeException("back() ERROR: tail = null");
        }else if(length <= 0){
            throw new RuntimeException("back() ERROR: length <= 0");
        }
        return tail.data;
    }
// Returns cursor element. Pre: length()>0, index()>=0
    public Object get(){
        if(length <= 0){
            throw new RuntimeException("get() ERROR: length <= 0");
        }else if(index < 0){
            throw new RuntimeException("get() ERROR: index is undefined");
        }
        return cursor.data;
    }
// Returns true if and only if this List and L are the same
// integer sequence. The states of the cursors in the two Lists
// are not used in determining equality.

//NEEDS TO BE COMPLETELY CHANGED TO WORK WITH 'OBJECT X'
    public boolean equals(Object x){ //Why is object parsed in?
        List L;
		if(x instanceof List && x!= null){
			L = (List) x;
		}else{
			System.out.println("equals() ERROR: x is not an instance of List or equals null");
			return false;
		}
		if(this.length == 0 && L.length() == 0){
            System.out.println("Both lists are null but equal");
            return true;
        }else if(this.length != L.length){
			return false;
		}
		moveFront();
		L.moveFront();
		for(int i = 0; i < length; i++){ //checking col entries not rows so start at 1
			if(cursor.data != L.get() || cursor == null){
				return false;
			}
			moveTo(i);
			L.moveTo(i);
		}
		return true;
    }

//moves cursor to desired index   
    public void moveTo(int m){
        if(m < 0 || m > length-1){
            //System.out.println("ERROR: moveTo() index out of bounds");
            cursor = null;
            index = -1;
        }else{
            cursor = head;
            index = 0;
            while(index < m){
                moveNext();
            }
        }
    }
// Manipulation procedures

// Resets this List to its original empty state.
    public void clear(){
        head = null;
        tail = null;
        cursor = null;
        index = -1;
        length = 0;
    }
    
// If List is non-empty, places the cursor under the front element,
// otherwise does nothing.
    public void moveFront(){
        if(head!=null){
            cursor = head;
            index = 0;
        }else{
            throw new RuntimeException("moveFront() ERROR: head equals NULL");
	  }
    }
    
// If List is non-empty, places the cursor under the back element,
// otherwise does nothing.
    public void moveBack(){
        if(tail!=null){
            cursor = tail;
            index = length-1;
        }else{
            throw new RuntimeException("moveBack() ERROR: tail equals NULL");
        }
    }
    
// If cursor is defined and not at front, moves cursor one step toward
// front of this List, if cursor is defined and at front, cursor becomes
// undefined, if cursor is undefined does nothing.
    public void movePrev(){
        if(index == -1){
            throw new RuntimeException("movePrev() ERROR:: preconditions not met");
        }
        if(cursor == null){
            index = -1;
        }else if(index == 0){
            cursor = null;	
            index=-1;	
       }else{
           cursor = cursor.prev;
           index--;
       }
    }
    
// If cursor is defined and not at back, moves cursor one step toward
// back of this List, if cursor is defined and at back, cursor becomes
// undefined, if cursor is undefined does nothing.
    public void moveNext(){
        if(index == -1){
            cursor = null;
            throw new RuntimeException("moveNext() ERROR: precondition index not met");
        }else if(cursor != null && cursor.next == null){
            cursor = null;
            index = -1;
        }else{
            cursor = cursor.next;
            index++;
      }
    }
    
// Insert new element into this List. If List is non-empty,
// insertion takes place before front element.
    public void prepend(Object data){
        Node N = new Node(data);
        if(length == 0){
            head = N;
            tail = N;
            length++;
            index = -1; //shouldnt be necessary
            return;
        } 
        if(index != -1){
            index++;
        }
        head.prev = N;
        N.next = head;
        head = N;
        length++;   
    }
// Insert new element into this List. If List is non-empty,
// insertion takes place after back element.
    public void append(Object data){
        Node N = new Node(data);
        if(length == 0){
            head = N;
            tail = N;
            length++;
        }else{
            tail.next = N;
            N.prev = tail;
            tail = N;
            length++;
        }
    }
// Insert new element before cursor.
// Pre: length()>0, index()>=0
    public void insertBefore(Object data) {
        if(index <= -1){
            throw new RuntimeException("insertBefore() ERROR: index is undefined");
        }else if(length == 0){
            append(data);
        }else{
            Node N = new Node(data);
            N.data = data;
            if(cursor.prev == null){
                prepend(data);
                return; 
            }
            cursor.prev.next = N;
            N.next = cursor;
            N.prev = cursor.prev;
            cursor.prev = N;
            length++;
            index++;
        }
    }
// Inserts new element after cursor.
// Pre: length()>0, index()>=0
    public void insertAfter(Object data) {
        if(index <= -1){
            throw new RuntimeException("insertAfter() ERROR: index is undefined");
        }else if(length == 0){
            Node N = new Node(data);
            N.data = data;
            append(data);
        }else if(cursor.next == null){
            Node N = new Node(data);
            N.data = data;
            append(data);
        }else{
            Node N = new Node(data);
            N.data = data;
            cursor.next.prev = N;
            N.prev = cursor;
            N.next = cursor.next;
            cursor.next = N;
            length++;
            
        }
    }
    
// Deletes the front element. Pre: length()>0
    public void deleteFront() {
        if(length == 0){
            return;
        }else if(head == null){
            throw new RuntimeException("deleteFront() ERROR: head is null");
        }else if(cursor == head){
            index = -1;
            cursor = null;
        }
        Node N = head;
        head.next.prev = null;
        head.next = head;
        length--;
        if(index <= -1){
            index--;
        }
    }
    
// Deletes the back element. Pre: length()>0
    public void deleteBack() {
        if(length == 0){
            return;
        }else if(tail == null){
            throw new RuntimeException("deleteBack() ERROR: tail equals null");
        }else if(cursor == tail){
            index = -1;
            cursor = null;
        }
        Node N = tail;
        if(length >= 1){
            tail = tail.prev;
        }
        length--;
    }
    
// Deletes cursor element, making cursor undefined.
// Pre: length()>0, index()>=0
// Other methods
    public void delete(){
        if(index == -1 || cursor == null){
            throw new RuntimeException("delete() ERROR: index equals null");
        }else if(cursor == head){
            deleteFront();
            return;
        }else if(cursor == tail){
            deleteBack();
            return;
        }
        Node N = cursor;
        cursor.next.prev = cursor.prev;
        cursor.prev.next = cursor.next;
        cursor.prev = null;
        cursor.next = null;
        length--;
        index=-1;
    }
    
// Overrides Object's toString method. Returns a String
// representation of this List consisting of a space
// separated sequence of integers, with front on left.  
 
    public String toString(){
        String str = "";
        for(Node N = head; N!=null; N=N.next){
            str += N.toString() + " ";
        }
        return str;
    }
    
}