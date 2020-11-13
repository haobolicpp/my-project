#pragma once
#include <vector>
#include <string>
#include "Python.h"
#include "frameobject.h"

class CCallPythonX
{
public:
	CCallPythonX();
	~CCallPythonX();

	bool init_python();
	bool test(std::vector<double> y0, std::vector<double> params, double t0, double T, int N,
		std::vector<std::vector<double>> &ret);
	std::string PythonException();
private:
	PyObject *m_pFunc;
};

