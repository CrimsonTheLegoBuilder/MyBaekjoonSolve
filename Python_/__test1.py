import tensorflow as tf

H = 170
S = 260

a = tf.Variable(0.1)
b = tf.Variable(0.2)


def loss_f():
    pre = (H * a + b)
    return tf.square(S - pre)


opt = tf.keras.optimizers.Adam(learning_rate=0.1)
for i in range(300):
    opt.minimize(loss_f, var_list=[a, b])
    print(a.numpy(), b.numpy())
    print(H * a.numpy() + b.numpy(), S)
