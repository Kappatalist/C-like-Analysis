#pragma once

public ref class Complex
{
private:
	int ITERATIONS;
	System::Random^ rng;
	System::Web::UI::DataVisualization::Charting::Chart^ stat;
public:
	value struct ComplexTelemetry
	{
		double runtime;
		float mem_usage;
		float cpu_usage;
	};

	double decirand();

	void RunComplex();

	Complex(int iter);
};
