#ifndef ECM_HPP_
#define ECM_HPP_

#include <cmath>

 /**
		@brief Constructor de la clase.

		Calcula el error cuadrático medio en porcentaje entre una colección de observaciones y de predicciones.

		@return 0 si las observaciones suman 0. El error cuadrático medio en porcentaje en caso contrario.
		@param observaciones iterador tipo forward para indicar el comienzo de las observaciones.
		@param observaciones iterador tipo forward para indicar el fin de las observaciones.
		@param observaciones iterador tipo forward para indicar el comienzo de las predicciones.
 */
template<typename forward_iterator_observaciones,
		typename forward_iterator_predicciones>
double ecm(forward_iterator_observaciones observaciones,
		forward_iterator_observaciones observaciones_fin,
		forward_iterator_predicciones predicciones) {
	double observacion_total = 0.;
  double suma_cuadrados = 0.;
  double raiz = 0.;
  int total_predicciones = 0;

	for (; observaciones != observaciones_fin;
			++observaciones, ++predicciones) {
		suma_cuadrados += std::pow( (*observaciones - *predicciones), 2);
		observacion_total += *observaciones;
    total_predicciones++;
	}

	if (observacion_total < 1e-7) {
		return 0.;
	}

  raiz = sqrt( suma_cuadrados / total_predicciones );

	return (raiz / observacion_total) * total_predicciones * 100.;
}

#endif /* ECM_HPP_ */
