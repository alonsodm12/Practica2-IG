/*	Prácticas de Informática Gráfica

	Grupo C					Curso 2022-23
 	
	Codigo base para la realización de las practicas de IG
	
	Estudiante: Alonso Doña Martinez

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
    std::vector < std::vector<float> >vertices;
    std::vector < std::vector<int> >caras;
    std::vector <std::vector<float>> normal_vertice;
    std::vector <std::vector<float>> normal_cara;


  mallaVirtual(){
    numero_vertices=0;
    numero_triangulos=0;
    
  }

  //Funcion que calcula las normales de cara y vértices
  //La introducimos en la clase general mallaVirtual ya que vamos a tener
  //que calcular la normal tanto de modelos PLY como de superficie de revolución
  void calculoNormales(){
  
  //Ajustamos el tamaño de cada uno y lo inicializamos a 0


  normal_cara.resize(numero_triangulos,std::vector<float>(3));
  normal_vertice.resize(numero_vertices,std::vector<float>(3));
  
  normal_cara.assign(numero_triangulos,std::vector<float>(3,0));
  normal_vertice.assign(numero_vertices,std::vector<float>(3,0));
 
  for(int i=0;i<numero_triangulos;i++){
        
        int vertice1=caras[i][0];
        int vertice2=caras[i][1];
        int vertice3=caras[i][2];

        std::vector <float> vector1(3); 
        std::vector <float> vector2(3);

        //Calculamos el vector
        for(int j=0;j<3;j++){
          vector1[j]= vertices[vertice2][j]-vertices[vertice1][j];
          vector2[j] = vertices[vertice3][j]-vertices[vertice1][j];
        }
        
        //Calculamos el producto vectorial de los vectores vector1 y vector2
        std::vector <float> producto_vectorial(3);
        
        producto_vectorial[0]=vector1[1]*vector2[2]-vector1[2]*vector2[1];
        producto_vectorial[1]=vector1[2]*vector2[0]-vector1[0]*vector2[2];
        producto_vectorial[2]=vector1[0]*vector2[1]-vector1[1]*vector2[0];

        //Calculamos el modulo
        float modulo=sqrt(producto_vectorial[0]*producto_vectorial[0]
          +producto_vectorial[1]*producto_vectorial[1]
            +producto_vectorial[2]*producto_vectorial[2]);
        
        //Dividimos entre el modulo para obtener vector perpendicular que será 
        //la normal de la cara
        if(modulo>0){
          for(int j=0;j<3;j++)
            normal_cara[i][j]=producto_vectorial[j]/modulo;
        }
         
        //Finalizado el calculo de la normal de la cara
        //Pasamos a calcular la normal de los vertices

        //Sumamos la normal de la cara a sus tres vertices correspondientes

        for(int j=0;j<3;j++){
          normal_vertice[vertice1][j]+=normal_cara[i][j];
          normal_vertice[vertice2][j]+=normal_cara[i][j];
          normal_vertice[vertice3][j]+=normal_cara[i][j];
        
        
          
        }

  }
    //Para cada vertice normalizamos su normal seguimos los mismos pasos que antes
    for(int j=0;j<numero_vertices;j++){
        float modulo=sqrt(normal_vertice[j][0]*normal_vertice[j][0]+normal_vertice[j][1]*normal_vertice[j][1]+normal_vertice[j][2]*normal_vertice[j][2]);
        if(modulo>0){
          for(int k=0;k<3;k++)
            normal_vertice[j][k]=normal_vertice[j][k]/modulo;
        }
    }
      
  }
};


//Clase mallaVirtual
//En ella implementaremos el constructor para que el código sea orientado a objetos

class SuperficieRevolucion:mallaVirtual
{
  std::vector <float> vertices_ply;
  int num_copias=0;

  public:
  SuperficieRevolucion(char *nombre_archivo_pse,int numeroCopias){

    //Comprobamos que el numero de copias sea mayor que 3
    if(numeroCopias<=3)
      exit;

    num_copias=numeroCopias;
    ply::read_vertices(nombre_archivo_pse,vertices_ply);

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
    //ultimo vertice no se accede y num_copias no se accede a la ultima porque es igual q la primera
    caras.resize((  (vertices_ply.size()/3-1)*(num_copias-1))*2,std::vector<int>(3));
    vertices.resize((vertices_ply.size()/3)*num_copias,std::vector<float>(3));
    
    for(int i=0;i<num_copias;i++){
      
      int contador=0;
      for(int j=0;j<vertices_ply.size()/3;j++){
        float angulo = 2*M_PI*i / (num_copias-1);
        
        float coordenadaX=vertices_ply[j+contador]*sin(angulo);
        float coordenadaY=vertices_ply[j+1+contador];
        float coordenadaZ=vertices_ply[j+contador]*cos(angulo);
        contador=contador+2;
        //std::vector<float> new_vertice={coordenadaX,coordenadaY,coordenadaZ};
        //vertices.push_back(new_vertice);
        vertices[i*(vertices_ply.size()/3)+j][0]=coordenadaX;
        vertices[i*(vertices_ply.size()/3)+j][1]=coordenadaY;
        vertices[i*(vertices_ply.size()/3)+j][2]=coordenadaZ;
        
      }
    }

    //Ya tendriamos la lista de vertices almacenada en nuestra estructura de datos original

    //Pasamos a la creacion de la lista de caras en nuestra estructura de datos original
    
    int cara=0;
    for(int i=0;i<num_copias-1;i++){
      for(int j=0;j<(vertices_ply.size()/3)-1;j++){
        int k = i*(vertices_ply.size()/3) + j;
        caras[cara][0]= k;
        caras[cara][1]= k+(vertices_ply.size()/3);
        caras[cara][2]= k+(vertices_ply.size()/3)+1;
        cara++;
        caras[cara][0]= k;
        caras[cara][1]= k+(vertices_ply.size()/3)+1;
        caras[cara][2]= k+1;
        cara++;
      }
    }

    numero_triangulos=caras.size();
    
    numero_vertices=vertices.size();
    std::cout<<"NUMERO CARASSSSSS: "<<numero_vertices<<std::endl;

    

    calculoNormales();
  }


void draw(void){
}  
void drawFLAT(void){

  glShadeModel(GL_FLAT);
  
  //glDisable(GL_LIGHTING);
  glBegin(GL_TRIANGLES);  
  for(int i=0;i<numero_triangulos;i++){
    glNormal3f(normal_cara[i][0],normal_cara[i][1],normal_cara[i][2]);
    for(int j=0;j<3;j++){
      int vertice_index = caras[i][j];
      
        double coor1=vertices[vertice_index][0];
        double coor2=vertices[vertice_index][1];
        double coor3=vertices[vertice_index][2];
        
        glVertex3f(coor1,coor2,coor3);
      

    }
  }  
  glEnd();
  glEnable (GL_LIGHTING);
  //glEnable (GL_LIGHTING);
}
void drawSMOOTH(void){

  glShadeModel(GL_SMOOTH);
  
  //glDisable(GL_LIGHTING);
  glBegin(GL_TRIANGLES);  
  for(int i=0;i<numero_triangulos;i++){
    glNormal3f(normal_cara[i][0],normal_cara[i][1],normal_cara[i][2]);
    for(int j=0;j<3;j++){
      int vertice_index = caras[i][j];
      
        double coor1=vertices[vertice_index][0];
        double coor2=vertices[vertice_index][1];
        double coor3=vertices[vertice_index][2];
        glNormal3f(normal_vertice[vertice_index][0],normal_vertice[vertice_index][1],normal_vertice[vertice_index][2]);
        glVertex3f(coor1,coor2,coor3);
      

    }
  }  
  glEnd();
  glEnable (GL_LIGHTING);
  //glEnable (GL_LIGHTING);
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
    caras.resize(caras_ply.size()/3,std::vector<int>(3));
    vertices.resize(vertices_ply.size()/3,std::vector<float>(3));
    
    
    for(int i=0;i<caras_ply.size();i++){
      caras[numero_triangulos][cuenta]=caras_ply[i];
      cuenta++;

      if(cuenta==3){
        numero_triangulos++;
        cuenta=0;
      }

    }
    for(int j=0;j<vertices_ply.size();j++){
        
        vertices[numero_vertices][varia]= vertices_ply[j];
        varia++;
        if(varia==3){
          numero_vertices++;
          varia=0;
        }
    }
  std::cout<<"Numero de caras: "<<caras_ply.size()<<std::endl; 
  std::cout<<"Numero de vertices: "<<numero_vertices<<std::endl;
  calculoNormales();
    
    
  }
void draw(void){
}  
void drawFLAT(void){

  glShadeModel(GL_FLAT);
  
  //glDisable(GL_LIGHTING);
  glBegin(GL_TRIANGLES);  
  for(int i=0;i<numero_triangulos;i++){
    glNormal3f(normal_cara[i][0],normal_cara[i][1],normal_cara[i][2]);
    for(int j=0;j<3;j++){
      int vertice_index = caras[i][j];
      
        double coor1=vertices[vertice_index][0];
        double coor2=vertices[vertice_index][1];
        double coor3=vertices[vertice_index][2];
        
        glVertex3f(coor1,coor2,coor3);
      

    }
  }  
  glEnd();
  //glEnable (GL_LIGHTING);
  //glEnable (GL_LIGHTING);
}
void drawSMOOTH(void){

  glShadeModel(GL_SMOOTH);
  
  //glDisable(GL_LIGHTING);
  glBegin(GL_TRIANGLES);  
  for(int i=0;i<numero_triangulos;i++){
    for(int j=0;j<3;j++){
      int vertice_index = caras[i][j];
      
        double coor1=vertices[vertice_index][0];
        double coor2=vertices[vertice_index][1];
        double coor3=vertices[vertice_index][2];
        glNormal3f(normal_vertice[vertice_index][0],normal_vertice[vertice_index][1],normal_vertice[vertice_index][2]);
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

  float  color[4] = { 1.0, 0.5, 0.0, 1.0 };
  float  color2[4] = {1.0, 0.0, 0.0, 1.0 };
  float color3[4] = {0.0, 1.0, 0.0, 1.0};

  glPushMatrix ();		// Apila la transformacion geometrica actual

  glClearColor (0.0, 0.0, 0.0, 1.0);	// Fija el color de fondo a negro

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Inicializa el buffer de color y el Z-Buffer

  transformacionVisualizacion ();	// Carga transformacion de visualizacion

  glLightfv (GL_LIGHT0, GL_POSITION, pos);	// Declaracion de luz. Colocada aqui esta fija en la escena

  ejesCoordenadas.draw();			// Dibuja los ejes
/*
  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);

  // Dibuja el modelo (A rellenar en prácticas 1,2 y 3)          
  
  CreadorMallas alonso("big_dodge.ply");
  alonso.drawFLAT();

  glTranslatef(10,0,0);

  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color2);
  /*
  SuperficieRevolucion lata("lata-pcue",70);
  lata.drawSMOOTH();

  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  SuperficieRevolucion alonsogod("lata-psup.ply",70);
  alonsogod.drawSMOOTH();
  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  SuperficieRevolucion alonsopep("lata-pinf.ply",70);
  alonsopep.drawSMOOTH();
  */
 
  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color2);
  SuperficieRevolucion peon("./Archivos.ply/perfil_cerrado.ply",68);
  peon.drawSMOOTH();
  glTranslatef(7,0,0);
  
  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color3);

  CreadorMallas creador("./Archivos.ply/beethoven.ply");
  creador.drawSMOOTH();

  glTranslatef(15,0,0);
  
  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);

  CreadorMallas big_dodge("./Archivos.ply/big_dodge.ply");
  big_dodge.drawFLAT();

  glTranslatef(15,0,0);
  
 

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
