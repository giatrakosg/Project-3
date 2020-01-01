from keras.models import load_model ,Sequential
from keras import backend as K
from keras.layers import Dense, Activation
import pandas as pd
import numpy as np
import sys
import time


model = load_model('../data/WindDenseNN.h5',compile=False)
model.summary()


# Create new model with only the first layer
# https://stackoverflow.com/questions/43871162/how-to-get-output-of-hidden-layer-given-an-input-weights-and-biases-of-the-hidd
#create new model (optimizer , loss  ? correct ?)
new_model= Sequential([
    Dense(64, input_dim=128), # first number is output_dim
    Activation('relu')])

#set weights of the first layer
new_model.set_weights(model.layers[0].get_weights())

#compile it after setting the weights
new_model.compile(optimizer='adam', loss='categorical_crossentropy')


# First from nn_represetations.csv
example = np.array([[0.16640529,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.009707384,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.039109826,0.0,0.0,0.0,0.01669026,0.0,0.062246006,0.0,0.0,0.011104327,0.0,0.12976784,0.0,0.0,0.0,0.0,0.0,0.00021743169,0.0,0.022795128,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.13960335,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.091833055,0.0,0.0,0.0,0.0,0.0,0.005580768,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.026229408,0.0,0.0,0.07827236,0.0,0.0,0.14493635,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.15339124,0.0,0.0,0.0,0.0,0.0,0.0]])


#get output of the first dens layer
out_2 = new_model.predict(example)
print(out_2,out_2.size)
