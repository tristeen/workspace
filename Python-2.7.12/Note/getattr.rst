1.	查找 instance 的属性。

1.1.	exmample 1: getattr(a, '__hash__') or a.__hash__

		>>> import dis
		>>> class A(object): 
		...  v1 = 1
		...  def __init__(self):
		...   self.v2 = 2
		... 
		>>> a = A()
		>>> dis.dis(compile("getattr(a, '__hash__')", "", "exec"))
		  1           0 LOAD_NAME                0 (getattr)
		              3 LOAD_NAME                1 (a)
		              6 LOAD_CONST               0 ('__hash__')
		              9 CALL_FUNCTION            2
		             12 POP_TOP             
		             13 LOAD_CONST               1 (None)
		             16 RETURN_VALUE        

		>>> dis.dis(compile("a.__hash__", "", "exec"))
		  1           0 LOAD_NAME                0 (a)
		              3 LOAD_ATTR                1 (__hash__)
		              6 POP_TOP             
		              7 LOAD_CONST               0 (None)
		             10 RETURN_VALUE      


1.2. 	getattr(a, '__hash__') 和 a.__hash__最终都通过调用
		PyObject_GetAttr来实现功能。

1.3.	调用 a.ob_type 即 instance 的 tp_getattro 或者 tp_getattr。 instance_getattr?
		A 为 PyClass_Object. tp_getattro 为 class_getattr。

1.4.	instance_getattr。 先查找 in_dict, 然后调用 class_lookup。 
		A 以及 A->cl_bases， 返回 object->__hash__。

		descr 为 object.__hash__。
		descr->ob_type 为 wrapper_descriptor。
		wrapper_descriptor 的 tp_descr_get 为 wrapperdescr_get。
		wrapper_get 调用 PyWrapper_New 新建 wrapperobject。 
		wrapperobject->self 设置为 a， descr 为 object.__hash__。

		>>> A.__hash__
		<slot wrapper '__hash__' of 'object' objects>
		>>> object.__hash__
		<slot wrapper '__hash__' of 'object' objects>
		>>> a.__hash__
		<method-wrapper '__hash__' of A object at 0x106228550>

2.	查找 class 的属性。

2.1.	example 2: getattr(A, '__hash__') or A.__hash__ 

		>>> dis.dis(compile('A.__hash__', '', 'exec'))
		  1           0 LOAD_NAME                0 (A)
		              3 LOAD_ATTR                1 (__hash__)
		              6 POP_TOP             
		              7 LOAD_CONST               0 (None)
		             10 RETURN_VALUE        
		>>> dis.dis(compile('getattr(A, "__hash__")', '', 'exec'))
		  1           0 LOAD_NAME                0 (getattr)
		              3 LOAD_NAME                1 (A)
		              6 LOAD_CONST               0 ('__hash__')
		              9 CALL_FUNCTION            2
		             12 POP_TOP             
		             13 LOAD_CONST               1 (None)
		             16 RETURN_VALUE  

2.2.	调用PyObject_GetAttr。	

2.3.	调用 A.ob_type 即 type 的 tp_getattro 或者 tp_getattr。
		PyTypeObject 的 tp_getattro 为 type_getattro。

2.4.	type_getattro.
		2.4.1.	先通过 metatype 的 mro 查找。A 的 metatype 为 type。
				通过 metatype 找到 meta_attribute: type.__hash__。 
				>>> type.__hash__
				<slot wrapper '__hash__' of 'type' objects>
				但是，PyDescr_IsData(meta_attribute)检测不过，继续执行。

		2.4.2	通过 A 的 mro 查找。
				>>> object.__hash__
				<slot wrapper '__hash__' of 'object' objects>
				>>> type(object.__hash__)
				<type 'wrapper_descriptor'>
				attribute 为 object.__hash__。

				调用 wrapper_descriptor 的 tp_descr_get: wrapperdescr_get
				返回wrapperobject。
				wrapperdescr_get 中，在 obj 为 NULL 的时候， 直接返回 descr，
				即 object.__hash__。


3. 其他:

3.1.	A 是 PyClassObject。object 是 PyBaseObject_Type。A 是 object 的子类。

		>>> type(a)
		<class '__main__.A'>
		>>> A
		<class '__main__.A'>
		>>> object
		<type 'object'>
		>>> issubclass(A, object)
		True
		>>> type(A) == type(object) == type
		True

3.2.	PyObject_GenericGetAttr

		3.2.1	通过 mro 依次进行查找。（会优先使用和维护 method cache）
				>>> A.__mro__
				(<class '__main__.A'>, <type 'object'>)
				如果是 class，则检查 ((PyClassObject *)base)->cl_dict。
				如果是 type，则检查 ((PyTypeObject *)base)->tp_dict。

		3.2.2	在 A 的 cl_dict 中，没有 __hash__。
				在 object 的 tp_dict 中，有 __hash__。

				>>> id(A.__hash__)
				4396249248
				>>> id(object.__hash__)
				4396249248

		3.2.3 	descr 为 object.__hash__。
				descr->ob_type 为 wrapper_descriptor。
				wrapper_descriptor 的 tp_descr_get 为 wrapperdescr_get。
				wrapper_get 调用 PyWrapper_New 新建 wrapperobject。 
				wrapperobject->self 设置为 a， descr 为 object.__hash__。

				>>> A.__hash__
				<slot wrapper '__hash__' of 'object' objects>
				>>> object.__hash__
				<slot wrapper '__hash__' of 'object' objects>
				>>> a.__hash__
				<method-wrapper '__hash__' of A object at 0x106228550>
