/* Boolean type, a subtype of int */

#include "Python.h"

static int
vector_print(PyVectorObject *op, FILE *fp, int flags)
{
    PyObject *item;
    int i = 0;
    Py_BEGIN_ALLOW_THREADS
    fprintf(fp, "<");
    Py_END_ALLOW_THREADS
    for (i = 0; i < Py_SIZE(op); i++) {
        item = op->ob_item[i];
        Py_INCREF(item);
        if (i > 0) {
            Py_BEGIN_ALLOW_THREADS
            fprintf(fp, ", ");
            Py_END_ALLOW_THREADS
        }
        PyObject_Print(item, fp, 0);
        Py_DECREF(item);
    }
    Py_BEGIN_ALLOW_THREADS
    fprintf(fp, ">");
    Py_END_ALLOW_THREADS
    return 0;
}

static PyObject *
vector_repr(PyVectorObject *op)
{
    PyObject *s = PyString_InternFromString("Undefined");
    Py_XINCREF(s);
    return s;
}

/* We define vector_new to always return either Py_True or Py_False */

static PyObject *
vector_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    printf("vector_new 1");
    PyVectorObject *op;
    int size;
    int nbytes;

    if (!PyArg_ParseTuple(args, "i", &size))
        return NULL;

    printf("vector_new 2");

    if (size <= 0)
        return NULL;

    nbytes = size * sizeof(PyObject *);

    op = PyObject_New(PyVectorObject, &PyVector_Type);
    if (op == NULL)
        return NULL;
    printf("vector_new 3");


    op->ob_item = (PyObject **)PyMem_MALLOC(nbytes);
    if (op->ob_item == NULL) {
        Py_DECREF(op);
        return PyErr_NoMemory();
    }
    printf("vector_new 4");

    memset(op->ob_item, 0, nbytes);
    Py_SIZE(op) = size;

    return (PyObject *) op;
}

/* Doc string */

PyDoc_STRVAR(vector_doc,
"vector type");


PyTypeObject PyVector_Type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    "vector",
    sizeof(PyVectorObject),
    0,
    0,                                          /* tp_dealloc */
    (printfunc)vector_print,                      /* tp_print */
    0,                                          /* tp_getattr */
    0,                                          /* tp_setattr */
    0,                                          /* tp_compare */
    (reprfunc)vector_repr,                        /* tp_repr */
    0,                                          /* tp_as_number */
    0,                                          /* tp_as_sequence */
    0,                                          /* tp_as_mapping */
    0,                                          /* tp_hash */
    0,                                          /* tp_call */
    (reprfunc)vector_repr,                        /* tp_str */
    0,                                          /* tp_getattro */
    0,                                          /* tp_setattro */
    0,                                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_CHECKTYPES, /* tp_flags */
    vector_doc,                                   /* tp_doc */
    0,                                          /* tp_traverse */
    0,                                          /* tp_clear */
    0,                                          /* tp_richcompare */
    0,                                          /* tp_weaklistoffset */
    0,                                          /* tp_iter */
    0,                                          /* tp_iternext */
    0,                                          /* tp_methods */
    0,                                          /* tp_members */
    0,                                          /* tp_getset */
    0,                                         /* tp_base */
    0,                                          /* tp_dict */
    0,                                          /* tp_descr_get */
    0,                                          /* tp_descr_set */
    0,                                          /* tp_dictoffset */
    0,                                          /* tp_init */
    0,                                          /* tp_alloc */
    vector_new,                                   /* tp_new */
};
