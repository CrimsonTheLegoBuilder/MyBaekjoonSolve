import tensorflow as tf

train_x = [0,1,2,3,4,5,6,7]
# train_y = [3,5,7,9,11,13,15]
train_y = [1,5,9,13,17,21,25,29]

a = tf.Variable(0.1)
b = tf.Variable(0.1)

opt = tf.keras.optimizers.Adam(learning_rate=0.001)


def f():
    pre_y = train_x * a + b
    return tf.keras.losses.mse(train_y, pre_y)


for i in range(3000):
    opt.minimize(f, var_list=[a, b])
    print(a.numpy(), b.numpy())

