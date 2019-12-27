from keras.models import load_model

model = load_model('../data/WindDenseNN.h5')

model.summary()

model.layers[0].get_weights()
