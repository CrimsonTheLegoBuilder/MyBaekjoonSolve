import tensorflow as tf
import pandas as pd


model = tf.keras.models.Sequential([
    tf.keras.Dense(64, activation ='tanh'),
    tf.keras.Dense(128, activation ='tanh'),
    tf.keras.Dense(1, activation ='sigmoid'),
])

model.compile(optimizer='adam', loss='binary_crossentropy', metrics=['accuracy'])

model.fit()