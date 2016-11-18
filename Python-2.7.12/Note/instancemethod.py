
class A(object):

	def f(self):
		pass

a = A()

print id(a.f) # get a.f, decref a.f, save in freelist
print id(a.f) # get a.f from freelist, ...
print id(a.f)
print id(a.f)
print id(a.f)

b = []
for i in xrange(5):
	b.append(id(a.f)) # get a.f, calculate attr, and decref a.f
			
print b

c = []

for i in xrange(5):
	c.append(a.f) # get a.f

print map(lambda i: id(i), c)
