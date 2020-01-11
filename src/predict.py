from keras.models import load_model ,Sequential
from keras import backend as K
from keras.layers import Dense, Activation
import pandas as pd
import numpy as np
import sys
import time
import itertools as IT

# Gets two numpy array and calculates the manhattan_distance
def manhattan_distance(xvec,yvec):
    sum = 0
    for x,y in zip(xvec,yvec):
        sum += np.abs(x-y)
    return sum

# https://stackoverflow.com/questions/47648133/mape-calculation-in-python
def mean_absolute_error(y_true,y_pred) :
    y_true = y_true.values
    return manhattan_distance(y_true,y_pred)

def mean_absolute_percentage_error(y_true, y_pred):
    y_pred = y_pred.values
    #import pdb; pdb.set_trace()
    sum = 0
    for x,y in zip(y_pred , y_true) :
        sum += (np.abs(x-y)) / y
    return sum / y_true.size

def mean_square_root(y_true,y_pred) :
    return (np.square(y_pred - y_true)).mean(axis=None)

def predict(input,model):
    y_prob = model.predict(input)
    return y_prob


model = load_model('../data/WindDenseNN.h5',compile=False)
#odel.summary()


idf = pd.read_csv(sys.argv[2],header=None)
actualdf = pd.read_csv(sys.argv[3],header=None)

mean_actual = actualdf.mean(axis=0)
print(mean_actual)


begin = time.time()
predicted = idf.apply(lambda x : predict(x[1:].values.reshape(1,-1),model),axis=1)
end = time.time()

print("Time=",end-begin)
print("====Predicted====")
print(predicted)
print("====Actual====")
print(actualdf)
print("====Combined====")
actualdf['predicted'] = predicted
print(actualdf)

error_series = actualdf.apply(lambda x : mean_absolute_error(x[1:8],x['predicted']),axis=1)

mae = error_series.mean().mean()
actualdf['abs_error'] = error_series

print("====Error====")
print(actualdf)
print("====MAE====")
print(mae)
