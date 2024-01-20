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
#include "lector-jpg.h"

using namespace std;

/**	void initModel()

Inicializa el modelo y de las variables globales


**/

unsigned ancho=0;
unsigned alto=0;

int luzControl2=0;
int luzControl=0;

bool noUsaAmbiente=true;

//Funcion para controlar las luces
void setLuz(int luz){
  if(luz==1){
    if(luzControl==0){
      luzControl=1;
      glEnable(GL_LIGHT0);
    }  
    else{  
      luzControl=0;
      glDisable(GL_LIGHT0);
    }  
  }
  if(luz==2){
    if(luzControl2==0){
      luzControl2=1;
      glEnable(GL_LIGHT1);
    }  
    else{ 
      luzControl2=0;
      glDisable(GL_LIGHT1);
    }  
  }

}

void
initModel ()
{
}

//Funcion que nos permite asignar una textura procedente de un archivo
void Objeto3D::asignarTexturaArchivo(const char * nombre_arch){

  unsigned char * imagen = LeerArchivoJPEG(nombre_arch,ancho,alto);
  glGenTextures(1,&texId);
  glBindTexture(GL_TEXTURE_2D, texId);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   // Especifica el formato de la textura
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ancho, alto, 0, GL_RGB, GL_UNSIGNED_BYTE, imagen);

    // Libera la memoria de la imagen después de cargarla en la textura
    free(imagen);
}


//Clase cubo
//
//Nos permitirá crear un cubo sobre el que aplicaremos la textura del dado
//
class Cubo : public Objeto3D {
private:
    float lado;
public:
    Cubo(float lado) : lado(lado) {}

void draw() {
    
    
    glBegin(GL_QUADS);

    // Cara frontal
    glNormal3f (0.0, 0.0, 1.0); 
    glTexCoord2f(0.5,0.5); glVertex3f(-lado, -lado, lado);
    glTexCoord2f(0.75,0.5); glVertex3f(lado, -lado, lado);
    glTexCoord2f(0.75,0.75); glVertex3f(lado,lado, lado);
    glTexCoord2f(0.5,0.75); glVertex3f(-lado, lado, lado);

    // Cara posterior
    glNormal3f (0.0, 0.0, -1.0);
    glTexCoord2f(0.5,0.25); glVertex3f(lado, -lado, -lado);
    glTexCoord2f(0.75,0.25); glVertex3f(-lado, -lado, -lado);
    glTexCoord2f(0.75,0.5); glVertex3f(-lado, lado, -lado);
   glTexCoord2f(0.5,0.5); glVertex3f(lado, lado, -lado);

    // Cara superior
    glNormal3f (0.0, 1.0, 0.0);
    glTexCoord2f(0.25,0.5); glVertex3f(-lado, lado, lado);
    glTexCoord2f(0.5,0.5); glVertex3f(lado, lado, lado);
    glTexCoord2f(0.5,0.75); glVertex3f(lado, lado, -lado);
    glTexCoord2f(0.25,0.75); glVertex3f(-lado, lado, -lado);
    
    // Cara inferior
    glNormal3f (0.0, -1.0, 0.0);
    glTexCoord2f(0.0, 0.500); glVertex3f(-lado, -lado, lado);
    glTexCoord2f(0.25, 0.500); glVertex3f(-lado, -lado, -lado);
    glTexCoord2f(0.25, 0.75); glVertex3f(lado, -lado, -lado);
    glTexCoord2f(0.0, 0.75); glVertex3f(lado, -lado, lado);

    // Cara derecha
    glNormal3f (1.0, 0.0, 0.0);
    glTexCoord2f(0.75,0.5); glVertex3f(lado, -lado, lado);
    glTexCoord2f(1,0.5); glVertex3f(lado, -lado, -lado);
    glTexCoord2f(1,0.75); glVertex3f(lado, lado, -lado);
    glTexCoord2f(0.75,0.75); glVertex3f(lado, lado, lado);

    // Cara izquierda
    glNormal3f (-1.0, 0.0, 0.0);
    glTexCoord2f(0.5, 0.75); glVertex3f(-lado, -lado, -lado);
    glTexCoord2f(0.75, 0.75); glVertex3f(-lado, -lado, lado);
    glTexCoord2f(0.75, 1); glVertex3f(-lado, lado, lado);
    glTexCoord2f(0.5, 1); glVertex3f(-lado, lado, -lado);

    glEnd();
}
};


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

//Clase mallaVirtual en ella establecemos las estructuras de datos
//que nos permitiran representar nuestra malla de triangulos y calcular su normal 
//Metodos:
//Contructor basico 
//Funcion general de calculoNormales

class mallaVirtual : public Objeto3D
{
  public:
    
    int numero_vertices;
    int numero_triangulos;
    GLfloat reflectividad_difusa[4];
    GLfloat reflectividad_especular[4];
    GLfloat reflectividad_ambiente[4];
    GLfloat brillo;
    

    std::vector < std::vector<float> >vertices;
    std::vector < std::vector<int> >caras;
    std::vector <std::vector<float>> normal_vertice;
    std::vector <std::vector<float>> normal_cara;
    std::vector <std::pair<float,float>> coordenadas_textura;

  public :
  mallaVirtual(){
    GLfloat valor_por_defecto[4]={0.2,0.2,0.2,1.0};
    numero_vertices=0;
    numero_triangulos=0;
    for (int i = 0; i < 4; ++i) {
      reflectividad_difusa[i] = valor_por_defecto[i];
      reflectividad_especular[i] = valor_por_defecto[i];
      reflectividad_ambiente[i] = valor_por_defecto[i];
    }
  }

  void asignarMaterial(){
    if(noUsaAmbiente)    
      glMaterialfv(GL_FRONT,GL_AMBIENT,reflectividad_difusa);
    else
       glMaterialfv(GL_FRONT,GL_AMBIENT,reflectividad_ambiente); 
    glMaterialfv(GL_FRONT,GL_SPECULAR,reflectividad_especular);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,reflectividad_difusa);
    glMaterialf( GL_FRONT, GL_SHININESS, brillo) ;
  }

  void setReflectividad_difusa(float r,float g, float b,float a){
    
    reflectividad_difusa[0]=r;
    reflectividad_difusa[1]=g;
    reflectividad_difusa[2]=b;
    reflectividad_difusa[3]=a;
  }

  void setReflectividad_especular(float r,float g, float b,float a){
    
    reflectividad_especular[0]=r;
    reflectividad_especular[1]=g;
    reflectividad_especular[2]=b;
    reflectividad_especular[3]=a;
  }

  void setReflectividad_ambiente(float r,float g, float b,float a){
    reflectividad_ambiente[0]=r;
    reflectividad_ambiente[1]=g;
    reflectividad_ambiente[2]=b;
    reflectividad_ambiente[3]=a;
    noUsaAmbiente=false;
  }
  void setBrillo(float e){
    brillo=e;
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

        //Calculamos los vectores con los puntos p0,p1  p0,p2 
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


//Clase CreadorMallas
//En ella implementaremos el constructor para que el código sea orientado a objetos

class CreadorMallas: public mallaVirtual
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
  calculoNormales();
       
  }

void draw(void){
}  
void drawFLAT(void){

  if(texId!=0){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texId);

  }


  glShadeModel(GL_FLAT);

  asignarMaterial();
  
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
  if (texId!=0) {
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
  }
}
void drawSMOOTH(void){

  glShadeModel(GL_SMOOTH);
  
  asignarMaterial();
  
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
  
}

//Una funcion para dibujar caras planas y otra para lo otro
};

//Clase SuperficieRevolucion heredada de mallaVirtual nos permite
//implementar el objeto de revolucion

class SuperficieRevolucion:public mallaVirtual
{
  std::vector <float> vertices_ply;
  int num_copias=0;

  public:
  SuperficieRevolucion(char *nombre_archivo_pse,int numeroCopias){

    //Comprobamos que el numero de copias sea mayor que 3
    if(numeroCopias<=3)
      exit(1);

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
    caras.resize(((vertices_ply.size()/3-1)*(num_copias-1))*2,std::vector<int>(3));
    vertices.resize((vertices_ply.size()/3)*num_copias, std::vector<float>(3));
    
    
    for(int i=0;i<num_copias;i++){
      
      int contador=0;
      for(int j=0;j<vertices_ply.size()/3;j++){
        float angulo = 2*M_PI*i / (num_copias-1);
        
        float coordenadaX=vertices_ply[j+contador]*sin(angulo);
        float coordenadaY=vertices_ply[j+1+contador];
        float coordenadaZ=vertices_ply[j+contador]*cos(angulo);
        contador=contador+2;
        
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
    

    TexturasRevolucion();

    calculoNormales();
  }


void TexturasRevolucion(){
    
  float altura_maxima=0;
  float altura_minima=0;


  // Encuentra las alturas mínima y máxima
  for (int i = 0; i < vertices.size(); i++) {
      float altura_actual = vertices[i][1];  // Asumiendo que la altura está en la coordenada y

      if (altura_actual < altura_minima) {
          altura_minima = altura_actual;
      }

      if (altura_actual > altura_maxima) {
          altura_maxima = altura_actual;
      }
  }


  // Calcula las coordenadas de textura
  for (int i = 0; i < vertices.size(); i++) {
      float alfa = atan2(vertices[i][2], vertices[i][0]);
      float h = vertices[i][1];

      float u = 1-(alfa / (2 * M_PI) +0.5);
      float v = (h - altura_minima) / (altura_maxima - altura_minima);

      coordenadas_textura.push_back({u, v});
  }

}


void draw(void){
}  
void drawFLAT(void){


  if(texId!=0){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texId);
  }

  glShadeModel(GL_FLAT);

  //Asignamos materiales
  
  asignarMaterial();
  
  glBegin(GL_TRIANGLES);  

  for(int i=0;i<numero_triangulos;i++){
    glNormal3f(normal_cara[i][0],normal_cara[i][1],normal_cara[i][2]);
    for(int j=0;j<3;j++){
      
      int vertice_index = caras[i][j];
      
        double coor1=vertices[vertice_index][0];
        double coor2=vertices[vertice_index][1];
        double coor3=vertices[vertice_index][2];
        
        if(texId!=0){
          glTexCoord2f(coordenadas_textura[vertice_index].first,coordenadas_textura[vertice_index].second);
        }      
        glVertex3f(coor1,coor2,coor3);    
    }
  }  
  glEnd();
   
  if (texId!=0) {
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
  }
  

}
void drawSMOOTH(void){


  if(texId!=0){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texId);
  }
  glShadeModel(GL_SMOOTH);
  
  asignarMaterial();
  glBegin(GL_TRIANGLES);  
  for(int i=0;i<numero_triangulos;i++){
    glNormal3f(normal_cara[i][0],normal_cara[i][1],normal_cara[i][2]);
    for(int j=0;j<3;j++){
      int vertice_index = caras[i][j];
      
        double coor1=vertices[vertice_index][0];
        double coor2=vertices[vertice_index][1];
        double coor3=vertices[vertice_index][2];
        glNormal3f(normal_vertice[vertice_index][0],normal_vertice[vertice_index][1],normal_vertice[vertice_index][2]);
        if(texId!=0){
          glTexCoord2f(coordenadas_textura[vertice_index].first,coordenadas_textura[vertice_index].second);
        }    
        glVertex3f(coor1,coor2,coor3);
      

    }
  }  
  glEnd();
    if (texId!=0) {
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
  }
  

}


};

/**	void Dibuja( void )

Procedimiento de dibujo del modelo. Es llamado por glut cada vez que se debe redibujar.

**/

void Dibuja (void)
{
  static GLfloat  pos[4] = { 5.0, 5.0, 10.0, 0.0 };	// Posicion de la fuente de luz

  float  naranja[4] = { 1.0, 0.5, 0.0, 1.0 };
  float  rojo[4] = {1.0, 0.0, 0.0, 1.0 };
  float blanco[4] = {1.0, 1.0, 1.0, 1.0};


  glPushMatrix ();		// Apila la transformacion geometrica actual

  glClearColor (0.0, 0.0, 0.0, 1.0);	// Fija el color de fondo a negro

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Inicializa el buffer de color y el Z-Buffer

  transformacionVisualizacion ();	// Carga transformacion de visualizacion
  glEnable(GL_LIGHTING);

  ////////////////////////////////////////////////////////////////////////
  //LUCES 
  ////////////////////////////////////////////////////////////////////////
  
  ////////////////////////////////////////////////////////////////////////
  //DEFINIMOS LUZ 1
  ////////////////////////////////////////////////////////////////////////
  GLfloat luz_ambiente1[] = { 0.2, 0.2, 0.2, 1.0 };
  GLfloat luz_difusa1[] = {1.0, 1.0, 0.0, 1.0};
  GLfloat luz_posicion1[] = {-50, 50, 100, 1.0};
  GLfloat luz_especular1[] = {1,1, 1, 1.0};
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  //DEFINIMOS LUZ 2
  ////////////////////////////////////////////////////////////////////////
  GLfloat luz_ambiente2[4] = {0.5, 1, 1, 1.0};
  GLfloat luz_especular2[4] = {0.5, 1, 1,1.0};
  GLfloat luz_posicion2[4] = {50, 50, -100, 1};
  
  //LUZ DIFUSA + AMBIENTE + ESPECULAR (COMBINACIÓN DE LOS TRES TIPOS)
  glLightfv(GL_LIGHT0,GL_AMBIENT,luz_ambiente1);
  glLightfv(GL_LIGHT0,GL_DIFFUSE,luz_difusa1);
  glLightfv (GL_LIGHT0, GL_SPECULAR, luz_especular1);
  glLightfv (GL_LIGHT0, GL_POSITION, luz_posicion1);


  //LUZ ESPECULAR + AMBIENTE
  glLightfv (GL_LIGHT1, GL_AMBIENT, luz_ambiente2);
  glLightfv (GL_LIGHT1, GL_SPECULAR, luz_especular2);
  glLightfv (GL_LIGHT1, GL_POSITION, luz_posicion2);

  
  ejesCoordenadas.draw();			// Dibuja los ejes
  
  //////////////////////////////////////////////////////////////////////
  //REPRESENTAMOS CUBO
  //////////////////////////////////////////////////////////////////////

  glEnable(GL_TEXTURE_2D);
  glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,blanco);
  Cubo cubo(0.5);
  cubo.asignarTexturaArchivo("./Archivos.ply/dado.jpg");
  cubo.draw();
  glDisable(GL_TEXTURE_2D);
  
  glTranslatef(2,0,0);
  
  //////////////////////////////////////////////////////////////////////
  //REPRESENTAMOS LATA
  //////////////////////////////////////////////////////////////////////
  SuperficieRevolucion tapa_sup("./Archivos.ply/lata-psup.ply",12);
  tapa_sup.drawSMOOTH();
  SuperficieRevolucion lata("./Archivos.ply/lata-pcue.ply",60);
  lata.asignarTexturaArchivo("./Archivos.ply/alhambra.jpg");

  //////////////////////////////////////////////////////////////////
  //REFLECTIVIDAD DIFUSA + AMBIENTAL + ESPECULAR
  //////////////////////////////////////////////////////////////////

  lata.setReflectividad_ambiente(1.0,1.0, 1.0, 1.0);
  lata.setReflectividad_difusa(1.0, 1.0, 1.0, 1.0);
  lata.setReflectividad_especular(1, 1, 1, 1.0);
  lata.setBrillo(5);

  lata.drawSMOOTH();

  SuperficieRevolucion tapa_inf("./Archivos.ply/lata-pinf.ply",12);
  tapa_inf.drawSMOOTH();

  
  glTranslatef(7,0,0);
  CreadorMallas pie("./Archivos.ply/footbones.ply");

  ///////////////////////////////////////////////////////////////////
  //MATERIAL AMBIENTAL
  //////////////////////////////////////////////////////////////////

  pie.setReflectividad_ambiente(1.0, 1.0,1.0, 1.0);
  pie.setReflectividad_difusa(0.0, 0.0, 0.0, 1.0);
  pie.setReflectividad_especular(0.0, 0.0, 0.0, 1.0);
  pie.setBrillo(0);

  pie.drawSMOOTH();

  
  glTranslatef(12,0,0);

  ///////////////////////////////////////////////////////////////////
  //MATERIAL DIFUSO + AMBIENTAL
  //////////////////////////////////////////////////////////////////

  CreadorMallas pie2("./Archivos.ply/footbones.ply");
  pie2.setReflectividad_ambiente(1.0, 1.0, 1.0, 1.0);
  pie2.setReflectividad_difusa(1.0, 1.0, 1.0, 1.0);
  pie2.setReflectividad_especular(0.0, 0.0, 0.0, 1.0);
  pie2.setBrillo(0);

  pie2.drawSMOOTH();


  glTranslatef(12,0,0);

  ///////////////////////////////////////////////////////////////////
  //MATERIAL ESPECULAR
  //////////////////////////////////////////////////////////////////

  CreadorMallas pie3("./Archivos.ply/footbones.ply");
  pie3.setReflectividad_ambiente(0,0,0, 1.0);
  pie3.setReflectividad_difusa(0.0, 0.0, 0.0, 1.0);
  pie3.setReflectividad_especular(1, 1, 1, 1.0);
  pie3.setBrillo(5);

  pie3.drawSMOOTH();

  glTranslatef(12,0,0);

  ///////////////////////////////////////////////////////////////////
  //MATERIAL COMBINADO AMBIENTAL + DIFUSA + ESPECULAR
  //////////////////////////////////////////////////////////////////
  
  CreadorMallas pie4("./Archivos.ply/footbones.ply");
  pie4.setReflectividad_ambiente(1.0, 1, 1.0, 1.0);
  pie4.setReflectividad_difusa(1.0, 1, 1.0, 1.0);
  pie4.setReflectividad_especular(1, 1, 1, 1.0);
  pie4.setBrillo(5);

  pie4.drawSMOOTH();

  glDisable(GL_LIGHTING);
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
