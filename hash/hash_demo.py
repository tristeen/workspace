import random
import pprint


FOOD_SPACE_SIZE = 1024


class Earth(object):

	def __init__(self):
		pass

	def produce(self, n=1):
		if n == 1:
			return random.randint(0, FOOD_SPACE_SIZE - 1)
		else:
			return [random.randint(0, FOOD_SPACE_SIZE - 1) for i in xrange(n)]	


class God(object):

	def __init__(self, num):
		self._num = num
		self.cur_allocation = dict([(k, []) for k in xrange(num)])
		self.shift_history = []
	
	@property
	def num(self):
		return self._num

	@num.setter
	def num(self, v):
		old, new = self._num, v
		self._num = new
		self.on_num_changed(old, new)

	def on_num_changed(self, old, new):
		new_allocation = dict([(k, []) for k in xrange(new)])
		changed, foodsum = 0, sum(map(lambda x: len(x), self.cur_allocation.values()))
		for old_index, foods in self.cur_allocation.iteritems():
			for food in foods:
				new_index = self.allocate(food, new_allocation)	
				if old_index != new_index:
					changed += 1
		self.shift_history.append((changed, foodsum))

	def calc_index(self, food):
		return 0

	def allocate(self, food, data=None):
		index = self.calc_index(food)
		if data is None:
			data = self.cur_allocation
		data[index].append(food)
		return index


	def stat(self):
		return 'self.cur_allocation:\n%s\nshift_history:\n%s\n' %\
				(pprint.pformat(dict(map(lambda x: (x[0], len(x[1])), self.cur_allocation.iteritems()))),
				pprint.pformat(map(lambda x: '%d/%d' % (x[0], x[1]), self.shift_history)), )


class ModGod(God):

	def calc_index(self, food):
		return food % self.num


class DivGod(God):

	def calc_index(self, food):
		return int(food / (1.0 * FOOD_SPACE_SIZE / self.num))

class RingGod(God):

	def __init__(self, num):
		super(RingGod, self).__init__(num)
		self.points = [] 

	def hash(self, v):
		return (v * 92323 + 2897) % FOOD_SPACE_SIZE

	def calc_index(self, food):
		if not self.points:
			self.points = [self.hash(i) for i in xrange(self.num)]
		index = 0
		# calc
		return index

def run():
	earth = Earth()	
	foods = earth.produce(256)
	
	GodCls = [ModGod, DivGod, RingGod]
	
	for gs in GodCls:
		print '\n%s begin.' % gs
		god = gs(10)
		for food in foods:
			god.allocate(food)
		print god.stat()
		god.num += 1
		print god.stat()
		print '%s end.\n' % gs

if __name__ == '__main__':
	run()

