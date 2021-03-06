////////////////////////////////////////////////////////////////////
//
// Dean Andrew Hidas <dhidas@bnl.gov>
//
// Created on: Tue Jan 31 17:38:57 EST 2017
//
// This is the python-C extension which allows access to the c++
// class OSCARSTH.
//
////////////////////////////////////////////////////////////////////

// Include Python.h first!
#include <Python.h>


#include "OSCARSTH.h"
#include "OSCARSTH_Python.h"

#include "TOSCARSSR.h"
#include "TVector2D.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <stdexcept>






static TVector2D OSCARSTH_ListAsTVector2D (PyObject* List)
{
  TVector2D V;
  if (PyList_Size(List) == 2) {
    Py_INCREF(List);
    V.SetXY(PyFloat_AsDouble(PyList_GetItem(List, 0)),
             PyFloat_AsDouble(PyList_GetItem(List, 1)));
    Py_DECREF(List);
  } else {
    throw std::length_error("number of elements not 2");
  }

  // Return the python list
  return V;
}




static void OSCARSTH_dealloc(OSCARSTHObject* self)
{
  // Python needs to know how to deallocate things in the struct

  delete self->obj;
  //self->ob_type->tp_free((PyObject*) self);
  Py_TYPE(self)->tp_free((PyObject*) self);
}




static PyObject* OSCARSTH_new (PyTypeObject* type, PyObject* args, PyObject* kwds)
{
  // Python needs to know how to create things in this struct

  OSCARSTHObject* self = (OSCARSTHObject*) type->tp_alloc(type, 0);
  if (self != NULL) {

    // Create the new object for self
    self->obj = new OSCARSTH();
  }

  // Return myself
  return (PyObject*) self;
}





static int th_init(OSCARSTHObject* self, PyObject* args, PyObject* kwds)
{
  return 0;
}
















const char* DOC_OSCARSTH_UndulatorK = "Get the undulator K parameter vaule";
static PyObject* OSCARSTH_UndulatorK (OSCARSTHObject* self, PyObject* args, PyObject* keywds)
{
  // Return the undulator K parameter given peak bfield and period

  // Require 2 arguments
  double BFieldMax = 0;
  double Period = 0;

  // Input variables and parsing
  static char *kwlist[] = {"bfield", "period", NULL};
  if (!PyArg_ParseTupleAndKeywords(args, keywds, "dd", kwlist, &BFieldMax, &Period)) {
    return NULL;
  }


  // Return the internal OSCARSTH number constant pi
  return Py_BuildValue("d", self->obj->UndulatorK(BFieldMax, Period));
}





const char* DOC_OSCARSTH_DipoleSpectrum= "Get the spectrum from ideal dipole field";
static PyObject* OSCARSTH_DipoleSpectrum(OSCARSTHObject* self, PyObject* args, PyObject* keywds)
{
  // Return a list of points corresponding to the flux in a given energy range for a given vertical angle.
  // This approximation assumes that the particle beam is perpendicular to the magnetic field

  // Require 2 arguments
  double BField = 0;
  double BeamEnergy = 0;
  double Angle = 0;
  PyObject* List_EnergyRange = PyList_New(0);

  // Input variables and parsing
  static char *kwlist[] = {"bfield", "beam_energy_GeV", "angle", "energy_range_eV", NULL};
  if (!PyArg_ParseTupleAndKeywords(args, keywds, "dddO", kwlist, &BField, &BeamEnergy, &Angle, &List_EnergyRange)) {
    return NULL;
  }

  // Check that beam energy makes sense
  if (BeamEnergy <= 0) {
    PyErr_SetString(PyExc_ValueError, "'beam_energy_GeV' must be > 0");
    return NULL;
  }

  TVector2D const EnergyRange = OSCARSTH_ListAsTVector2D(List_EnergyRange);
  if (EnergyRange[0] >= EnergyRange[1] || EnergyRange[0] <= 1 || EnergyRange[1] <= 0) {
    PyErr_SetString(PyExc_ValueError, "'energy_range_eV' is incorrect");
    return NULL;
  }

  // Calculate the spectrum
  self->obj->DipoleSpectrum(BField, BeamEnergy, Angle, EnergyRange);

  // Must return python object None in a special way
  Py_INCREF(Py_None);
  return Py_None;
}



































static PyMethodDef OSCARSTH_methods[] = {
  // We must tell python about the function we allow access as well as give them nice
  // python names, and tell python the method of input parameters.

  {"undulator_K",                                (PyCFunction) OSCARSTH_UndulatorK,                              METH_VARARGS | METH_KEYWORDS,                  DOC_OSCARSTH_UndulatorK},

  {"dipole_spectrum",                            (PyCFunction) OSCARSTH_DipoleSpectrum,                          METH_VARARGS | METH_KEYWORDS,                  DOC_OSCARSTH_DipoleSpectrum},

  {NULL}  /* Sentinel */
};


#if PY_MAJOR_VERSION >= 3
PyMODINIT_FUNC PyInit_th(void);
#else
PyMODINIT_FUNC initth(OSCARSTHObject* self, PyObject* args, PyObject* kwds);
#endif





#if PY_MAJOR_VERSION >= 3
static PyTypeObject OSCARSTHType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "th",            /* tp_name */
  sizeof(OSCARSTHObject),       /* tp_basicsize */
  0,                          /* tp_itemsize */
  (destructor)OSCARSTH_dealloc, /* tp_dealloc */
  0,                          /* tp_print */
  0,                          /* tp_getattr */
  0,                          /* tp_setattr */
  0,                          /* tp_reserved */
  0,                          /* tp_repr */
  0,                          /* tp_as_number */
  0,                          /* tp_as_sequence */
  0,                          /* tp_as_mapping */
  0,                          /* tp_hash  */
  0,                          /* tp_call */
  0,                          /* tp_str */
  0,                          /* tp_getattro */
  0,                          /* tp_setattro */
  0,                          /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT |
  Py_TPFLAGS_BASETYPE,        /* tp_flags */
  "oscars th class",           /* tp_doc */
  0,                          /* tp_traverse */
  0,                          /* tp_clear */
  0,                          /* tp_richcompare */
  0,                          /* tp_weaklistoffset */
  0,                          /* tp_iter */
  0,                          /* tp_iternext */
  OSCARSTH_methods,             /* tp_methods */
  0,                          /* tp_members */
  0,                          /* tp_getset */
  0,                          /* tp_base */
  0,                          /* tp_dict */
  0,                          /* tp_descr_get */
  0,                          /* tp_descr_set */
  0,                          /* tp_dictoffset */
  0,      /* tp_init */
  0,                          /* tp_alloc */
  OSCARSTH_new,                 /* tp_new */
};
#else
static PyTypeObject OSCARSTHType = {
  // The python object.  Fully defined elsewhere.  only put here what you need,
  // otherwise default values

  PyObject_HEAD_INIT(NULL)
  0,                                        /* ob_size */
  "th",                                 /* tp_name */
  sizeof(OSCARSTHObject),                         /* tp_basicsize */
  0,                                        /* tp_itemsize */
  (destructor) OSCARSTH_dealloc,                 /* tp_dealloc */
  0,                                        /* tp_print */
  0,                                        /* tp_getattr */
  0,                                        /* tp_setattr */
  0,                                        /* tp_compare */
  0,                                        /* tp_repr */
  0,                                        /* tp_as_number */
  0,                                        /* tp_as_sequence */
  0,                                        /* tp_as_mapping */
  0,                                        /* tp_hash */
  0,                                        /* tp_call */
  0,                                        /* tp_str */
  0,                                        /* tp_getattro */
  0,                                        /* tp_setattro */
  0,                                        /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
  "oscars th class",                              /* tp_doc */
  0,                                        /* tp_traverse */
  0,                                        /* tp_clear */
  0,                                        /* tp_richcompare */
  0,                                        /* tp_weaklistoffset */
  0,                                        /* tp_iter */
  0,                                        /* tp_iternext */
  OSCARSTH_methods,                             /* tp_methods */
  0,                                        /* tp_members */
  0,                                        /* tp_getset */
  0,                                        /* tp_base */
  0,                                        /* tp_dict */
  0,                                        /* tp_descr_get */
  0,                                        /* tp_descr_set */
  0,                                        /* tp_dictoffset */
  0,                                        /* tp_init */
  0,                                        /* tp_alloc */
  OSCARSTH_new,                                  /* tp_new */
};
#endif




static PyMethodDef module_methods[] = {
  // I do not need
  {NULL}  /* Sentinel */
};


#if PY_MAJOR_VERSION >= 3
static PyModuleDef OSCARSTHmodule = {
  PyModuleDef_HEAD_INIT,
  "th",
  "OSCARSTH module extension.",
  -1,
  NULL, NULL, NULL, NULL, NULL
};
#endif


#if PY_MAJOR_VERSION >= 3
PyMODINIT_FUNC PyInit_th(void)
#else
PyMODINIT_FUNC initth(OSCARSTHObject* self, PyObject* args, PyObject* kwds)
#endif
{
  if (PyType_Ready(&OSCARSTHType) < 0) {
#if PY_MAJOR_VERSION >= 3
    return NULL;
#else
    return;
#endif
  }

#if PY_MAJOR_VERSION >= 3
  PyObject* m = PyModule_Create(&OSCARSTHmodule);
#else
  PyObject *m = Py_InitModule("oscars.th", OSCARSTH_methods);
#endif
  if (m == NULL) {
#if PY_MAJOR_VERSION >= 3
    return NULL;
#else
    return;
#endif
  }

  Py_INCREF(&OSCARSTHType);
  PyModule_AddObject(m, "th", (PyObject *)&OSCARSTHType);

  // Print copyright notice
  PyObject* sys = PyImport_ImportModule( "sys");
  PyObject* s_out = PyObject_GetAttrString(sys, "stdout");
  //std::string Message = "OSCARS v" + OSCARSTH::GetVersionString() + " - Open Source Code for Advanced Radiation Simulation\nBrookhaven National Laboratory, Upton NY, USA\nhttp://oscars.bnl.gov\noscars@bnl.gov\n";
  std::string Message = "OSCARS vTH - Open Source Code for Advanced Radiation Simulation\nBrookhaven National Laboratory, Upton NY, USA\nhttp://oscars.bnl.gov\noscars@bnl.gov\n";
  PyObject_CallMethod(s_out, "write", "s", Message.c_str());

#if PY_MAJOR_VERSION >= 3
  return m;
#else
  return;
#endif
}



