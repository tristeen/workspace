1.	class A(object): pass

	>>> import dis
	>>> dis.dis(compile('class A(object): pass', '', 'exec'))
	  1           0 LOAD_CONST               0 ('A')
	              3 LOAD_NAME                0 (object)
	              6 BUILD_TUPLE              1
	              9 LOAD_CONST               1 (<code object A at 0x1049062b0, file "", line 1>)
	             12 MAKE_FUNCTION            0
	             15 CALL_FUNCTION            0
	             18 BUILD_CLASS         
	             19 STORE_NAME               1 (A)
	             22 LOAD_CONST               2 (None)
	             25 RETURN_VALUE        


	>>> c = compile('class A(object): pass', '', 'exec')
	>>> c.co_consts[1]
	<code object A at 0x1049062b0, file "", line 1>
	>>> dis.dis(c.co_consts[1])
	  1           0 LOAD_NAME                0 (__name__)
	              3 STORE_NAME               1 (__module__)
	              6 LOAD_LOCALS         
	              7 RETURN_VALUE        

	>>> class A(object): pass
	... 
	>>> class B: pass
	... 
	>>> A
	<class '__main__.A'>
	>>> B
	<class __main__.B at 0x104920328>
	>>> type(A)
	<type 'type'>
	>>> type(B)
	<type 'classobj'>
	>>> dir(A)
	['__class__', '__delattr__', '__dict__', '__doc__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__']
	>>> dir(B)
	['__doc__', '__module__']
	>>> B.__hash__
	Traceback (most recent call last):
	  File "<stdin>", line 1, in <module>
	AttributeError: class B has no attribute '__hash__'
	>>> A.__hash__
	<slot wrapper '__hash__' of 'object' objects>

	A 与 B 是不同的类型。

2.	MAKE_FUNCTION 从 codeobject 中构造出 funcobject：<function A at 0x102347450>。
	CALL_FUNCTION 调用 <function A at 0x102347450>。
	栈顶为 LOAD_LOCALS 加载的 locals。接下来是之前压入栈的两个参数 PyObject *bases, PyObject *name。


3.	BUILD_CLASS 调用 build_class， 查找对应的 metaclass。 调用 metaclass 的 tp_call 来构造 classobj。
	result = PyObject_CallFunctionObjArgs(metaclass, name, bases, methods, NULL);

	build_class 的传入参数为：
		name: 'A'
		bases: (<type 'object'>,) 或 ()
		methods: {'__module__': '__main__'}

	其中：
		对 class A(object): pass 来说：
  			metaclass is: <type 'type'>
  		对 class B: pass 来说：
  			metaclass is: <type 'classobj'>

4.	PyObject_CallFunctionObjArgs 调用 PyObject_Call。
	PyObject_Call(PyObject *func, PyObject *arg, PyObject *kw) 中，调用 (*func->ob_type->tp_call)(func, arg, kw)

5.	两种路径都是通过调用 PyType_Type->tp_call，即 type_call 来实现。type_call 调用 type_new 和 type_init。

	static PyObject *type_call(PyTypeObject *type, PyObject *args, PyObject *kwds) 中 type 分别为 PyType_Type 或 PyClass_Type, args 为 （‘A', (<type 'object'>,) 或 (), {'__module__': '__main__'}), kwrds: <nil>。

	5.1.	当 type 为 PyType_Type 时， 调用 PyType_Type->tp_new，即 type_new。
			返回一个 PyTypeObject。

	5.2.	当 type 为 PyClass_Type 时， 调用 PyClass_Type->tp_new，即 class_new。
			PyObject *PyClass_New(PyObject *bases, PyObject *dict, PyObject *name)
			其中， name: 'A', bases: () dict: {'__module__': '__main__'}
			返回一个 PyClassObject。







