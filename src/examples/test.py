from keras.models import load_model

model = load_model('../../data/WindDenseNN.h5',compile=False)

model.summary()

x = model.layers[0].get_weights()
print(x,x[0].size)
