/* Vector object interface */

#ifndef Py_VECTOROBJECT_H
#define Py_VECTOROBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	PyObject_VAR_HEAD
	PyObject **ob_item;
} PyVectorObject;

PyAPI_DATA(PyTypeObject) PyVector_Type;

#ifdef __cplusplus
}
#endif
#endif /* !Py_VECTOROBJECT_H */
