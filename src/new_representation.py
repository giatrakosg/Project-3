from keras.models import load_model ,Sequential
from keras import backend as K
from keras.layers import Dense, Activation
import pandas as pd
import numpy as np
import sys
import time


model = load_model('../data/WindDenseNN.h5',compile=False)



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


# Input csv file given by user
idf = pd.read_csv(sys.argv[2],header=None)

predictions = idf.apply(lambda x : new_model.predict(x[1:].values.reshape(1,-1)).tolist(),axis=1)
print(idf)

predictions = pd.DataFrame(item for item in predictions)
predictions = pd.DataFrame(predictions[0].values.tolist())
predictions = pd.DataFrame(predictions.values)


timestamps = idf.loc[:,0]
timestamps = timestamps.to_frame('ts')

combined = timestamps.join(predictions,how='left')
#print(timestamps)
#print(predictions2)
#print(predictions)
print(combined)

combined.to_csv(header=False,index=False,sep="\t",path_or_buf="../data/new_representation.csv")
