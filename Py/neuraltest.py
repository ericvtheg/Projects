import numpy as np
import time

# 1.Define Problem.
# 2.Prepare Data.
# 3.Evaluate Algorithms.
# 4.Improve Results.
# 5.Present Results.

# sigmoid function "We use it to convert numbers to probabilities"
# sigmoid function maps any value to a value between 0 and 1
def nonlin(x,deriv=False):
    if(deriv==True):
        return x*(1-x) # slope of sigmoid function at a give point
    return 1/(1+np.exp(-x)) #calculates 1/1+e^(-x)
                            #what does this give?

# input dataset
X = np.array([  [0,0,1], #Each row is a single "training example"
                [0,1,1], #Each column corresponds to one of our input nodes
                [1,0,1],
                [1,1,1] ])

# output dataset ".T for transpose"
y = np.array([[0,0,1,1]]).T

# seed random numbers to make calculation
# deterministic (just a good practice)
# "randomly distributed in exactly the same way each time you train"
# think of a minecraft seed
print("random seed:")
np.random.seed(1)
print(np.random.seed(1))

# initialize weights randomly with mean 0 "good practice"
# creates 3x1 matrix multiplies by 2 and subs 1
# "synapse zero"
# "All of the learning is stored in the syn0 matrix."
syn0 = 2*np.random.random((3,1)) - 1
print("syn0: \n" + str(syn0))

# **important**
# Another way of looking at it is that l0 is of size 3
# and l1 is of size 1. Thus, we want to connect every node
# in l0 to every node in l1, which requires a matrix of
# dimensionality (3,1). :)


for iter in xrange(10000):

    # forward propagation
    print("forward propagation")
    l0 = X
    print("input data set: \n" + str(l0))
    l1 = nonlin(np.dot(l0,syn0)) # prediction step
    # sigmoid("(4 x 3) dot (3 x 1) = (4 x 1)")


    print("result of nonlin(np.dot(l0,syn0)): \n" + str(l1))

    # how much did we miss?
    l1_error = y - l1
    print("l1_error: \n" + str(l1_error))


    # multiply how much we missed by the
    # slope of the sigmoid at the values in l1
    # "When we multiply the "slopes" by the error,
    # we are reducing the error of high confidence predictions"
    l1_delta = l1_error * nonlin(l1,True)
    print("l1_delta: \n" + str(l1_delta))

    # update weights
    syn0 += np.dot(l0.T,l1_delta)
    print("syn 0: \n" + str(syn0))
    time.sleep(45)
    print("***NEW ITERATION****")

print "Output After Training:"
print l1
