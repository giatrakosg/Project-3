from keras.models import load_model ,Sequential
from keras import backend as K
from keras.layers import Dense, Activation
import pandas as pd
import numpy as np
import sys
import time
import itertools as IT


# https://stackoverflow.com/questions/47648133/mape-calculation-in-python
def mean_absolute_error(y_true,y_pred) :
    return np.sum(np.absolute((y_pred - y_true)) / y_true)

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
actualdf = pd.read_csv('../data/actual.csv',header=None)

mean_actual = actualdf.mean(axis=0)
print(mean_actual)


begin = time.time()
predicted = idf.apply(lambda x : predict(x[1:].values.reshape(1,-1),model),axis=1)
end = time.time()

print("Time=",end-begin)
print(predicted)
print(actualdf)
#r = data.apply(lambda x : (x[0],predict(list((x[1:].values)),model)),axis=1)
#print(predicted)


#mapedf = combined.apply(lambda x : mean_absolute_percentage_error(x[1],x[3:].values.reshape(1,-1)),axis=1)
#mape_av = mapedf.mean(axis=0)
mapedf = predicted.apply(lambda x:mean_absolute_percentage_error(x[0],mean_actual))
print(mapedf)
mape = mapedf.mean()
print(mape)

combined = pd.concat([predicted,actualdf] , axis=1).reindex(predicted.index)
print(combined)

maedf = combined.apply(lambda x : mean_absolute_error(x[0],x[3:].values.reshape(1,-1)),axis=1)
#mae_av = maedf.mean(axis=0)
#print(maedf,mae_av)
print(maedf)

#msedf = combined.apply(lambda x : #mean_square_root(x[1],x[3:].values.reshape(1,-1)),axis=1)
#mse_av = msedf.mean(axis=0)

#print(msedf)


#print("MAE=",mae_av,"MAPE=",mape_av,"MSE=",mse_av)
#print(combined)
