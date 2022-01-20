#ifndef DEMING_HPP_
#define DEMING_HPP_

/** @brief Clase Deming usada para almacenar los datos y realizar el ajuste.

    @author Alberto Sánchez Romero
    @date Septiembre 2019
    */

#include <vector>
#include <fstream>
#include <functional>

class Deming
{
  struct Time
  {
    int hora;
    int minutos;
  };

  struct Fecha
  {
    int anho;
    int mes;
    int dia;
    Time tiempo;
  };

private:

  struct Observaciones
  {
    std::vector<Fecha> fecha;
    std::vector<double> v_viento;
    std::vector<double> produccion;
  } observaciones; /**<struct para las observaciones*/

  struct Predicciones
  {
    std::vector<Fecha> fecha;
    std::vector<double> v_viento;
    std::vector<double> produccion;
  } predicciones; /**<struct para las predicciones*/

  struct Mes
  {
    std::vector<Fecha> fecha;
    std::vector<double> deming_produccion;
    std::vector<double> produccion_mes;
    std::pair< double, double > errores; //first -> ema
  } ajuste[12]; /**<struct para cada mes con los datos de entrada y del ajuste
  para calcular los errores por mes*/

  std::pair<double,double> slope_ord; /**<pair donde first es la pendiente
  y second la ordenada*/

  /**
     @brief Introduce los datos de infile en los miembros de la clase.

     @param infile Input stream con los datos.
     @param fechas_observaciones Vector de struct Fecha para las observaciones.
     @param fechas_predicciones Vector de struct Fecha para las predicciones.
     @param observaciones_v_viento Vector de double con las observaciones
     de módulo del viento.
     @param observaciones_produccion Vector de double para las observaciones
     de cantidad de energía.
     @param predicciones_v_viento Vector de double para las predicciones de
     módulo de viento de entrada.
  */
  void IntroduceDatos (std::ifstream & infile, std::vector<Fecha>& fechas_observaciones,
    std::vector<Fecha>& fechas_predicciones, std::vector<double>& observaciones_v_viento,
    std::vector<double>& observaciones_produccion, std::vector<double>& predicciones_v_viento);

    /**
       @brief Functor que implementa el algoritmo de burbuja.

       @param fechas vector de struct Fecha para ordenar
       @param ordenado vector de double a ordenar
       @param i int pasado por referencia para evaluar expr
       @param expr Expresión que da un valor booleano respecto a fechas e i.
       @pre fechas y ordenado tienen el mismo tamaño
    */
  template<class Expresion> void burbuja (std::vector<Fecha>& fechas,
    std::vector<double>& ordenado, int & i, Expresion expr) const
  {
    bool cambio = true;
    const int POSICIONES = fechas.size();
    Fecha intercambia_fecha;
    double intercambia_ordenado;

    for (int izda = 0; izda < POSICIONES && cambio; izda++)
    {
      cambio = false;

      for ( i = POSICIONES - 1; i > izda; i--)
      {
        if ( expr( fechas, i ) )
        {
          cambio = true;

          intercambia_fecha = fechas[i];
          fechas[i] = fechas[ i - 1 ];
          fechas[ i - 1 ] = intercambia_fecha;

          intercambia_ordenado = ordenado[i];
          ordenado[i] = ordenado[ i - 1 ];
          ordenado[ i - 1 ] = intercambia_ordenado;
        }
      }
    }
  }

  /**
    @brief Ordena un vector de valores double b respecto a un vector de struct
    de Fecha b.
    @pre a y b tienen el mismo tamaño
  */
  void Ordena (std::vector<Fecha>& a, std::vector<double>& b) const;

  /**
    @brief Realiza el ajuste por regresión de Deming.

    Se asume que los valores de producción no pueden ser menores que 0.
  */
  void Ajuste();

  /**
    @brief Predice nuevos resultados a
    partir del ajuste con las predicciones de entrada.
  */
  void Predice();

public:

  /**
     @brief Constructor de la clase.

     @param a Input stream con los datos.
  */
  Deming( std::ifstream & a );

  /**
     @brief Imprime los resultados por pantalla.
  */
  void ImprimeResultado() const;

};

#endif /* DEMING_HPP_ */
