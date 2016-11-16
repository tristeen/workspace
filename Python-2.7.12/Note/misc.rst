1. class_lookup 和 type_lookup。 前者通过 cl_bases 查找所有的 cl_dict，后者通过 tp_mro 查找所有的 cl_dict 和 tp_dict。

	static PyObject * class_lookup(PyClassObject *cp, PyObject *name, PyClassObject **pclass)
	{
		// query cp->cl_dict
		...

		// query every cl_dict for cp->cl_bases
		...
	}

	/* Internal API to look for a name through the MRO. */
	PyObject * _PyType_Lookup(PyTypeObject *type, PyObject *name)
	{
		
		// query cache
		...

		// query every cl_dict or tp_dict for tp_mro
		...

		// save in cache
		...
	}