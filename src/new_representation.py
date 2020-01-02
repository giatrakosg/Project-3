from keras.models import load_model ,Sequential
from keras import backend as K
from keras.layers import Dense, Activation
import pandas as pd
import numpy as np
import sys
import time


model = load_model('../data/WindDenseNN.h5',compile=False)
#model.summary()


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


idf = pd.read_csv(sys.argv[2],header=None)
#print(idf)


predictions = idf.apply(lambda x : new_model.predict(x[1:].values.reshape(1,-1)).tolist(),axis=1)
print(idf)

#predictions = predictions.to_frame()
predictions = pd.DataFrame(item for item in predictions)
predictions = pd.DataFrame(predictions[0].values.tolist())
print (predictions)

#predictions = pd.DataFrame.from_items(zip(predictions.index, predictions.values)).T


print(predictions)
predictions = pd.DataFrame(predictions.values)

#predictions = pd.DataFrame.from_records(predictions.values,index=predictions.index)


#print(predictions)
#print(predictions2)
#predictions2 = predictions.apply(pd.Series)

#expanded = predictions.apply(lambda x : np.reshape(x,64),axis=1)
#print(expanded)

timestamps = idf.loc[:,0]
#timestamps = pd.DataFrame.from_records(timestamps.values,index=timestamps.index)

#timestamps = np.vstack(timestamps)

#timestamps.squeeze()
timestamps = timestamps.to_frame('ts')
print(timestamps)
#predictions['ts'] = timestamps.values

#combined = timestamps.merge(predictions)

#combined = pd.concat([timestamps, predictions], axis=1, ignore_index=True)

combined = timestamps.join(predictions,how='left')
#print(timestamps)
#print(predictions2)
#print(predictions)
print(combined)

combined.to_csv(index=False,sep="\t",path_or_buf="out.csv")
