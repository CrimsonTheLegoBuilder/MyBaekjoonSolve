import pandas as pd
import tensorflow as tf
import numpy as np

data = pd.read_csv("gpascore.csv")

data = data.dropna()

y_data = data['admit'].values
# print(y_data)
x_data = []
for i, rows in data.iterrows():
    x_data.append([rows['gre'], rows['gpa'], rows['rank']])

for row in x_data:
    print(row)

model = tf.keras.models.Sequential([
    tf.keras.layers.Dense(64, activation='tanh'),
    tf.keras.layers.Dense(128, activation='tanh'),
    tf.keras.layers.Dense(64, activation='tanh'),
    tf.keras.layers.Dense(1, activation='sigmoid'),
])

model.compile(optimizer='adam', loss='binary_crossentropy', metrics=['accuracy'])
model.fit(np.array(x_data), np.array(y_data), epochs=1000)

pred = model.predict([[750, 3.70, 2], [400, 2.20, 1], [800, 4.45, 1]])
print(pred)