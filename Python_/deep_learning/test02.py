import tensorflow as tf
import pandas as pd
import numpy as np

data = pd.read_csv("gpascore.csv")
data = data.dropna()

y_data = data['admit']

x_data = []

for i, rows in data.iterrows():
    x_data.append([rows['gre'], rows['gpa'], rows['rank']])

model = tf.keras.models.Sequential([
    tf.keras.layers.Dense(64, activation ='tanh'),
    tf.keras.layers.Dense(128, activation ='tanh'),
    tf.keras.layers.Dense(1, activation ='sigmoid'),
])

model.compile(optimizer='adam', loss='binary_crossentropy', metrics=['accuracy'])

model.fit(np.array(x_data), np.array(y_data), epochs=600)

pred = model.predict([[750, 3.70, 3], [400, 2.2, 1]])
print(pred)