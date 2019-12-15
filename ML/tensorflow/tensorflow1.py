import tensorflow as tf
import numpy as np

input1 = tf.placeholder(tf.float32)
input2 = tf.placeholder(tf.float32)
output = tf.multiply(input1, input2)

with tf.Session() as sess:
    print sess.run(output, feed_dict={input1:[7.], input2:[2.]})

exit(0)


#create data
x_data = np.random.rand(100).astype(np.float32)
y_data = x_data * 0.1 + 0.3

a = tf.random_uniform([1], -1.0, 1.0)

Weights = tf.Variable(tf.random_uniform([1], -1.0, 1.0))
biases = tf.Variable(tf.zeros([1]))

y = Weights * x_data + biases

loss = tf.reduce_mean(tf.square(y - y_data))

optimizer = tf.train.GradientDescentOptimizer(0.5)
train = optimizer.minimize(loss)

init = tf.global_variables_initializer()

with tf.Session() as sess:
    sess.run(init)

    for step in range(201):
        sess.run(train)
        if step % 20 == 0:
            print step, sess.run(Weights), sess.run(biases)