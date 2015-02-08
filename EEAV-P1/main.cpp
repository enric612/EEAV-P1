/*****************************************************************************/
/*                                                                           */
/*                                 EEAV                                      */
/*                     Bloque de Procesado de Imagen                         */
/*                              Práctica 1                                   */
/*                                                                           */
/*****************************************************************************/
/*                             Ximo Cerdà                                    */
/*                      Valencia, Febrero de 2014                            */
/*****************************************************************************/

// Programa principal

// Includes
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <FreeImage.h>
#include <limits.h>

using namespace std ; 

// variables principales
#define MAINPROGRAM 
#include "variables.h" 
#include "readfile.h" // prototipos de readfile.cpp  


// Función que muestra texto por pantalla
void printHelp(string fichero) {
  std::cout << " === RASTER, Versión 1.0 === \n\n" 
    << "Programado por Ximo Cerdà\n" 
    << "Febrero de 2014\n"
	<< "Procesando...\n\n"
	<< "Realizando la lectura del fichero " << fichero << "...\n\n";
}


// Función principal
int main(int argc, char* argv[]) {
  // Sólo para ver, quitar al final
  char c;

  // Variables afegides
  char dd;
  int t, num_p, d_x, d_y, grosor_int;

  if (argc < 2) {
    cerr << "Por favor, indica el fichero que debe renderizarse\n"; 
    exit(-1); 
  }
  
  // Preparamos el terreno: inicializamos imagen y leemos fichero
  FreeImage_Initialise();
  printHelp(argv[1]);
  readfile(argv[1]) ; 

  // Creamos rejilla de pixels, según las dimensiones globales
  unsigned char *pixels = new unsigned char[3 * numPix];	
  // Pero trabajaremos en flotante
  float color[3];

  // Primero inicializamos al color de fondo
  for (int i = 0; i < h; i++) {
	  for (int j = 0; j < w; j++) {

		  // Inicilizo los pixels a un valor conocido (negro)
		  for (int c = 0; c < 3; c++)
			color[c] = background[c];

		  // Se escribe en memoria de video
		  pixels[i * w * 3 + j * 3 + 0] = (unsigned char)(255.0 * color[0]);
		  pixels[i * w * 3 + j * 3 + 1] = (unsigned char)(255.0 * color[1]);
		  pixels[i * w * 3 + j * 3 + 2] = (unsigned char)(255.0 * color[2]);		
	  }
  }

  // Ahora recorremos el vector de objetos
  for (int o = 0; o < numobjects; o++) {
	  if (objects[o].type == point) {

		  // Coordenadas de pantalla
		  int x_pant = (int) (w * (objects[o].punto0[0]- x_min) / ( x_max - x_min)) ;
		  int y_pant = (int) (h * (objects[o].punto0[1]- y_min) / ( y_max - y_min)) ;
		  
		  // Se escribe en memoria de video. Pero antes comprobamos que está dentro del margen de representación

		  if ((x_pant >= 0) && (y_pant >=0) && (x_pant < w) && (y_pant < h) ) {
				pixels[y_pant * w * 3 + x_pant * 3 + 0] = (unsigned char)(255.0 * objects[o].color[0]);
				pixels[y_pant * w * 3 + x_pant * 3 + 1] = (unsigned char)(255.0 * objects[o].color[1]);
				pixels[y_pant * w * 3 + x_pant * 3 + 2] = (unsigned char)(255.0 * objects[o].color[2]);		
		  

		 /*
		  *  Punt Cuadrat
		  */

		  if (objects[o].grosor > 1){

			  dd = 'r';
			  num_p = 1;
			  t = 1;
			  d_x = 0;
			  d_y = 0;
			  grosor_int = floor(objects[o].grosor);
			  for (int k = 0; k <= grosor_int - 2; k++) {
				  for (int j = 0; j <= 1; j++) {

					  while (t > 0) {

						  switch (dd) {
						  case 'r':
							  d_x = d_x + 1;
							  d_y = d_y + 0;
							  break;
						  case 'l':
							  d_x = d_x - 1;
							  d_y = d_y + 0;
							  break;
						  case 'd':
							  d_x = d_x + 0;
							  d_y = d_y - 1;
							  break;
						  case 'u':
							  d_x = d_x + 0;
							  d_y = d_y + 1;
							  break;
						  default:
							  d_x = 0;
							  d_y = 0;
							  break;
						  } // Switch 

						  t = t - 1;
						  pixels[(y_pant + d_y) * w * 3 + (x_pant + d_x) * 3 + 0] = (unsigned char)(255.0 * objects[o].color[0]);
						  pixels[(y_pant + d_y) * w * 3 + (x_pant + d_x) * 3 + 1] = (unsigned char)(255.0 * objects[o].color[1]);
						  pixels[(y_pant + d_y) * w * 3 + (x_pant + d_x) * 3 + 2] = (unsigned char)(255.0 * objects[o].color[2]);
					  } // while 

					  // Cambio de dirección
					  if (dd == 'r'){
						  dd = 'u';
					  }
					  else if (dd == 'l') {
						  dd = 'd';
					  }
					  else if (dd == 'u') {
						  dd = 'l';
					  }
					  else if (dd == 'd') {
						  dd = 'r';
					  }	// if 
					  t = num_p;
				  }// for
				  num_p = num_p + 1;
				  t = num_p;
			  } // for 		
			  for (int j = 0; j < grosor_int - 1; j++) {

				  switch (dd) {
				  case 'r':
					  d_x = d_x + 1;
					  d_y = d_y + 0;
					  break;
				  case 'l':
					  d_x = d_x - 1;
					  d_y = d_y + 0;
					  break;
				  case 'd':
					  d_x = d_x + 0;
					  d_y = d_y - 1;
					  break;
				  case 'u':
					  d_x = d_x + 0;
					  d_y = d_y + 1;
					  break;
				  default:
					  d_x = 0;
					  d_y = 0;
					  break;
				  } // Switch 

				  pixels[(y_pant + d_y) * w * 3 + (x_pant + d_x) * 3 + 0] = (unsigned char)(255.0 * objects[o].color[0]);
				  pixels[(y_pant + d_y) * w * 3 + (x_pant + d_x) * 3 + 1] = (unsigned char)(255.0 * objects[o].color[1]);
				  pixels[(y_pant + d_y) * w * 3 + (x_pant + d_x) * 3 + 2] = (unsigned char)(255.0 * objects[o].color[2]);
			  } // for 


		  }	//if 

		}
	  }
  }

  // Y ya hemos terminado!!
  // Volcamos en la imagen
  FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);

  // Una vez terminado, informamos que hemos acabado y escribimos en fichero
  std::cout << "Proceso terminado. Guardando fichero: " << fileName.str() << "\n";
  FreeImage_Save(FIF_PNG, img, fileName.str().c_str(), 0);

  // Liberamos memoria (limpio que es uno)
  delete pixels;

  // Cerramos la librería y salimos
  FreeImage_DeInitialise();

  // Sólo para ver, quitar al final
  std::cin >> c;

  return 0;
}
