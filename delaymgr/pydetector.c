#include <Python.h>
#include "detector.h"

#if PY_MAJOR_VERSION >= 3
#define PY3
#endif

static PyObject *DeteError;
const char DetectorDoc[] = "hello.";


static void detector_free(PyObject* deteptr)
{
    DetectorInst* handle = PyCapsule_GetPointer(deteptr, "DetectorInstPtr");
    DetectorFree(handle);
}

static PyObject* detector_create(PyObject *self, PyObject *args)
{
  DetectorInst *handle = DetectorCreate();
  PyObject *deteptr = PyCapsule_New(handle, "DetectorInstPtr", detector_free);
  return Py_BuildValue("O", deteptr);
}

static PyObject* detector_set_audio_length(PyObject *self, PyObject *args)
{
    PyObject *deteptr;
    int32_t length;
    if (!PyArg_ParseTuple(args, "Oi", &deteptr, &length)) {
        return NULL;
    }
    if (length < 10 || length % 10 != 0) {

        PyErr_Format(PyExc_ValueError, "%d is an invalid length", length);
        return NULL;

    }

    DetectorSetAudioLength(PyCapsule_GetPointer(deteptr, "DetectorInstPtr"), length);

    Py_RETURN_NONE;
}

static PyObject* detector_tick(PyObject *self, PyObject *args)
{
    PyObject *deteptr;
    int32_t tick;

    if (!PyArg_ParseTuple(args, "Oi", &deteptr, &tick)) {
        return NULL;
    }
    if (tick <= 0) {

        PyErr_Format(PyExc_ValueError, "%d is an invalid tick", tick);
        return NULL;
    }

    DetectorTickTimes(PyCapsule_GetPointer(deteptr, "DetectorInstPtr"), tick);

    Py_RETURN_NONE;
}

static PyObject* detector_update(PyObject *self, PyObject *args)
{
    PyObject *deteptr;
  
    uint16_t seq;
    uint32_t timestamp;
    int32_t hz;

    if (!PyArg_ParseTuple(args, "OHIi", &deteptr, &seq, &timestamp, &hz)) {
        return NULL;
    }

    DetectorUpdate(PyCapsule_GetPointer(deteptr, "DetectorInstPtr"), seq, timestamp, hz);

    Py_RETURN_NONE;
}

static PyObject* detector_target_level(PyObject *self, PyObject *args)
{
    PyObject *deteptr;

    if (!PyArg_ParseTuple(args, "O", &deteptr)) {

        return NULL;
    }

    int32_t target = DetectorTargetLevel(PyCapsule_GetPointer(deteptr, "DetectorInstPtr"));

    return Py_BuildValue("i", target);
}

static PyObject* detector_get_histogram(PyObject *self, PyObject *args)
{
    PyObject *deteptr;

    if (!PyArg_ParseTuple(args, "O", &deteptr)) {

        return NULL;
    }

    static const int32_t cszie = 1024;
    char content[1024] = {0};
    int32_t length= DetectorGetHistogram(PyCapsule_GetPointer(deteptr, "DetectorInstPtr"), content, cszie);
    if (length <= 0) {
        return NULL;
    }

    PyObject *histogram;
    #ifdef PY3
        histogram = PyUnicode_FromString(content);
    #else
        histogram= PyBytes_FromString(content);
    #endif

    return histogram;
}

static PyMethodDef DetectorMethods[] = {
    {"create",  detector_create, METH_NOARGS, "Create a detector."},
    {"set_audio_length",  detector_set_audio_length, METH_VARARGS, "Set audio length."},
    {"tick",  detector_tick, METH_VARARGS, "time tick."},
    {"update", detector_update, METH_VARARGS, "detector update."},
    {"target_level", detector_target_level, METH_VARARGS, "target level."},
    {"histogram", detector_get_histogram, METH_VARARGS, "detector histogram."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

#ifdef PY3
static struct PyModuleDef DetectorNetEqModule = {
   PyModuleDef_HEAD_INIT,
   "_pydetector",   /* name of module */
   DetectorDoc, /* module documentation, may be NULL */
   -1,       /* size of per-interpreter state of the module,
                or -1 if the module keeps state in global variables. */
   DetectorMethods
};

#define INITERROR return NULL

PyMODINIT_FUNC
PyInit__pydetector(void)

#else

#define INITERROR return

void
init_pydetector(void)

#endif

{
#ifdef PY3
    PyObject *module = PyModule_Create(&DetectorNetEqModule);
#else
    PyObject *module = Py_InitModule3("_pydetector", DetectorMethods, DetectorDoc);
#endif
    if (module == NULL) {
      INITERROR;
    }

    DeteError = PyErr_NewException("detector.Error", NULL, NULL);
    Py_INCREF(DeteError);
    PyModule_AddObject(module, "Error", DeteError);

#ifdef PY3
    return module;
#endif
}
