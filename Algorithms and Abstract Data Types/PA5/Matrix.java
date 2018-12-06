//Eric Ventor 
//Eventor - 1492157
//PA3
//11/4/2017
//Prof. Tantalo
//Matrix ADT

import java.lang.RuntimeException;

public class Matrix {
    
    private class Entry{
        
        //Fields
        int col;
        double val;
        
        //Entry Constructor
        Entry(int column, double value){
            col = column;
            val = value;
        }
        
        //equals()
        //pre: x is an object of Entry
        public boolean equals(Object x){
            if(!(x instanceof Entry)){ //checks to see if object exists 
                return false;
            }
            Entry e = (Entry) x;
            
            if(this.val == e.val && this.col == e.col){
                return true;
            }
            return false;
            
        }
        
        //toString()
        public String toString(){
            return "(" + col + ", " + val + ")";
        }
    }
    private int size;
    private int nnz;
    private List[] row;
    
// Constructor
// Makes a new n x n zero Matrix. pre: n>=1
    Matrix(int n){
        if(n < 1){
            throw new RuntimeException("Matrix() Error: n must be > 1");
        }
        size = n;
        nnz = 0;
        row = new List[n+1]; //starts at row 1
        for(int i =1; i <= size; i++){ //therefore increment starting at 1
            row[i] = new List();
        }
    }

// Access functions
// Returns n, the number of rows and columns of this Matrix
    int getSize(){
        return size;
    }
// Returns the number of non-zero entries in this Matrix
    int getNNZ(){
        return nnz;
    }
// overrides Object's equals() method
    public boolean equals(Object x){
        if(x == null || !(x instanceof Matrix)){
			return false;
		}
		Matrix check = (Matrix) x;
		if(check.getSize() != this.size || check.getNNZ() != this.nnz){
			return false;
		}
		
		for(int i = 1; i <= this.size; i++){
			if(check.row[i].length() != this.row[i].length()){
				return false;
			}
			if(check.row[i] != null && this.row[i] != null){	
				check.row[i].moveTo(0);
				this.row[i].moveTo(0);
			}else{
				continue;
			}
			for(int j = 0; j < this.size; j++){
				if(this.row[i].index() >= 0){
					Entry e = (Entry) this.row[i].get();
					Entry g = (Entry) check.row[i].get();
					if(!e.equals(g)){
						return false;
					}
					this.row[i].moveTo(j);
					check.row[i].moveTo(j);
				}
			}
		}
		return true;
    }

// Manipulation procedures

// sets this Matrix to the zero state
// pre: nnz != 0
    void makeZero(){
        for(int i =1; i <= this.size; i++){
            this.row[i].clear();
        }
        nnz = 0;
    }

// returns a new Matrix having the same entries as this Matrix
    Matrix copy(){
        Matrix Copy = new Matrix(this.size);
		Copy.nnz = this.nnz;
		
		for(int i =1; i <=size; i++){
			if(this.row[i] != null){	
				this.row[i].moveTo(0);
			}else{ //if row is null then skip that row
				continue; 
			}
			for(int j = 0; j <= size; j++){ //think i may be able to change < to avoid needing conditional
				this.row[i].moveTo(j);
				if(this.row[i].index() >= 0){ //this conditional may not be necessary
					Entry temp = (Entry) this.row[i].get();
					Copy.row[i].append(temp);
				}else{
					continue;
				}
				
			}
		}
		return Copy;
    }

// changes ith row, jth column of this Matrix to x
// pre: 1<=i<=getSize(), 1<=j<=getSize()
    void changeEntry(int i, int j, double x){
        if(1 > i || i > this.getSize()){
            throw new RuntimeException("changeEntry() ERROR: invalid row number");
        }else if(1 > j || j > this.getSize()){
            throw new RuntimeException("changeEntry() ERROR: invalid column number");
        }
        
        Entry newEnt = new Entry(j, x);
        
        if(row[i].length() == 0 && x == 0){ // do nothing
            return; 
        }else if(row[i].length() == 0 && x != 0){ // insert new entry object
            row[i].append(newEnt);
            nnz++;
            return;
        }else if(row[i].length() != 0 && x == 0){ // delete existing entry object 
            for(row[i].moveFront(); row[i].index() >= 0; row[i].moveNext()){
                Entry ent = (Entry) row[i].get();
                if(ent.col == newEnt.col){
                    row[i].delete();
                    nnz--;
                    return;
                }
            }
        }else{ // overwrite data in existing entry object or insert between two existing cols
            row[i].moveTo(0);
            Entry head = (Entry) row[i].front();
            Entry tail = (Entry) row[i].back();
            
            if(newEnt.col > tail.col){ //if newEnt col # is after tail append
                row[i].append(newEnt);
                nnz++;
                return;
            }else if(newEnt.col < head.col){ //if newEnt col # is before prepend
                row[i].prepend(newEnt);
                nnz++;
                return;
            }else if(newEnt.col < tail.col && newEnt.col > head.col){//if col # is inbetween two existing col values
				for(row[i].moveTo(0); row[i].index() >= 0; row[i].moveNext()){
					Entry temp = (Entry) row[i].get(); //gets one value
					
					if(row[i].index() < row[i].length()-1){ 
						row[i].moveNext();
						Entry temp1 = (Entry) row[i].get(); //gets next value
						
						if(newEnt.col > temp.col && newEnt.col < temp1.col){ //checks if col # is between temp1 and temp2
							row[i].insertBefore(newEnt); //if so put inbetween
							nnz++;
							return;
						}
						row[i].movePrev(); //used to hold two different temp1's and temp2's
					}
				}
			}
            /*for(row[i].moveTo(0); row[i].index() >= 0; row[i].moveNext()){ //is this necessary anymore?
                Entry ent = (Entry) row[i].get();
                if(ent.col == newEnt.col){
                    ent = newEnt;
                    return;
                }
            }*/
        }
    }

// returns a new Matrix that is the scalar product of this Matrix with x
    Matrix scalarMult(double x){
        Matrix S = new Matrix(size);
		
        if(S.size == 0){
            throw new RuntimeException("scalarMult() ERROR: matrix size is 0");
        }
		
        for(int i = 1; i <=this.size; i++){
			if(this.row[i] != null){	
				this.row[i].moveTo(0);
			}else{
				continue;
			}
            for(int j = 1; j <=this.size; j++){
                if(row[i].index() >= 0){
                    Entry e = (Entry) row[i].get();
					e = new Entry(e.col, e.val);
                    e.val = x*e.val;
                    S.row[i].append(e);
                    row[i].moveNext();
                }
            }
        }
        S.nnz = this.nnz;
        return S;
    }

// returns a new Matrix that is the sum of this Matrix with M
// pre: getSize()==M.getSize()
    Matrix add(Matrix M){
        if(M.size != this.size){
            throw new RuntimeException("add() ERROR: matrices are different sizes");
        }
        //if both matrices are the same just mult by 2
		//also was getting weird error when attempting to add same matrix with itself
        if(this.equals(M)){
			return M.scalarMult(2);
		}
		
        Matrix D = new Matrix(size);
		
        for(int i = 1; i <= M.getSize(); i++){ //increments row #
            if(M.row[i] != null || this.row[i] != null){	
				M.row[i].moveTo(0);
				this.row[i].moveTo(0);
			}else{ //if row is null then skip that row
				continue;
			}
             for(int j = 0; j <= this.size; j++){ //increments col #
                 if(M.row[i].index() >= 0 && this.row[i].index() >= 0){
					 D.nnz++;
                     Entry e = (Entry) M.row[i].get();
                     Entry g = (Entry) this.row[i].get();
                     double t = 0;
					 
                     if(e.col == g.col){ //if col #s match then add values
                        t = e.val + g.val;
                        Entry tmp = new Entry(e.col, t);
                        D.row[i].append(tmp);
						if(this.row[i].index() >= 0){	
							this.row[i].moveTo(j+1);
						}
						if(M.row[i].index() >= 0){
							M.row[i].moveTo(j+1);
						}
                    }else if(e.col > g.col){ //if one matrix col # is behind the other, just append the existing one's val
                         D.row[i].append(g);
                         D.nnz++;
						 if(this.row[i].index() >= 0){ //was getting moveNext() errors thrown without this
							this.row[i].moveNext(); //and moveNext on col# that is behind
						 }
                     }else if(e.col < g.col){ 
                         D.row[i].append(e);
                         D.nnz++;
						 if(M.row[i].index() >= 0){
							M.row[i].moveNext();
						 }
                     }
                 }else{ //if one of the cursors are null, append the one that is not null until it equals the length of that row
						if(M.row[i].index() >= 0){
							Entry e = (Entry) M.row[i].get();
							D.row[i].append(e);
							D.nnz++;
							if(M.row[i].index() == M.row[i].length()-1){
								break;
							}
							M.row[i].moveNext();
						}
						if(this.row[i].index() >= 0){
							Entry g = (Entry) this.row[i].get();
							D.row[i].append(g);
							D.nnz++;
							if(this.row[i].index() == this.row[i].length()-1){
								break;
							}
							this.row[i].moveNext();
						}
					}
             }
        }
        return D;
    }

// returns a new Matrix that is the difference of this Matrix with M
// nearly the same as add
// pre: getSize()==M.getSize()
    Matrix sub(Matrix M){ // this.matrix - m = E
		if(M.size != this.size){
            throw new RuntimeException("sub() ERROR: matrices are different sizes");
        }
		
        Matrix E = new Matrix(size);
		
		double y;
        for(int i = 1; i <= M.getSize(); i++){ 
			if(M.row[i] != null && this.row[i] != null){	
				M.row[i].moveTo(0);
				this.row[i].moveTo(0);
			}else{ 
				continue;
			}
			for(int j = 0; j <= this.size; j++){ 
                if(M.row[i].index() >= 0 && this.row[i].index() >= 0){// should this be or, or and
                    Entry e = (Entry) M.row[i].get();
                    Entry g = (Entry) this.row[i].get();
                    double t = 0;
                    if(e.col == g.col){
						t = g.val - e.val;
                        Entry tmp = new Entry(e.col, t);
						if(t > 0 || t < 0){
							E.row[i].append(tmp);
							E.nnz++;							
						}else{
							if(E.nnz > 0){
								E.nnz--; // reduced nnz by 1 since non zero value became zero
							}
						}
						if(this.row[i].index() >= 0){	
							this.row[i].moveTo(j+1);
						}
						if(M.row[i].index() >= 0){
							M.row[i].moveTo(j+1);
						}
                    }else if(e.col > g.col){ 
						 //y = e.val * -1; 
						 Entry L = new Entry(g.col, g.val);
						 E.row[i].append(L);
                         E.nnz++;
						 if(this.row[i].index() >= 0){
							this.row[i].moveNext();
						}
                     }else if(e.col < g.col){ 
						 y = e.val * -1; // multiply by -1 since value is now negative then append
                         Entry L = new Entry(e.col, y);
						 E.row[i].append(L);
                         E.nnz++;
						 if(M.row[i].index() >= 0){
							M.row[i].moveNext();
						}
                     }
                     
                 }else{ //if one of the cursors are null, append the negative value of the one that is not null until it equals the length of that row
						if(M.row[i].index() >= 0){
							Entry e = (Entry) M.row[i].get();
							e.val = e.val*-1;
							E.row[i].append(e);
							E.nnz++;
							if(M.row[i].index() == M.row[i].length()-1){
								break;
							}
							M.row[i].moveNext();
						}
						if(this.row[i].index() >= 0){
							Entry g = (Entry) this.row[i].get();
							E.row[i].append(g);
							E.nnz++;
							if(this.row[i].index() == this.row[i].length()-1){
								break;
							}
							this.row[i].moveNext();
						}
					}
             }
        }
        return E;
    }

// returns a new Matrix that is the transpose of this Matrix
    Matrix transpose(){
        Matrix Trans = new Matrix(this.size);
		
		for(int i = 1; i <= this.size; i++){ 
			if(this.row[i] != null){	
				this.row[i].moveTo(0);
			}else{ 
				continue;
			}
			for(int j = 0; j < size; j++){
				if(this.row[i].index() >= 0){	
					Entry temp = (Entry) row[i].get();
					Trans.changeEntry(temp.col, i, temp.val); //swapped row and col values
					this.row[i].moveTo(j+1);
				}
			}
		}
		Trans.nnz = this.nnz; 
		return Trans;
		
    }
 
// returns a new Matrix that is the product of this Matrix with M
// pre: getSize()==M.getSize()
	Matrix mult(Matrix M){
		if(M.getSize() != this.size){
			throw new RuntimeException("mult() ERROR: matrices not the same size");
		}
		List P = null;
		List Q = null;
		double t;
		Matrix mult = new Matrix(size);
		
		Matrix trans = M.transpose(); //using transpose as a helper function to make mult a lot easier
		  
		for(int i = 1; i <= this.size; i++){
			if(this.row[i] != null){
				P = this.row[i];
			}else{ 
				continue;
			}
			for(int j = 1; j <= size; j++){
				Q = trans.row[j];
				if(Q == null || Q.length() <= 0){ //if col is null then skip that col
					continue;
				}
				t = dotProd(P, Q);
				mult.changeEntry(i, j, t);
			}
		}
		
        return mult;
    }
 

// Other functions
// overrides Object's toString() method
    public String toString(){
        String str = "";
        Entry e;
        
        for(int i = 1;i <= this.size ; i++){
            if(row[i].length() > 0){
                str += i + ": ";
                for(row[i].moveFront(); row[i].index() > -1; row[i].moveNext()){
                    e = (Entry) row[i].get();
                    str += e.toString() + " ";
            }
                str += "\n";
            }
        }
        return str;
    }
	
// helper functions
	
	//calculates dot product of two lists
	public double dotProd(List P, List Q){	
		P.moveTo(0);
		Q.moveTo(0);
		Entry e;
		Entry g;
		double total = 0;
		
		while(P.index() >= 0 && Q.index() >= 0){
			double t = 0;
			e = (Entry) P.get();
			g = (Entry) Q.get();
			if(e.col == g.col){
				t = e.val * g.val;
				total = total + t; //total dot prod
				P.moveNext();
				Q.moveNext();
			}else if(e.col > g.col){
				Q.moveNext();
			}else if(g.col > e.col ){
				P.moveNext();
			}
		}
		return total; 
	}
}
    /*public boolean checkNull(Matrix M, i){
        entry tmp;
		tmp = M.row[i].moveNext();
		if(M.row[i].get() != null){
			M.row[i].movePrev();
            return true;
        }else if(M == null){
			if(this.row[i]
		}
        return false;
    }*/
