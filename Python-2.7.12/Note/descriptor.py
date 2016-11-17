
class A1(list):

	def __get__(self, obj, cls):
		print 'A1.__get__'

class B1(object):
	value = A1()

b1 = B1()

print b1.value, type(b1.value)

b2 = B1()
b2.value = 2

print b2.value, b2.__dict__['value'], b2.__class__.__dict__['value']

class A2(list):

	def __get__(self, obj, cls):
		print 'A2.__get__'

	def __set__(self, obj, value):
		print 'A.__set__'
		self.append(value)

class B2(object):
	value = A2()

b3 = B2()
b3.value = 3
b3.__class__.__dict__['value']
# b3.__dict__['value']


B1.value
b4 = B1()
b4.value
b4.instance_value = A1()
b4.instance_value
