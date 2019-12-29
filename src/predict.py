from keras.models import load_model ,Sequential
from keras import backend as K
from keras.layers import Dense, Activation
import pandas as pd
import numpy as np
import sys

# https://stackoverflow.com/questions/47648133/mape-calculation-in-python
def mean_absolute_percentage_error(y_true, y_pred):
    y_true, y_pred = np.array(y_true), np.array(y_pred)
    return np.mean(np.abs((y_true - y_pred) / y_true)) * 100


# Why compile false ?
# Ans: https://stackoverflow.com/questions/53295570/userwarning-no-training-configuration-found-in-save-file-the-model-was-not-c
model = load_model('../data/WindDenseNN.h5',compile=False)

model.summary()
# https://stackoverflow.com/questions/48265926/keras-find-out-the-number-of-layers
layers = model.layers
print("Our model has {} layers".format(len(layers)))

# https://machinelearningmastery.com/how-to-make-classification-and-regression-predictions-for-deep-learning-models-in-keras/
# First from nn_represetations.csv
example = np.array([[0.16640529,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.009707384,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.039109826,0.0,0.0,0.0,0.01669026,0.0,0.062246006,0.0,0.0,0.011104327,0.0,0.12976784,0.0,0.0,0.0,0.0,0.0,0.00021743169,0.0,0.022795128,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.13960335,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.091833055,0.0,0.0,0.0,0.0,0.0,0.005580768,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.026229408,0.0,0.0,0.07827236,0.0,0.0,0.14493635,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.15339124,0.0,0.0,0.0,0.0,0.0,0.0]])

y_prob = model.predict(example)
print(y_prob)

true = [0.1250811072805022,0.1354710268703837,0.08830001267604261,0.11507460316874236,0.1199261992619926,0.1595343820476544,0.1254658523289835]

#https://stackoverflow.com/questions/33359411/mean-absolute-error-python/33359549
mae = np.sum(np.absolute((y_prob - true)) / true)
print("MAE=",mae)

mape = mean_absolute_percentage_error(true,y_prob)
print("MAPE",mape)

# https://stackoverflow.com/questions/16774849/mean-squared-error-in-numpy
# Correct axis ?
mse = (np.square(y_prob - true)).mean(axis=None)
print("MSE",mse)
