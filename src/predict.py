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
    return np.sum(np.abs(y_true - y_pred)) / y_true.size

def mape_error(y_true, y_pred):
    y_true = y_true.values
    #import pdb; pdb.set_trace()
    return np.sum(np.abs(y_true - y_pred) / y_pred) / y_true.size

    sum = 0
    for x,y in zip(y_pred , y_true) :
        sum += np(np.abs(x-y)) / y
    return sum / y_true.size

def mean_square_root(y_true,y_pred) :
    return (np.square(y_pred - y_true)).mean(axis=None)

def predict(input,model):
    y_prob = model.predict(input)
    return y_prob.flatten()


model = load_model('../data/WindDenseNN.h5',compile=False)
#odel.summary()


idf = pd.read_csv(sys.argv[2],header=None)
actualdf = pd.read_csv('../data/actual.csv',header=None)

mean_actual = actualdf.mean(axis=0)
#print(mean_actual)


begin = time.time()
predicted = idf.apply(lambda x : predict(x[1:].values.reshape(1,-1),model),axis=1)
end = time.time()

combined = actualdf.copy()
pd.concat([combined,pd.DataFrame(predicted)],axis=1)
#print("====Combined====")
#print(combined)

print("Time=",end-begin)
#print("====Predicted====")
#print(predicted)
#print("====Actual====")
#print(actualdf)
#print("====Combined====")
actualdf['predicted'] = predicted
#print(actualdf)

error_series = actualdf.apply(lambda x : mean_absolute_error(x[1:8],x['predicted']),axis=1)

mae = error_series.mean()
actualdf['abs_error'] = error_series
#print("====Absolute Error====")
#print(actualdf)
print("====MAE====")
print(mae)

square_error_series = error_series.apply(lambda x : x*x )
#print(square_error_series)
actualdf['abs_sq_error'] = square_error_series
#print("====Square Error====")
#print(actualdf)
mse = square_error_series.mean()
print("====MSE====")
print(mse)


# Calculate average actual value to use in the MAPE
mean_actual = actualdf[1:8].mean(axis=1)
#print(mean_actual)

mape_series = actualdf.apply(lambda x : mape_error(mean_actual,x['predicted']),axis=1)
mape = mape_series.mean() * 100
#print(mape_series)
print("====MAPE====")
print(mape)

with open('../data/predicted.csv',"w")  as out:
    out.write("MAE:{} MAPE:{} MSE:{}\n".format(mae,mape,mse))
    for row in actualdf.iterrows():
        ts = str(row[1][0])
        pr = str(row[1]['predicted'].tolist())
        pr = pr[1:-1]
        pr = pr.replace(',','')
        out.write(ts)
        out.write('\t')
        out.write(pr)
        out.write('\n')


#actualdf.to_csv(header=False,index=False,sep="\t",path_or_buf="predicted.csv",columns=[0,'predicted'])
