from keras.models import load_model ,Sequential
from keras import backend as K
from keras.layers import Dense, Activation
import pandas as pd
import numpy as np
import sys
import time

# MAE function
def mean_absolute_error(y_true,y_pred) :
    return np.sum(np.absolute((y_pred - y_true)) / y_true)

# https://stackoverflow.com/questions/47648133/mape-calculation-in-python
def mean_absolute_percentage_error(y_true, y_pred):
    y_true, y_pred = np.array(y_true), np.array(y_pred)
    return np.mean(np.abs((y_true - y_pred) / y_true)) * 100

def mean_square_root(y_true,y_pred) :
    return (np.square(y_pred - y_true)).mean(axis=None)

def predict(input,model):
    y_prob = model.predict(input)
    return y_prob


model = load_model('../data/WindDenseNN.h5',compile=False)
model.summary()


idf = pd.read_csv(sys.argv[2],header=None)
actualdf = pd.read_csv('../data/actual.csv',header=None)


begin = time.time()
r = idf.apply(lambda x : predict(x[1:].values.reshape(1,-1),model),axis=1)
end = time.time()
print("Time=",end-begin)
#r = data.apply(lambda x : (x[0],predict(list((x[1:].values)),model)),axis=1)
print(r)
