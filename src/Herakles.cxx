/************************************
 * Herakles
 * Brief: IPbus module for python
 * 
 * Author: Daniel Bullock
 *         Carlos.Solans@cern.ch
 ************************************/

#define PY_SSIZE_T_CLEAN //Not sure we need this
#include <Python.h>
#include <stddef.h>
#include "ipbus/Uhal.h"
#include <iostream>

#if PY_VERSION_HEX < 0x020400F0

#define Py_CLEAR(op)				\
  do {						\
    if (op) {					\
      PyObject *tmp = (PyObject *)(op);		\
      (op) = NULL;				\
      Py_DECREF(tmp);				\
    }						\
  } while (0)

#define Py_VISIT(op)				\
  do {						\
    if (op) {					\
      int vret = visit((PyObject *)(op), arg);	\
      if (vret)					\
	return vret;				\
    }						\
  } while (0)

#endif //PY_VERSION_HEX < 0x020400F0


#if PY_VERSION_HEX < 0x020500F0

typedef int Py_ssize_t;
#define PY_SSIZE_T_MAX INT_MAX
#define PY_SSIZE_T_MIN INT_MIN
typedef inquiry lenfunc;
typedef intargfunc ssizeargfunc;
typedef intobjargproc ssizeobjargproc;

#endif //PY_VERSION_HEX < 0x020500F0

#ifndef PyVarObject_HEAD_INIT
#define PyVarObject_HEAD_INIT(type, size)	\
  PyObject_HEAD_INIT(type) size,
#endif


#if PY_VERSION_HEX >= 0x03000000

#define MOD_ERROR NULL
#define MOD_INIT(name) PyObject* PyInit_##name(void)
#define MOD_RETURN(val) val

#else

#define MOD_ERROR
#define MOD_INIT(name) PyMODINIT_FUNC init##name(void)
#define MOD_RETURN(val)

#endif //PY_VERSION_HEX >= 0x03000000

/************************************
 *
 * Module declaration
 *
 ************************************/

typedef struct {
  PyObject_HEAD
  ipbus::Uhal *obj;
  std::vector<uint32_t> vec;
} PyIpbusUhal;

static int _PyIpbusUhal_init(PyIpbusUhal *self, PyObject *args, PyObject *kwargs)
{
    const char *hostname;
    Py_ssize_t hostname_len;
    unsigned int port;
    const char *keywords[] = {"hostname", "port", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "s#I", (char **) keywords, 
				     &hostname, &hostname_len, &port)) {
      return -1;
    }
    self->obj = new ipbus::Uhal(std::string(hostname, hostname_len), port);
    return 0;
}

static void _PyIpbusUhal_dealloc(PyIpbusUhal *self)
{
  delete self->obj;
  Py_TYPE(self)->tp_free((PyObject*)self);
}

PyObject * _PyIpbusUhal_Read(PyIpbusUhal *self, PyObject *args, PyObject *kwargs)
{
  uint32_t address;
  uint32_t size=1;
  PyObject *py_fifo = NULL;
  const char *keywords[] = {"address", "size", "fifo", NULL};
  if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "I|IO", (char **) keywords, 
				   &address, &size, &py_fifo)) {
    Py_INCREF(Py_None);
    return Py_None;
  }
  bool fifo = py_fifo? (bool) PyObject_IsTrue(py_fifo) : false;
  self->obj->Read(address, self->vec, size, fifo);
  PyObject *py_ret;
  
  //size to read is one or greater
  if(size>1){
    py_ret = PyList_New(size); 
    for(uint32_t i=0;i<size;i++){
      PyObject * it = PyInt_FromLong(self->vec.size()>i?(long)self->vec.at(i):0);
      if(PyList_SetItem(py_ret,i,it)!=0){
	// Error parsing item
	Py_INCREF(Py_None);
	return(Py_None);
      }
    }
  }else{
    py_ret = PyInt_FromLong(self->vec.size()>0?(long)self->vec.at(0):0);
  }
  Py_INCREF(py_ret);
  return py_ret;
}

PyObject * _PyIpbusUhal_Write(PyIpbusUhal *self, PyObject *args, PyObject *kwargs)
{
  /*
  uint32_t address;
  uint32_t value;
  const char *keywords1[] = {"address", "value", NULL};
  if(PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "II", (char **) keywords1, 
				 &address, &value)) {
				 self->obj->Write(address, value);
				 Py_INCREF(Py_None);
				 return Py_None;
				 }
  */

  PyObject *py_fifo = NULL;
  PyObject *py_values = NULL;
  uint32_t address;
  uint32_t len_values;
  //(I:)# values, len
  const char *keywords2[] = {"address", "values", "fifo", NULL};
  if(PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "IO|O", (char **) keywords2, 
				 &address, &py_values, &py_fifo)) {
    self->vec.clear();
    if(PyList_Check(py_values)){ //accept a python list [1,1,2]
      len_values = PyList_Size(py_values);
      for(uint32_t i=0;i<len_values;i++){
	self->vec.push_back(PyInt_AsLong(PyList_GetItem(py_values,i)));
      }
    }else if(PyTuple_Check(py_values)){ //accept a python tuple (1,1,2)
      len_values = PyTuple_Size(py_values);
      for(uint32_t i=0;i<len_values;i++){
	self->vec.push_back(PyInt_AsLong(PyTuple_GetItem(py_values,i)));
      }
    }else if(PyInt_Check(py_values)){ //accept an integer
      uint32_t value = PyInt_AsLong(py_values);
      self->obj->Write(address, value);
    }else{
      Py_INCREF(Py_None);
      return Py_None;
    }
    bool fifo = py_fifo? (bool) PyObject_IsTrue(py_fifo) : false;
    self->obj->Write(address, self->vec, fifo);
    Py_INCREF(Py_None);
    return Py_None;
  }
  //!case we couldn't parse args
  Py_INCREF(Py_None);
  return Py_None;
}

PyObject * _PyIpbusUhal_SetVerbose(PyIpbusUhal *self, PyObject *args)
{
  PyObject * enable = NULL;
  if(PyArg_ParseTuple(args, (char *) "O",&enable)){
    if(enable!=NULL){
      self->obj->SetVerbose((bool) PyObject_IsTrue(enable));
    }
  }
  Py_INCREF(Py_None);
  return Py_None;
}

PyObject * _PyIpbusUhal_Sync(PyIpbusUhal *self)
{
  self->obj->Sync();
  Py_INCREF(Py_None);
  return Py_None;
}

static PyMethodDef PyIpbusUhal_methods[] = {
  {(char *) "Read",      (PyCFunction) _PyIpbusUhal_Read,       METH_KEYWORDS|METH_VARARGS, NULL },
  {(char *) "Write",     (PyCFunction) _PyIpbusUhal_Write,      METH_KEYWORDS|METH_VARARGS, NULL },
  {(char *) "SetVerbose",(PyCFunction) _PyIpbusUhal_SetVerbose, METH_VARARGS, NULL },
  {(char *) "Sync",      (PyCFunction) _PyIpbusUhal_Sync,       METH_NOARGS, NULL },
  {NULL, NULL, 0, NULL}
};

PyTypeObject PyIpbusUhal_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    (char *) "Herakles.Uhal",            /* tp_name */
    sizeof(PyIpbusUhal),                 /* tp_basicsize */
    0,                                   /* tp_itemsize */
    /* methods */
    (destructor) _PyIpbusUhal_dealloc,   /* tp_dealloc */
    (printfunc)0,                        /* tp_print */
    (getattrfunc)NULL,                   /* tp_getattr */
    (setattrfunc)NULL,                   /* tp_setattr */
    (cmpfunc)NULL,                       /* tp_compare */
    (reprfunc)NULL,                      /* tp_repr */
    (PyNumberMethods*)NULL,              /* tp_as_number */
    (PySequenceMethods*)NULL,            /* tp_as_sequence */
    (PyMappingMethods*)NULL,             /* tp_as_mapping */
    (hashfunc)NULL,                      /* tp_hash */
    (ternaryfunc)NULL,                   /* tp_call */
    (reprfunc)NULL,                      /* tp_str */
    (getattrofunc)NULL,                  /* tp_getattro */
    (setattrofunc)NULL,                  /* tp_setattro */
    (PyBufferProcs*)NULL,                /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,                  /* tp_flags */
    NULL,                                /* Documentation string */
    (traverseproc)NULL,                  /* tp_traverse */
    (inquiry)NULL,                       /* tp_clear */
    (richcmpfunc)NULL,                   /* tp_richcompare */
    0,                                   /* tp_weaklistoffset */
    (getiterfunc)NULL,                   /* tp_iter */
    (iternextfunc)NULL,                  /* tp_iternext */
    (struct PyMethodDef*)PyIpbusUhal_methods, /* tp_methods */
    (struct PyMemberDef*)0,              /* tp_members */
    0,                                   /* tp_getset */
    NULL,                                /* tp_base */
    NULL,                                /* tp_dict */
    (descrgetfunc)NULL,                  /* tp_descr_get */
    (descrsetfunc)NULL,                  /* tp_descr_set */
    0,                                   /* tp_dictoffset */
    (initproc)_PyIpbusUhal_init,         /* tp_init */
    (allocfunc)PyType_GenericAlloc,      /* tp_alloc */
    (newfunc)PyType_GenericNew,          /* tp_new */
    (freefunc)0,                         /* tp_free */
    (inquiry)NULL,                       /* tp_is_gc */
    NULL,                                /* tp_bases */
    NULL,                                /* tp_mro */
    NULL,                                /* tp_cache */
    NULL,                                /* tp_subclasses */
    NULL,                                /* tp_weaklist */
    (destructor) NULL                    /* tp_del */
};

static PyMethodDef PyIpbus_methods[] = {
  {NULL, NULL, 0, NULL}
};

#if PY_VERSION_HEX >= 0x03000000
static struct PyModuleDef Herakles_moduledef = {
    PyModuleDef_HEAD_INIT,
    "Herakles",
    NULL,
    -1,
    PyIpbus_methods,
};
#endif

MOD_INIT(Herakles)
{
    PyObject *m;
    #if PY_VERSION_HEX >= 0x03000000
    m = PyModule_Create(&Herakles_moduledef);
    #else
    m = Py_InitModule3((char *) "Herakles", PyIpbus_methods, NULL);
    #endif
    if(PyType_Ready(&PyIpbusUhal_Type)<0){
      MOD_RETURN(MOD_ERROR);
    }
    PyModule_AddObject(m, (char *) "Uhal", (PyObject *) &PyIpbusUhal_Type);
    MOD_RETURN(m);
}
