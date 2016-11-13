1.  A() 或 B()。 以下将不考虑 B()。

	>>> class A(object): pass
	... 
	>>> class B: pass
	... 
	>>> A()
	<__main__.A object at 0x10493ff10>
	>>> B()
	<__main__.B instance at 0x10493e950>

	>>> dis.dis(compile('A()', '', 'exec'))
	  1           0 LOAD_NAME                0 (A)
	              3 CALL_FUNCTION            0
	              6 POP_TOP             
	              7 LOAD_CONST               0 (None)
	             10 RETURN_VALUE 
	>>> A
	<class '__main__.A'>


2.	CALL_FUNCTION 调用的为上一条指令压入栈顶的 A.
	call_function 将调用 do_call。

	static PyObject *do_call(PyObject *func, PyObject ***pp_stack, int na, int nk) 中， func 为 A。
    result = PyObject_Call(func, callargs, kwdict);

	在 PyObject_Call 中调用 （*A->ob_type->tp_call)(A, (), {}), 即 type_call(A, (), {})。

	A->tp_new 为 object_new。
	A->tp_init 为 object_init, 当 A 没有实现 __init__ 的时候。
	A->tp_init 为 slot_tp_init，当 A 实现 __init__ 的时候。
	object_init 不做实际事情, 所有没什么必要super(object, self).__init__()。
	slot_tp_init 查找定义的 __init__ 方法，并调用。
