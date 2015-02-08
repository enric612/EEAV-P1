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

	  if (objects[o].type == line){


		  int x_pant0 = (int)(w * (objects[o].punto0[0] - x_min) / (x_max - x_min));
		  int y_pant0 = (int)(h * (objects[o].punto0[1] - y_min) / (y_max - y_min));

		  int x_pant1 = (int)(w * (objects[o].punto1[0] - x_min) / (x_max - x_min));
		  int y_pant1 = (int)(h * (objects[o].punto1[1] - y_min) / (y_max - y_min));

		  grosor_int = floor(objects[o].grosor); //Grosor de la linea

		  int x_pant0_original = x_pant0;
		  if (x_pant0_original < 0) x_pant0_original = 0;
		  if (x_pant0_original > w) x_pant0_original = w;
		  int y_pant0_original = y_pant0;
		  if (y_pant0_original < 0) y_pant0_original = 0;
		  if (y_pant0_original > h) y_pant0_original = h;
		  int x_pant1_original = x_pant1;
		  if (x_pant1_original < 0) x_pant1_original = 0;
		  if (x_pant1_original > w) x_pant1_original = w;
		  int y_pant1_original = y_pant1;
		  if (y_pant1_original < 0) y_pant1_original = 0;
		  if (y_pant1_original > h) y_pant1_original = h;

		  for (int icont = 1; icont <= grosor_int; icont++){

			  // Ens assegurem que un dels dos punts de la línea estiga dintre dels llimits
			  if (((x_pant0 >= 0) && (y_pant0 >= 0) && (x_pant0 < w) && (y_pant0 < h)) || ((x_pant1 >= 0) && (y_pant1 >= 0) && (x_pant1 < w) && (y_pant1 < h))) {

				  // Umbralitzem els punts que ixen fora dels llimits

				  if (x_pant0 < 0) x_pant0 = 0;
				  if (x_pant0 > w) x_pant0 = w;
				  if (y_pant0 < 0) y_pant0 = 0;
				  if (y_pant0 > h) y_pant0 = h;
				  if (x_pant1 < 0) x_pant1 = 0;
				  if (x_pant1 > w) x_pant1 = w;
				  if (y_pant1 < 0) y_pant1 = 0;
				  if (y_pant1 > h) y_pant1 = h;


				  int x_dif = abs(x_pant1 - x_pant0); // diferencia entre les coordenades x dels dos punts

				  int y_dif = abs(y_pant1 - y_pant0); // diferencica entre les coordenades y dels dos punts 

				  int Err;
				  int step_x = 1;
				  int step_y = 1;
				  d_x = 0;
				  d_y = 0;
				  if (x_pant0 > x_pant1) {		  //Mirem quin punt està per davant, si es el primer punt, els pasos aniran cap arrere

					  step_x = -1;
				  }
				  if (y_pant0 > y_pant1){           //Mirem quin punt està per dalt, si es el primer punt, els pasos aniran cap avall

					  step_y = -1;
				  }
				  if (x_dif > y_dif) {              // Si la diferencia entre les x es més gran que en les y, hem d'acostar primerament les x...
					  Err = x_dif / 2;
					  while ((x_pant0 + d_x) != x_pant1){  // Mentre la linea no arribe al punt final, calculem més punts de la recta
						  pixels[(y_pant0 + d_y) * w * 3 + (x_pant0 + d_x) * 3 + 0] = (unsigned char)(255.0 * objects[o].color[0]);
						  pixels[(y_pant0 + d_y) * w * 3 + (x_pant0 + d_x) * 3 + 1] = (unsigned char)(255.0 * objects[o].color[1]);
						  pixels[(y_pant0 + d_y) * w * 3 + (x_pant0 + d_x) * 3 + 2] = (unsigned char)(255.0 * objects[o].color[2]);

						  Err = Err - y_dif;

						  if (Err < 0){

							  d_y = d_y + step_y;
							  Err = Err + x_dif;
						  } //if		

						  d_x = d_x + step_x;
					  } // while 	

				  }
				  else  {

					  Err = y_dif / 2;
					  while ((y_pant0 + d_y) != y_pant1){ // <-----
						  pixels[(y_pant0 + d_y) * w * 3 + (x_pant0 + d_x) * 3 + 0] = (unsigned char)(255.0 * objects[o].color[0]);
						  pixels[(y_pant0 + d_y) * w * 3 + (x_pant0 + d_x) * 3 + 1] = (unsigned char)(255.0 * objects[o].color[1]);
						  pixels[(y_pant0 + d_y) * w * 3 + (x_pant0 + d_x) * 3 + 2] = (unsigned char)(255.0 * objects[o].color[2]);
						  Err = Err - x_dif;

						  if (Err < 0){
							  d_x = d_x + step_x;
							  Err = Err + y_dif;
						  } //if		
						  d_y = d_y + step_y;
					  } // while 	
				  }// if 


			  }// if llimits
		  
			  /*
			  * Creem nous punts per a les linees adicionals que es pintaran per a crear el grosor 
			  */
			  if (icont % 2 == 0){ // Nombre Par
				  
				  if (y_pant0_original - y_pant1_original == 0){ // línea horitzontal
					  y_pant0 = floor(y_pant0_original + (icont / 2));
					  y_pant1 = floor(y_pant1_original + (icont / 2));
					  					
				  }
				  else{

					  x_pant0 = floor(x_pant0_original - (icont / 2));
					  x_pant1 = floor(x_pant1_original - (icont / 2));

					  if (x_pant0 - x_pant1 != 0){ // Linees diagonals
						  if (((x_pant0 < x_pant1) && (y_pant0 > y_pant1)) || (((x_pant0 > x_pant1) && (y_pant0 < y_pant1)))){// /Diagonal de esquerra a dreta

							  if (x_pant0 < 0 && x_pant1 < 0){
								  x_pant0 = 0;
								  x_pant1 = 0;
							  }
							  else{
								  if (x_pant0 < 0){
									  x_pant0 = 0;
									  y_pant0 = y_pant0 - 1;
								  }
							  }
							 

						  }//if diagonal dreta a esquerra
						  else {

							  //Umbral especial
							  if (x_pant0 < 0 && x_pant1 < 0){
								  x_pant0 = 0;
								  x_pant1 = 0;
							  }
							  else{
								  if (x_pant0 < 0){
									  x_pant0 = 0;
									  y_pant0 = y_pant0 + 1;
								  }
							  }

						  }//else

					  } //if diagonals
					  
				  }
			  
			  }
			  else{ // Nombre impar
				  if (y_pant0_original - y_pant1_original == 0){ // línea horitzontal
					  y_pant0 = floor(y_pant0_original - ((icont - 1) / 2));
					  y_pant1 = floor(y_pant1_original - ((icont - 1) / 2));

				  }
				  else{
					  
					  x_pant0 = floor(x_pant0_original + ((icont - 1) / 2));
					  x_pant1 = floor(x_pant1_original + ((icont - 1) / 2));


					  if (x_pant0 - x_pant1 != 0){ // Linees diagonals
						  if (((x_pant0 < x_pant1) && (y_pant0 > y_pant1)) || (((x_pant0 > x_pant1) && (y_pant0 < y_pant1)))){// /Diagonal de esquerra a dreta

							  //Umbral especial
							  if (x_pant0 > w){
								 x_pant0 = w;
								 x_pant1 = w;
							  }
							  else{
								  if (x_pant1 > w){
									  x_pant1 = w;
									  y_pant1 = y_pant1 + 1;
								  }
							  }

							  
						  
						  }//if /Diagonal de esquerra a dreta
						  else { 
							  
							  //Umbral especial
							  if (x_pant0 > w){
								  x_pant0 = w;
								  x_pant1 = w;
							  }
							  else{
								  if (x_pant1 > w){
									  x_pant1 = w;
									  y_pant1 = y_pant1 - 1;
								  }
								  
							  }



						  }// else

					  } //if diagonals
				  }

			  }
			  
		  
		  }//for grosor
		  
	  }// if line



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
