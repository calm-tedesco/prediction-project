#include "ecm.hpp"
#include "ema.hpp"
#include "deming.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <limits>

///////////////////////////////////////////////////////////////////
//////////////   PUBLIC METHODS ///////////////////////////////////
///////////////////////////////////////////////////////////////////

Deming::Deming( std::ifstream & infile ){

  IntroduceDatos( infile, (*this).observaciones.fecha, (*this).predicciones.fecha,
	(*this).observaciones.v_viento, (*this).observaciones.produccion, (*this).predicciones.v_viento);

  Ajuste();

  Predice();

}

void Deming::ImprimeResultado() const
{
  std::vector<Fecha> fechas_ordenar = (*this).predicciones.fecha;
  std::vector<double> predicciones_ordenar = (*this).predicciones.produccion;

  Ordena( fechas_ordenar, predicciones_ordenar );

  std::vector<Fecha>::const_iterator it_fecha = fechas_ordenar.begin();
  std::vector<Fecha>::iterator const IT_FECHA_END = fechas_ordenar.end();
  std::vector<double>::const_iterator produccion = predicciones_ordenar.begin();

  std::cout << std::fixed << std::setprecision(2)
            << (*this).slope_ord.first << ' ' << (*this).slope_ord.second
            << std::endl;

  for (int i = 0; i < 12; i++)
  {
    if ( !(*this).ajuste[i].fecha.empty() )
    {
      std::cout << "Mes" << i + 1 << ' '
                << (*this).ajuste[i].errores.first << ' '
                << (*this).ajuste[i].errores.second
                << std::endl;
    }
  }

  std::cout.precision(0);

  for (; it_fecha != IT_FECHA_END; it_fecha++, produccion++)
  {
    std::cout.fill('0');
    std::cout << (*it_fecha).anho << '-'
              << std::setw(2) << (*it_fecha).mes << '-'
              << std::setw(2) << (*it_fecha).dia << ' '
              << std::setw(2) << (*it_fecha).tiempo.hora << ':'
              << std::setw(2) << (*it_fecha).tiempo.minutos << ' '
              << *produccion << std::endl;
  }
}

///////////////////////////////////////////////////////////////////
//////////////   PRIVATE METHODS //////////////////////////////////
///////////////////////////////////////////////////////////////////

void Deming::IntroduceDatos (std::ifstream & infile, std::vector<Fecha>& fechas_observaciones,
  std::vector<Fecha>& fechas_predicciones, std::vector<double>& observaciones_v_viento,
  std::vector<double>& observaciones_produccion, std::vector<double>& predicciones_v_viento)
{
	std::string anhomesdia, horaminutos;
	double produccion, v_viento;
	Fecha fecha_temp;
  std::stringstream iss;

  infile.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
  while ( infile.peek() != 'p' && infile >> anhomesdia >> horaminutos >> produccion >> v_viento )
    {
			fecha_temp.anho = std::stoi(anhomesdia.substr(0,4));
			fecha_temp.mes = std::stoi(anhomesdia.substr(5,2));
			fecha_temp.dia = std::stoi(anhomesdia.substr(8,2));
			fecha_temp.tiempo.hora = std::stoi(horaminutos.substr(0,2));
			fecha_temp.tiempo.minutos = std::stoi(horaminutos.substr(3,2));

			fechas_observaciones.push_back(fecha_temp);
			observaciones_produccion.push_back(produccion);
			observaciones_v_viento.push_back(v_viento);

      infile.ignore( 1, '\n' );
    }

    infile.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );

    while ( infile >> anhomesdia >> horaminutos >> v_viento )
      {
				fecha_temp.anho = std::stoi(anhomesdia.substr(0,4));
				fecha_temp.mes = std::stoi(anhomesdia.substr(5,2));
				fecha_temp.dia = std::stoi(anhomesdia.substr(8,2));
				fecha_temp.tiempo.hora = std::stoi(horaminutos.substr(0,2));
				fecha_temp.tiempo.minutos = std::stoi(horaminutos.substr(3,2));

				fechas_predicciones.push_back(fecha_temp);
				predicciones_v_viento.push_back(v_viento);
      }
}

void Deming::Ordena (std::vector<Fecha>& fechas, std::vector<double>& ordenado) const
{
  int i = 0;

  //usamos expresiones lambda para pasar la condición que ordena sin evaluar
  burbuja (fechas, ordenado, i,
    []( std::vector<Fecha>& fechas, int i)
    { return fechas[i].mes < fechas[i-1].mes; } );

  burbuja (fechas, ordenado, i,
    []( std::vector<Fecha>& fechas, int i)
    { return fechas[i].dia < fechas[i-1].dia && fechas[i].mes == fechas[i-1].mes; } );

  burbuja (fechas, ordenado, i,
    []( std::vector<Fecha>& fechas, int i)
    { return fechas[i].tiempo.hora < fechas[i-1].tiempo.hora &&
      fechas[i].dia == fechas[i-1].dia && fechas[i].mes == fechas[i-1].mes; } );
}

void Deming::Ajuste()
{
  const int DELTA = 1;
  const int TOTAL_ELEM = (*this).observaciones.v_viento.size();

  double media_x  = 0,
         media_y  = 0,
         s_xx     = 0,
         s_xy     = 0,
         s_yy     = 0;

  std::vector<double>::const_iterator v_iter, p_iter;
  std::vector<double>::iterator const OBS_V_VIENTO_END = (*this).observaciones.v_viento.end();

  v_iter = (*this).observaciones.v_viento.begin();
  p_iter = (*this).observaciones.produccion.begin();

  //CÁLCULO DE LAS MEDIAS
  for (; v_iter != OBS_V_VIENTO_END; v_iter++, p_iter++)
  {
    media_x += *v_iter;
    media_y += *p_iter;
	}

    media_x /= TOTAL_ELEM;
    media_y /= TOTAL_ELEM;

  //CÁLCULO DE LAS MEDIAS DE LOS CUADRADOS

  v_iter = (*this).observaciones.v_viento.begin();
  p_iter = (*this).observaciones.produccion.begin();

  for (; v_iter != OBS_V_VIENTO_END; v_iter++, p_iter++)
  {
    s_xx += pow( *v_iter - media_x , 2 );
    s_yy += pow( *p_iter - media_y , 2 );
    s_xy += ( *v_iter - media_x ) * ( *p_iter - media_y );
  }

    s_xx /= TOTAL_ELEM - 1;
    s_yy /= TOTAL_ELEM - 1;
    s_xy /= TOTAL_ELEM - 1;

  //CÁLCULO DE LA PENDIENTE
  (*this).slope_ord.first = s_yy - DELTA * s_xx;
  (*this).slope_ord.first += sqrt ( pow( (*this).slope_ord.first , 2 ) + 4 * DELTA * pow( s_xy , 2 ) );
  (*this).slope_ord.first /= 2 * s_xy;

  //CÁLCULO DE LA ORDENADA EN EL ORIGEN
  (*this).slope_ord.second = media_y - (*this).slope_ord.first * media_x;

  //CÁLCULO DE LOS VALORES DEL AJUSTE ORDENADOS
  std::vector<double> v_viento_entrada = (*this).observaciones.v_viento;
  std::vector<double> produccion_entrada = (*this).observaciones.produccion;
  std::vector<Fecha> fechas = (*this).observaciones.fecha;

  Ordena ( fechas, v_viento_entrada );
  fechas = (*this).observaciones.fecha;
  Ordena ( fechas, produccion_entrada );

  int mes;
  double valor;

  v_iter = v_viento_entrada.begin();
  p_iter = produccion_entrada.begin();
  std::vector<Fecha>::const_iterator f_iter = fechas.begin();
  std::vector<double>::iterator const ENTRADA_V_VIENTO_END = v_viento_entrada.end();

  for(; v_iter != ENTRADA_V_VIENTO_END; v_iter++, f_iter++, p_iter++)
  {
    mes = (*f_iter).mes - 1;
    (*this).ajuste[ mes ].fecha.push_back(*f_iter);
    (*this).ajuste[ mes ].produccion_mes.push_back(*p_iter);

    valor = (*this).slope_ord.second + (*this).slope_ord.first * *v_iter;
    if (valor < 0) valor = 0;
    (*this).ajuste[ mes ].deming_produccion.push_back(valor);
  }

  //CÁLCULO DE ERRORES POR MES
  std::vector<double>::iterator observaciones, predicciones, observaciones_fin;
  double medio_absoluto, cuadratico_medio;

  for (int i = 0; i < 12; i++)
  {
    if ( !(*this).ajuste[i].produccion_mes.empty() )
    {
      observaciones = (*this).ajuste[i].produccion_mes.begin();
      predicciones = (*this).ajuste[i].deming_produccion.begin();
      observaciones_fin = (*this).ajuste[i].produccion_mes.end();

      cuadratico_medio = ecm ( observaciones, observaciones_fin, predicciones );

      observaciones = (*this).ajuste[i].produccion_mes.begin();
      predicciones = (*this).ajuste[i].deming_produccion.begin();
      observaciones_fin = (*this).ajuste[i].produccion_mes.end();

      medio_absoluto = ema ( observaciones, observaciones_fin, predicciones );

      (*this).ajuste[i].errores.first = medio_absoluto;
      (*this).ajuste[i].errores.second = cuadratico_medio;
    }
  }
}

void Deming::Predice()
{
  std::vector<double>::const_iterator v_iter = (*this).predicciones.v_viento.begin();
  std::vector<double>::iterator const ENTRADA_V_VIENTO_END = (*this).predicciones.v_viento.end();
  int valor;

  for(; v_iter != ENTRADA_V_VIENTO_END; v_iter++)
  {
    valor = (*this).slope_ord.second + (*this).slope_ord.first * *v_iter;
    if (valor < 0) valor = 0;
    (*this).predicciones.produccion.push_back(valor);
  }
}
