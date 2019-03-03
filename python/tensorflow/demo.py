
import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt


n = 1024    # data size
X = np.random.normal(0, 1, n)
print X.shape
Y = np.random.normal(0, 1, n)
T = np.arctan2(Y,X) # for color value
print T.shape

plt.scatter(X, Y, s=75, c=T, alpha=.5)

plt.xlim(-1.5, 1.5)
plt.xticks(())  # ignore xticks
plt.ylim(-1.5, 1.5)
plt.yticks(())  # ignore yticks

plt.show()

exit(0)







x = np.linspace(-3, 3, 50)
print x
y1 = 0.1 * x
y2 = x ** 2


print x.shape

plt.figure(num=3, figsize=(8, 5))
l1, = plt.plot(x, y1, linewidth=10, zorder=1, label='linear line')
# l2,  = plt.plot(x, y1, color='red', linewidth=1.0, linestyle='--', label='square line')
# plt.xlim((-1, 2))
plt.ylim((-2, 2))
# plt.xlabel('I am x')
# plt.ylabel('I am y')

ax = plt.gca()
ax.spines['top'].set_color('none')
ax.spines['right'].set_color('none')
ax.spines['bottom'].set_position(('data', 0))
ax.yaxis.set_ticks_position('left')
ax.spines['left'].set_position(('data', 0))

x0 = 1
y0 = 0.1*x0
plt.plot([x0, x0], [0, y0], 'k--', linewidth=2.5)
plt.scatter([x0, ], [y0, ], s=100, color='b')

plt.annotate(r'$2x+1=%s$' % y0, xy=(x0, y0), xycoords='data', xytext=(+30, -30),
             textcoords='offset points', fontsize=16,
             arrowprops=dict(arrowstyle='->', connectionstyle="arc3,rad=.2"))
plt.text(-3.7, 3, r'$This\ is\ the\ some\ text. \mu\ \sigma_i\ \alpha_t$',
         fontdict={'size': 16, 'color': 'r'})


plt.show()
exit(0)

new_ticks = np.linspace(-1, 2, 5)
plt.xticks(new_ticks)
plt.yticks([-2, -1.8, -1, 1.22, 3],['$really\ bad$', '$bad$', '$normal$', '$good$', '$really\ good$'])


ax = plt.gca()
ax.spines['right'].set_color('none')
ax.spines['top'].set_color('none')
ax.spines['bottom'].set_position(('data', 0))
ax.spines['left'].set_position(('data', 0))

plt.legend(handles=[l1, l2], labels=['up', 'down'],  loc = 'best')

plt.show()





exit(0)

A = np.array([1, 2, 3])[:, np.newaxis]
B = np.array([4, 5, 6])[:, np.newaxis]
print A

print np.concatenate((A, B), axis=1)

A = np.arange(12).reshape((3, 4))
print A

print np.array_split(A, 3, axis=1)

print '==============='
a = np.arange(4)
b = a
print a, b
a[0] = 99
print b
b = a.copy()
a[0] = 112
print a, b


exit(0)















hello = tf.constant('Hello, TensorFlow')
node1 = tf.constant(3.0, dtype=tf.float32)
node2 = tf.constant(4.0)
node3 = tf.add(node1, node2)


sess = tf.Session()
print sess.run([node1, node2])
print "node3:", node3
print "sess.run(node3):", sess.run(node3)

a = tf.placeholder(tf.float32)
b = tf.placeholder(tf.float32)
adder_node = a + b
