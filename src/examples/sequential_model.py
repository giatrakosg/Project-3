import keras
import numpy as np
import pandas as pd
from keras import layers, optimizers, losses, metrics
# Initializes the model
model = keras.Sequential()
# Adds a densely-connected layer with 64 nodes to the model:
# input shape defines the number of input features (dimensions)
# activation defines the activation function of each layer
model.add(layers.Dense(64, activation='relu', input_shape=(10,)))
# Add another:
model.add(layers.Dense(64, activation='relu'))
# Add a softmax layer with 10 output nodes:
model.add(layers.Dense(10, activation='softmax'))

model.compile(optimizer=optimizers.RMSprop(0.01),
loss=losses.CategoricalCrossentropy(),
metrics=[metrics.CategoricalAccuracy()])

# The number of columns of the input must be equal with the number of
# rows of the input shape of the first layer. The number of columns of
# the "labels" must be equal with the number of the output nodes.
# The number of rows of data and labels is the size of the training set.
data = np.random.random((1000, 10))
labels = np.random.random((1000, 10))

# From file (slide 49)
# in place of the corresponding lines of code
#data = pd.read_csv('data.csv')
#labels = pd.read_csv('labels.csv')

model.fit(data, labels, epochs=10, batch_size=100)

# Evaluate prediction according to defined metrics (slide 53)
model.evaluate(data, labels, batch_size=32)
# Get a random data set for prediction
test_data = np.random.random((1000, 10))
result = model.predict(test_data, batch_size=32)
print(result.shape)
print(result)
