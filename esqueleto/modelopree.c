/*	Prácticas de Informática Gráfica

	Grupo C					Curso 2022-23
 	
	Codigo base para la realización de las practicas de IG
	
	Estudiante: 

=======================================================
	G. Arroyo, J.C. Torres 
	Dpto. Lenguajes y Sistemas Informticos
	(Univ. de Granada)

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details 
 http://www.gnu.org/copyleft/gpl.html

=======================================================/
modulo modelo.c
    Representación del modelo
    Funciones de dibujo
    Función Idle

*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>		// Libreria de utilidades de OpenGL
#include "practicasIG.h"

/**	void initModel()

Inicializa el modelo y de las variables globales


**/
void
initModel ()
{

}



class Ejes:Objeto3D 
{ 
public: 
    float longitud = 30;
// Dibuja el objeto
void draw( )
{
  glDisable (GL_LIGHTING);
  glBegin (GL_LINES);
  {
    glColor3f (0, 1, 0);
    glVertex3f (0, 0, 0);
    glVertex3f (0, longitud, 0);

    glColor3f (1, 0, 0);
    glVertex3f (0, 0, 0);
    glVertex3f (longitud, 0, 0);

    glColor3f (0, 0, 1);
    glVertex3f (0, 0, 0);
    glVertex3f (0, 0, longitud);
  }
  glEnd ();
  glEnable (GL_LIGHTING);

}
} ; 

Ejes ejesCoordenadas;

class mallaVirtual : Objeto3D
{
  public:
    int numero_vertices;
    int numero_triangulos;
    std::vector < std::vector<float> >vertice;
    std::vector < std::vector<int> >triangulo;
    std::vector <std::vector<float>> normal_vertice;
    std::vector <std::vector<int>> normal_cara;


  mallaVirtual(){
    numero_vertices=0;
    numero_triangulos=0;
    
  }
};

/* v8 = 3 * (8-1) */
/* vi = 3 * (i-1) */

class SuperficieRevolucion:mallaVirtual
{
  std::vector <float> vertices;
  int num_copias=0;

  public:
  SuperficieRevolucion(char *nombre_archivo_pse,int numeroCopias){

    //Comprobamos que el numero de copias sea mayor que 3
    if(numeroCopias<=3)
      exit;

    num_copias=numeroCopias;
    ply::read_vertices(nombre_archivo_pse,vertices);

    generarObjetoPorRevolucion();
  }

  void generarObjetoPorRevolucion(){

    //Generamos en primer lugar la lista de vértices

    //El perfil tiene m vertices dados de abajo hacia arriba
    //De ese perfil generaremos m replicas rotadas=num_copias
    //cada replica se denominara instancia de perfil
    //num_copias>3
    //cada instancia del perfil forma un angulo de 2pi/(n-1)rad con la siguiente o anterior

        //Ajustamos el tamaño de cada uno
    triangulo.resize(vertices.size()*num_copias/3,std::vector<int>(3));
    vertice.resize(vertices.size()*num_copias,std::vector<float>(3));

    for(int i=0;i<num_copias;i++){
      int j=0;
      for(auto it=vertices.begin();it!=vertices.end();++it){
        float angulo = 2*M_PI*i / (num_copias-1);
        float coor_x=vertices[0]*cos(angulo);
        float coor_y=vertices[1];
        float coor_z=vertices[0]*sin(angulo);

        vertice[i*vertices.size()+j][0]=coor_x;
        vertice[i*vertices.size()+j][1]=coor_y;
        vertice[i*vertices.size()+j][2]=coor_z;
        j++;
      }
    }

    //Ya tendriamos la lista de vertices almacenada en nuestra estructura de datos original

    //Pasamos a la creacion de la lista de caras en nuestra estructura de datos original
    int l=0;
    for(int i=0;i<num_copias-1;i++){
      for(int j=0;j<vertices.size()-2;j++){
        int k = i*vertices.size() + j;
        triangulo[l][0]= k;
        triangulo[l][1]= k+m;
        triangulo[l][2]= k+m+1;
        l++;
        triangulo[l][0]= k;
        triangulo[l][1]= k+m+1;
        triangulo[l][2]= k+1;
      
      }
    }
    calculoNormales();
  }

};


class CreadorMallas:mallaVirtual
{
  std::vector < float >vertices_ply;
  std::vector < int >caras_ply;

  public:
  CreadorMallas(char *nombre_archivo_pse ) : mallaVirtual()
  {
    int cuenta=0;
    int varia=0;
    

    ply::read(nombre_archivo_pse,vertices_ply,caras_ply);
    

    //Ajustamos el tamaño de cada uno
    triangulo.resize(caras_ply.size(),std::vector<int>(3));
    vertice.resize(vertices_ply.size(),std::vector<float>(3));
    
    
    for(int i=0;i<caras_ply.size();i++){
      triangulo[numero_triangulos][cuenta]=caras_ply[i];
      cuenta++;

      if(cuenta==3){
        numero_triangulos++;
        cuenta=0;
      }

    }
    for(int j=0;j<vertices_ply.size();j++){
        
        vertice[numero_vertices][varia]= vertices_ply[j];
        varia++;
        if(varia==3){
          numero_vertices++;
          varia=0;
        }
    }
  std::cout<<"Numero de caras: "<<numero_triangulos<<std::endl; 
  std::cout<<"Numero de vertices: "<<numero_vertices<<std::endl;
  calculoNormales();
    
    
  }


  //Funcion que calcula las normales de cara y vértices
  void calculoNormales(){

  
  //Ajustamos el tamaño de cada uno
/*
  std::vector <std::vector<float>> normal_vertice(numero_vertices,std::vector<float>(3,0));
  std::vector <std::vector<int>> normal_cara(numero_triangulos/3,std::vector<int>(3,0));
*/

  normal_cara.resize(numero_triangulos,std::vector<int>(3));
  normal_vertice.resize(numero_vertices,std::vector<float>(3));
  
  normal_cara.assign(numero_triangulos,std::vector<int>(3,0));
  normal_vertice.assign(numero_vertices,std::vector<float>(3,0));
 
  for(int i=0;i<numero_triangulos;i++){
        int v1=triangulo[i][0];
        int v2=triangulo[i][1];
        int v3=triangulo[i][2];

        std::vector <double> edge1 = {vertice[v2][0]-vertice[v1][0],vertice[v2][1]-vertice[v1][1],vertice[v2][2]-vertice[v1][2] };
        std::vector <double> edge2 = {vertice[v3][0]-vertice[v1][0],vertice[v3][1]-vertice[v1][1],vertice[v3][2]-vertice[v1][2] };
        
        //Calculamos el producto vectorial de los vectores 0,1 0,2
        std::vector <double> caraNormal(3);
        
        caraNormal[0]=edge1[2]*edge2[3]-edge1[3]*edge2[2];
        caraNormal[1]=edge1[3]*edge2[1]-edge1[1]*edge2[3];
        caraNormal[2]=edge1[1]*edge2[2]-edge1[2]*edge2[1];

        double modulo=sqrt(caraNormal[0]*caraNormal[0]+caraNormal[1]*caraNormal[1]+caraNormal[2]*caraNormal[2]);
        
        
        normal_cara[i][0]=caraNormal[0]/modulo;
        normal_cara[i][1]=caraNormal[1]/modulo;
        normal_cara[i][2]=caraNormal[2]/modulo;
        
        //En intermediario tenemos la normal de la cara

        normal_vertice[v1][0]+=normal_cara[i][0];
        normal_vertice[v1][1]+=normal_cara[i][1];
        normal_vertice[v1][2]+=normal_cara[i][2];

        normal_vertice[v2][0]+=normal_cara[i][0];
        normal_vertice[v2][1]+=normal_cara[i][1];
        normal_vertice[v2][2]+=normal_cara[i][2];


        normal_vertice[v3][0]+=normal_cara[i][0];
        normal_vertice[v3][1]+=normal_cara[i][1];
        normal_vertice[v3][2]+=normal_cara[i][2];

    }

    for(int j=0;j<numero_vertices;j++){
        double modulo=sqrt(normal_vertice[j][0]*normal_vertice[j][0]+normal_vertice[j][1]*normal_vertice[j][1]+normal_vertice[j][2]*normal_vertice[j][2]);
        normal_vertice[j][0]=normal_vertice[j][0]/modulo;
        normal_vertice[j][1]=normal_vertice[j][1]/modulo;
        normal_vertice[j][2]=normal_vertice[j][2]/modulo;

    }
      
  }
 
void draw(void){

  glShadeModel(GL_SMOOTH);
  
  //glDisable(GL_LIGHTING);
  glBegin(GL_TRIANGLES);  
  for(int i=0;i<numero_triangulos;i++){
    glNormal3f(normal_cara[i][0],normal_cara[i][1],normal_cara[i][2]);
    for(int l=0;l<3;l++){
      int vertice_index = triangulo[i][l];
      
        double coor1=vertice[vertice_index][0];
        double coor2=vertice[vertice_index][1];
        double coor3=vertice[vertice_index][2];
        
        glVertex3f(coor1,coor2,coor3);
      

    }
  }  
  glEnd();
  glEnable (GL_LIGHTING);
  //glEnable (GL_LIGHTING);
}
//Una funcion para dibujar caras planas y otra para lo otro
};
/**	void Dibuja( void )

Procedimiento de dibujo del modelo. Es llamado por glut cada vez que se debe redibujar.

**/

void Dibuja (void)
{
  static GLfloat  pos[4] = { 5.0, 5.0, 10.0, 0.0 };	// Posicion de la fuente de luz

  float  color[4] = { 0.8, 0.0, 1, 1 };

  glPushMatrix ();		// Apila la transformacion geometrica actual

  glClearColor (0.0, 0.0, 0.0, 1.0);	// Fija el color de fondo a negro

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Inicializa el buffer de color y el Z-Buffer

  transformacionVisualizacion ();	// Carga transformacion de visualizacion

  glLightfv (GL_LIGHT0, GL_POSITION, pos);	// Declaracion de luz. Colocada aqui esta fija en la escena

  ejesCoordenadas.draw();			// Dibuja los ejes

  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);

  // Dibuja el modelo (A rellenar en prácticas 1,2 y 3)          

  CreadorMallas alonso("beethoven.ply");
  alonso.draw();
  glPopMatrix ();		// Desapila la transformacion geometrica


  glutSwapBuffers ();		// Intercambia el buffer de dibujo y visualizacion
}


/**	void idle()

Procedimiento de fondo. Es llamado por glut cuando no hay eventos pendientes.

**/
void idle (int v)
{

  glutPostRedisplay ();		// Redibuja
  glutTimerFunc (30, idle, 0);	// Vuelve a activarse dentro de 30 ms
}
